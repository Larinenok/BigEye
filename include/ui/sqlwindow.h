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

private slots:
    void on_database_activated(QString str);

private:
    Ui::SqlWindow *ui;

    void closeEvent(QCloseEvent *e);
};

#endif // SQLWINDOW_H
