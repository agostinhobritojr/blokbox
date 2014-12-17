#ifndef INTERFACEDESIGNER_H
#define INTERFACEDESIGNER_H

#include <QWidget>
#include <QAction>
#include <QSplitter>
#include <QLayout>

class InterfaceDesigner : public QWidget
{
  Q_OBJECT
private:
  QAction *split, *collapse;
  QSplitter *splitter;
  InterfaceDesigner *first, *second;
public:
  explicit InterfaceDesigner(QWidget *parent = 0);

signals:
  void colapseMe(void);
public slots:

protected slots:
  void splitWindow();
  void colapseWindow();
  void contextMenuEvent(QContextMenuEvent *e);
};

#endif // INTERFACEDESIGNER_H
