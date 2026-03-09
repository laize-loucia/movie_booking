#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{

    // Déclaration des variables
    int pid;                    // ID des processus
    int p1[2];                  // tube client à serveur
    int p2[2];                  // tube serveur à client
    int nb_places[] = {50, 32, 12}; // tableau des places
    //int id_film[] = {1, 2, 3};  // ID des films
    int choix;                  // ID du film pour consultation
    int res;                    // ID du film pour réservation
    int nbp;                    // Nombre de places à réserver
    char resaServeur[10];       // Buffer pour réponse du serveur


    // Création des tubes
    if (pipe(p1)) {
        perror("pipe");
        exit(1);
    }

    if (pipe(p2)) {
        perror("pipe");
        exit(1);
    }


    // Création du processus fils
    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    // Processus fils (client)
    if (pid == 0) {
        // Fermeture des extrémités non utilisées
        close(p1[0]);
        close(p2[1]);

	//etape de consultation
        // Affichage du menu pour consultation
        printf("MENU:\n");
        printf("Aladdin - n° 1\n");
        printf("La princesse camomille - n° 2\n");
        printf("Les Aristochats - n° 3\n");
        printf("Donnez l'id du film dont vous voulez les informations:\n");
        fflush(stdout);
        scanf("%d", &choix);

        // CONSULTATION
        // Envoi de la requête au père
        write(p1[1], &choix, sizeof(choix));

        // réception de la réponse
        int places_restantes;
        read(p2[0], &places_restantes, sizeof(places_restantes));

        // Affichage du résultat
        printf("CLIENT : Places restantes = %d\n", places_restantes);

	// etape de réservation
        //  Affichage de la demande de réservation
        printf("Donnez l'id du film que vous voulez réserver:\n");
        scanf("%d", &res);
        printf("combien de places voulez vous réserver? \n");
        scanf("%d", &nbp);

        // RESERVATION
        // Envoi de la requête au père
        write(p1[1], &res, sizeof(res));
        write(p1[1], &nbp, sizeof(nbp));

        // Lecture de la réponse du père
        read(p2[0], resaServeur, sizeof(resaServeur));

        printf("Vous avez choisi de réserver le film n°%d\n", res);
        printf("ETAT DE VOTRE RESERVATION : %s\n", resaServeur);

        // Fermeture des descripteurs
        close(p1[1]);
        close(p2[0]);

        exit(0);
    }

    // Processus père (serveur)
    else {
        // Fermeture des extrémités descripteurs
        close(p1[1]);
        close(p2[0]);

        printf("SERVEUR : je suis le père\n");

        // CONSULTATION
        // Lecture de la requête du client
        int film_choisi; //creation d'une variable locale pour afficher le film
        read(p1[0], &film_choisi, sizeof(film_choisi));

        printf("SERVEUR : j'ai reçu l'id n° %d\n", film_choisi);

        // Envoi du nombre de places restantes au client
        write(p2[1], &nb_places[film_choisi - 1], sizeof(nb_places[film_choisi - 1]));

        // RESERVATION
	// lecture de la requete de réservation du client
        read(p1[0], &res, sizeof(res));
        read(p1[0], &nbp, sizeof(nbp));

        printf("SERVEUR : Réservation demandée - Film n°%d, %d places\n", res, nbp);

        //vérifier si le nb de places est pas superieure au nb de places restantes
        if (nb_places[res - 1] < nbp) {
            printf("Pas assez de places\n");
            sprintf(resaServeur, "ECHEC");
        }
        else {
            printf("Réservation faite\n");
            // Décrémenter
            nb_places[res - 1] -= nbp;
            sprintf(resaServeur, "SUCCES");
        }

        // Envoi de la réponse au client
        write(p2[1], resaServeur, sizeof(resaServeur));

        // Fermeture des descripteurs
        close(p1[0]);
        close(p2[1]);

        // Attendre la fin du processus client
        wait(NULL);

        exit(0);
    }

    return 0;
}
