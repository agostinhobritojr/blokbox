#ifndef MEDIALIBRARY_H
#define MEDIALIBRARY_H

//#define USE_TAGLIB

#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QStack>
#include <QDir>
#include <QMediaPlayer>
#include <QString>
#include <QSet>

#ifdef USE_TAGLIB
#include <taglib/fileref.h>
#include <taglib/tag.h>
#endif
namespace Ui {
class MediaLibrary;
}

class MediaLibrary : public QWidget
{
  Q_OBJECT

public:
  explicit MediaLibrary(QWidget *parent = 0);
  ~MediaLibrary();
  void addToSearchPath(QString path);
  /**
   * @brief getSearchPath returns a QSet containing QStrings with song paths
   * @return the set of paths
   */
  QSet<QString> getSearchPath(void);
private:

#ifdef USE_TAGLIB
  TagLib::FileRef fileref;
  TagLib::Tag *tag;
#endif

  Ui::MediaLibrary *ui;
  QMediaPlayer *auxplayer;
  /**
   * @brief searchPath stores the search path for seeking songs
   */
  QSet<QString> searchPath;
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
