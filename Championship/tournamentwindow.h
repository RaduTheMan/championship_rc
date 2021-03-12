#ifndef TOURNAMENTWINDOW_H
#define TOURNAMENTWINDOW_H

#include <QMainWindow>
#include <tournamentobj.h>
#include <QWidget>
#include <vector>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

namespace Ui {
class tournamentWindow;
}

class tournamentWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit tournamentWindow(QWidget *parent = nullptr, TournamentObj *turneu = nullptr, int id_username = 0, int soc = 0);
    void setSoc(int soc);
    ~tournamentWindow();

private:
    Ui::tournamentWindow *ui;
    void closeEvent(QCloseEvent *event);
    QWidget *mainapp;
    std::vector<QLabel*> playeri;
    QLabel* label_registered;
    QLabel* inProgress;
    QPushButton* record;
    QPushButton* refresh;
    QPushButton* start;
    QPushButton* submitScore;
    QLineEdit* score;
    TournamentObj* turneu;
    void recordPlayer(int idPlayer, int idTournament, int maxPlayers);
    void updateLabels();
    void startTournament(int idTournament);
    void enterScore(int idTournament, int idPlayer1, int idPlayer2, int indexMeci);
    int idLogged;
    int sd;
};

#endif // TOURNAMENTWINDOW_H
