#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define CLE 12

// les structures de messages
struct requete_cr {
    long letype;           // 10 = consultation, 11 = reservation
    int id_film;
    int nb_places;
    pid_t client_pid;
};

struct reponse_cr {
    long letype;           // pid du client
    char etat;            // permet de voir si la demander est ok : 'O' ou non ok :'N'
    int places_restantes;
};

//Variables globales partagées

int places[3] = {50, 30, 20};
int msqid;

//Sémaphore POSIX qui va garantir l'exlusion mutuelle
sem_t sem;


//Fonction exécutée par chaque thread
void *traitement(void *arg)
{
    //On copie la requête reçue
    struct requete_cr requete = *(struct requete_cr *)arg;
    free(arg); // Libération de la mémoire allouée dynamiquement

    struct reponse_cr reponse;

    //SECTION CRITIQUE
    sem_wait(&sem); //décrémente le sémaphore et permet de fermer le verrou pour que personne a accès à la ressource partagée rapidement

    //Simulation d'un traitement long pour voir le blocage
    printf("Thread pour client %d : Je verrouille la base pendant 5s...\n", requete.client_pid);
    sleep(5); //il va juste testé si le blocage est fait sur un autre client qui veut accèder au nb places

    //les if permet à un seul thread d'accéder aux places_restantes


    //consultation
    if (requete.letype == 10)
    {
        if (requete.id_film >= 0 && requete.id_film <= 2)
        {
            reponse.places_restantes = places[requete.id_film];
            reponse.etat = 'O';
        }
        else
        {
            reponse.places_restantes = -1;
            reponse.etat = 'N';
        }
    }

    //réservation
    if (requete.letype == 11)
    {
        if (requete.id_film >= 0 && requete.id_film <= 2
            && places[requete.id_film] >= requete.nb_places)
        {
            reponse.places_restantes = places[requete.id_film];

            printf("Nb avant reservation : %d\n",
                   reponse.places_restantes);

            places[requete.id_film] =
                places[requete.id_film] - requete.nb_places;

            reponse.places_restantes = places[requete.id_film];

            printf("Nb apres reservation : %d\n",
                   reponse.places_restantes);

            reponse.etat = 'O';
        }
        else
        {
            reponse.places_restantes = -1;
            reponse.etat = 'N';
        }
    }

       //FIN SECTION CRITIQUE
    sem_post(&sem); //incrémente le sémaphore et elle libère l'accès pour un autre thread.

    //envoi de la réponse au client à l'aide de son pid
    reponse.letype = requete.client_pid;

    if (msgsnd(msqid, &reponse,
               sizeof(reponse) - sizeof(long),
               0) == -1)
    {
        perror("erreur msgsnd");
    }

    pthread_exit(NULL); //ferme le thread
}




//Fonction principale
int main()
{
    pthread_t th;
    struct requete_cr *requete_ptr;

    //récupérer le msqid reçue
    msqid = msgget((key_t)CLE, 0666 | IPC_CREAT);
    if (msqid == -1)
    {
        perror("Erreur msgget");
        exit(EXIT_FAILURE);
    }

    // on initialise le sémaphore, si l'argument est à 0 : il est partagé entre les threads du même processus
    //ou
    //s'il est à 1 : c'est une valeur initiale qui donne l'accès autorisé à 1 thread à la fois
    if (sem_init(&sem, 0, 1) == -1)
    {
        perror("Erreur sem_init");
        exit(EXIT_FAILURE);
    }

    printf("Serveur démarré...\n");

    while(1)
    {
        //malloc est une allocation dynamique pour éviter que les threads se partagent la même variable
        requete_ptr = malloc(sizeof(struct requete_cr));

        //On reçoit tous les types de message
        if (msgrcv(msqid, requete_ptr,
                   sizeof(struct requete_cr) - sizeof(long),
                   0, 0) == -1)
        {
            perror("Erreur msgrcv");
            free(requete_ptr);
            continue;
        }

        //Création dynamique d’un thread et chaque requête aura son propre thread
        if (pthread_create(&th, NULL,
                           traitement,
                           requete_ptr) != 0)
        {
            perror("Erreur pthread_create");
            free(requete_ptr);
            continue;
        }

        //On détache le thread pour éviter une fuite de mémoire
        pthread_detach(th);
    }

    //il va supprimer le semaphore existant, plus utilisé
    sem_destroy(&sem);
    return 0;
}
