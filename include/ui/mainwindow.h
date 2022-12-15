#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QImage>
#include <QMessageBox>

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

    bool isClosed = false;

    void addNewServiceItem(std::string id, std::string type, std::string data);
    void addNewJournalItem(std::string time, std::string camera, std::string status, std::string id, std::vector<unsigned char> image);
    void updateFrame(QImage qimage);

    void easterSecret(std::string hostname);

private slots:
    void on_actionAbout_Qt_triggered();
    void on_actionAbout_application_triggered();
    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;

    void closeEvent(QCloseEvent *e);

};
#endif // MAINWINDOW_H
