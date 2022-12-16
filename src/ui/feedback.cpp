#include <QMessageBox>
#include <QApplication>
#include <iostream>
#include "ui/feedback.hpp"

namespace ui {

    void error(std::string msg) {
        std::cerr << "[" <<RED<<BOLD<<"ERROR"<<CLR<< "] "
            << BOLD<<msg<<CLR 
            << std::endl;
        
        QMessageBox* box = new QMessageBox();
        box->setWindowTitle("Error");
        box->setText(msg.c_str());
        box->setIcon(QMessageBox::Critical);
        box->exec();
        QCoreApplication::processEvents();
    }

    void warn(std::string msg) {
        std::cerr << "[" <<YEL<<BOLD<<"WARN!"<<CLR<< "] "
            << BOLD<<msg<<CLR
            << std::endl;

        QMessageBox* box = new QMessageBox();
        box->setWindowTitle("Warning");
        box->setText(msg.c_str());
        box->setIcon(QMessageBox::Warning);
        box->show();
        QCoreApplication::processEvents();
    }

    void info(std::string msg) {
        std::cout << "[INFO_] " << msg << std::endl;

        QMessageBox* box = new QMessageBox();
        box->setWindowTitle("Info");
        box->setText(msg.c_str());
        box->setIcon(QMessageBox::Information);
        box->show();
        QCoreApplication::processEvents();
    }

    void msg(std::string msg) {
        std::cout << msg << std::endl;
    }

}
