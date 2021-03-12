#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
///cod server
struct comenzi
{
    char comanda[100];
} C[15];
void initializare_comenzi()
{
    strcpy(C[0].comanda,"create");
    strcpy(C[1].comanda,"login");
    strcpy(C[2].comanda,"recordChamp");
    strcpy(C[3].comanda,"setGame");
    strcpy(C[4].comanda,"setNrPlayers");
    strcpy(C[5].comanda,"setRules");
    strcpy(C[6].comanda,"setDraw");
    strcpy(C[7].comanda,"recordPlayer");
    strcpy(C[8].comanda,"reprogramMatch");
    strcpy(C[9].comanda,"quit");
}
void prelucrareInput(char commandName[], char commandParameter[], char input[]) ///functie de prelucrare al inputului
{
    char *aux;
    aux=strchr(input,' ');
    if(aux==NULL)
    {
        strcpy(commandName,input);
        commandName[strlen(commandName)-1]='\0';
        commandParameter[0]='\0';
    }
    else
    {

        input[aux-input]='\0';
        strcpy(commandName,input);
        input[aux-input]=' ';
        strcpy(commandParameter,aux+1);
        aux=strchr(aux+1,' ');
        commandParameter[strlen(commandParameter)-1]='\0';
    }
}
int gasit=0,nrComenzi=10;
#define PORT 2024
int main ()
{
    struct sockaddr_in server;
    struct sockaddr_in from;
    char msg[100],commandName[100],commandParameter[100],deTrimis[100];
    int sd, client;
    initializare_comenzi();

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
            ///wait(NULL);
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


        if (read (client, msg, 100) <= 0)
        {
            perror ("[server]Eroare la citire de la client:");
            return -1;
        }

        printf ("[server]Mesajul a fost receptionat...%s\n", msg);

        strcpy(deTrimis,"Am primit de la client username-ul ");
        strcat(deTrimis,msg);
        strcat(deTrimis,"\n");

        printf("[server]Trimitem mesajul inapoi...%s\n",deTrimis);



        if (write (client, deTrimis, 100) <= 0)
        {
            perror ("[server]Eroare la scriere catre client:");
            return -1;
        }
        else
            printf ("[server]Mesajul a fost trasmis cu succes.\n");

        if(strcmp(commandParameter,"quit")==0)
        {
            break;
        }


    }
}
