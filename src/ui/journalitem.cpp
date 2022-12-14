#include "journalitem.h"
#include "ui_journalitem.h"
#include "itemwindow.h"

JournalItem::JournalItem(std::string time, std::string camera, std::string status, std::string id, std::vector<unsigned char> *image, QWidget *parent) :
    time(time),
    camera(camera),
    status(status),
    id(id),
    image(image),
    QWidget(parent),
    ui(new Ui::JournalItem)
{
    ui->setupUi(this);

    ui->time->setText(QString::fromStdString(this->time));
    ui->camera->setText(QString::fromStdString(this->camera));
    ui->status->setText(QString::fromStdString(this->status));
    ui->id->setText(QString::fromStdString(this->id));
}

JournalItem::~JournalItem()
{
    delete ui;
}

void JournalItem::mouseDoubleClickEvent(QMouseEvent *e)
{
    ItemWindow *itemWindow = new ItemWindow(this->time, this->camera, this->status, this->id, this->image);
    itemWindow->show();
}
