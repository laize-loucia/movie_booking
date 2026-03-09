#ifndef VAR_H
#define VAR_H

#include <sys/types.h>

#define CLE 12

// le serveur doit prelever que les msg de requete ce qui se ait avec le type
struct requete_cr {
    long letype; // 11 : message requete
    int typerequete; // type de requete si  0 = consultation ou 1 = reservation
    int id_film;
    int nb_places;
    pid_t client_pid;
};

struct reponse_cr {
    long letype; //12 : message reponse
    char etat;
    int places_restantes; // nombre de places restantes après réservation
};


#endif


/*
// for header files in system/default
// directory
#include <filename.h>   

// for Header files in same directory as
// source file
#include "filename.h"   
// for header files in system/default
// directory
#include <filename.h>   

// for Header files in same directory as
// source file
#include "filename.h"   
*/



