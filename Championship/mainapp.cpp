#include "mainapp.h"
#include "ui_mainapp.h"
#include <QMessageBox>
#include <createchampionship.h>
#include "tournamentobj.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLayoutItem>
#include <QWidget>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <QShowEvent>
#include <QSignalMapper>
MainApp::MainApp(QWidget *parent, int soc) :
    QMainWindow(parent),
    ui(new Ui::MainApp)
{
    ui->setupUi(this);
    main=parent;
    this->setFixedSize(800,600);
    this->lay = new QVBoxLayout(this);
    this->sd=soc;
    this->updateTournaments();
    QIcon ButtonIcon("./refresh.png");
    ui->pushButton_refresh->setIcon(ButtonIcon);
    ui->pushButton_refresh->setIconSize(ui->pushButton_refresh->size());
}

MainApp::~MainApp()
{
    delete ui;
}
void MainApp::showEvent(QShowEvent *ev)
{
    this->updateTournaments();
    QMainWindow::showEvent(ev);
}
void MainApp::showTournament(TournamentObj* turneu)
{
    //ar trebui sa afisez un window specific acestui turneu dat ca parametru
    this->turneu = new tournamentWindow(this,turneu,this->id_username, this->sd);
    this->turneu->show();
    hide();
}
void MainApp::updateTournaments()
{
    char msg[100],receive[4096],*p;
    strcpy(msg,"[4]");
    if(write(this->sd,msg,100)<=0)
    {
       perror("[client]Eroare scriere catre server:");
    }
    if (read (this->sd, receive, 4096) < 0)
    {
        perror ("[client]Eroare citire de la server:");
    }
    QLayoutItem * item;
    QWidget * widget;
    QSignalMapper mapper;
    while ((item = this->lay->takeAt(0))) {
        if ((widget = item->widget()) != 0) {widget->hide(); delete widget;}
        else {delete item;}
    }
    delete this->lay;
    this->lay = new QVBoxLayout(this);
    turnee.clear();
    p=strchr(receive,'$');
    while(p!=NULL)
    {
        receive[p-receive]='\0';
        strcpy(msg,receive);

        TournamentObj *tournament = new TournamentObj(msg, this->sd);
        turnee.push_back(tournament);
        this->lay->addWidget(tournament->getButton());
        connect(tournament->getButton(), &QPushButton::clicked, [=]{
            this->showTournament(tournament);
        });

        receive[p-receive]='$';
        strcpy(receive,p+1);
        p=strchr(receive,'$');
    }
    ui->scrollAreaWidgetContents->setLayout(lay);
    lay->setSizeConstraint(QLayout::SetFixedSize);
}

void MainApp::on_pushButton_refresh_clicked()
{
    this->updateTournaments();
}
void MainApp::closeEvent(QCloseEvent *event)
{
   QMessageBox::StandardButton confirmClose = QMessageBox::question(this,"Exit","Are you sure you want to logout?",
                                                                    QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);
   if (confirmClose != QMessageBox::Yes)
       event->ignore();
   else
   {
       event->accept();
       main->show();
   }
}
void MainApp::setSoc(int sd)
{
    this->sd=sd;
}
int MainApp::getSoc()
{
    return this->sd;
}
void MainApp::setLoginData(QString username, int id)
{
    this->username=username;
    this->id_username=id;
    ui->label_username->setText(this->username);
}

void MainApp::on_pushButton_create_clicked()
{
    this->champ = new createChampionship(this);
    champ->show();
    champ->setIdCreate(this->id_username);
    champ->setSoc(this->sd);
    hide();
}


