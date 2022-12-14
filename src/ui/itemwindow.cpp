#include "ui/itemwindow.h"
#include "ui_itemwindow.h"
#include <QLabel>
#include <QPixmap>

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
    imageMap->loadFromData(this->image->data(), this->image->size());
    rootImage->setPixmap(*imageMap);
}

ItemWindow::~ItemWindow()
{
    delete ui;
}
