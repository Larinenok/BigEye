#include "ui/mainwindow.h"
#include "./ui_mainwindow.h"
#include "ui/journalitem.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->journalRoot.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->journalScroll->setWidget(&this->journalRoot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addNewServiceItem(std::string id, std::string type, std::string data) {
    //
}

void MainWindow::addNewJournalItem(std::string time, std::string camera, std::string status, std::string id, std::vector<unsigned char> image)
{
    this->journalList.push_back(new JournalItem(time, camera, status, id, &image));
    this->journalLayout.addWidget(this->journalList.back());
}

void MainWindow::updateFrame(QImage qimage)
{
    ui->video->setPixmap(QPixmap::fromImage(qimage));
}

void MainWindow::easterSecret(std::string hostname) {
    if (hostname == "DESKTOP-H0ID0JL")
        ui->tabWidget->setTabText(1, "⬛ Планшеты");
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    std::cout << "close" << std::endl;
    this->isClosed = true;
}
