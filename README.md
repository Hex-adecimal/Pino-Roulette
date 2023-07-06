# Pino-Roulette
Questo progetto, sviluppato Laboratorio di Sistemi Operativi, implementa un sistema di comunicazione client-server utilizzando un server scritto in C e un'applicazione client per dispositivi Android. Lo scopo del progetto è dimostrare i concetti fondamentali dell'architettura client-server e della programmazione delle socket.

Il componente server è implementato nel linguaggio di programmazione C, utilizzando tecniche di programmazione socket. Fornisce una piattaforma solida e scalabile per gestire le richieste dei client e facilitare la comunicazione tra il server e più client contemporaneamente. Il server funge da hub centrale, consentendo ai client di stabilire connessioni, scambiare dati ed eseguire varie operazioni.

Lato client, è stata sviluppata un'applicazione Android per interagire con il server. L'applicazione client offre un'interfaccia utente intuitiva che consente agli utenti di visualizzare e inviare messaggi, accedere e creare gruppi. Attraverso l'app client, gli utenti possono avviare trasferimenti di dati, richiedere operazioni lato server e interagire con il server in tempo reale.

## Usage
Prima di tutto bisogna assicurarsi di avere il database 'pino-roulette.db' all'interno della cartella dove si trova il server. In alternativa è possibile copiare il contentuo del file 'sample.sql' all'interno di Visual Studio Code, scaricare l'estensione 'SQLite', e cliccare command + P e successivamente '>'. Poi potremmo eseguire le query. Per poter selezionare un database su cui eseguire le query, è possibile farlo in basso a destra, di default dovrebbe esserci scritto 'SQLite:memory:'. 

Per usare il progetto seguire questi passi:
- Eseguire il comando 'gcc server.c -l sqlite3' per ottenere l'eseguibile 'a.out';
- Eseguire 'a.out' tramite il comando './a.out';
- Modificare all'interno del codice Android l'indirizzo IP del server, inserendo quello su cui sta girando il server;
- Installare l'applicazione sul dispositivo Android;
- Cliccare sull'icona dell'applicazione;

Da qui sarà possibile eseguire l'accesso oppure registrarsi, supponendo di fare il login si possono usare le seguenti credenziali:
- Username: Effeo
- Password: Ricarica

Dopo l'accesso ci troveremo in una schermata dove è possibile visualizzare i gruppi a cui si appartiene, oppure scegliere di andare in una delle seguenti schermate:
- Creazione di un nuovo gruppo;
- Richiedere l'accesso ad un gruppo già esistente;
- Gestione degli utenti che vogliono entrare in un nostro gruppo.

Per quanto riguarda la crezione di un nuovo gruppo, oppure l'accesso ad un gruppo già esistente, basta semplicemente scrivere il nome del gruppo nel form e cliccare il bottone per proseguire.

Per la gestione degli utenti, invece, ci sarà una lista dei gruppi che abbiamo creato, selezionando uno dei gruppi otterremo una lista di utenti che hanno richiesto di entrare nel gruppo. Da qui sarà possibile, tramite i due bottoni, scegliere se accettarli oppure no. 

## Acknowledgments

- https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server/
- https://www.sqlite.org/cintro.html
- https://lia.disi.unibo.it/Courses/RetiDiCalcolatori/Es_SockJ_Stream.pdf

## Frequently Asked Questions (FAQ)
- Puoi dirmi due bugie ed una verità? Certamente, (i) non è stata usata nessuna IA basata su chat per lo sviluppo di qusto progetto, (ii) non è stato usato il sito mucchio straripante per lo sviluppo di questo progetto, (iii) in C while(x=y) {...} fa loop infinito.
