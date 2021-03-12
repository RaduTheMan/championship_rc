#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    int sd, port;
    struct sockaddr_in server;
    if (argc != 3)
        {
            printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
            return -1;
        }
    port = atoi (argv[2]);
    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror ("Eroare la socket:");
            return -1;
        }


    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons (port);

    if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
        {
            perror ("[client]Eroare la connect:");
            return -1;
        }
    w.setSoc(sd);
    return a.exec();
}
