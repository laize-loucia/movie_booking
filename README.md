## Projet

Communication interprocessus entre un client et un serveur simple pour accomplir des tâches de consultation et de réservation.

```mermaid
sequenceDiagram

    Client_1 Client_2->>Server: Envoi une requête (consultation, réservation)
    Server-->>Client_1 Client_2: Répond à la requête
```

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

## Contexte :

Durant notre licence en informatique, nous avons réalisé en binôme ce projet en langage en C, exécuté en ligne de commande. 


## Outils 
Le C montre les bases bas niveau (sockets, mémoire, protocole).


Le Java montre que tu sais industrialiser un service réseau. 👉 Présente ça comme : « même logique protocolaire, autre stack, plus orientée production ».


- Java 17
- Spring Boot
- API REST (consultation / réservation)
- Spring Boot (meilleur pour le web)
- architecture, API REST, config propre
Stack simple et propre :


Pas forcément de DB au début (ou H2)
- Hébergement (Render / / ) 
Très simple
Supporte Spring Boot 👉 le plus recommandé pour toi
🟡 Railway (

- Fly.io (Très bon mais quotas)



## Etape 1 : Mise en place en place d'une 1 ère communication client-serveur avec une communication inter-processus 

```mermaid

sequenceDiagram
    participant C as Client (Fils)
    participant S as Serveur (Père)
    
    Note over C,S: Initialisation: pipe() + fork()
    
    Note right of C: Menu & Saisie ID
    C->>S: write(p1[1], &choix) [ID Spectacle]
    S->>S: read(p1[0]) & Recherche places
    S->>C: write(p2[1], &nb_places) [Disponibilité]
    
    Note right of C: Saisie Réservation (ID + Quantité)
    C->>S: write(p1[1], &res)
    C->>S: write(p1[1], &nbp)
    
    alt Places suffisantes
        S->>S: Mise à jour tableau (nb_places -= nbp)
        S->>C: write(p2[1], "SUCCES")
    else Places insuffisantes
        S->>C: write(p2[1], "ECHEC")
    end
    
    Note over C: close() & exit()
    S->>S: wait(NULL)
    Note over S: close() & exit()
```


    
## Etape 2 : Implémentation des threads


Après avoir mise en place ue communication inter-processus par tubes avec des processus lourds, nous avons utilisées des threads pour répondre
à de nouvelles contraintes du cahier des charges de l'exercice.

L'avantage des threads est en effet la mémoire partagée qui permet aux processus de commmuniquer directement et rapidement sans objets complexes.
Cela nou a permis de simplifier l'échange de données par rapport à la première implémentation et première version que nous avions avec un échange avec un client et un serveur fils et père échangeant avec les tubes.

Ici, nous n'avons pas eu besoin de copie; En effet, chaque fichier a directement pris un rôle de client et de serveur en envoyant et réceptionnant les données dans une **communication bidirectionnelle**.


## Etape 3 : Manipulation des threads


Inconvénient : nécessite synchronisation (sémaphores).


## Etape 4 : Synchronisation des threads


## use cases


```mermaid
sequenceDiagram
    participant Client as MCP Client
    participant Server as MCP Server
    
    Note over Client: Initialize Client
    
    Client->>+Server: HTTP POST /mcp (Initialize Request)
    Note over Server: Create new transport<br/>Generate session ID
    Server-->>-Client: HTTP 200 OK (with session ID)
    
    Note over Client: Store session ID
    
    Client->>+Server: HTTP GET /mcp (SSE Connection with session ID)
    Note over Server: Establish SSE stream
    Server-->>-Client: SSE: Connection established notification
    
    Note over Client,Server: SSE Stream is now active for server-to-client notifications
    
    Client->>+Server: HTTP POST /mcp (List Tools Request)
    Note over Server: Process request
    Server-->>-Client: HTTP 200 OK (Available Tools)
    
    Note over Client: Store available tools
    
    Client->>+Server: HTTP POST /mcp (Call Tool: single-greet)
    Note over Server: Process tool call
    Server-->>-Client: HTTP 200 OK (Single Greeting Response)
    
    Client->>+Server: HTTP POST /mcp (Call Tool: multi-greet)
    Note over Server: Process tool call
    
    Server-->>Client: SSE: First greeting notification
    Note over Server: Wait 1 second
    Server-->>Client: SSE: Second greeting notification
    Note over Server: Wait 1 second
    
    Server-->>-Client: HTTP 200 OK (Final Greeting Response)
    Server-->>Client: SSE: Streaming complete notification
    
    Client->>+Server: HTTP POST /mcp (Close Connection)
    Note over Server: Clean up resources
    Server-->>-Client: HTTP 200 OK (Connection Closed)

```

Plus “systèmes”, un peu plus technique (bien vu pour ton profil)
➡️ Render + Docker = énorme bonus DevOps.

4️⃣ Ce que ça montre pour les masters
Ça coche EXACTEMENT leurs attentes :

Réseau (client/serveur)
Systèmes (C)
Architecture applicative (Spring)
Déploiement (Docker + hébergeur)
Démarche personnelle (très apprécié)
👉 Dans ton dossier :

« Projet initial en C (client/serveur). Reprise volontaire en Java Spring Boot pour exposer le service via API REST et déploiement sur plateforme cloud. »


➡️ Fais-le en 2 étapes

### Etape 1 : Java Spring Boot local



### Etape 2 : Déploiement Render (+ Docker si possible)

l’architecture exacte
le plan de repo GitHub


### COnclusion
J'ai repris ce projet en Java Spring Boot pour exposer le service via API REST et le déploier une vraie plateformer afin de le tester en condititon réelle.


## Sources

https://www.freecodecamp.org/news/diagrams-as-code-with-mermaid-github-and-vs-code/
https://github.com/mermaid-js/mermaid/blob/develop/docs/syntax/entityRelationshipDiagram.md
https://github.com/mermaid-js/mermaid/blob/develop/README.md
