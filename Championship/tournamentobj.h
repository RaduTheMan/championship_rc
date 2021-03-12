#ifndef TOURNAMENTOBJ_H
#define TOURNAMENTOBJ_H
#include <QPushButton>
#include <vector>

class TournamentObj
{
public:
    TournamentObj(char input[], int soc);
    QPushButton* getButton();
    char* getName();
    char* getJoc();
    int getId();
    int getNrJucatori();
    char* getElimination();
    int getIdAdmin();
    char* getStatus();
    char* getNameAdmin();
    void refreshPlayers();
    int getNrRegistered();
    char* getUserFromIndex(int i);
    int getIdUserFromIndex(int i);
    char* getUserFromId(int id);
    bool isIdRegistered(int id);

private:
    QPushButton *button;
    int id;
    int sd;
    char nume[30];
    char joc[30];
    char numeAdmin[30];
    int nrJucatori;
    char elimination[30];
    int id_admin;
    char status[30];
    std::vector<int> idPlayers;
    std::vector<char*> usernamePlayers;
};

#endif // TOURNAMENTOBJ_H
