#include "interfacedesigner.h"
#include <QMenu>
#include <QContextMenuEvent>
InterfaceDesigner::InterfaceDesigner(QWidget *parent) :
  QWidget(parent){
  // split action text
  split = new QAction("split",this);

  // colapse action text
  collapse = new QAction("colapse",this);

  // call collapseWindow when this window has to be collapsed
  connect(collapse, SIGNAL(triggered()),SLOT(colapseWindow()));
  connect(split,SIGNAL(triggered()),SLOT(splitWindow()));
}

void InterfaceDesigner::colapseWindow(){
  emit colapseMe();
}

void InterfaceDesigner::splitWindow(){
  QVBoxLayout *layout;
  layout = new QVBoxLayout;
  splitter = new QSplitter(this);
  splitter->addWidget(new InterfaceDesigner(this));
  splitter->addWidget(new InterfaceDesigner(this));
  layout->addWidget(splitter);
  setLayout(layout);
}

void InterfaceDesigner::contextMenuEvent(QContextMenuEvent *e){
  QMenu menu;
  menu.addAction(split);
  menu.addAction(collapse);
  menu.exec(e->globalPos());
}
