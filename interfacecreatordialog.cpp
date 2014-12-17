#include "interfacecreatordialog.h"
#include "ui_interfacecreatordialog.h"

InterfaceCreatorDialog::InterfaceCreatorDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::InterfaceCreatorDialog)
{
  ui->setupUi(this);
}

InterfaceCreatorDialog::~InterfaceCreatorDialog()
{
  delete ui;
}
