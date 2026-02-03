
## Contexte du projet :


Communication interprocessus entre un client et un serveur simple pour accomplir des tâches de consultation et de réservation.


```mermaid
sequenceDiagram
    Client
    Server
    Client->>Server: Make a reservation. 
    activate Server
    Server-->>Client: Display the places left
    if = 0 then display No more reservation possible for this movie.
    deactivate Server

```


## Etape 1 : Traduction du C en Java


## use cases

```mermaid
sequenceDiagram
    Client
    Server
    Client->>Server: Register user
    activate Server
    Server-->>Client: User already exists.
    deactivate Server

```



## Sources

https://www.freecodecamp.org/news/diagrams-as-code-with-mermaid-github-and-vs-code/
