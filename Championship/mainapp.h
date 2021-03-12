#ifndef MAINAPP_H
#define MAINAPP_H
#include <QString>
#include <QMainWindow>
#include <QCloseEvent>
#include <createchampionship.h>
#include <tournamentwindow.h>
#include <vector>
#include <tournamentobj.h>
#include <QVBoxLayout>
#include <QShowEvent>
namespace Ui {
class MainApp;
}

class MainApp : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainApp(QWidget *parent = nullptr, int soc = 0);
    ~MainApp();
    void setLoginData(QString username, int id);
    void setSoc(int sd);
    int getSoc();
    void updateTournaments();

protected:
    void showEvent(QShowEvent *ev);

private slots:
    void on_pushButton_create_clicked();

    void on_pushButton_refresh_clicked();



private:
    void showTournament(TournamentObj* turneu);
    Ui::MainApp *ui;
    void closeEvent(QCloseEvent *event);
    QWidget *main;
    QVBoxLayout *lay;
    QString username;
    int id_username;
    createChampionship *champ;
    tournamentWindow *turneu;
    int sd;
    std::vector<TournamentObj*> turnee;

};

#endif // MAINAPP_H
