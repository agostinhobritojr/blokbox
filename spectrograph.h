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
#include <QAction>


// spectrograph class is used to display fourier spectrum
// bars
/**
 * @brief The Spectrograph class is used to draw the fourier spectrum on the screen.
 * @details It periodically receives an array containing the spectrum with values
 * scaled within the range [0,1], and the left and right mean audio values
 */
class Spectrograph : public AbstractSpectrograph{
  Q_OBJECT
public:
  /**
   * @bparent is the pointer to parent widget (it is supposed to be the place where this
   * widget has to be drawn)rief Class constructor. Setup initial behavior for the widget
   * @param
   *
   */
  explicit Spectrograph(QWidget *parent = 0);

signals:

public slots:
  /**
   * @brief Paints the screen for spectrum displaing
   * @param e stores information about the painting event
   */
  void paintEvent(QPaintEvent *e);

  /**
   * @brief This method decides what to do when a new spectrum sample
   * has arrived.
   * @detailed This method is called periodically by mainwindow every time a
   * new spectrum is calculated. the _spectrum array reference stores double values
   * within the range [0,1] and it has 256 elements.
   * @param _spectrum stores the spectrum.
   */
  void loadSamples(QVector<double> &_spectrum);

  /**
   * @brief Used to modify spectrum while a new sample does not arrive
   * @param e
   */
  void timerEvent(QTimerEvent *e);

  /**
   * @brief What to do when widget size changes
   * @details It is used to recalculate the width of the bars and
   * some gradient colors
   * @param e stores information about resize event
   */
  void resizeEvent(QResizeEvent *e);

  /**
   * @brief Open context menu
   * @details This is what is called when user clicks on the
   * component using mouse right button
   * @param e This event is used to return the position of mouse click
   * when the context menu is activated
   */
 // void contextMenuEvent(QContextMenuEvent *e);

  /**
   * @brief Loads mean left and right audio channel values
   * @details This method is invoked by mainwindow component
   * every time a new audio sample is processed, supplying left
   * and right mean audio values
   *
   * It also performs some scaling for better display purposes

   * @param left left mean audio value
   * @param right right mean audio value
   */
  void loadLevels(double left, double right);

  /**
   * @brief Do some example action when user activates context menu
   * @details You can add new of such functions to allow new context
   * menu entries
   */
//  void doAction();
private:
  /**
   * @brief Stores the current fft spectrum.
   * @details spectrum is an array that should have a MAXIMUM of 256 entries.
   * You should not trespass this limit
   */
  QVector<int> spectrum;
  /**
   * @brief newSpectrum stores the new fft spectrum that arrives to this object
   * @details since we are doing spectrum simulation just like a graphical equalizer
   * the new spectrum values are displayed only when the are larger than the previous
   * ones
   */
  QVector<double> newSpectrum;

  /**
   * @brief gravity decides when each decay bar has to start to decay
   * @details This array simulates for each decay bar a small "freeze" to make it stopped
   * when a spectrum bar reaches a new maximum
   */
  QVector<double> gravity;
  /**
   * @brief decay stores the position of the decay bars at the top of a spectrum bar
   */
  QVector<int> decay;

  /**
   * @brief Stores the delay for decay bars and spectrum bars to simulate gravity
   * @details delay[0] stores how much have to be subtracted from each spectrum bar value
   * to display a given spectrum bar
   * @details delay[1] stores how much have to be subtracted from each decay bar value
   * to display a given decay bar
   */
  QVector<int> delay[2];

  /**
   * @brief Left and right level bar size (mean audio level)
   */
  int leftLevel, rightLevel;
  /**
   * @brief Number of spectrum bands (MAX=256!!!)
   */
  int NUM_BANDS;
  /**
   * @brief Holds the gradient for drawing the spectrum bars
   */
  QLinearGradient gradient;
  /**
   * @brief Brushes for drawing the gradient and background patterns
   */
  QBrush gradientBrush, backgroundBrush, decayBrush;
  /**
   * @brief A pen for drawing lines
   */
  QPen pen;
  /**
   * @brief barSpacing defines the space between spectrum bars. Usually, just one pixel
   */
  float barSpacing;
  /**
   * @brief barWidth stores spectrum bar width
   */
  float barWidth;
  /**
   * @brief widgetHeight stores the widget height
   */
  float widgetHeight;
};

#endif // SPECTROGRAM_H
