#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <string>
#include <cstring>
#include <unistd.h>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <iostream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(800,600);
    QRegularExpression username("^[A-Za-z0-9]{4,10}$");
    QRegularExpression password("^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d]{8,}$");
    QValidator *validatorUsername = new QRegularExpressionValidator(username, this);
    QValidator *validatorPassword = new QRegularExpressionValidator(password, this);
    ui->lineEdit_username->setValidator(validatorUsername);
    ui->lineEdit_password->setValidator(validatorPassword);
    this->soc=0;
}

MainWindow::~MainWindow()
{
    delete ui;
}
int MainWindow::getSoc()
{
    return this->soc;
}
void MainWindow::setSoc(int sd)
{
    this->soc=sd;
}

void MainWindow::on_pushButton_login_clicked()
{
   bool correctInput = true;
   QString username = ui->lineEdit_username->text();
   QString password = ui->lineEdit_password->text();
   if(!ui->lineEdit_username->hasAcceptableInput() || !ui->lineEdit_password->hasAcceptableInput())
       correctInput=false;
   //teoretic de aici ar trebui sa verific daca aceasta combinatie (username,password) se regaseste in fisierul XML
   //trebuie sa trimit cumva username si password catre server
   if(correctInput)
   {
   std::string aux=username.toStdString();
   std::string aux2=password.toStdString();
   char mesaj[100];
   strcpy(mesaj,"[1]");
   strcat(mesaj,aux.c_str());
   strcat(mesaj,"#");
   strcat(mesaj,aux2.c_str());
   if(write(this->soc,mesaj,100)<=0)
   {
       perror("[client]Eroare scriere catre server:");
   }
   if (read (this->soc, mesaj, 100) < 0)
   {
       perror ("[client]Eroare citire de la server:");
   }

   printf ("[client]Mesajul primit este: %s\n", mesaj);

   if(strcmp(mesaj,"0")==0)
     QMessageBox::warning(this,"Login","username and password are not correct.");
   else
   {
    QMessageBox::information(this,"Login","username and password are correct.");
    hide();
    int id=atoi(mesaj);
    mainApp = new MainApp(this,this->getSoc());
    mainApp->show();
    mainApp->setLoginData(username,id);
    //mainApp->setSoc(this->getSoc());
   }

   }
   else
   {
      QMessageBox::warning(this,"Login","username and password are not correct.");
   }
}
void MainWindow::closeEvent(QCloseEvent *event)
{
   QMessageBox::StandardButton confirmClose = QMessageBox::question(this,"Exit","Are you sure you want to exit?",
                                                                    QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);
   if (confirmClose != QMessageBox::Yes)
       event->ignore();
   else
      {
       if(write(this->soc,"quit",10)<=0)
       {
           perror("[client]Eroare scriere catre server:");
       }
       event->accept();
      }
}

void MainWindow::on_pushButton_register_clicked()
{
    hide();
    mainRegister = new MainRegister(this);
    mainRegister->show();
    mainRegister->setSoc(this->soc);

}
