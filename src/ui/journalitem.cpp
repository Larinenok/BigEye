#include "journalitem.h"
#include "ui_journalitem.h"

JournalItem::JournalItem(std::string time, std::string camera, std::string status, std::string id, QWidget *parent) :
    time(time),
    camera(camera),
    status(status),
    id(id),
    QWidget(parent),
    ui(new Ui::JournalItem)
{
    ui->setupUi(this);

    // ui->time->setText(QString::fromStdString(this->time));
    // ui->time->setText("asd");
    ui->camera->setText(QString::fromStdString(this->camera));
    ui->status->setText(QString::fromStdString(this->camera));
    ui->id->setText(QString::fromStdString(this->id));
}

JournalItem::~JournalItem()
{
    delete ui;
}
