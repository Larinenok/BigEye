#include "ui/sqlwindow.h"
#include "ui_sqlwindow.h"

SqlWindow::SqlWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SqlWindow)
{
    ui->setupUi(this);
}

SqlWindow::~SqlWindow()
{
    delete ui;
}

void SqlWindow::closeEvent(QCloseEvent *e)
{
    this->isClosed = true;
}

void SqlWindow::on_pushButton_clicked()
{
    this->database = this->ui->database->currentText().toStdString();
    this->address = this->ui->address->text().toStdString();
    this->user = this->ui->user->text().toStdString();
    this->password = this->ui->password->text().toStdString();
    this->name = this->ui->name->text().toStdString();

    this->close();
}
