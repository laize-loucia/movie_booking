#include <stdio.h>
#include "common.h"
#include "var.h"

// v4

//serveur

//structure unique qui nous permettrait de stocker
//lit dans la file de msg mais s ne sait pas si il dot stocker dans une req de s ou de c dans le type commun permet de dire dans quelle structure je le stocke
// choix applicatif 1 ou 2 pour r ou c et tester ces champs depuis serveur

// Contenu : afficher le nombre de places


int main(int argc, char **argv){
    int msqid, i; // compteur pour gérer la réservation

    struct requete_cr requete_cr;
    struct reponse_cr reponse_cr;

    int places[3] = {50, 30, 20};

    // récuperation de le l'ID de la  MSQ ???
    msqid = msgget((key_t)CLE, 0750 | IPC_CREAT);

    printf("Serveur démarré, en attente de requêtes...\n");
    //fflush(stdout);

    while(1)
    {

        // recevoir la requete
        // cette ligne s'occupe de de traiter le type de requete et de reponse
        msgrcv(msqid, &requete_cr, sizeof(requete_cr)-sizeof(long), 11, 0);

        // Condition pour tester quel est le type de la requete envoyé
        //une r ou c donc tester la valeur de le type (choix encodage) (déterminer le type de req recu)


        //vérifier si requete de type consultation
        if(requete_cr.typerequete == 0) {
           printf("requete de type consultation"); //    requete_cr.typerequete = 0;  // type consultation
           if (requete_cr.id_film >=0 && requete_cr.id_film <=2) //ce if permet d'afficher le nb des places restantes selon l'id du film
            {
                reponse_cr.places_restantes = places[requete_cr.id_film];

                printf("Nb de places restantes pour ce film: %d \n", reponse_cr.places_restantes);
            }


        }
        //vérifier si requete de réservation
        if(requete_cr.typerequete == 1) {
            printf("requete de type reservation"); //    requete_cr.typerequete = 1;  // type réservation
            //réservation
         if (requete_cr.id_film >=0 && requete_cr.id_film <=2 &&
                places[requete_cr.id_film] >= requete_cr.nb_places)

            {
                printf("réservation acceptée, %d places restantes\n",reponse_cr.places_restantes);
                places[requete_cr.id_film] = places[requete_cr.id_film] - requete_cr.nb_places;

                reponse_cr.places_restantes = places[requete_cr.id_film];

                printf("Nb de places restantes pour ce film: %d  \n", reponse_cr.places_restantes);

                //reponse_cr.etat = "O";   // ou 'N'
                //reponse_cr.etat = "N";   // ou 'N'

                strcpy(&(reponse_cr.etat), "OK");
            }
            else {
                strcpy(&(reponse_cr.etat), "NON OK");
                reponse_cr.places_restantes = -1;

                printf("réservation refusée\n");
            }
        }

        reponse_cr.letype = requete_cr.client_pid;



        //consultation
        msgsnd(msqid, &reponse_cr, sizeof(reponse_cr)-sizeof(long), 0);

    }

    return 0;
  }

/*





*/
