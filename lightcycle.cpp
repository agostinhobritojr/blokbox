#include "lightcycle.h"

LightCycle::LightCycle(QWidget *parent) :
  QWidget(parent)
{
  pixmap = QPixmap(800,SEGMENT_HEIGHT);
  image = pixmap.toImage();
  column.resize(SEGMENT_HEIGHT);
  startTimer(80);
  startColumn=0;
  setText("----");
  ncols=1;
}

void LightCycle::setText(QString text){
  QColor color;
  QPainter p;
  int textWidthinPixels;
  if(text.size() == 0)
    text = QString("Desconhecido");
  QFont font;
  font.setStyleHint(QFont::Monospace);
  font.setKerning(false);
  font.setPixelSize(SEGMENT_HEIGHT);
  QFontMetrics fm(font);
  textWidthinPixels = fm.width(text);
  pixmap = QPixmap(textWidthinPixels,SEGMENT_HEIGHT);
  qDebug() << pixmap.rect();
  p.begin(&pixmap);
  font.setStyleStrategy(QFont::NoAntialias);
  p.setFont(font);
  pen.setStyle(Qt::NoPen);
  p.setPen(pen);
  p.setBrush(Qt::white);
  p.drawRect(pixmap.rect());
  p.setBrush(Qt::black);
  pen.setStyle(Qt::SolidLine);
  pen.setColor(Qt::black);
  p.setPen(pen);
  p.drawText(pixmap.rect(),Qt::AlignVCenter|Qt::AlignLeft,text);
  p.end();
  image = pixmap.toImage();
  matrix.clear();
  qDebug() << image.size();
  for(int i=0; i<image.width(); i++){
    for(int j=0; j<SEGMENT_HEIGHT; j++){
      color = image.pixel(i,j);
      if(color.value()==0)
        column[j] = true;
      else
        column[j] = false;
    }
    matrix.push_back(column);
  }
  repaint();
}
void LightCycle::resizeEvent(QResizeEvent *e){
  Q_UNUSED(e);
  lightDiameter = height()/SEGMENT_HEIGHT;
  if(lightDiameter <= 1)
    lightDiameter = 2;
  ncols = width()/lightDiameter;
  qDebug() << ncols;
  if(ncols > image.width()){
    for(int i=0; i<SEGMENT_HEIGHT; i++){
      column[i] = false;
    }
    for(int i=0; i<ncols-matrix.size(); i++){
      matrix.push_back(column);
    }
  }
  else{
    matrix.resize(image.width());
  }
  qDebug() << ncols << "/" << matrix.size();
}

void LightCycle::timerEvent(QTimerEvent *e){
  Q_UNUSED(e);
  /*
  QRect rect;
  QVector<QRgb> column(SEGMENT_HEIGHT);
  for(int i=0; i<SEGMENT_HEIGHT; i++){
    column[i] = image.pixel(0,i);
  }
  pixmap.scroll(-1,0,pixmap.rect());
  image=pixmap.toImage();
  for(int i=0; i<SEGMENT_HEIGHT; i++){
    image.setPixel(pixmap.width()-1,i,column[i]);
  }
  pixmap = pixmap.fromImage(image);*/
  startColumn++;
  if(startColumn > matrix.size())
    startColumn=0;
  repaint();
}

void LightCycle::paintEvent(QPaintEvent *e){
  Q_UNUSED(e);
  int index;
  QColor color;
  QPainter p(this);
  p.setBrush(Qt::white);
  p.setPen(Qt::NoPen);
  p.setRenderHint(QPainter::Antialiasing);
  p.setPen(Qt::white);
  p.drawRect(rect());
  for(int i=0; i<ncols; i++){
    index = (i+startColumn)%matrix.size();
   // qDebug() << "index=" << index << "/" << matrix.size();
    for(int j=0; j<SEGMENT_HEIGHT; j++){
      if(matrix[index][j] == true){
        p.setBrush(Qt::red);
        p.drawEllipse(lightDiameter*i,lightDiameter*j,lightDiameter,lightDiameter);
      }
      else{
        p.setBrush(Qt::gray);
        p.drawEllipse(lightDiameter*i,lightDiameter*j,lightDiameter,lightDiameter);
      }
    }
  }
}
