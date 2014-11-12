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
  startTimer(15);
  NUM_BANDAS = 32;
  spectrum.resize(NUM_BANDAS);
  decay.resize(NUM_BANDAS);
  gravity.resize(NUM_BANDAS);
  delay[0].resize(NUM_BANDAS);
  delay[1].resize(NUM_BANDAS);
  for(int i=0; i<NUM_BANDAS; i++){
      decay[i]=0;
      gravity[i]=1;
      spectrum[i]=1;
      delay[0][i]=1;
      delay[1][i]=1;
  }
  leftLevel = rightLevel = 1;
  gradient = QLinearGradient(rect().topLeft(), rect().bottomRight());
  gradient.setColorAt(1, Qt::black);
  gradient.setColorAt(0, Qt::black);
  gradientBrush = QBrush(gradient);
  gradientBrush.setStyle(Qt::SolidPattern);
  backgroundBrush.setColor(Qt::white);
  backgroundBrush.setStyle(Qt::SolidPattern);
  transparentBrush.setColor(Qt::transparent);
  pen.setColor(qRgb(0,0,0));
  decayBrush.setColor(QColor(255,0,0,255));
  decayBrush.setStyle(Qt::SolidPattern);
  barWidth = MIN_BARWIDTH;
  barSpacing = 1;
}

void Spectrograph::resizeEvent(QResizeEvent *e){
  e->accept();
  gradient = QLinearGradient(rect().topLeft(), rect().bottomRight());
  gradientBrush = QBrush(gradient);
  NUM_BANDAS = width()/barWidth;
  if(NUM_BANDAS > BAND_MAX){
    NUM_BANDAS = BAND_MAX;
    barWidth = (float)width()/NUM_BANDAS;
  }
  else{
    barWidth = MIN_BARWIDTH;
  }
  spectrum.resize(NUM_BANDAS);
  decay.resize(NUM_BANDAS);
  gravity.resize(NUM_BANDAS);
  delay[0].resize(NUM_BANDAS);
  delay[1].resize(NUM_BANDAS);
  for(int i=0; i<NUM_BANDAS; i++){
      decay[i]=0;
      gravity[i]=1;
      spectrum[i]=1;
      delay[0][i]=1;
      delay[1][i]=1;
  }
  altura = height();
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
  p.setBrush(Qt::black);
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
  for(int i=0; i<NUM_BANDAS;i++){
    p1x = i*barWidth;
    p2x = p1x+barWidth;
    p1y = altura-spectrum[i];
    p.setBrush(Qt::white);
//    p.setBrush(gradient);
    p.drawRect(QRectF(QPointF(p1x,p1y),QPointF(p2x,altura)));
    p1y = (float)altura -decay[i];
    p.setBrush(decayBrush);
    p.drawRect(QRectF(QPointF(p1x,p1y),QPointF(p2x,p1y+3)));
    }
  p.setBrush(Qt::black);
  p.drawRect(0,height()-7,width(),7);
  p.setBrush(Qt::red);
  p.drawRoundedRect(width()/2-leftLevel,height()-6,leftLevel,6,3,3);
  p.setBrush(Qt::blue);
  p.drawRoundedRect(width()/2,height()-6,rightLevel,6,3,3);
  p.setPen(pen);
  p.setBrush(transparentBrush);
  p.drawRect(rect());
}

void Spectrograph::timerEvent(QTimerEvent *e){
  e->accept();
  for(int i=0; i<NUM_BANDAS; i++){
    spectrum[i]-=delay[0][i];
    if(spectrum[i] <0 ){
      spectrum[i]=0;
    }
    if(gravity[i] < 0.8){
      decay[i]-=delay[1][i];
      if(decay[i] < 0){
        decay[i] = 0;
      }
      delay[1][i]++;
    }
    delay[0][i]++;
    gravity[i] = gravity[i]*0.98;
  }
  if(leftLevel > 0)
    leftLevel--;
  if(rightLevel > 0)
    rightLevel--;
  repaint();
}

void Spectrograph::loadSamples(QVector<double> &_spectrum){
  int incr, value;
  incr = _spectrum.size()/NUM_BANDAS;
  newSpectrum=_spectrum;
  for(int i=0; i<NUM_BANDAS;i++){
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
