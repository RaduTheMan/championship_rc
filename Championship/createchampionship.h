#ifndef CREATECHAMPIONSHIP_H
#define CREATECHAMPIONSHIP_H

#include <QMainWindow>
namespace Ui {
class createChampionship;
}

class createChampionship : public QMainWindow
{
    Q_OBJECT

public:
    explicit createChampionship(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event);
    ~createChampionship();
    void setIdCreate(int id);
    void setSoc(int sd);

private slots:
    void on_pushButton_clicked();

private:
    Ui::createChampionship *ui;
    QWidget *main;
    int id_username;
    int sd;


};

#endif // CREATECHAMPIONSHIP_H
