#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>

#define CLE 12

// v5
/*
quesion 3
A présent, le processus serveur créé d’une part un thread qui traite les consultations,
et d’autre part un thread qui traite les réservations.
Chaque thread prélève les messages qui le concerne dans la MSQ,
les traite et renvoie la réponse.

Mettez en place la structure de votre application en considérant
que serveur et client sont sur une même machine.
Codez votre solution en prenant soin de documenter votre code.
*/

struct requete_cr {
    long letype; // ce champs permet de segmenter le message pour savoir si c'est une requete ou une reponse
    int id_film;
    int nb_places;
    pid_t client_pid;
};

struct reponse_cr {
    long letype; // message reponse
    char etat;   // renvoi O ou N si la réservation est faite ou non
    int places_restantes; // nombre de places restantes après réservation
};

void *consultation(void *arg);
void *reservation(void *arg);

int places[3] = {50, 30, 20};
int msqid;   // variable globale partagée entre threads

int main(int argc, char **argv)
{
    //creation des threads
    pthread_t th_consultation;
    pthread_t th_reservation;

    // récuperation de la file de message
    msqid = msgget((key_t)CLE, 0750 | IPC_CREAT);

    if (msqid == -1)
    {
        perror("Erreur msgget");
        exit(EXIT_FAILURE);
    }

    //creation du thread de consultation
    if (pthread_create(&th_consultation, NULL, consultation, NULL) != 0)
    {
        perror("Erreur pthread_create consultation");
        exit(EXIT_FAILURE);
    }

    //Création du thread réservation
    if (pthread_create(&th_reservation, NULL, reservation, NULL) != 0)
    {
        perror("Erreur pthread_create reservation");
        exit(EXIT_FAILURE);
    }

    // attente des threads
    pthread_join(th_consultation, NULL);
    pthread_join(th_reservation, NULL);

    return 0;
}


// Le while permet à chaque thread de traiter pls messages sans mourir
//Les deux threads doivent lire le type voulu de requete faite par le client dans la file de message

void *consultation(void *arg)
{
    struct requete_cr requete;
    struct reponse_cr reponse;

    printf("Serveur démarré, en attente de requêtes...\n"); //fflush(stdout);

    while(1)
    {
        // lecture message type 1 (consultation)
        if(msgrcv(msqid, &requete,
                  sizeof(requete) - sizeof(long),
                  10, 0) == -1)
        {
            perror("erreur msgrcv consultation");
            continue;
        }

        // traitement simple
        if(requete.id_film >= 0 && requete.id_film <= 2)
        {
            reponse.places_restantes = places[requete.id_film];
            reponse.etat = 'O';
        }
        else
        {
            reponse.places_restantes = -1;
            reponse.etat = 'N';
        }

        reponse.letype = requete.client_pid;

        // envoi réponse
        if(msgsnd(msqid, &reponse,
                  sizeof(reponse) - sizeof(long),
                  0) == -1)
        {
            perror("erreur msgsnd consultation");
        }
    }
}


void *reservation(void *arg)
{
    struct requete_cr requete;
    struct reponse_cr reponse;

    while(1)
    {
        // lecture message type 11 (requete du client)
        if(msgrcv(msqid, &requete,
                  sizeof(requete) - sizeof(long),
                  11, 0) == -1)
        {
            perror("erreur msgrcv reservation");
            continue;
        }

        // afficher le nb des places restantes selon l'id du film
        if (requete.id_film >= 0 && requete.id_film <= 2
            && places[requete.id_film] >= requete.nb_places)
        {
            reponse.places_restantes = places[requete.id_film];

            printf("Nb de places restantes pour ce film: %d \n",
                   reponse.places_restantes);

            printf("réservation acceptée\n");

            places[requete.id_film] =
                places[requete.id_film] - requete.nb_places;

            reponse.places_restantes = places[requete.id_film];

            printf("Nb de places restantes pour ce film: %d\n",
                   reponse.places_restantes);

            reponse.etat = 'O';
        }
        else
        {
            reponse.places_restantes = -1;
            reponse.etat = 'N';
            printf("réservation refusée\n");
        }

        reponse.letype = requete.client_pid;

        if(msgsnd(msqid, &reponse,
                  sizeof(reponse) - sizeof(long),
                  0) == -1)
        {
            perror("erreur msgsnd reservation");
        }
    }
}
