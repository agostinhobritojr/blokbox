#include "fftcalc.h"

#undef CLAMP
#define CLAMP(a,min,max) ((a) < (min) ? (min) : (a) > (max) ? (max) : (a))

// fftcalc class is designed to treat with fft calculations
FFTCalc::FFTCalc(QObject *parent)
  :QObject(parent){

  // fftcalc is done in other thread
  // so it cannot overload the main thread
  processor.moveToThread(&processorThread);

  qRegisterMetaType< QVector<double> >("QVector<double>");
  connect(&processor, SIGNAL(calculatedSpectrum(QVector<double>)), SLOT(setSpectrum(QVector<double>)));
  connect(&processor, SIGNAL(allDone()),SLOT(freeCalc()));
  processorThread.start(QThread::LowestPriority);
  isBusy = false;
}

FFTCalc::~FFTCalc(){
  processorThread.quit();
  processorThread.wait(10000);
}

void FFTCalc::calc(QVector<double> &_array, int duration){
  if(isBusy)
    return;
  isBusy = true;
  QMetaObject::invokeMethod(&processor, "processBuffer",
                            Qt::QueuedConnection, Q_ARG(QVector<double>, _array), Q_ARG(int, duration));
}

void FFTCalc::setSpectrum(QVector<double> spectrum){
  emit calculatedSpectrum(spectrum);
}

void FFTCalc::freeCalc()
{
  isBusy = false;
}
/*
 * processes the buffer for fft calculation
 */

BufferProcessor::BufferProcessor(QObject *parent){
  Q_UNUSED(parent);
  timer = new QTimer(this);
  connect(timer,SIGNAL(timeout()),this,SLOT(run()));
  window.resize(SPECSIZE);
  complexFrame.resize(SPECSIZE);
  spectrum.resize(SPECSIZE/2);
  logscale.resize(SPECSIZE/2+1);
  compressed = true;
  for(int i=0; i<SPECSIZE;i++){
    window[i] = 0.5 * (1 - cos((2*PI*i)/(SPECSIZE)));
  }
  for(int i=0; i<=SPECSIZE/2; i++){
    logscale[i] = powf (SPECSIZE/2, (float) 2*i / SPECSIZE) - 0.5f;
  }
  running = false;
  timer->start(100);
}

BufferProcessor::~BufferProcessor(){
  timer->stop();

}

void BufferProcessor::processBuffer(QVector<double> _array, int duration){
  // if the music is new, new sizes may arrive
  if(array.size() != _array.size()){
    numberOfChunks = _array.size()/SPECSIZE;
    array.resize(_array.size());
  }
  interval = duration/numberOfChunks;
  if(interval < 1)
    interval = 1;
  array = _array;
  pass = 0;
  timer->start(interval);
}

void BufferProcessor::run(){
  unsigned long bufferSize;
  qreal amplitude;
  if(pass == numberOfChunks){
    emit allDone();
    return;
  }
  bufferSize = array.size();
  if(bufferSize < SPECSIZE){
    return;
  }
  for(uint i=0; i<SPECSIZE; i++){
    complexFrame[i] = Complex(window[i]*array[i+pass*SPECSIZE],0);
  }
  fft(complexFrame);
  for(uint i=0; i<SPECSIZE/2;i++){
    qreal SpectrumAnalyserMultiplier = 1e-2;
    amplitude = SpectrumAnalyserMultiplier*std::abs(complexFrame[i]);
    amplitude = qMax(qreal(0.0), amplitude);
    amplitude = qMin(qreal(1.0), amplitude);
    complexFrame[i] = amplitude;
  }

  if(compressed){
    for (int i = 0; i <SPECSIZE/2; i ++){
      /* sum up values in freq array between logscale[i] and logscale[i + 1],
         including fractional parts */
      int a = ceilf (logscale[i]);
      int b = floorf (logscale[i+1]);
      float sum = 0;

      if (b < a)
        sum += complexFrame[b].real()*(logscale[i+1]-logscale[i]);
      else{
        if (a > 0)
          sum += complexFrame[a-1].real()*(a-logscale[i]);
        for (; a < b; a++)
          sum += complexFrame[a].real();
        if (b < SPECSIZE/2)
          sum += complexFrame[b].real()*(logscale[i+1] - b);
      }

      /* fudge factor to make the graph have the same overall height as a
         12-band one no matter how many bands there are */
      sum *= (float) SPECSIZE/24;

      /* convert to dB */
      float val = 20*log10f (sum);

      /* scale (-DB_RANGE, 0.0) to (0.0, 1.0) */
      val = 1 + val / 40;
      spectrum[i] = CLAMP (val, 0, 1);
    }
  }
  else{
    for(int i=0; i<SPECSIZE/2; i++){
      spectrum[i] = CLAMP(complexFrame[i].real()*100,0,1);
    }
  }
  emit calculatedSpectrum(spectrum);
  pass++;
}


