#ifndef INTERFACECREATORDIALOG_H
#define INTERFACECREATORDIALOG_H

#include <QDialog>

namespace Ui {
class InterfaceCreatorDialog;
}

class InterfaceCreatorDialog : public QDialog
{
  Q_OBJECT

public:
  explicit InterfaceCreatorDialog(QWidget *parent = 0);
  ~InterfaceCreatorDialog();

private:
  Ui::InterfaceCreatorDialog *ui;
};

#endif // INTERFACECREATORDIALOG_H
