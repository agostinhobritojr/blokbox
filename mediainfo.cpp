#include "mediainfo.h"
#include "ui_mediainfo.h"
#include <QDebug>
MediaInfo::MediaInfo(QWidget *parent) :
  AbstractMediaInfo(parent),
  ui(new Ui::MediaInfo){
  ui->setupUi(this);

}

MediaInfo::~MediaInfo(){
  delete ui;
}

void MediaInfo::setAtribute(QString property, QString value){
  if(property == "AlbumArtist"){
    ui->labelAlbumArtist->setText(value);
    ui->widgetLightCycle->setText(value);
  }
  else if(property == "Title")
      ui->labelTitle->setText(value);
  else if(property == "AlbumTitle")
    ui->labelAlbumTitle->setText(value);
  else if(property == "TrackNumber")
    ui->labelTrackNumber->setText(value);
  else if(property == "AudioBitRate")
    ui->labelAudioBitRate->setText(value);
  else if(property == "AudioCodec")
    ui->labelAudioCodec->setText(value);
}

