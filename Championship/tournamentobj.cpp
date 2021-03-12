#include "tournamentobj.h"
#include <QPushButton>
#include <mainapp.h>
#include <cstring>
#include <string>
#include <iostream>
#include <QVBoxLayout>
#include <stdio.h>
#include <vector>
#include <unistd.h>

TournamentObj::TournamentObj(char input[], int soc)
{
    char *y,msg[100];
    int cntr=0;
    this->sd=soc;
    y=strtok(input,"#");
    while(y)
    {
        switch(cntr)
        {
           case 0:
             this->id=atoi(y);
             break;
           case 1:
             strcpy(this->nume,y);
             break;
           case 2:
             strcpy(this->joc,y);
             break;
           case 3:
             this->nrJucatori=atoi(y);
             break;
           case 5:
             strcpy(this->elimination,y);
             break;
           case 6:
             this->id_admin=atoi(y);
             break;
           case 7:
             strcpy(this->status,y);
             break;
        }
        cntr++;
        y=strtok(NULL,"#");
    }
    std::string string_nume(this->nume);
    std::string string_joc(this->joc);
    std::string string_button ="Nume: " + string_nume + " Joc: " + this->joc;
    this->button = new QPushButton(string_button.c_str());
    //request username of idAdmin from server
    sprintf(msg,"[5]%d",this->id_admin);
    if(write(this->sd,msg,100)<=0)
    {
       perror("[client]Eroare scriere catre server:");
    }
    if (read (this->sd, msg, 100) < 0)
    {
        perror ("[client]Eroare citire de la server:");
    }
    strcpy(this->numeAdmin,msg);
    this->refreshPlayers();

}
bool TournamentObj::isIdRegistered(int id)
{
   for(int i=0;i<(int)this->idPlayers.size();++i)
       if(idPlayers[i]==id)
           return true;
   return false;
}
char* TournamentObj::getUserFromId(int id)
{
   for(int i=0;i<(int)this->idPlayers.size();++i)
       if(idPlayers[i]==id)
           return this->usernamePlayers[i];
   return nullptr;
}
char* TournamentObj::getName()
{
    return this->nume;
}
char* TournamentObj::getJoc()
{
    return this->joc;
}
int TournamentObj::getId()
{
    return this->id;
}
int TournamentObj::getNrJucatori()
{
    return this->nrJucatori;
}
char* TournamentObj::getElimination()
{
    return this->elimination;
}
int TournamentObj::getIdAdmin()
{
    return this->id_admin;
}
char* TournamentObj::getStatus()
{
    return this->status;
}
char* TournamentObj::getNameAdmin()
{
   return this->numeAdmin;
}
void TournamentObj::refreshPlayers()
{
   char msg[100],*p;
   sprintf(msg,"[6]%d",this->id);
   if(write(this->sd,msg,100)<=0)
   {
      perror("[client]Eroare scriere catre server:");
   }
   if (read (this->sd, msg, 100) < 0)
   {
       perror ("[client]Eroare citire de la server:");
   }
   this->idPlayers.clear();
   this->usernamePlayers.clear();
   p=strtok(msg,"#");
   while(p!=NULL)
   {
    this->idPlayers.push_back(atoi(p));
    p=strtok(NULL,"#");
   }
   msg[0]='\0';
   for(int i=0;i<(int)this->idPlayers.size();++i)
   {
      sprintf(msg,"[5]%d",this->idPlayers[i]);
      if(write(this->sd,msg,100)<=0)
      {
         perror("[client]Eroare scriere catre server:");
      }
      if (read (this->sd, msg, 100) < 0)
      {
          perror ("[client]Eroare citire de la server:");
      }
      this->usernamePlayers.push_back((char *)malloc(strlen(msg)));
      strcpy(this->usernamePlayers[i],msg);
   }
}
int TournamentObj::getNrRegistered()
{
   return this->usernamePlayers.size();
}
char* TournamentObj::getUserFromIndex(int i)
{
   if(i<(int)this->usernamePlayers.size())
       return this->usernamePlayers[i];
   return nullptr;
}
int TournamentObj::getIdUserFromIndex(int i)
{
   if(i<(int)this->idPlayers.size())
       return this->idPlayers[i];
   return 0;
}
QPushButton* TournamentObj::getButton()
{
    return this->button;
}

