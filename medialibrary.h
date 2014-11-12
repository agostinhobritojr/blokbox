#ifndef MEDIALIBRARY_H
#define MEDIALIBRARY_H

#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QStack>
#include <QDir>
#include <QMediaPlayer>
#include <QString>
#include <taglib/fileref.h>
#include <taglib/tag.h>
namespace Ui {
class MediaLibrary;
}

class MediaLibrary : public QWidget
{
  Q_OBJECT

public:
  explicit MediaLibrary(QWidget *parent = 0);
  ~MediaLibrary();
  void searchPath(QString path);
private:
  Ui::MediaLibrary *ui;
  QMediaPlayer *auxplayer;
  TagLib::FileRef fileref;
  TagLib::Tag *tag;
  QStandardItemModel *treeModel, *tableModel;
  QStandardItem* searchArtist(QString artist);
  QStandardItem *searchAlbum(QStandardItem *artistItem, QString album);
  QStandardItem *searchSong(QStandardItem *albumItem, QString song);

protected:
  void resizeEvent(QResizeEvent *e);
  void setTableTag(QString property, QString value);

private slots:
  void onSelectItem(QModelIndex index);
  void onDoubleClickItem(QModelIndex index);

signals:
  void addMediaToPlayList(QString);
};

#endif // PLAYLIST_H
