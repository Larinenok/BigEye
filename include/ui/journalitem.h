#ifndef JOURNALITEM_H
#define JOURNALITEM_H

#include <QWidget>

namespace Ui {
class JournalItem;
}

class JournalItem : public QWidget
{
    Q_OBJECT

public:
    explicit JournalItem(std::string time, std::string camera, std::string status, std::string id, QWidget *parent = nullptr);
    ~JournalItem();

    std::string time;
    std::string camera;
    std::string status;
    std::string id;

private:
    Ui::JournalItem *ui;
};

#endif // JOURNALITEM_H
