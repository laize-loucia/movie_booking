#include "common.h"
#include "var.h"



int main() {
    int msqid, rep;
    pid_t pid = getpid();//traitement de requete

    struct requete_cr requete_cr;
    struct reponse_cr reponse_cr;

    msqid = msgget((key_t)CLE, 0);

    // afficher le menu côté client
    printf("----- MENU FILMS -----\n");
    printf("0 - Aladdin\n");
    printf("1 - La princesse camomille\n");
    printf("2 - Les Aristochats\n");

    //verifer etat de la variable etat
    //printf(etat);
    printf("quel type de requete voulez vous faire : 0 = consultation, 1 = reservation: \n");
    scanf("%d", &rep);

    if (rep == 0) {

        printf("Numéro du film (0-2) : ");
        scanf("%d", &requete_cr.id_film);


        //valeurs de traitement envoyé par le client
        requete_cr.letype = 11;  // traitement de requete
        requete_cr.typerequete = 0;  // type consultation //         //segmenter les msg de c ou de r pour permettre au serveur de distinguer quel traitement faire
        requete_cr.client_pid = pid;


        // envoi de la requete de type consultation
        //c'est une option qu'on garde
        msgsnd(msqid, &requete_cr, sizeof(requete_cr)-sizeof(long), 0);

        // réception de la réponse
        //par rapport a son pid
        msgrcv(msqid, &reponse_cr, sizeof(reponse_cr)-sizeof(long), pid, 0);
        printf("Consultation : %d \n", reponse_cr.etat);
        printf("Places restantes pour ce film : %d\n", reponse_cr.places_restantes);

    }
    if (rep == 1) {

        printf("Numéro du film à réserver (0-2) : ");
        scanf("%d", &requete_cr.id_film);

        printf("Nombre de places : ");
        scanf("%d", &requete_cr.nb_places);

        //valeurs de traitement envoyé par le client
        requete_cr.letype = 11;  // traitement de requete
        requete_cr.typerequete = 1;  // type reservation //         //segmenter les msg de c ou de r pour permettre au serveur de distinguer quel traitement faire
        requete_cr.client_pid = pid;


        // envoi de la requete de type consultation
        //c'est une option qu'on garde
        msgsnd(msqid, &requete_cr, sizeof(requete_cr)-sizeof(long), 0);

        // réception de la réponse
        //par rapport a son pid
        msgrcv(msqid, &reponse_cr, sizeof(reponse_cr)-sizeof(long), pid, 0);

         printf("Réservation : %c\n", reponse_cr.etat);
        printf("Places restantes pour ce film : %d\n", reponse_cr.places_restantes);
    }

    return 0;
}
