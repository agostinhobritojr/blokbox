#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <climits>
#include <QAudioBuffer>
#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <QAudioProbe>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QHeaderView>
#include <QImage>
#include <QIODevice>
#include <QMainWindow>
#include <QMediaMetaData>
#include <QMediaPlaylist>
#include <QMessageBox>
#include <QMimeDatabase>
#include <QSettings>
#include <QStandardItemModel>
#include <QString>
#include <QtMultimedia>
#include <QtMultimedia/QMediaPlayer>
#include <QUrl>
#include <QVector>

// mainwindow uses a fft calculator and a playlist model for displaying the playlist
#include "fftcalc.h"
#include "playlistmodel.h"
/** @mainpage Blokbox: a modular music player
  */
/**
 * @brief This is the namespace that defines the MainWindow1 ui widget.
 * This is not the MainWindow class that is defined in this file
 */
namespace Ui {
class MainWindow;
}

// the MainWindow class
/**
 * @brief The MainWindow class is the conductor of this player.
 * @details The MainWindow class is used to manage with all stuff
 * fft, audio probing, widget properties.
 */
class MainWindow : public QMainWindow{
  Q_OBJECT

public:
  /**
   * @brief MainWindow is the class constructor. It prepares the
   * whole player for full operation
   * @param parent
   */
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

public slots:
  // what to do when user select a new song to play
  /**
   * @brief goToItem tells the player to play the selected song on the playlist
   * @param index stores the index of the song
   */
  void goToItem(const QModelIndex &index);

  /**
   * @brief loadMedia allows the user to load a new file for playing
   */
  void loadMedia();
  /**
   * @brief loadPlaylist should prepare the playlist
   * @details nothing here yet
   */
  void loadPlaylist();
  /**
   * @brief mediaStatusChanged is activated when the media on player changes
   * @details It is used to communicate the song duration has changed
   * @param status is not used here
   */
  void mediaStatusChanged(QMediaPlayer::MediaStatus status);
  /**
   * @brief mediaStateChanged is activated when media player status changes
   * @details It deals with player's playback status
   * @param state receives current state. Possible values are:
   *  -QMediaPlayer::StoppedState
   *  -QMediaPlayer::PlayingState
   * - QMediaPlayer::PausedState
   */
  void mediaStateChanged(QMediaPlayer::State state);
  /**
   * @brief metaDataChanged is activated every time a given metadata changes
   * on player
   * @details Metadata are used to store on song files information such as
   * artist, song title, album title, year etc.
   */
  void metaDataChanged();
  /**
   * @brief next should be activated if it wants to play previous media on sequence
   */
  void next();
  /**
   * @brief onAddFolderToLibrary allows the user to insert a whole folder
   * on its audio library.
   * @details A library component is specially crafted to deal with library
   * manipulation details
   */
  void onAddFolderToLibrary();
  /**
   * @brief onAddMediaToPlayList allows the user to insert a new file on the library
   * @param media is the path to the file the user wants to insert
   */
  void onAddMediaToPlayList(QString media);

  /**
   * @brief playPause should be activated if it wants to play or pause the music
   */
  void playPause();
  /**
   * @brief prev should be activated if it wants to play next media on sequence
   */
  void prev();
  /**
   * @brief processBuffer is responsible to deliver the current audio buffer to
   * a calculator to perform fft calculations
   * @param buffer stores the audio buffer that is sent by the player
   */
  void processBuffer(QAudioBuffer buffer);
  /**
   * @brief setMediaAt alows to forward/rewind the song
   * @details The user may have multiple controls ant this will help to keep controls coherent
   * @param percent tells the percentage of stream it wants to play
   */
  void setMediaAt(qint32 percent);
  /**
   * @brief setVolume should
   * @param volume
   */
  void setVolume(int volume);

  /**
   * @brief spectrumAvailable is emitted every time a new spectrum is ready
   * to be displayed
   * @param spectrum is an array of doubles containing the Fourier spectrum
   */
  void spectrumAvailable(QVector<double> spectrum);

private:
  /**
   * @brief ui is a pointer to the user interface widget (not this mainwindow class!!!)
   */
  Ui::MainWindow *ui;

  /**
   * @brief player is a pointer to the media player
   */
  QMediaPlayer *player;

  /**
   * @brief playlist stores the playlist
   */
  QMediaPlaylist *playlist;

  // audio info... we do not use it
  // QAudioDeviceInfo audioInfo;

  /**
   * @brief probe is the audio prober responsible for listening to the music
   * and retrieve audio buffers
   */
  QAudioProbe *probe;

  // a fft calculator object
  /**
   * @brief calculator is a Fourier spectrum calculator.
   * @details The calculator "object" receives audio samples, perform fft calculations
   * and tells Fourier spectrums when they are ready for displaying
   */
  FFTCalc *calculator;

  /**
   * @brief model stores the playlist model
   * @details We use a model/view approach to store the playlist elements. This approach
   * allows listview elements access the playlist structure and interpret it as kind of tree
   * with branches of artists... and disks... and songs.
   */
  QStandardItemModel *model;

  /**
   * @brief sample stores an audio sample to be sent for fft calculator
   */
  QVector<double> sample;

  /**
   * @brief spectrum stores the output Fourier spectrum
   */
  QVector<double> spectrum;

  /**
   * @brief playlistModel stores the playlist model for insertions and display purposes...
   */
  PlaylistModel *playlistModel;

  /**
   * @brief levelLeft and rightlevel stores audio mean left and right levels
   */
  double levelLeft, levelRight;
signals:
  /**
   * @brief spectrumChanged is emitted when a new spectrum is
   * available for display purposes
   * @param sample stores the array of doubles with the Fourier spectrum
   */
  int spectrumChanged(QVector<double> &sample);

  /**
   * @brief positionChanged is emitted when media playing position changes
   * @param position is a value on range [0,total duration of song]
   */
  int positionChanged(qint64 position);

  /**
   * @brief levels tells the new left and right mean audio levels
   * @param left stores left mean audio level
   * @param right stores right mean audio level
   */
  int levels(double left, double right);

  /**
   * @brief elapsedTimeChanged tells the duration of media
   * when a new media arrives on media player
   * @param elapsed is the amount of time elapsed from the beginning
   * of the song
   */
  int elapsedTimeChanged(qint64 elapsed);

   /**
   * @brief addFolderToLibrary tells there are new directories
   * to be added to the music library
   * @details We do not threat this signal yet. It is here
   * just to remember include library customization in the future
   * @param folder stores string folder
   */
  int addFolderToLibrary(QString folder);

  /**
   * @brief playPauseChanged emits the state of media player.
   * @details It may be used to update a play/pause button somewhere
   * since the user may decide to user different icons for different
   * states.
   * - true: Player is on playing state
   * - false: Player is on paused state
   * @return
   */
  int playPauseChanged(bool);

protected slots:
};


#endif // MAINWINDOW_H
