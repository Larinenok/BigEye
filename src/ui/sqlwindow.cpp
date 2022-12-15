#include "sqlwindow.h"
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
