#ifndef LIGHTCYCLE_H
#define LIGHTCYCLE_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QImage>
#include <QVector>

#define SEGMENT_HEIGHT 11
class LightCycle : public QWidget
{
  Q_OBJECT
private:
  QPixmap pixmap;
  QImage image;
  QPen pen;
  QBrush brush;
  QVector< QVector<bool> > matrix;
  QVector<bool> column, tmp;
  int lightDiameter;
  int ncols;
  int startColumn;
public:
  explicit LightCycle(QWidget *parent = 0);

  void paintEvent(QPaintEvent *e);
  void setText(QString text);

signals:

public slots:

protected:
  void timerEvent(QTimerEvent *e);
  void resizeEvent(QResizeEvent *e);
};

#endif // LIGHTCYCLE_H
