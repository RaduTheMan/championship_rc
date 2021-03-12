#include "tournamentwindow.h"
#include "ui_tournamentwindow.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QLayoutItem>
#include <QWidget>
#include <QLabel>
#include <QWidget>
#include <QRect>
#include <QLineEdit>
#include <QRegularExpression>
#include <QValidator>
#include <QRegExpValidator>
#include <string>
#include <QString>
#include <vector>
#include <QPushButton>
#include <unistd.h>
#include <iostream>

tournamentWindow::tournamentWindow(QWidget *parent, TournamentObj *turneu, int id_username, int soc) :
    QMainWindow(parent),
    ui(new Ui::tournamentWindow)
{
    ui->setupUi(this);
    this->setFixedSize(800,600);
    this->setWindowTitle(turneu->getName());
    this->mainapp=parent;
    this->idLogged=id_username;
    this->sd=soc;
    //test creare label
    QLabel *label_info = new QLabel(this);
    QString name_string=turneu->getName();
    QString game_string=turneu->getJoc();
    QString nrPlayers_string=QString::number(turneu->getNrJucatori());
    QString structure_string="Brackets & " + QString(turneu->getElimination());
    QString administrator_string=QString(turneu->getNameAdmin());
    QString info="Name: "+name_string+'\n'+"Game: "+game_string+'\n'+nrPlayers_string+" players\n"+structure_string+'\n'+
            "Administrator: "+administrator_string;
    label_info->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    label_info->setText(info);
    label_info->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    label_info->adjustSize();

    this->label_registered = new QLabel(this);
    QFont f( "Arial", 14, QFont::Bold);
    turneu->refreshPlayers();
    this->label_registered->setText("Registered players("+QString::number(turneu->getNrRegistered())+"/"+QString::number(turneu->getNrJucatori())+")");
    this->label_registered->setFont(f);
    this->label_registered->setGeometry(300,0,0,0);
    this->label_registered->adjustSize();
    this->playeri.resize(turneu->getNrJucatori());
    for(int i=0;i<(int)this->playeri.size();++i)
    {
        this->playeri[i]=new QLabel(this);
        this->playeri[i]->setText(turneu->getUserFromIndex(i));
        this->playeri[i]->setGeometry(300,25*(i+1),0,0);
        this->playeri[i]->adjustSize();
    }

    this->turneu=turneu;
    if(strcmp(turneu->getStatus(),"inregistrare")==0) //turneul este in faza de inregistrare
    {
    this->record = new QPushButton("Register",this);
    this->record->setGeometry(0,label_info->size().height()+10,0,0);
    this->record->adjustSize();
    connect(this->record, &QPushButton::clicked, [=]{
       this->recordPlayer(this->idLogged,turneu->getId(),turneu->getNrJucatori());
    });

    this->refresh = new QPushButton(this);
    QIcon ButtonIcon("./refresh.png");
    this->refresh->setIcon(ButtonIcon);
    this->refresh->setIconSize(refresh->size());
    this->refresh->setGeometry(label_info->size().width()+10,0,32,32);
    connect(this->refresh, &QPushButton::clicked, [=]{
       this->updateLabels();
    });



    if(turneu->getNrJucatori()==turneu->getNrRegistered() && turneu->getIdAdmin()==this->idLogged)
    {
       this->start = new QPushButton("Start tournament",this);
       this->start->setGeometry(0,record->y()+30,0,0);
       this->start->adjustSize();
       connect(this->start, &QPushButton::clicked, [=]{
           this->startTournament(turneu->getId());
       });
    }
    }
    else //turneul este in desfasurare
    {
       if(this->turneu->isIdRegistered(this->idLogged) ) //utilizatorul logat este inregistrat in turneu
       {
         //ii cautam meciul
          char msg[100];
          sprintf(msg,"[10]%d#%d",this->idLogged,this->turneu->getId());

          if(write(this->sd,msg,100)<=0) //cautare meci pentru idLogged,  primul meci din fisierul xml in care se regaseste id-ul si nu are scor trecut
          {
             perror("[client]Eroare scriere catre server:");
          }
          if (read (this->sd, msg, 100) < 0)
          {
              perror ("[client]Eroare citire de la server:");
          }
          if(strchr(msg,'#')) //avem meci
          {
                 char player1Text[30],player2Text[30],*p,aux[30]="vs. ";
                 int idUser1,idUser2,indexMeci;
                 p=strchr(msg,'#');
                 msg[p-msg]='\0'; idUser1=atoi(msg);
                 msg[p-msg]='#';
                 strcpy(msg,p+1);
                 p=strchr(msg,'#');
                 msg[p-msg]='\0'; idUser2=atoi(msg);
                 msg[p-msg]='#';
                 strcpy(msg,p+1); indexMeci=atoi(msg);
                 std::cout<<indexMeci<<'\n';
                 strcpy(player1Text,this->turneu->getUserFromId(idUser1));
                 strcpy(player2Text,this->turneu->getUserFromId(idUser2));
                 strcat(aux,player2Text); strcpy(player2Text,aux);

                 QLabel *player1= new QLabel(this);
                 QLabel *player2= new QLabel(this);
                 this->submitScore= new QPushButton("Enter score",this);
                 this->score = new QLineEdit(this);
                 QRegularExpression scoreRegExp("^(?:0|[1-9][0-9]*)-(?:0|[1-9][0-9]*)$");
                 QValidator *validatorScore = new QRegularExpressionValidator(scoreRegExp, this);


                 player1->setText(player1Text);
                 player2->setText(player2Text);
                 player1->setFont(f);
                 player2->setFont(f);
                 player1->adjustSize();
                 player2->adjustSize();
                 player1->setGeometry(300,300,player1->size().width(),player1->size().height());
                 player2->setGeometry(player1->x()+player1->size().width()+10,300,player2->size().width(),player2->size().height());

                 this->submitScore->setGeometry(player1->x(),player1->y()+player1->size().height()+10,0,0);
                 this->submitScore->adjustSize();

                 this->score->setGeometry(this->submitScore->x()+this->submitScore->size().width()+10,
                                          this->submitScore->y(),this->score->size().width(),this->score->size().height());
                 this->score->setValidator(validatorScore);

                 connect(this->submitScore, &QPushButton::clicked, [=]{
                       this->enterScore(this->turneu->getId(),idUser1,idUser2,indexMeci);
                 });

          }
          else if(strcmp(msg,"0")==0) //inca nu s-a stabilit meciul
          {
              QLabel *wait=new QLabel(this);
              wait->setText("Your next match hasn't been decided yet.\n Waiting for other players to finish their matches..");
              wait->setFont(f);
              wait->adjustSize();
              wait->setGeometry(300,300,wait->size().width(),wait->size().height());
          }
          else //ai fost eliminat din turneu sau esti castigatorul turneului
          {
             if(strcmp(msg,"-1")==0) //a fost eliminat din turneu
             {
                 QLabel *lose=new QLabel(this);
                 lose->setText("You've lost the tournament!");
                 lose->setFont(f);
                 lose->adjustSize();
                 lose->setGeometry(300,300,lose->size().width(),lose->size().height());
             }
             else //a castigat turneul
             {
                 QLabel *win=new QLabel(this);
                 win->setText("You've won the tournament!");
                 win->setFont(f);
                 win->adjustSize();
                 win->setGeometry(300,300,win->size().width(),win->size().height());
             }
          }
       }
       else //ii spunem ca turneul este in desfasurare
       {
            QFont f( "Arial", 14, QFont::Bold);
            this->inProgress = new QLabel(this);
            this->inProgress->setText("Tournament in progress..");
            this->inProgress->setFont(f);
            this->inProgress->setGeometry(0,label_info->size().height()+10,0,0);
            this->inProgress->adjustSize();

       }
    }
}
void tournamentWindow::enterScore(int idTournament, int idPlayer1, int idPlayer2, int indexMeci)
{
    char msg[100];

    if(this->score->hasAcceptableInput())
    {
    QString scoreQString=this->score->text();
    char scoreText[100];
    strcpy(scoreText,scoreQString.toStdString().c_str());
    sprintf(msg,"[11]%d#%d#%d#%d#%s",idTournament,idPlayer1,idPlayer2,indexMeci,scoreText);
    if(write(this->sd,msg,100)<=0)
    {
       perror("[client]Eroare scriere catre server:");
    }
    if (read (this->sd, msg, 100) < 0)
    {
        perror ("[client]Eroare citire de la server:");
    }
    if(strcmp(msg,"1")==0)
    {
       QMessageBox::information(this,"Submit score","The score has been submited successfully!");
    }
    else
         QMessageBox::warning(this,"Submit score","The score has already been submited by your opponent!");
    }
    else
    {
       QMessageBox::warning(this,"Submit score","The score is incorrect.");
    }
}
void tournamentWindow::startTournament(int idTournament)
{
    char msg[100],aux[100];
    int idAux;
    delete this->record;
    delete this->refresh;
    delete this->start;
    sprintf(msg,"[8]%d",idTournament);
    if(write(this->sd,msg,100)<=0)
    {
       perror("[client]Eroare scriere catre server:");
    }
    if (read (this->sd, msg, 100) < 0)
    {
        perror ("[client]Eroare citire de la server:");
    }
    sprintf(msg,"[9]%d#%d#%s",idTournament,this->turneu->getNrJucatori(),this->turneu->getElimination());
    for(int i=0;i<(int)this->turneu->getNrJucatori();++i)
    {
      idAux=this->turneu->getIdUserFromIndex(i);
      sprintf(aux,"#%d",idAux);
      strcat(msg,aux);
    }
    if(write(this->sd,msg,100)<=0) //generare meciuri
    {
       perror("[client]Eroare scriere catre server:");
    }
    if (read (this->sd, msg, 100) < 0)
    {
        perror ("[client]Eroare citire de la server:");
    }

    if(this->turneu->isIdRegistered(this->idLogged) ) //utilizatorul logat este inregistrat in turneu
    {
      //ii cautam meciul
        char msg[100];
        sprintf(msg,"[10]%d#%d",this->idLogged,this->turneu->getId());

        if(write(this->sd,msg,100)<=0) //cautare meci pentru idLogged,  primul meci din fisierul xml in care se regaseste id-ul si nu are scor trecut
        {
           perror("[client]Eroare scriere catre server:");
        }
        if (read (this->sd, msg, 100) < 0)
        {
            perror ("[client]Eroare citire de la server:");
        }
        std::cout<<msg<<'\n';
    }
    else //ii spunem ca turneul este in desfasurare
    {
         QFont f( "Arial", 14, QFont::Bold);
         this->inProgress = new QLabel(this);
         this->inProgress->setText("Tournament in progress..");
         this->inProgress->setFont(f);
         this->inProgress->setGeometry(300,0,0,0);
         this->inProgress->adjustSize();
    }
}
void tournamentWindow::setSoc(int soc)
{
   this->sd=soc;
}
void tournamentWindow::updateLabels()
{
  this->turneu->refreshPlayers();
  this->label_registered->setText("Registered players("+QString::number(turneu->getNrRegistered())+"/"+QString::number(turneu->getNrJucatori())+")");
  this->label_registered->adjustSize();
    for(int i=0;i<(int)this->playeri.size();++i)
    {
        this->playeri[i]->setText(this->turneu->getUserFromIndex(i));
        this->playeri[i]->setGeometry(300,25*(i+1),0,0);
        this->playeri[i]->adjustSize();
    }

}
void tournamentWindow::recordPlayer(int idPlayer, int idTournament, int maxPlayers)
{
   char msg[100];
   //de pus inca un parametru, nr maxim de jucatori din turneu
   sprintf(msg,"[7]%d#%d#%d",idPlayer,idTournament,maxPlayers);
   if(write(this->sd,msg,100)<=0)
   {
      perror("[client]Eroare scriere catre server:");
   }
   if (read (this->sd, msg, 100) < 0)
   {
       perror ("[client]Eroare citire de la server:");
   }
   if(strcmp(msg,"1")==0)
   {
      QMessageBox::information(this,"Register","You have successfully registered at this tournament!");
      this->updateLabels();
   }
   else
   {
      QMessageBox::warning(this,"Register","You can't register at this tournament!(you already are or the list is full)");
   }
}
void tournamentWindow::closeEvent(QCloseEvent *event)
{
   QMessageBox::StandardButton confirmClose = QMessageBox::question(this,"Exit","Are you sure you want to exit this window?",
                                                                    QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);
   if (confirmClose != QMessageBox::Yes)
       event->ignore();
   else
   {
       event->accept();
       this->mainapp->show();
   }
}
tournamentWindow::~tournamentWindow()
{
    delete ui;
}
