#ifndef SQLWINDOW_H
#define SQLWINDOW_H

#include <QWidget>

namespace Ui {
class SqlWindow;
}

class SqlWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SqlWindow(QWidget *parent = nullptr);
    ~SqlWindow();

private:
    Ui::SqlWindow *ui;
};

#endif // SQLWINDOW_H
