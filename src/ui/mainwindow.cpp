#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "journalitem.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->journalRoot.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->journalScroll->setWidget(&this->journalRoot);

    for (int i = 0; i < 10; i++)
        this->addNewJournalItem("00:00", "camera 1", "old", "123456");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addNewJournalItem(std::string time, std::string camera, std::string status, std::string id)
{
    this->journalList.push_back(new JournalItem(time, camera, status, id));
    this->journalLayout.addWidget(this->journalList.back());
}