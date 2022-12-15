#ifndef ITEMWINDOW_H
#define ITEMWINDOW_H

#include <QWidget>

namespace Ui {
class ItemWindow;
}

class ItemWindow : public QWidget
{
    Q_OBJECT

public:
    ItemWindow(std::string time, std::string camera, std::string status, std::string id, std::vector<unsigned char> image, QWidget *parent = nullptr);
    ~ItemWindow();

    std::string time;
    std::string camera;
    std::string status;
    std::string id;

    std::vector<unsigned char> image;

private:
    Ui::ItemWindow *ui;
};

#endif // ITEMWINDOW_H
