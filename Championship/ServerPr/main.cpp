#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <sys/wait.h>
#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QString>
#include <iostream>
#include <vector>
#include <QDomElement>
#include <QDomDocument>
#include <QDomText>
#include <QTextStream>
///cod server
struct fields
{
    char *data;
}x[15];
void prelucrare_data(char sir[],fields x[])
{
    char *p;
    int cntr=0;
    p=strtok(sir,"#");
    while(p)
    {
      x[cntr].data=(char *)malloc(strlen(p));
      strcpy(x[cntr].data,p);
      p=strtok(NULL,"#");
      cntr++;
    }
}
void prelucrareLogin(char username[], char password[], char input[]) ///functie de prelucrare al inputului
{
    char *aux,*aux2;
    aux=strchr(input,'#');
    if(aux==NULL)
    {
        strcpy(username,input);
        username[strlen(username)-1]='\0';
        password[0]='\0';
    }
    else
    {
        input[aux-input]='\0';
        strcpy(username,input);
        aux2=strstr(username,"[1]");
        strcpy(username,aux2+3);
        input[aux-input]='#';
        strcpy(password,aux+1);
        aux=strchr(aux+1,'#');
    }
}
void prelucrareRegister(char username[],char password[], char email[], char input[])
{
    char *aux,*aux2;
    aux=strchr(input,'#');
    if(aux==NULL)
    {
        strcpy(username,input);
        username[strlen(username)-1]='\0';
        password[0]='\0';
    }
    else
    {
        input[aux-input]='\0';
        strcpy(username,input);
        aux2=strstr(username,"[2]");
        strcpy(username,aux2+3);
        strcpy(aux,aux+1);
        strcpy(input,aux);
        aux=strchr(input,'#');
        input[aux-input]='\0';
        strcpy(password,input);
        strcpy(aux,aux+1);
        strcpy(input,aux);
        strcpy(email,input);
    }
}
int gasit=0,nrComenzi=10;
#define PORT 2024
int main ()
{
    struct sockaddr_in server;
    struct sockaddr_in from;
    char msg[100],username[100],password[100],email[100],deTrimis[100];
    int sd, client;

    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror ("[server]Eroare la socket:");
        return -1;
    }


    bzero (&server, sizeof (server));
    bzero (&from, sizeof (from));



    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);


    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
        perror ("[server]Eroare la bind():");
        return -1;
    }


    if (listen (sd, 5) == -1)
    {
        perror ("[server]Eroare la listen():");
        return -1;
    }


    while (1) ///prima bucla while infinita ce mentine serverul activ
    {
        socklen_t length = sizeof (from);
        pid_t pid;

        printf ("[server]Asteptam la portul %d...\n",PORT);
        fflush (stdout);


        client = accept (sd, (struct sockaddr *) &from, &length);


        if (client < 0)
        {
            perror ("[server]Eroare la accept():");
            return -1;
        }
        pid=fork();
        if(pid==-1)
        {
            perror("[server]Eroare la fork():");
            return -1;
        }
        if(pid)
        {
            close (client);
            continue;
        }
        break;
    }
    close(sd);
    while(1) ///a doua bucla while infinita pentru a gestiona cerintele clientului
    {

        bzero (msg, 100);
        printf ("[server]Asteptam mesajul...\n");
        fflush (stdout);
        int requestTournaments=0;


        if (read (client, msg, 100) <= 0)
        {
            perror ("[server]Eroare la citire de la client:");
            return -1;
        }

        printf ("[server]Mesajul a fost receptionat...%s\n", msg);

        if(strstr(msg,"[1]")) //logare
        {
           bool canLogin=false;
           prelucrareLogin(username,password,msg);
           char usernameFile[100],passwordFile[100],idFile[100];
           std::string aux_username,aux_password,aux_id;
           qDebug()<<"clientul doreste sa se logheze"<<'\n';
           QFile file("useri.xml");
              if(!file.open(QFile::ReadOnly | QFile::Text))
              {
                  qDebug() << "Cannot read file" << file.errorString();
                  exit(0);
              }

           QXmlStreamReader reader(&file);
           if (reader.readNextStartElement())
                  {
                   if (reader.name() == "useri")
                       while(reader.readNextStartElement())
                           if(reader.name() == "user")
                           {
                              while(reader.readNextStartElement())
                               {
                                   QString s = reader.readElementText();
                                   if(reader.name()=="username")
                                          aux_username=s.toStdString(),strcpy(usernameFile,aux_username.c_str());
                                   else if (reader.name()=="password")
                                          aux_password=s.toStdString(),strcpy(passwordFile,aux_password.c_str());
                                   else if(reader.name()=="id")
                                          aux_id=s.toStdString(),strcpy(idFile,aux_id.c_str());
                               }
                              if(strcmp(username,usernameFile)==0 && strcmp(password,passwordFile)==0)
                                    {
                                       canLogin=true;
                                       break;
                                    }
                           }

                  }
                   else
                       reader.raiseError(QObject::tr("Incorrect file"));

            if(canLogin)
                  strcpy(deTrimis,idFile);
            else
                  strcpy(deTrimis,"0");
         }
        else if(strstr(msg,"[2]")) //inregistrare
        {
            bool isRegistered=false;
            std::string aux_username,aux_email,aux_id;
            char usernameFile[100],emailFile[100],id_file[100];
            prelucrareRegister(username,password,email,msg);
            QFile file("useri.xml");
               if(!file.open(QFile::ReadWrite| QFile::Text))
               {
                   qDebug() << "Cannot read file" << file.errorString();
                   exit(0);
               }

            QXmlStreamReader reader(&file);
            if (reader.readNextStartElement())
                   {
                    if (reader.name() == "useri")
                        while(reader.readNextStartElement())
                            if(reader.name() == "user")
                            {
                               while(reader.readNextStartElement())
                                {
                                    QString s = reader.readElementText();
                                    if(reader.name()=="username")
                                           aux_username=s.toStdString(),strcpy(usernameFile,aux_username.c_str());
                                    else if (reader.name()=="email")
                                           aux_email=s.toStdString(),strcpy(emailFile,aux_email.c_str());
                                    else if (reader.name()=="id")
                                           aux_id=s.toStdString(),strcpy(id_file,aux_id.c_str());
                                }
                               if(strcmp(username,usernameFile)==0 || strcmp(email,emailFile)==0)
                                     {
                                        isRegistered=true;
                                     }
                            }

                   }
                    else
                        reader.raiseError(QObject::tr("Incorrect file"));
            if(isRegistered)
            {
                strcpy(deTrimis,"0"); ///username-ul sau/si emailul sunt deja folosite
            }
            else
            {
                strcpy(deTrimis,"1");
                int nr=atoi(id_file);
                nr++;
                aux_id=std::to_string(nr);
                strcpy(id_file,aux_id.c_str());
                //actualizare fisier xml cu un nou utilizator
                  file.close();
                  if(!file.open(QFile::ReadOnly | QFile::Text))
                  {
                      qDebug() << "Cannot read file" << file.errorString();
                      exit(0);
                  }
                  QDomDocument document;
                  if (!document.setContent(&file))
                      {
                          qDebug() << "failed to parse file";
                          file.close();
                          exit(0);
                      }
                  file.close();

                  QDomElement docEle = document.documentElement();
                  QDomNodeList elements = docEle.elementsByTagName("useri");
                  QDomElement light1 = document.createElement( "user" );
                     QDomElement state = document.createElement("id");
                     QDomText nextNode = document.createTextNode(id_file);
                     QDomElement secondState = document.createElement("username");
                     QDomText secondNextNode = document.createTextNode(username);
                     QDomElement thirdState = document.createElement("password");
                     QDomText thirdNextNode = document.createTextNode(password);
                     QDomElement fourthState = document.createElement("email");
                     QDomText fourthNextNode = document.createTextNode(email);
                     state.appendChild(nextNode);
                     secondState.appendChild(secondNextNode);
                     thirdState.appendChild(thirdNextNode);
                     fourthState.appendChild(fourthNextNode);
                     light1.appendChild(state);
                     light1.appendChild(secondState);
                     light1.appendChild(thirdState);
                     light1.appendChild(fourthState);
                     docEle.appendChild( light1 );

                     QFile outFile( "useri.xml" );
                     if( !outFile.open( QIODevice::WriteOnly | QIODevice::Text ) )
                     {
                         qDebug( "Failed to open file for writing." );
                         exit(0);
                     }

                     QTextStream stream( &outFile );
                     stream << document.toString();

                     outFile.close();


            }
        }
        else if(strstr(msg,"[3]")) // creare turneu
        {
            char name[100],nameFile[100],idFile[100];
            bool exists=false,atLeastOne=false;
            strcpy(msg,msg+3);
            prelucrare_data(msg,x);
            strcpy(name,x[0].data);
            QFile file("campionate.xml");
               if(!file.open(QFile::ReadWrite| QFile::Text))
               {
                   qDebug() << "Cannot read file" << file.errorString();
                   exit(0);
               }
            QXmlStreamReader reader(&file);
            if (reader.readNextStartElement())
                      {
                       if (reader.name() == "campionate")
                           while(reader.readNextStartElement())
                               if(reader.name() == "campionat")
                               {
                                  atLeastOne=true;
                                   while(reader.readNextStartElement())
                                   {
                                       QString s = reader.readElementText();
                                       if(reader.name()=="nume")
                                              strcpy(nameFile,s.toStdString().c_str());
                                       if(reader.name()=="id")
                                              strcpy(idFile,s.toStdString().c_str());
                                   }
                                  if(strcmp(name,nameFile)==0)
                                        {
                                           exists=true;
                                        }
                               }

                      }
                       else
                           reader.raiseError(QObject::tr("Incorrect file"));
            if(exists) //exista turneul deja
            {
              strcpy(deTrimis,"0");
            }
            else
            {
                struct champ
                {
                  char id[10];
                  char nume[30];
                  char joc[30];
                  char nrJuc[5];
                  char phase[30];
                  char elimination[30];
                  char idAdmin[10];
                  char status[30];
                }c;
                int nr;
                bool adminIsPlayer=false;
                if(atLeastOne)
                  nr=atoi(idFile),nr++;
                else nr=1;
                sprintf(c.id,"%d",nr); strcpy(c.nume,name); strcpy(c.joc,x[1].data); strcpy(c.nrJuc,x[3].data);
                strcpy(c.phase,x[2].data); strcpy(c.elimination,x[4].data); strcpy(c.idAdmin,x[6].data); strcpy(c.status,"inregistrare");
                adminIsPlayer=atoi(x[5].data);

                strcpy(deTrimis,c.id);
                file.close();
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Cannot read file" << file.errorString();
                    exit(0);
                }
                QDomDocument document;
                if (!document.setContent(&file))
                    {
                        qDebug() << "failed to parse file";
                        file.close();
                        exit(0);
                    }
                file.close();
                QDomElement docEle = document.documentElement();
                QDomNodeList elements = docEle.elementsByTagName("campionate");
                QDomElement light1 = document.createElement( "campionat" );
                QDomElement state;
                QDomText node;
                for(int i=0;i<8;++i)
                {
                   switch(i)
                   {
                      case 0:
                       state=document.createElement("id"),node=document.createTextNode(c.id);
                       break;
                      case 1:
                       state=document.createElement("nume"),node=document.createTextNode(c.nume);
                       break;
                      case 2:
                       state=document.createElement("joc"),node=document.createTextNode(c.joc);
                       break;
                      case 3:
                       state=document.createElement("nrjuc"),node=document.createTextNode(c.nrJuc);
                       break;
                      case 4:
                       state=document.createElement("phases"),node=document.createTextNode(c.phase);
                       break;
                      case 5:
                       state=document.createElement("elimination"),node=document.createTextNode(c.elimination);
                       break;
                      case 6:
                       state=document.createElement("idadmin"),node=document.createTextNode(c.idAdmin);
                       break;
                      case 7:
                       state=document.createElement("status"),node=document.createTextNode(c.status);
                       break;
                   }
                   state.appendChild(node);
                   light1.appendChild(state);
                }
                docEle.appendChild(light1);

                QFile outFile( "campionate.xml" );
                if( !outFile.open( QIODevice::WriteOnly | QIODevice::Text ) )
                {
                    qDebug( "Failed to open file for writing." );
                    exit(0);
                }

                QTextStream stream( &outFile );
                stream << document.toString();

                outFile.close();

                if(adminIsPlayer) //inregistrez admin-ul ca jucator
                {
                  QFile file2("inregistrari.xml");
                  if(!file2.open(QFile::ReadOnly | QFile::Text))
                  {
                      qDebug() << "Cannot read file" << file2.errorString();
                      exit(0);
                  }
                  QDomDocument document2;
                  if (!document2.setContent(&file2))
                      {
                          qDebug() << "failed to parse file";
                          file2.close();
                          exit(0);
                      }
                  file2.close();
                  QDomElement docEle2 = document2.documentElement();
                  QDomNodeList elements2 = docEle2.elementsByTagName("inregistrari");
                  QDomElement light2 = document2.createElement( "inregistrare" );
                  QDomElement state2;
                  QDomText node2;
                  state2=document2.createElement("iduser"); node2=document2.createTextNode(c.idAdmin);
                  state2.appendChild(node2); light2.appendChild(state2);

                  state2=document2.createElement("idcamp"); node2=document2.createTextNode(c.id);
                  state2.appendChild(node2); light2.appendChild(state2);
                  docEle2.appendChild(light2);

                  QFile outFile2( "inregistrari.xml" );
                  if( !outFile2.open( QIODevice::WriteOnly | QIODevice::Text ) )
                  {
                      qDebug( "Failed to open file for writing." );
                      exit(0);
                  }

                  QTextStream stream2( &outFile2 );
                  stream2 << document2.toString();

                  outFile2.close();
                }
            }

        }
        else if(strstr(msg,"[4]")) //request tournaments
        {
               char sender[4096];
               requestTournaments=1;
               sender[0]='\0';
               QFile file("campionate.xml");
                  if(!file.open(QFile::ReadWrite| QFile::Text))
                  {
                      qDebug() << "Cannot read file" << file.errorString();
                      exit(0);
                  }
               QXmlStreamReader reader(&file);
               if (reader.readNextStartElement())
                         {
                          if (reader.name() == "campionate")
                              while(reader.readNextStartElement())
                                  if(reader.name() == "campionat")
                                  {
                                      while(reader.readNextStartElement())
                                      {
                                          QString s = reader.readElementText();
                                          strcat(sender,s.toStdString().c_str());
                                          strcat(sender,"#");
                                      }
                                      strcat(sender,"$");
                                  }
                         }
                          else
                              reader.raiseError(QObject::tr("Incorrect file"));

               printf("[server]Trimitem mesajul inapoi...%s\n",sender);
               if(write(client,sender,4096)<=0)
               {
                   perror ("[server]Eroare la scriere catre client:");
                   return -1;
               }
               else
                   printf ("[server]Mesajul a fost trasmis cu succes.\n");
        }
        else if(strstr(msg,"[5]")) //request username of administrator of tournament
        {
            char usernameFile[100],idFile[100],idAdmin[100];
            strcpy(idAdmin,msg+3);
            QFile file("useri.xml");
               if(!file.open(QFile::ReadOnly | QFile::Text))
               {
                   qDebug() << "Cannot read file" << file.errorString();
                   exit(0);
               }

            QXmlStreamReader reader(&file);
            if (reader.readNextStartElement())
                   {
                    if (reader.name() == "useri")
                        while(reader.readNextStartElement())
                            if(reader.name() == "user")
                            {
                               while(reader.readNextStartElement())
                                {
                                    QString s = reader.readElementText();
                                    if(reader.name()=="username")
                                           strcpy(usernameFile,s.toStdString().c_str());
                                    else if(reader.name()=="id")
                                           strcpy(idFile,s.toStdString().c_str());
                                }
                               if(strcmp(idAdmin,idFile)==0) //daca am gasit idAdmin
                                     {
                                        break;
                                     }
                            }

                   }
                    else
                        reader.raiseError(QObject::tr("Incorrect file"));
            strcpy(deTrimis,usernameFile);
        }
        else if(strstr(msg,"[6]")) //request of id players who are registered in a tournament
        {
            char idUserFile[100],idCampFile[100],idCamp[100];
            strcpy(idCamp,msg+3);
            deTrimis[0]='\0';
            QFile file("inregistrari.xml");
               if(!file.open(QFile::ReadOnly | QFile::Text))
               {
                   qDebug() << "Cannot read file" << file.errorString();
                   exit(0);
               }

            QXmlStreamReader reader(&file);
            if (reader.readNextStartElement())
                   {
                    if (reader.name() == "inregistrari")
                        while(reader.readNextStartElement())
                            if(reader.name() == "inregistrare")
                            {
                               while(reader.readNextStartElement())
                                {
                                    QString s = reader.readElementText();
                                    if(reader.name()=="iduser")
                                           strcpy(idUserFile,s.toStdString().c_str());
                                    else if(reader.name()=="idcamp")
                                           strcpy(idCampFile,s.toStdString().c_str());
                                }
                               if(strcmp(idCamp,idCampFile)==0)
                                     {
                                        strcat(deTrimis,idUserFile);
                                        strcat(deTrimis,"#");
                                     }
                            }

                   }
                    else
                        reader.raiseError(QObject::tr("Incorrect file"));
        }
        else if(strstr(msg,"[7]")) //record player into tournament
        {

            char idUserFile[100],idCampFile[100],idCamp[100],idUser[100],maxPlayers[100];
            int registered=0;
            bool alreadyExists=false,full=false;
            prelucrare_data(msg,x);
            strcpy(idUser,x[0].data);
            strcpy(idCamp,x[1].data);
            strcpy(maxPlayers,x[2].data);
            deTrimis[0]='\0';
            strcpy(idUser,idUser+3);

            QFile file("inregistrari.xml");
               if(!file.open(QFile::ReadOnly | QFile::Text))
               {
                   qDebug() << "Cannot read file" << file.errorString();
                   exit(0);
               }

            QXmlStreamReader reader(&file);
            if (reader.readNextStartElement())
                   {
                    if (reader.name() == "inregistrari")
                        while(reader.readNextStartElement())
                            if(reader.name() == "inregistrare")
                            {
                               while(reader.readNextStartElement())
                                {
                                    QString s = reader.readElementText();
                                    if(reader.name()=="iduser")
                                           strcpy(idUserFile,s.toStdString().c_str());
                                    else if(reader.name()=="idcamp")
                                           strcpy(idCampFile,s.toStdString().c_str());
                                }
                               if(strcmp(idCamp,idCampFile)==0)
                                     {
                                        registered++;
                                        if(strcmp(idUser,idUserFile)==0)
                                        {
                                            alreadyExists=true;
                                            break;
                                        }
                                        if(registered==atoi(maxPlayers))
                                        {
                                            full=true;
                                            break;
                                        }
                                     }
                            }

                   }
                    else
                        reader.raiseError(QObject::tr("Incorrect file"));
          if(!alreadyExists && !full) //daca nu este deja inregistrat si nu s-a atins capacitatea maxima de jucatori
          {
              QFile file2("inregistrari.xml");
              if(!file2.open(QFile::ReadOnly | QFile::Text))
              {
                  qDebug() << "Cannot read file" << file2.errorString();
                  exit(0);
              }
              QDomDocument document2;
              if (!document2.setContent(&file2))
                  {
                      qDebug() << "failed to parse file";
                      file2.close();
                      exit(0);
                  }
              file2.close();
              QDomElement docEle2 = document2.documentElement();
              QDomNodeList elements2 = docEle2.elementsByTagName("inregistrari");
              QDomElement light2 = document2.createElement( "inregistrare" );
              QDomElement state2;
              QDomText node2;
              state2=document2.createElement("iduser"); node2=document2.createTextNode(idUser);
              state2.appendChild(node2); light2.appendChild(state2);

              state2=document2.createElement("idcamp"); node2=document2.createTextNode(idCamp);
              state2.appendChild(node2); light2.appendChild(state2);
              docEle2.appendChild(light2);

              QFile outFile2( "inregistrari.xml" );
              if( !outFile2.open( QIODevice::WriteOnly | QIODevice::Text ) )
              {
                  qDebug( "Failed to open file for writing." );
                  exit(0);
              }

              QTextStream stream2( &outFile2 );
              stream2 << document2.toString();

              outFile2.close();
              strcpy(deTrimis,"1");
          }
          else
          {
              strcpy(deTrimis,"0");
          }

        }
        else if(strstr(msg,"[8]")) //start tournament
        {
             char idCamp[100],idCampFile[100];
             bool deSchimbat=false;
             strcpy(idCamp,msg+3);
             QFile file2("campionate.xml");
             if(!file2.open(QFile::ReadOnly | QFile::Text))
             {
                 qDebug() << "Cannot read file" << file2.errorString();
                 exit(0);
             }
             QDomDocument document2;
             if (!document2.setContent(&file2))
                 {
                     qDebug() << "failed to parse file";
                     file2.close();
                     exit(0);
                 }
             file2.close();
             QDomElement docEle2 = document2.documentElement();
             QDomNodeList elements2 = docEle2.childNodes();
             for(int i=0;i<elements2.size();++i)
             {
                QDomNode turneu;
                QDomNodeList atribute;
                turneu=elements2.at(i);
                atribute=turneu.childNodes();
                for(int j=0;j<atribute.count();++j)
                {
                    if(atribute.at(j).nodeName()=="id")
                      {
                        if( atribute.at(j).firstChild().nodeValue()==QString(idCamp) )
                          deSchimbat=true;
                        continue;
                      }
                    if(atribute.at(j).nodeName()=="status" && deSchimbat)
                    {
                       atribute.at(j).firstChild().setNodeValue("desfasurare");
                       break;
                    }
                }
                if(deSchimbat)
                   {
                      break;
                   }
             }
             QFile outFile2( "campionate.xml" );
             if( !outFile2.open( QIODevice::WriteOnly | QIODevice::Text ) )
             {
                 qDebug( "Failed to open file for writing." );
                 exit(0);
             }

             QTextStream stream2( &outFile2 );
             stream2 << document2.toString();

             outFile2.close();
             if(deSchimbat)
                 strcpy(deTrimis,"1");
             else
                 strcpy(deTrimis,"0");
        }
        else if(strstr(msg,"[9]")) //generare meciuri turneu
        {
          char idCamp[100],elimination[30],nameFile[100];
          int nrPlayers,indexMeci=1,nrMatches;
          std::vector<int> playeri;
          prelucrare_data(msg,x);
          strcpy(idCamp,x[0].data);
          strcpy(idCamp,idCamp+3);
          nrPlayers=atoi(x[1].data);
          strcpy(elimination,x[2].data);
          for(int i=3;i<3+nrPlayers;++i)
              playeri.push_back(atoi(x[i].data));
          QFile file2("meciuri.xml");
          if(!file2.open(QFile::ReadOnly | QFile::Text))
          {
              qDebug() << "Cannot read file" << file2.errorString();
              exit(0);
          }
          QDomDocument document2;
          if (!document2.setContent(&file2))
              {
                  qDebug() << "failed to parse file";
                  file2.close();
                  exit(0);
              }
          file2.close();
          QDomElement docEle2 = document2.documentElement();
          QDomNodeList elements2 = docEle2.elementsByTagName("meciuri");

          for(int i=0;i<nrPlayers/2;++i)
          {
          QDomElement light2 = document2.createElement( "meci" );
          QDomElement state2;
          QDomText node2;
          for(int j=0;j<7;++j)
          {
              switch(j)
              {
                 case 0:
                  state2=document2.createElement("indexmeci"),node2=document2.createTextNode(QString::number(indexMeci));
                  break;
                 case 1:
                  state2=document2.createElement("idcamp"),node2=document2.createTextNode(idCamp);
                  break;
                 case 2:
                  state2=document2.createElement("indexmeci1"),node2=document2.createTextNode("#");
                  break;
                 case 3:
                  state2=document2.createElement("indexmeci2"),node2=document2.createTextNode("#");
                  break;
                 case 4:
                  state2=document2.createElement("idplayer1"),node2=document2.createTextNode(QString::number(playeri[i*2]));
                  break;
                 case 5:
                  state2=document2.createElement("idplayer2"),node2=document2.createTextNode(QString::number(playeri[i*2+1]));
                  break;
                 case 6:
                  state2=document2.createElement("scor"),node2=document2.createTextNode("#");
                  break;
              }
          state2.appendChild(node2);
          light2.appendChild(state2);
          }
          indexMeci++;
          docEle2.appendChild(light2);
          }
          sprintf(nameFile,"%d",nrPlayers);
          if(strcmp(elimination,"Single elimination")==0)
              strcat(nameFile,"SE"),nrMatches=nrPlayers-1;
          else
              strcat(nameFile,"DE"),nrMatches=2*nrPlayers-1;
          strcat(nameFile,".txt");
          std::ifstream fin(nameFile);

          for(int i=indexMeci;i<=nrMatches;++i)
          {
          QDomElement light2 = document2.createElement( "meci" );
          QDomElement state2;
          QDomText node2;
          char index1[20],index2[20];
          fin>>index1>>index2;
          for(int j=0;j<7;++j)
          {
              switch(j)
              {
                 case 0:
                  state2=document2.createElement("indexmeci"),node2=document2.createTextNode(QString::number(indexMeci));
                  break;
                 case 1:
                  state2=document2.createElement("idcamp"),node2=document2.createTextNode(idCamp);
                  break;
                 case 2:
                  state2=document2.createElement("indexmeci1"),node2=document2.createTextNode(index1);
                  break;
                 case 3:
                  state2=document2.createElement("indexmeci2"),node2=document2.createTextNode(index2);
                  break;
                 case 4:
                  state2=document2.createElement("idplayer1"),node2=document2.createTextNode("#");
                  break;
                 case 5:
                  state2=document2.createElement("idplayer2"),node2=document2.createTextNode("#");
                  break;
                 case 6:
                  state2=document2.createElement("scor"),node2=document2.createTextNode("#");
                  break;
              }
          state2.appendChild(node2);
          light2.appendChild(state2);
          }
          indexMeci++;
          docEle2.appendChild(light2);
          }


          QFile outFile2( "meciuri.xml" );
          if( !outFile2.open( QIODevice::WriteOnly | QIODevice::Text ) )
          {
              qDebug( "Failed to open file for writing." );
              exit(0);
          }

          QTextStream stream2( &outFile2 );
          stream2 << document2.toString();

          outFile2.close();

        }
        else if (strstr(msg,"[10]")) //cautare meci
        {
            char idUser[100],idTournament[100],idTournamentFile[100],idUser1File[100],idUser2File[100],indexMeciFile[100],scorFile[100];
            char idUser1lm[100],idUser2lm[100],scorlm[100];
            bool esteScorNul=false,areAdversar=true,gasit=false;
            prelucrare_data(msg,x);
            strcpy(idUser,x[0].data);
            strcpy(idUser,idUser+4);
            strcpy(idTournament,x[1].data);
            deTrimis[0]='\0';

            QFile file("meciuri.xml");
               if(!file.open(QFile::ReadOnly | QFile::Text))
               {
                   qDebug() << "Cannot read file" << file.errorString();
                   exit(0);
               }

            QXmlStreamReader reader(&file);
            if (reader.readNextStartElement())
                   {
                    if (reader.name() == "meciuri")
                        while(reader.readNextStartElement())
                            if(reader.name() == "meci")
                            {
                                esteScorNul=false; areAdversar=true;
                                while(reader.readNextStartElement())
                                {
                                    QString s = reader.readElementText();
                                    if(reader.name()=="idcamp")
                                           strcpy(idTournamentFile,s.toStdString().c_str());
                                    else if(reader.name()=="idplayer1")
                                           {
                                        if(s=="#")
                                            areAdversar=false;
                                        else
                                            strcpy(idUser1File,s.toStdString().c_str());
                                           }
                                    else if(reader.name()=="idplayer2")
                                           {
                                          if(s=="#")
                                              areAdversar=false;
                                          else
                                              strcpy(idUser2File,s.toStdString().c_str());
                                            }
                                    else if(reader.name()=="scor" && s=="#")
                                           esteScorNul=true;
                                    else if(reader.name()=="scor" && s!="#")
                                          strcpy(scorFile,s.toStdString().c_str());
                                    else if(reader.name()=="indexmeci")
                                          strcpy(indexMeciFile,s.toStdString().c_str());
                                }
                                if(strcmp(idTournament,idTournamentFile)==0) //am gasit un meci din turneul bun
                                {
                                    if((strcmp(idUser,idUser1File)==0 || strcmp(idUser,idUser2File)==0) && esteScorNul)
                                    {
                                        if(areAdversar) //am gasit meciul
                                        {
                                             strcpy(deTrimis,idUser1File);
                                             strcat(deTrimis,"#");
                                             strcat(deTrimis,idUser2File);
                                             strcat(deTrimis,"#");
                                             strcat(deTrimis,indexMeciFile);
                                        }
                                        else //inca nu are adversar stabilit
                                        {
                                            strcpy(deTrimis,"0");
                                        }
                                        gasit=true;
                                        break;
                                    }
                                    else if(!esteScorNul)
                                    {
                                        strcpy(idUser1lm,idUser1File);
                                        strcpy(idUser2lm,idUser2File);
                                        strcpy(scorlm,scorFile);
                                    }
                                }
                            }

                   }
                    else
                        reader.raiseError(QObject::tr("Incorrect file"));
            if(!gasit) //daca nu am gasit meciul, inseamna ca playerul ori a fost eliminat din competite, ori a castigat acea competitie
            {
                if(strcmp(idUser,idUser1lm) && strcmp(idUser,idUser2lm))
                strcpy(deTrimis,"-1");
                else
                {
                   char winner[100],loser[100],*p;
                   int nr1,nr2;
                   p=strchr(scorlm,'-');
                   scorlm[p-scorlm]='\0';
                   nr1=atoi(scorlm);
                   scorlm[p-scorlm]='-';
                   strcpy(scorlm,p+1); nr2=atoi(scorlm);
                   if(nr1>nr2)
                       strcpy(winner,idUser1lm),strcpy(loser,idUser2lm);
                   else
                       strcpy(winner,idUser2lm),strcpy(loser,idUser1lm);
                   if(strcmp(winner,idUser)==0)
                       strcpy(deTrimis,"-2");
                   else
                       strcpy(deTrimis,"-1");
                }
            }
        }
        else if(strstr(msg,"[11]")) //actualizare scor
        {
            char idTurneu[100],idPlayer1[100],idPlayer2[100],indexMeci[100],score[100];
            char winner[100],loser[100];
            bool gasitMeci=true,amModificat=false,suntInTurneu=true,winnerIndex1=false,winnerIndex2=false,loserIndex1=false,loserIndex2=false;
            int prim=-1;
            prelucrare_data(msg,x);
            strcpy(idTurneu,x[0].data);
            strcpy(idPlayer1,x[1].data);
            strcpy(idPlayer2,x[2].data);
            strcpy(indexMeci,x[3].data);
            strcpy(score,x[4].data);
            strcpy(idTurneu,idTurneu+4);

            QFile file2("meciuri.xml");
            if(!file2.open(QFile::ReadOnly | QFile::Text))
            {
                qDebug() << "Cannot read file" << file2.errorString();
                exit(0);
            }
            QDomDocument document2;
            if (!document2.setContent(&file2))
                {
                    qDebug() << "failed to parse file";
                    file2.close();
                    exit(0);
                }
            file2.close();

            QDomElement docEle2 = document2.documentElement();
            QDomNodeList elements2 = docEle2.childNodes();
            for(int i=0;i<elements2.size();++i)
            {
               QDomNode meci;
               QDomNodeList atribute;
               meci=elements2.at(i);
               atribute=meci.childNodes();
               gasitMeci=true;
               for(int j=0;j<atribute.count() && gasitMeci;++j)
               {
                   if(atribute.at(j).nodeName()=="indexmeci")
                     {
                       if( atribute.at(j).firstChild().nodeValue()!=QString(indexMeci) )
                           gasitMeci=false;

                     }
                   else if(atribute.at(j).nodeName()=="idcamp")
                   {
                       if( atribute.at(j).firstChild().nodeValue()!=QString(idTurneu) )
                           gasitMeci=false;
                       else if(prim==-1)
                           prim=i;
                   }
                   else if(atribute.at(j).nodeName()=="idplayer1")
                   {
                       if( atribute.at(j).firstChild().nodeValue()!=QString(idPlayer1) )
                           gasitMeci=false;
                   }
                   else if(atribute.at(j).nodeName()=="idplayer2")
                   {
                       if( atribute.at(j).firstChild().nodeValue()!=QString(idPlayer2) )
                           gasitMeci=false;
                   }
                   else if(atribute.at(j).nodeName()=="scor")
                   {
                       if( atribute.at(j).firstChild().nodeValue()=="#" && gasitMeci)
                            {
                           atribute.at(j).firstChild().setNodeValue(score);
                           amModificat=true;
                           int nr1,nr2;
                           char *p;
                           p=strchr(score,'-'); score[p-score]='\0';
                           nr1=atoi(score);
                           score[p-score]='-'; strcpy(score,p+1);
                           nr2=atoi(score);
                           if(nr1>nr2)
                               strcpy(winner,idPlayer1),strcpy(loser,idPlayer2);
                           else
                               strcpy(winner,idPlayer2),strcpy(loser,idPlayer1);
                            }
                   }
               }
               if(amModificat)
               {
                   break;
               }
            }
            if(amModificat)
            {
                for(int i=prim;i<elements2.size();++i)
                {
                   QDomNode meci;
                   QDomNodeList atribute;
                   meci=elements2.at(i);
                   atribute=meci.childNodes();
                   suntInTurneu=true;
                   winnerIndex1=false,winnerIndex2=false,loserIndex1=false,loserIndex2=false;
                   for(int j=0;j<atribute.count() && suntInTurneu ;++j)
                   {
                       if(atribute.at(j).nodeName()=="idcamp")
                       {
                           if( atribute.at(j).firstChild().nodeValue()!=QString(idTurneu) )
                               suntInTurneu=false;

                       }
                       else if (atribute.at(j).nodeName()=="indexmeci1")
                       {
                             if( strchr(atribute.at(j).firstChild().nodeValue().toStdString().c_str(),'W') )
                             {
                                      char *p,aux[100];
                                      strcpy(aux,atribute.at(j).firstChild().nodeValue().toStdString().c_str());
                                      p=strchr(aux,'W');
                                      aux[p-aux]='\0';
                                      if(strcmp(indexMeci,aux)==0)
                                      {
                                        winnerIndex1=true;
                                      }
                             }
                             else if( strchr(atribute.at(j).firstChild().nodeValue().toStdString().c_str(),'L') )
                             {
                                 char *p,aux[100];
                                 strcpy(aux,atribute.at(j).firstChild().nodeValue().toStdString().c_str());
                                 p=strchr(aux,'L');
                                 aux[p-aux]='\0';
                                 if(strcmp(indexMeci,aux)==0)
                                 {
                                    loserIndex1=true;
                                 }
                             }
                             else if(strcmp(atribute.at(j).firstChild().nodeValue().toStdString().c_str(),indexMeci)==0)
                             {
                                winnerIndex1=true;
                             }
                       }
                       else if (atribute.at(j).nodeName()=="indexmeci2")
                       {
                           if( strchr(atribute.at(j).firstChild().nodeValue().toStdString().c_str(),'W') )
                           {
                                    char *p,aux[100];
                                    strcpy(aux,atribute.at(j).firstChild().nodeValue().toStdString().c_str());
                                    p=strchr(aux,'W');
                                    aux[p-aux]='\0';
                                    if(strcmp(indexMeci,aux)==0)
                                    {
                                       winnerIndex2=true;
                                    }
                           }
                           else if( strchr(atribute.at(j).firstChild().nodeValue().toStdString().c_str(),'L') )
                           {
                               char *p,aux[100];
                               strcpy(aux,atribute.at(j).firstChild().nodeValue().toStdString().c_str());
                               p=strchr(aux,'L');
                               aux[p-aux]='\0';
                               if(strcmp(indexMeci,aux)==0)
                               {
                                  loserIndex2=true;
                               }
                           }
                           else if(strcmp(atribute.at(j).firstChild().nodeValue().toStdString().c_str(),indexMeci)==0)
                           {
                              winnerIndex2=true;
                           }
                       }
                       else if(atribute.at(j).nodeName()=="idplayer1")
                       {
                           if(winnerIndex1)
                           {
                              atribute.at(j).firstChild().setNodeValue(winner);
                           }
                           else if(loserIndex1)
                           {
                             atribute.at(j).firstChild().setNodeValue(loser);
                           }


                       }
                       else if(atribute.at(j).nodeName()=="idplayer2")
                       {
                           if(winnerIndex2)
                           {
                             atribute.at(j).firstChild().setNodeValue(winner);
                           }
                           else if(loserIndex2)
                           {
                             atribute.at(j).firstChild().setNodeValue(loser);
                           }
                       }
                   }
                }
            }

            QFile outFile2( "meciuri.xml" );
            if( !outFile2.open( QIODevice::WriteOnly | QIODevice::Text ) )
            {
                qDebug( "Failed to open file for writing." );
                exit(0);
            }

            QTextStream stream2( &outFile2 );
            stream2 << document2.toString();

            outFile2.close();

            if(amModificat)
                strcpy(deTrimis,"1");
            else
                strcpy(deTrimis,"0");
        }
        else if(strcmp(msg,"quit")==0)
        {
             break;
        }
        else
        {
           qDebug()<<"unknown request from client"<<'\n';
        }
        if(!requestTournaments)
        {
        printf("[server]Trimitem mesajul inapoi...%s\n",deTrimis);

        if (write (client, deTrimis, 100) <= 0)
        {
            perror ("[server]Eroare la scriere catre client:");
            return -1;
        }
        else
            printf ("[server]Mesajul a fost trasmis cu succes.\n");
        }
//        if(strcmp(commandParameter,"quit")==0)
//        {
//            break;
//        }


    }
}
