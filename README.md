# Réservation de Films (Janvier - Avril 2026)

Application CLI réalisé en binôme en 3ème année de licence informatique.  
J'ai utilisé 3 outils de communication différents, ce qui m'a permis de comprendre quels mécanismes mettre en oeuvre lors des interactions client serveur.  

## Architecture commune

Chacunes des versions repose sur l'architecture suivante :  

```mermaid
sequenceDiagram

    Client_1 Client_2->>Server: Envoi une requête (consultation, réservation)
    Server-->>Client_1 Client_2: Répond à la requête
```
## Commandes


``` gcc serveur.c -o serveur ```

```./serveur```


# Communication par Tubes Anonymes

## Etape 1 : Mise en place en place d'une 1 ère communication client-serveur avec une communication inter-processus 


```mermaid
graph LR
    subgraph Client
    F[Processus Fils]
    end
    
    subgraph Pipes
    P1[Tube P1: Requêtes]
    P2[Tube P2: Réponses]
    end
    
    subgraph Serveur
    P[Processus Père]
    end
    
    F -- write p1-1 --> P1 -- read p1-0 --> P
    P -- write p2-1 --> P2 -- read p2-0 --> F
 ```

```mermaid

sequenceDiagram
    participant C as Client (Fils)
    participant S as Serveur (Père)
    C->>S: ID Spectacle (Consultation)
    S->>C: Nb Places restantes
    C->>S: ID + Nb Places (Réservation)
    Note over S: Mise à jour tableau
    S->>C: Statut (SUCCES/ECHEC)
```
Communication interprocessus entre un client et un serveur simple pour accomplir des tâches de consultation et de réservation.


# Communication par Files de Messages 

## Etape 2 : Implémentation des threads


Après avoir mise en place ue communication inter-processus par tubes avec des processus lourds, nous avons utilisées des threads pour répondre
à de nouvelles contraintes du cahier des charges de l'exercice.

L'avantage des threads est en effet la mémoire partagée qui permet aux processus de commmuniquer directement et rapidement sans objets complexes.
Cela nou a permis de simplifier l'échange de données par rapport à la première implémentation et première version que nous avions avec un échange avec un client et un serveur fils et père échangeant avec les tubes.

Ici, nous n'avons pas eu besoin de copie; En effet, chaque fichier a directement pris un rôle de client et de serveur en envoyant et réceptionnant les données dans une **communication bidirectionnelle**.




# Communication multithread avec Files de Messages 

## Etape 3 : Traitement avec des threads spécialisés pour la consultation et la réservation

Inconvénient : nécessite synchronisation (sémaphores).



# Communication multithread verrouillée par un Sémaphore
## Etape 4 : Synchronisation des threads


```mermaid
flowchart TB

Client1 -->|requete type 10| MSQ
Client2 -->|requete type 11| MSQ
Client3 -->|requete type 11| MSQ

MSQ --> Serveur

Serveur -->|pthread_create| Thread1
Serveur -->|pthread_create| Thread2
Serveur -->|pthread_create| Thread3

Thread1 -->|sem_wait| SEM
Thread2 -->|attend si SEM=0| SEM
Thread3 -->|attend si SEM=0| SEM

SEM --> places
places --> SEM

Thread1 -->|sem_post| SEM
Thread2 -->|sem_post| SEM
Thread3 -->|sem_post| SEM

Thread1 -->|reponse pid| MSQ
Thread2 -->|reponse pid| MSQ
Thread3 -->|reponse pid| MSQ

MSQ --> Clients
```


```mermaid
sequenceDiagram
participant ClientA
participant Serveur
participant ThreadA
participant SEM
participant Base

ClientA->>Serveur: requete
Serveur->>ThreadA: pthread_create
ThreadA->>SEM: sem_wait()
SEM-->>ThreadA: accès autorisé
ThreadA->>Base: lecture/modification places[]
ThreadA->>SEM: sem_post()
ThreadA->>Serveur: envoie réponse
Serveur->>ClientA: réponse
```


## Sources

https://www.freecodecamp.org/news/diagrams-as-code-with-mermaid-github-and-vs-code/
https://github.com/mermaid-js/mermaid/blob/develop/docs/syntax/entityRelationshipDiagram.md
https://github.com/mermaid-js/mermaid/blob/develop/README.md
