#include "spectrograph.h"
#include <iostream>
#include <QtCore/qmath.h>
#include <QBrush>
#include <QPen>
#include <fstream>
#include <QGradient>
#include <QDebug>
#include <QResizeEvent>
#include <QTimerEvent>
#include <QPainterPath>
#include <QEasingCurve>

Spectrograph::Spectrograph(QWidget *parent) :
  AbstractSpectrograph(parent){
  // at each 15ms, the timer clicks and timerEvent is called
  startTimer(15);
  // however 256 samples may be delivered to this widget, only
  // 64 are to be displayed becaus of speed limitations ;)
  NUM_BANDS = 64;

  //
  spectrum.resize(NUM_BANDS);
  decay.resize(NUM_BANDS);
  gravity.resize(NUM_BANDS);
  delay[0].resize(NUM_BANDS);
  delay[1].resize(NUM_BANDS);
  for(int i=0; i<NUM_BANDS; i++){
      decay[i]=0;
      gravity[i]=1;
      spectrum[i]=1;
      delay[0][i]=1;
      delay[1][i]=1;
  }
  leftLevel = rightLevel = 1;
  gradient = QLinearGradient(rect().topLeft(), rect().bottomRight());
  gradientBrush = QBrush(gradient);
  gradientBrush.setStyle(Qt::SolidPattern);
  backgroundBrush.setColor(Qt::white);
  backgroundBrush.setStyle(Qt::SolidPattern);
//  transparentBrush.setColor(Qt::transparent);
  pen.setColor(qRgb(0,0,0));
  decayBrush.setColor(QColor(255,0,0,255));
  decayBrush.setStyle(Qt::SolidPattern);
//  barWidth = MIN_BARWIDTH;
  barWidth = 1;
  barSpacing = 1;
}

void Spectrograph::resizeEvent(QResizeEvent *e){
  e->accept();
  gradient = QLinearGradient(rect().topLeft(), rect().bottomRight());
  gradient.setColorAt(1, Qt::black);
  gradient.setColorAt(0, Qt::white);
  gradientBrush = QBrush(gradient);
  barWidth = (float)width()/NUM_BANDS;
  qDebug() << "barwidth = " << barWidth;
  widgetHeight = height();
  repaint();
}

void Spectrograph::loadLevels(double left, double right){
  if(leftLevel < 5*width()/2*left)
    leftLevel = 5*width()/2*left;
  if(rightLevel < 5*width()/2*right)
    rightLevel = 5*width()/2*right;
}

void Spectrograph::paintEvent(QPaintEvent *e){
  QPainter p(this);
  QPen pen;
  QVector<QPointF> vec;
  QPainterPath path;
  float p1x, p1y, p2x;
  e->accept();
  QEasingCurve curve;
//  gradient.setColorAt(0, Qt::black);
//  gradient.setColorAt(1, Qt::white);
//  p.setBrush(gradient);
  p.setBrush(gradientBrush);
  p.drawRect(rect());
 // gradient.setColorAt(0, Qt::white);
 // gradient.setColorAt(1, Qt::black);
  p.setRenderHint(QPainter::Antialiasing);
  pen.setStyle(Qt::SolidLine);
  pen.setColor(Qt::black);
  pen.setWidth(1);
  p.setPen(pen);/*
  curve.addTCBSegment(QPointF(0,altura-spectrum[0]),0,0,0);
  for(int i=1; i<NUM_BANDAS;i++){
    p1x = i*barWidth;
    p1y = altura-spectrum[i];
    p.setBrush(Qt::white);
//    p.setBrush(gradient);
    curve.addTCBSegment(QPointF(p1x,p1y),0,0,0);
    p1y = (float)altura -decay[i];
    p.setBrush(decayBrush);
    p.drawRect(QRectF(QPointF(p1x,p1y),QPointF(p2x,p1y+3)));
  }
  pen.setColor(Qt::white);
  pen.setWidth(2);
  p.setPen(pen);
  p.setBrush(Qt::NoBrush);
  vec=curve.toCubicSpline();
  qDebug() << vec[0].rx();
  p.drawLines(vec);*/
  for(int i=0; i<NUM_BANDS;i++){
    p1x = i*barWidth;
    p2x = p1x+barWidth;
    p1y = widgetHeight-spectrum[i];
    p.setBrush(Qt::white);
//    p.setBrush(gradient);
    p.drawRect(QRectF(QPointF(p1x,p1y),QPointF(p2x,widgetHeight)));
    p1y = (float)widgetHeight -decay[i];
    p.setBrush(decayBrush);
    p.drawEllipse(QPointF(p1x+barWidth/2,p1y),3,3);
    p.drawRect(QRectF(QPointF(p1x,p1y),QPointF(p2x,p1y+3)));
    }
  p.setBrush(Qt::black);
  p.drawRect(0,height()-7,width(),7);
  p.setBrush(Qt::red);
  p.drawRoundedRect(width()/2-leftLevel,height()-6,leftLevel,6,3,3);
  p.setBrush(Qt::blue);
  p.drawRoundedRect(width()/2,height()-6,rightLevel,6,3,3);
  p.setPen(pen);
 // p.setBrush(transparentBrush);
 // p.drawRect(rect());
}

void Spectrograph::timerEvent(QTimerEvent *e){
  // accepts the event (boring messages are not displayed)
  e->accept();
  // processes all spectrum bands
  for(int i=0; i<NUM_BANDS; i++){
    // make the spectrum bar smaller
    spectrum[i]-=delay[0][i];
    // but not with negative size
    if(spectrum[i] <0 ){
      spectrum[i]=0;
    }
    // waits for a while while gravity forces start to act
    // it simulates a small stop when the the bar reachs its
    // maximum
    //
    // however, this only work for the decay bars that stay
    // at the top of each spectrum bar
    //
    // gravity stop does not affect the spectrum bars
    if(gravity[i] < 0.8){
      // make decay bar lower
      decay[i]-=delay[1][i];
      // but not too lower
      if(decay[i] < 0){
        decay[i] = 0;
      }
      // increments decay bar reduction
      delay[1][i]++;
    }
    delay[0][i]++;
    // reduces gravity for that decay bar (not the spectrum bar)
    gravity[i] = gravity[i]*0.98;
  }
  // reduces left and right audio mean level
  // please note that this reduction is linear
  if(leftLevel > 0)
    leftLevel--;
  if(rightLevel > 0)
    rightLevel--;
  // repaint the screen
  repaint();
}

void Spectrograph::loadSamples(QVector<double> &_spectrum){
  int incr, value;
  incr = _spectrum.size()/NUM_BANDS;
  newSpectrum=_spectrum;
  for(int i=0; i<NUM_BANDS;i++){
    value = ceil(newSpectrum[i*incr]*height());
    if(value > spectrum[i]){
      spectrum[i] = value;
      delay[0][i] = 0;
      delay[1][i] = 0;
    }
    if(value > decay[i]){
      decay[i]    = value;
      gravity[i]  = 1;
      delay[1][i] = 0;
    }
  }
  repaint();
}
