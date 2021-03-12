#include "mainregister.h"
#include "ui_mainregister.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QDebug>
#include <QPixmap>
#include <QIcon>
#include <QString>
#include <unistd.h>
MainRegister::MainRegister(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainRegister)
{
    ui->setupUi(this);
    main=parent;
    this->setFixedSize(800,600);
    QRegularExpression username("^[A-Za-z0-9]{4,}$");
    QRegularExpression password("^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d]{8,}$");
    QRegularExpression email("^[A-Za-z0-9-\\.]+@([A-Za-z0-9-]+\\.)+[A-Za-z0-9-]{2,4}$");
    QValidator *validatorUsername = new QRegularExpressionValidator(username, this);
    QValidator *validatorPassword = new QRegularExpressionValidator(password, this);
    QValidator *validatoremail = new QRegularExpressionValidator(email, this);
    ui->lineEdit_username->setValidator(validatorUsername);
    ui->lineEdit_password->setValidator(validatorPassword);
    ui->lineEdit_confirmPassword->setValidator(validatorPassword);
    ui->lineEdit_email->setValidator(validatoremail);


    QIcon ButtonIcon("./information_icon.png");
    ui->pushButton_info->setIcon(ButtonIcon);
    ui->pushButton_info->setIconSize(ui->pushButton_info->size());

    ui->label_rules->setStyleSheet(QString("QLabel {background-image: url(\"label_background.png\");}"));
    ui->label_rules->hide();
    areRulesHidden=true;
    this->soc=0;
}

MainRegister::~MainRegister()
{
    delete ui;
}
void MainRegister::setSoc(int sd)
{
    this->soc=sd;
}
void MainRegister::closeEvent(QCloseEvent *event)
{
   QMessageBox::StandardButton confirmClose = QMessageBox::question(this,"Exit","Are you sure you want to exit?",
                                                                    QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);
   if (confirmClose != QMessageBox::Yes)
       event->ignore();
   else
       event->accept();
}

void MainRegister::on_pushButton_signIn_clicked()
{
    main->show();
    this->~MainRegister();
}

void MainRegister::on_pushButton_submit_clicked()
{
    bool correctInput = true;
    char msg[100];
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();
    QString confirmPassword = ui->lineEdit_confirmPassword->text();
    QString email = ui->lineEdit_email->text();
    std::string aux_username,aux_password,aux_confirmPassword,aux_email;
    if(!ui->lineEdit_username->hasAcceptableInput() || !ui->lineEdit_password->hasAcceptableInput() ||
            !ui->lineEdit_confirmPassword->hasAcceptableInput() || !ui->lineEdit_email->hasAcceptableInput())
        correctInput=false;
    if(correctInput)
    {
       qDebug()<<"input corect dpdv sintactic\n";
       aux_username=username.toStdString();
       aux_password=password.toStdString();
       aux_confirmPassword=confirmPassword.toStdString();
       aux_email=email.toStdString();
       if(aux_password!=aux_confirmPassword)
           QMessageBox::warning(this,"Register","Passwords don't match!");
       else
       {
          strcpy(msg,"[2]");
          strcat(msg,aux_username.c_str());
          strcat(msg,"#");
          strcat(msg,aux_password.c_str());
          strcat(msg,"#");
          strcat(msg,aux_email.c_str());
          if(write(this->soc,msg,100)<=0)
          {
              perror("[client]Eroare scriere catre server:");
          }
          if (read (this->soc, msg, 100) < 0)
          {
              perror ("[client]Eroare citire de la server:");
          }
          if(strcmp(msg,"0")==0)
          {
             QMessageBox::warning(this,"Register","Username and/or email is already used!");
          }
          else
          {
            QMessageBox::information(this,"Register","Your account has been successfully created!");
          }
       }

    }
    else
    {
        QMessageBox::warning(this,"Register","Incorrect input!(read the rules carefully)");
    }
}


void MainRegister::on_pushButton_info_clicked()
{
    if(areRulesHidden)
      ui->label_rules->show();
    else
      ui->label_rules->hide();
    areRulesHidden=!areRulesHidden;
}
