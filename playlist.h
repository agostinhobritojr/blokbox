#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QWidget>
#include <QMediaPlaylist>
#include "playlistmodel.h"

namespace Ui {
class Playlist;
}

class Playlist : public QWidget
{
  Q_OBJECT

public:
  explicit Playlist(QWidget *parent = 0);
  ~Playlist();
  void setPlaylist(QMediaPlaylist *playlist);
private:
  Ui::Playlist *ui;
  PlaylistModel *model;
  QMediaPlaylist *m_playlist;
};

#endif // PLAYLIST_H
