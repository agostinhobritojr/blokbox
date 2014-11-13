#ifndef FFTCALC_H
#define FFTCALC_H

#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include <QVector>
#include <QDebug>
#include <QTimer>
#include <QObject>
#include "fft.h"

// SPECSIZE is the size of fft array that is dispatched to mainwindow
#define SPECSIZE 512

/**
 * @brief The BufferProcessor class is designed to process audio buffers sent by mainwindow
 * @details We observed that using the Qt version at the time of development of this code
 * the audiobuffer size changes with different songs and with different operating systems.
 *
 * Some of the experiments, audio buffer duration changed from 6ms to 1s, as well as
 * sample size. Therefore, we decide to split the sample in chuncks of 512 elements each
 *
 * numberOfChunks = buffer_size/SPECSIZE
 *
 * and calculate chunck duration based on the buffer duration as
 *
 * interval = duration/numberOfChunks
 */
class BufferProcessor: public QObject{
  Q_OBJECT
  //!array stores local audio buffer passed for processing
  QVector<double> array;

  //!window stores a window function.
  //!Windowing is used to produce better shaped Fourier spectra
  QVector<double> window;

  //!the fourier spectrum
  QVector<double> spectrum;

  //!a logarithmic scale to prepare spectrum storage
  QVector<double> logscale;

  //!this timer tells when to calc each spectrum
  QTimer *timer;

  //!defines is spectrum is compressed (logarithmic u scale)
  bool compressed;

  //!tells if fft is running
  bool running;

  //!stores the number of chunks
  int numberOfChunks;

  //!stores interval for send each chunk for spectrum calculation
  int interval;

  //!counts the number of passes until chunkNumber is reached
  int pass;

  //!stores the chunk to be sent for fft calculations
  CArray complexFrame;
public slots:
  /**
   * @brief processBuffer is the invoked every time a new audio buffer arrives on mainwindow
   * @param _array is a copy of the audio buffer
   * @param duration stores how many milliseconds this buffer takes to play
   */
  void processBuffer(QVector<double> _array, int duration);
signals:
  /**
   * @brief calculatedSpectrum is emitted every time a new spectrum is available for displaying
   * @details every interval ms, a timer sends a new chunk for calculate its fft. When it is
   * available, we can send a signal containing the spectrum array
   * @param spectrum stores the fft spectrum array in double format
   */
  void calculatedSpectrum(QVector<double> spectrum);
  /**
   * @brief allDone signal is emmited when all chunks has been processed
   */
  void allDone(void);
protected slots:
  /**
   * @brief run is called each interval ms to perform FFT calculation of a given chunk
   */
  void run();
public:
  explicit BufferProcessor(QObject *parent=0);
  ~BufferProcessor();
  void calc(QVector<double> &_array, int duration);
};

// fftcalc runs in a separate thread
class FFTCalc : public QObject{
  Q_OBJECT
private:
  bool isBusy;
  BufferProcessor processor;
  QThread processorThread;

public:
  explicit FFTCalc(QObject *parent = 0);
  ~FFTCalc();
  void calc(QVector<double> &_array, int duration);
public slots:
  void setSpectrum(QVector<double> spectrum);
  void freeCalc();
signals:
  void calculatedSpectrum(QVector<double> spectrum);
};

#endif // FFTCALC_H
