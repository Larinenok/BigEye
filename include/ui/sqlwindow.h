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

    bool isClosed = false;
    std::string database = "None";
    std::string address;
    std::string user;
    std::string password;
    std::string name;

private slots:
    void on_pushButton_clicked();

private:
    Ui::SqlWindow *ui;

    void closeEvent(QCloseEvent *e);
};

#endif // SQLWINDOW_H
