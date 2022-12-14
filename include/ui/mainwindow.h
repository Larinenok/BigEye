#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QWidget journalRoot;
    QVBoxLayout journalLayout{&journalRoot};
    std::vector<QWidget*> journalList;

    void addNewServiceItem(std::string id, std::string type, std::string data);
    void addNewJournalItem(std::string time, std::string camera, std::string status, std::string id, std::vector<unsigned char> image);

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
