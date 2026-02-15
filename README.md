## Projet

Communication interprocessus entre un client et un serveur simple pour accomplir des tâches de consultation et de réservation.

```mermaid
sequenceDiagram

    Client_1 Client_2->>Server: Envoi un type de requête (consultation, réservation)
    Server-->>Client_1 Client_2: Répond selon le type de requête
```

```mermaid
sequenceDiagram
Fred->>Jill:Hello my Snookums
note over Fred:True Love
Jill->>Fred:Oh my Darling!
note over Jill:True Love Returned

%%{init:{'theme':'forest'}}%%
%%{init:{'themeCSS':'.messageLine0:nth-of-type(2) { stroke: red; };.messageText:nth-of-type(1) { fill: green; font-size: 30px !important;}; g:nth-of-type(3) rect.actor { stroke:blue;fill: pink; }; g:nth-of-type(5) .note { stroke:blue;fill: crimson; };#arrowhead path {stroke: blue; fill:red;};'}}%%
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


## Etape 1 : Traduction du C en Java



## Etape 2 : Mise en place en place d'une 1 ère communication client-serveur avec une communication inter-processus 


## Etape 2 : Implémentation des threads


Après avoir mise en place ue communication inter-processus par tubes avec des processus lourds, nous avons utilisées des threads pour répondre
à de nouvelles contraintes du cahier des charges de l'exercice.

L'avantage des threads est en effet la mémoire partagée qui permet aux processus de commmuniquer directement et rapidement sans objets complexes.
Cela nou a permis de simplifier l'échange de données par rapport à la première implémentation et première version que nous avions avec un échange avec un client et un serveur fils et père échangeant avec les tubes.

Ici, nous n'avons pas eu besoin de copie; En effet, chaque fichier a directement pris un rôle de client et de serveur en envoyant et réceptionnant les données dans une **communication bidirectionnelle**.


## Etape 3 : Manipulation des threads


Inconvénient : nécessite synchronisation (sémaphores).

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
