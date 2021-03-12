#ifndef MAINREGISTER_H
#define MAINREGISTER_H

#include <QMainWindow>
#include <QCloseEvent>
namespace Ui {
class MainRegister;
}

class MainRegister : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainRegister(QWidget *parent = nullptr);
    void setSoc(int sd);
    ~MainRegister();

private slots:
    void on_pushButton_signIn_clicked();

    void on_pushButton_submit_clicked();

    void on_pushButton_info_clicked();

private:
    Ui::MainRegister *ui;
    QWidget *main;
    void closeEvent(QCloseEvent *event);
    bool areRulesHidden;
    int soc;
};

#endif // MAINREGISTER_H
