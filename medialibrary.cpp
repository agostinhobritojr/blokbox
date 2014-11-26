#include "medialibrary.h"
#include "ui_medialibrary.h"

MediaLibrary::MediaLibrary(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MediaLibrary)
{
  ui->setupUi(this);
  treeModel = new QStandardItemModel;
  ui->treeViewLibrary->setModel(treeModel);
  ui->treeViewLibrary->setEditTriggers(QAbstractItemView::NoEditTriggers);
  connect(ui->treeViewLibrary,SIGNAL(clicked(QModelIndex)),
          SLOT(onSelectItem(QModelIndex)));
  connect(ui->treeViewLibrary,SIGNAL(doubleClicked(QModelIndex)),
          SLOT(onDoubleClickItem(QModelIndex)));
  QStringList header;
  header << "Data" << "Path";
  treeModel->setHorizontalHeaderLabels(header);
  ui->treeViewLibrary->sortByColumn(0);
  ui->treeViewLibrary->setSortingEnabled(true);

  // prepares the tableView for viewing tags
  tableModel= new QStandardItemModel(1,2);
  tableModel->setHorizontalHeaderItem(0,new QStandardItem(QString("Propriedade")));
  tableModel->setHorizontalHeaderItem(1,new QStandardItem(QString("Valor")));
  ui->tableViewPlaylist ->verticalHeader()->setVisible(false);
  ui->tableViewPlaylist->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
  ui->tableViewPlaylist->horizontalHeader()->setStretchLastSection(true);
  ui->tableViewPlaylist->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->tableViewPlaylist->setAlternatingRowColors(true);
  ui->tableViewPlaylist->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->tableViewPlaylist->setModel(tableModel);
  ui->tableViewPlaylist->setFont(QFont("Times"));

}

void MediaLibrary::resizeEvent(QResizeEvent *e){
  Q_UNUSED(e);
  ui->treeViewLibrary->setColumnWidth(0,width()/2);
}

MediaLibrary::~MediaLibrary()
{
  delete ui;
}

void MediaLibrary::onDoubleClickItem(QModelIndex index){
  if(!index.model()->hasChildren(index)){
    emit(addMediaToPlayList(index.parent().child(index.row(),1).data().toString()));
  }
}

void MediaLibrary::onSelectItem(QModelIndex index){
  QString path, artist, album, title;
  if(index.model()->hasChildren(index))
    return;
  path = index.parent().child(index.row(),1).data().toString();
#ifdef USE_TAGLIB
  TagLib::FileRef ref(path.toStdString().c_str());
  if(!ref.isNull() && ref.tag()){
    tag=ref.tag();
    if(tag!=NULL){
      artist = tag->artist().toCString();
      if(artist == "")
        artist = "Desconhecido";
      album = tag->album().toCString();
      if(album == "")
        album = "Desconhecido";
      title = tag->title().toCString();
      if(title == "")
        title = "Desconhecido";
      setTableTag(QString("Artista"), artist);
      setTableTag("Album", album);
      setTableTag("Titulo",title);
    }
  }
#endif
}

void MediaLibrary::setTableTag(QString property, QString value){
  QList <QStandardItem*> list;
  QStandardItem *item;
  list = tableModel->findItems(property);
  if(!list.empty()){
    int row;
    row = list.at(0)->row();
    tableModel->item(row,0)->setText(property);
    tableModel->item(row,1)->setText(value);
  }
  else{
    int count;
    count = tableModel->rowCount();
    count++;
    item = new QStandardItem(property);
    tableModel->setItem(count,0,item);
    item = new QStandardItem(value);
    tableModel->setItem(count,1,item);
    tableModel->sortRole();
    tableModel->sort(0);
  }
}

QStandardItem *MediaLibrary::searchArtist(QString artist){
  QList<QStandardItem*> found;
  found = treeModel->findItems(artist,0);
  if(found.size() == 0)
    return NULL;
  else
    return found.at(0);
/*
  for(int i=0; i<model->rowCount(); i++){
    if(model->item(i,0)->text() == artist)
      return model->item(i);
  }
  return NULL;*/
}

QStandardItem* MediaLibrary::searchAlbum(QStandardItem *artistItem, QString album){
  for(int i=0; i<artistItem->rowCount(); i++){
    if(artistItem->child(i)->text() == album)
      return artistItem->child(i);
  }
  return NULL;
}

QStandardItem* MediaLibrary::searchSong(QStandardItem *albumItem, QString song){
  for(int i=0; i<albumItem->rowCount(); i++){
    if(albumItem->child(i)->text() == song)
      return albumItem->child(i);
  }
  return NULL;
}

void MediaLibrary::addToSearchPath(QString path){
  QDir dir;
  // check if the path is already on the set
  if(searchPath.contains(path)){
    // if yes, return
    return;
  }
  // insert path on searchPath
  searchPath.insert(path);
  dir.setPath(path);
  QStack<QString> stack;
  QString fullPath, artist, album, title;
  QStandardItem *artistItem, *albumItem, *songItem, *urlItem, *emptyItem;
  QList<QStandardItem*> list;
  stack.push(dir.absolutePath());
  while (!stack.isEmpty()) {
    QString sSubdir = stack.pop();
    QDir subdir(sSubdir);

#ifdef USE_TAGLIB
    // Check for the files.
    QStringList entries = subdir.entryList(QStringList() << "*mp3" << "*flac", QDir::Files);
    for (int i = 0; i < entries.size(); i++) {
      fullPath = (subdir.path()+"/"+entries[i]);
      TagLib::FileRef ref(fullPath.toStdString().c_str());
      if(!ref.isNull() && ref.tag()){
        tag=ref.tag();
        if(tag!=NULL){
          artist = tag->artist().toCString();
          if(artist == "")
            artist = "Desconhecido";
          album = tag->album().toCString();
          if(album == "")
            album = "Desconhecido";
          title = tag->title().toCString();
          if(title == "")
            title = "Desconhecido";

          // find the artist
          artistItem = searchArtist(artist);
          // if it does not exist, create it
          if(artistItem==NULL){
            artistItem = new QStandardItem(artist);
            emptyItem = new QStandardItem;
            list.clear();
            list << artistItem << emptyItem;
            treeModel->appendRow(list);
          }

          // search for the album
          albumItem = searchAlbum(artistItem, album);
          // if it does not exist, create it
          if(albumItem==NULL){
            albumItem = new QStandardItem(album);
            artistItem->appendRow(albumItem);
          }

          // search for the song
          songItem = searchSong(albumItem, title);
          // if it does not exist, create it
          if(songItem==NULL){
            list.clear();
            songItem = new QStandardItem(title);
//            albumItem->appendRow(songItem);
            urlItem = new QStandardItem(fullPath);
            list << songItem << urlItem;
            albumItem->appendRow(list);
          }
        }
      }
    }
#endif
    QFileInfoList infoEntries = subdir.entryInfoList(QStringList(),
                                                     QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    for (int i = 0; i < infoEntries.size(); i++) {
      QFileInfo& item = infoEntries[i];
      stack.push(item.absoluteFilePath());
    }
  }
  ui->treeViewLibrary->sortByColumn(0);
}
