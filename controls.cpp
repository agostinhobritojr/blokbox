#include "controls.h"
#include "ui_controls.h"
#include <QMouseEvent>
#include <QDebug>

Controls::Controls(QWidget *parent):
  AbstractControl(parent), ui(new Ui::Controls){
  Q_UNUSED(parent);
  ui->setupUi(this);
  ui->pushButtonPlayPause->setFocusPolicy(Qt::NoFocus);
  ui->pushButtonNext->setFocusPolicy(Qt::NoFocus);
  ui->pushButtonPrev->setFocusPolicy(Qt::NoFocus);
  ui->verticalSliderVolume->setFocusPolicy(Qt::NoFocus);
  ui->lcdNumberElapsed->setStyleSheet(QString("QLCDNumber{color:red;}"));
  ui->lcdNumberDuration->setStyleSheet(QString("QLCDNumber{color:red;}"));
  sliderPressed = false;
  connect(ui->horizontalSliderPosition,SIGNAL(sliderReleased()),
          this,SLOT(sliderReleased()));
  duration=1;
}

Controls::~Controls(){
  delete ui;
}

void Controls::sliderReleased(){
  emit elapsedSelected(ui->horizontalSliderPosition->value()*duration/100);
}

void Controls::myElapsedChanged(int percent){
  if(ui->horizontalSliderPosition->isSliderDown())
    emit elapsedSelected(percent*duration/100);
}

void Controls::onPlayPauseClicked(){
  emit playPause();
}

void Controls::onPrevClicked(){
  emit prev();
}

void Controls::onNextClicked(){
  emit next();
}

void Controls::onVolumeChanged(int value){
  if(!ui->verticalSliderVolume->isSliderDown()){
    ui->verticalSliderVolume->setValue(value);
  }
  else{
    emit volumeSelected(value);
  }
}

void Controls::onDurationChanged(qint64 value){
  duration = value;
  ui->lcdNumberDuration->display(QTime(0,0).addMSecs(value).toString(QString("hh:mm:ss")));
}

void Controls::onElapsedChanged(qint64 value){
  ui->lcdNumberElapsed->display(QTime(0,0).addMSecs(value).toString(QString("hh:mm:ss")));
  if(!ui->horizontalSliderPosition->isSliderDown()){
    ui->horizontalSliderPosition->setValue(100*value/duration);
  }
}

