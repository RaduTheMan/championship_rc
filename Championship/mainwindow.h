#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mainapp.h"
#include "mainregister.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setSoc(int sd);
    int getSoc();

private slots:


    void on_pushButton_login_clicked();

    void on_pushButton_register_clicked();

private:
    Ui::MainWindow *ui;
    MainApp *mainApp;
    MainRegister *mainRegister;
    void closeEvent(QCloseEvent *event);
    int soc;
};
#endif // MAINWINDOW_H
