#ifndef SPECTROGRAPH_H
#define SPECTROGRAPH_H
#include "abstractspectrograph.h"

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QAudioBuffer>
#include <complex>
#include <QVector>
#include <QDebug>
#include <QTimer>
#include <QGradient>

#define BAND_MAX 256
#define MIN_BARWIDTH 30

using namespace std;

class Spectrograph : public AbstractSpectrograph{
    Q_OBJECT
public:
    explicit Spectrograph(QWidget *parent = 0);
    
signals:
    
public slots:
    void paintEvent(QPaintEvent *e);
    void loadSamples(QVector<double> &_spectrum);
    void timerEvent(QTimerEvent *e);
    void resizeEvent(QResizeEvent *e);
    void loadLevels(double left, double right);
private:
    QVector<int> spectrum, delay[2];
    QVector<int> decay;
    QVector<double> gravity;
    QVector<double> newSpectrum;

    int leftLevel, rightLevel;
    int decayIncr;
    int NUM_BANDAS;
    QLinearGradient gradient;
    QBrush gradientBrush, backgroundBrush,
    transparentBrush, decayBrush;
    QPen pen;
    float barSpacing, barWidth, altura;
    double max;
    double time;
};

#endif // SPECTROGRAM_H
