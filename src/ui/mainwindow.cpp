#include "ui/mainwindow.h"
#include "./ui_mainwindow.h"
#include "ui/journalitem.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->journalRoot.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->journalScroll->setWidget(&this->journalRoot);

    for (int i = 0; i < 10; i++)
        this->addNewJournalItem("00:00", "camera 1", "old", std::to_string(i * 111), *new std::vector<unsigned char>(0));
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
