#include "ui/itemwindow.h"
#include "ui_itemwindow.h"
#include <QLabel>
#include <QPixmap>
#include <QBuffer>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <fstream>

ItemWindow::ItemWindow(std::string time, std::string camera, std::string status, std::string id, std::vector<unsigned char> *image, QWidget *parent) :
    QWidget(parent),
    time(time),
    camera(camera),
    status(status),
    id(id),
    image(image),
    ui(new Ui::ItemWindow)
{
    ui->setupUi(this);
    
    ui->time->setText(QString::fromStdString(this->time));
    ui->camera->setText(QString::fromStdString(this->camera));
    ui->status->setText(QString::fromStdString(this->status));
    ui->id->setText(QString::fromStdString(this->id));

    QLabel *rootImage = new QLabel(this);
    ui->image->addWidget(rootImage);
    QPixmap *imageMap = new QPixmap();

    std::ofstream textout ("./test3.jpg", std::ios::out | std::ios::binary);
    textout.write((const char*)&(*image)[0], image->size());
    textout.close();

    QImage tst;
    //QByteArray rawJPG = QByteArray::fromRawData(reinterpret_cast<const char *>(image->data()), image->size());
    //QBuffer buffer {&rawJPG};
    //tst.loadFromData(image->data(), image->size());

    //tst.load("./test.jpg");
    tst.save("./test2.jpg");
    imageMap->convertFromImage(tst);
    rootImage->setPixmap(*imageMap);
}

ItemWindow::~ItemWindow()
{
    delete ui;
}
