#ifndef ABSTRACTCONTROL_H
#define ABSTRACTCONTROL_H

#include <QWidget>

// this is the abstract class to implement audio controls
/**
 * @brief The AbstractControl class is a base abstract class for implementing
 * audio controls.
 * @details It provides a set of functions that shall be overloaded, since they
 * will be called when media control events arrives from the mainwindow widget.
 * It also provides signals that should be emitted when a media control widget
 * receive user events, such as play, pause, volume changing and so on.
 */
class AbstractControl : public QWidget{
private:
  Q_OBJECT
public:
  /**
   * @brief AbstractControl is the class constructor. In fact, it does nothing, but
   * it is used to tell who is the parent of this widget.
   * @param parent is a pointer to the parent widget. Class must know who is it in order
   * the widget to be drawn correctly.
   */
  explicit AbstractControl(QWidget *parent): QWidget(parent){}

public slots:
  /**
   * @brief onPlayPauseClicked listens when mainwindow tells user pressed play/pause button
   */
  virtual void onPlayPauseClicked(void)=0;

  /**
   * @brief onPrevClicked listens when mainwindow tells user pressed prev button
   */
  virtual void onPrevClicked(void)=0;

  /**
   * @brief onNextClicked listens when mainwindow tells user pressed next button
   */
  virtual void onNextClicked(void)=0;

  /**
   * @brief onVolumeChanged is activated when someone changed the volume somewhere
   */
  virtual void onVolumeChanged(int)=0;

  // elapsed time has changed somewhere
  virtual void onElapsedChanged(qint64)=0;

  // song duration has changed;
  virtual void onDurationChanged(qint64)=0;

  // deals with play/pause player state
  virtual void onPlayerStateChanged(bool)=0;

signals:
  // tells when user pressed play/pause button
  void playPause();

  // tells when user pressed next button
  void next();

  // tells when user pressed prevp button
  void prev();

  // tells when user changed volume
  void volumeSelected(int);

  // tells when user change music position
  void elapsedSelected(qint64);
};

#endif // ABSTRACTCONTROL_H
