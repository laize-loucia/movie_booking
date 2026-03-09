#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>

#define CLE 12

// le serveur doit prelever que les msg de requete ce qui se fait avec le type
struct requete_cr {
    long letype; // type du message  (10 = consultation, 11 = reservation)
    int id_film;
    int nb_places;
    pid_t client_pid;
};

struct reponse_cr {
    long letype; // message reponse (pid du client)
    char etat;
    int places_restantes; // nombre de places restantes après réservation
};


int main() {

    int msqid, rep;
    pid_t pid = getpid(); // traitement de requete

    struct requete_cr requete;
    struct reponse_cr reponse;

    msqid = msgget((key_t)CLE, 0);

    // afficher le menu côté client
    printf("----- MENU FILMS -----\n");
    printf("0 - Aladdin\n");
    printf("1 - La princesse camomille\n");
    printf("2 - Les Aristochats\n");

    printf("quel type de requete voulez vous faire : 0 = consultation, 1 = reservation: \n");
    scanf("%d", &rep);

    if (rep == 0) {

        printf("Numéro du film (0-2) : ");
        scanf("%d", &requete.id_film);

        // valeurs de traitement envoyé par le client
        requete.letype = 10;  // type consultation
        requete.nb_places = 0; // pas utilisé en consultation
        requete.client_pid = pid;

        // envoi de la requete
        msgsnd(msqid, &requete, sizeof(requete)-sizeof(long), 0);

        // réception de la réponse par rapport a son pid
        msgrcv(msqid, &reponse, sizeof(reponse)-sizeof(long), pid, 0);

        printf("Consultation : %c \n", reponse.etat);
        printf("Places restantes pour ce film : %d\n",
               reponse.places_restantes);
    }

    if (rep == 1) {

        printf("Numéro du film à réserver (0-2) : ");
        scanf("%d", &requete.id_film);

        printf("Nombre de places : ");
        scanf("%d", &requete.nb_places);

        // valeurs de traitement envoyé par le client
        requete.letype = 11;  // type reservation
        requete.client_pid = pid;

        // envoi de la requete
        msgsnd(msqid, &requete, sizeof(requete)-sizeof(long), 0);

        // réception de la réponse par rapport a son pid
        msgrcv(msqid, &reponse, sizeof(reponse)-sizeof(long), pid, 0);

        printf("Réservation : %c\n", reponse.etat);
        printf("Places restantes pour ce film : %d\n",
               reponse.places_restantes);
    }

    return 0;
}




