#include "createchampionship.h"
#include "ui_createchampionship.h"
#include <iostream>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include <unistd.h>
#include <cstring>
#include <cstdio>
createChampionship::createChampionship(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::createChampionship)
{
    ui->setupUi(this);
    this->setFixedSize(800,600);
    this->main=parent;
    QRegularExpression inputChamp("^[A-Za-z 0-9]{1,25}$");
    QValidator *validatorInputChamp = new QRegularExpressionValidator(inputChamp, this);
    ui->lineEdit_name->setValidator(validatorInputChamp);
    ui->lineEdit_game->setValidator(validatorInputChamp);
}

createChampionship::~createChampionship()
{
    delete ui;
}
void createChampionship::setIdCreate(int id)
{
    this->id_username=id;
}
void createChampionship::setSoc(int sd)
{
    this->sd=sd;
}
void createChampionship::closeEvent(QCloseEvent *event)
{
   QMessageBox::StandardButton confirmClose = QMessageBox::question(this,"Exit","Are you sure you want to discard this championship?",
                                                                    QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);
   if (confirmClose != QMessageBox::Yes)
       event->ignore();
   else
   {
       event->accept();
       main->show();
   }
}

void createChampionship::on_pushButton_clicked()
{
  /* qDebug(qPrintable(ui->lineEdit_name->text()));
   qDebug(qPrintable(ui->lineEdit_game->text()));
   qDebug(qPrintable(ui->comboBox_phases->currentText()));
   qDebug(qPrintable(ui->comboBox_nrPlayers->currentText()));
   qDebug(qPrintable(ui->comboBox_elimination->currentText()));
   std::cout<<ui->checkBox_admin->isChecked()<<'\n'*/;
   char msg[100];
   if(ui->lineEdit_name->hasAcceptableInput() && ui->lineEdit_game->hasAcceptableInput())
   {
       strcpy(msg,"[3]");
       strcat(msg,ui->lineEdit_name->text().toStdString().c_str());
       strcat(msg,"#");
       strcat(msg,ui->lineEdit_game->text().toStdString().c_str());
       strcat(msg,"#");
       strcat(msg,ui->comboBox_phases->currentText().toStdString().c_str());
       strcat(msg,"#");
       strcat(msg,ui->comboBox_nrPlayers->currentText().toStdString().c_str());
       strcat(msg,"#");
       strcat(msg,ui->comboBox_elimination->currentText().toStdString().c_str());
       strcat(msg,"#");
       char x[10];
       x[0]=(int)(ui->checkBox_admin->isChecked())+'0'; x[1]='\0';
       strcat(msg,x);
       strcat(msg,"#");
       sprintf(x,"%d",this->id_username);
       strcat(msg,x);
       if(write(this->sd,msg,100)<=0)
       {
          perror("[client]Eroare scriere catre server:");
       }
       if (read (this->sd, msg, 100) < 0)
       {
           perror ("[client]Eroare citire de la server:");
       }
       if(strcmp(msg,"0")==0)
       {
          QMessageBox::warning(this,"Create Championship","A tournament with this name has already been taken!");
       }
       else
       {
         QMessageBox::information(this,"Create Championship","Your tournament has been successfully created!");
       }
   }
   else
   {
       QMessageBox::warning(this,"Create Championship","The name and the game should have at least one character!");
   }


}
