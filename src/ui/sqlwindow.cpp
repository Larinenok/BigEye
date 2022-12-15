#include "ui/sqlwindow.h"
#include "ui_sqlwindow.h"
#include <iostream>

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

void SqlWindow::on_database_activated(QString str)
{
    std::cout << str.toStdString() << std::endl;
}
