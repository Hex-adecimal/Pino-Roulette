/*
Tutti i progetti hanno una struttura client-server, usano le socket per comunicare e devono funzionare su macchine diverse.
Per il nuovo ordinamento, il server deve essere sviluppato in C e il client come applicazione Android.

Tutti i progetti dovranno essere accompagnati da un breve report (massino 10 pagine) in cui dovranno descrivere le scelte 
fatte, la struttura del sistema, eventuali test effettuati e risultati. 
Una pagina aggiuntiva deve essere dedicata ad un breve README con le istruzioni per eseguire il sistema sviluppato.

- n utenti (client) possono accedere ad un sistema di chat
- gli utenti devono potersi registrare e loggare al server (mysql o file)
- gli utenti possono create stanze e possono chattare solo con gli utenti presenti nella stanza
- gli utenti possono accedere a stanze già create
- il creatore della stanza deve poter accettare un utente che si vuole aggiungere alla stanza

user
- login x
- signup x

group_admin
- createGroup x
- addGroup x
- acceptUser x

logged_user
- sendMessage
- recvMessage

*/

#define PORT 4200
#define BUFFER_SIZE 1024

#define MAX_QUERY_LEN 100
#define MAX_QUEUE_LEN 100
#define MAX_USERNAME_LEN 100
#define MAX_PASSWORD_LEN 100

#define DB_NAME ":memory:"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>

#include <netinet/in.h>

#include <sqlite3.h>

db_error(sqlite3 *db) {
    fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
}

int login(char *username, char *password) {
    sqlite3 *db;
    sqlite3_stmt *result;

    char *query = malloc(sizeof(char) * MAX_QUERY_LEN);
    sprintf(query, "SELECT COUNT(*) FROM user WHERE username = '%s' AND password = '%s';", username, password);

    if ( sqlite3_open(DB_NAME, &db) != SQLITE_OK ) db_error(db);

    if ( sqlite3_prepare_v2(db, query, -1, &result, 0) != SQLITE_OK ) db_error(db);

    if ( sqlite3_step(result) != SQLITE_ROW ) db_error(db);

    int ret = (int)sqlite3_column_int(result, 0);
    
    sqlite3_finalize(result);
    sqlite3_close(db);

    return ret;
}

int signup(char *username, char *password) {
    if ( login(username, password) != 0 ) {
        printf("Account already exists!");
        return 0;
    }

    // Missing the case in which the username is already used!

    sqlite3 *db;
    sqlite3_stmt *result;

    char *query = malloc(sizeof(char) * MAX_QUERY_LEN);
    sprintf(query, "INSERT INTO user VALUE ('%s', '%s');", username, password);

    if ( sqlite3_open(DB_NAME, &db) != SQLITE_OK ) db_error(db);

    if ( sqlite3_prepare_v2(db, query, -1, &result, 0) != SQLITE_OK ) db_error(db);

    if ( sqlite3_step(result) != SQLITE_ROW ) db_error(db);

    int ret = (int)sqlite3_column_int(result, 0);
    
    sqlite3_finalize(result);
    sqlite3_close(db);

    return ret;
}

int createGroup(char *name, char *username) {
    sqlite3 *db;
    sqlite3_stmt *result;

    char *query = malloc(sizeof(char) * MAX_QUERY_LEN);
    sprintf(query, "INSERT INTO group(group_name, admin_username) VALUES ('%s', '%s');", name, username);

    if ( sqlite3_open(DB_NAME, &db) != SQLITE_OK ) db_error(db);

    char *err_msg;
    if ( sqlite3_exec(db, query, 0, 0, &err_msg) != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(db);
    }

    sqlite3_finalize(result);
    sqlite3_close(db);
}

int addChat(char *username, int group_id) {
    sqlite3 *db;
    sqlite3_stmt *result;

    char *query = malloc(sizeof(char) * MAX_QUERY_LEN);
    sprintf(query, "INSERT INTO queue_for_group VALUES ('%s', %d);", username, group_id);

    if ( sqlite3_open(DB_NAME, &db) != SQLITE_OK ) db_error(db);

    char *err_msg;
    if ( sqlite3_exec(db, query, 0, 0, &err_msg) != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(db);
    }

    sqlite3_finalize(result);
    sqlite3_close(db);
}

int acceptUser(char *admin_username, int group_id) {
    // Show every one queued to that group

    // Accept one
}

int sendMessage(char *sender, int group_id, char *actual_message) {
    // Insert into
}

int recvMessage(int group_id) {

}   

void server(int connected_fd, struct sockaddr peer, int addr_len) {
    char buffer[BUFFER_SIZE];
    int bytes_recived;

    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];

    while (1) {
        bzero(buffer, sizeof(buffer) * BUFFER_SIZE);
        bytes_recived = recvfrom(connected_fd, buffer, BUFFER_SIZE, 0, &peer, &addr_len);
        buffer[bytes_recived] = '\0';

        if ( strcmp(buffer, "login") == 0 ) {
            bytes_recived = recvfrom(connected_fd, buffer, BUFFER_SIZE, 0, &peer, &addr_len);
            buffer[bytes_recived] = '\0';
            stpcpy(username, buffer);

            bytes_recived = recvfrom(connected_fd, buffer, BUFFER_SIZE, 0, &peer, &addr_len);
            buffer[bytes_recived] = '\0';
            stpcpy(password, buffer);

            login(username, password);
            // Send something back
        }

        if ( strcmp(buffer, "signup") == 0 ) {
            bytes_recived = recvfrom(connected_fd, buffer, BUFFER_SIZE, 0, &peer, &addr_len);
            buffer[bytes_recived] = '\0';
            stpcpy(username, buffer);

            bytes_recived = recvfrom(connected_fd, buffer, BUFFER_SIZE, 0, &peer, &addr_len);
            buffer[bytes_recived] = '\0';
            stpcpy(password, buffer);

            signup(username, password);
            // Send something back
        }

        if ( strcmp(buffer, "createGroup") == 0 ) {
            bytes_recived = recvfrom(connected_fd, buffer, BUFFER_SIZE, 0, &peer, &addr_len);
            buffer[bytes_recived] = '\0';

            createGroup(buffer, username);
            // Send something back
        }

        if ( strcmp(buffer, "addChat") == 0 ) {
            bytes_recived = recvfrom(connected_fd, buffer, BUFFER_SIZE, 0, &peer, &addr_len);
            buffer[bytes_recived] = '\0';

            addChat(username, atoi(buffer));
            // Send something back
        }

        if ( strcmp(buffer, "acceptUser") == 0 ) {
            bytes_recived = recvfrom(connected_fd, buffer, BUFFER_SIZE, 0, &peer, &addr_len);
            buffer[bytes_recived] = '\0';

            acceptUser(username, buffer);
            // Send something back
        }

        if ( strcmp(buffer, "sendMessage") == 0 ) {
            bytes_recived = recvfrom(connected_fd, buffer, BUFFER_SIZE, 0, &peer, &addr_len);
            buffer[bytes_recived] = '\0';
            int group_id = atoi(buffer);

            bytes_recived = recvfrom(connected_fd, buffer, BUFFER_SIZE, 0, &peer, &addr_len);
            buffer[bytes_recived] = '\0';

            sendMessage(username, group_id, buffer);
            // Send something back
        }
            

        if ( strcmp(buffer, "recvMessage") == 0 ) {
            bytes_recived = recvfrom(connected_fd, buffer, BUFFER_SIZE, 0, &peer, &addr_len);
            buffer[bytes_recived] = '\0';
            int group_id = atoi(buffer);

            recvMessage(group_id);
            // Send something back
        }
    }
}

int main(int argc, char *argv[]){
    int listen_fd, connected_fd, addr_len;
    struct sockaddr_in servaddr;
    struct sockaddr peer;

    // Questa system-call serve ad instanziare un nuovo descrittore di socket.
    printf("Creating the socket...");
    if ( (listen_fd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ) 
        err_sys("socket error");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    
    printf("Binding the socket...");
    // Questa system-call serve ad assegnare un indirizzo locale (name) ad una socket.
    if ( bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) 
        err_sys("bind error");
    
    printf("Listening...");
    // Attraverso la chiamata listen(), un server manifesta la sua volontà di apprestarsi a ricevere connessioni.
    if ( listen(listen_fd, MAX_QUEUE_LEN) < 0 ) 
        err_sys("listen error");

    int current_iteration = 0;
    while (1) {
        // Dopo la chiamata listen(), un server si mette realmente in attesa di connessioni attraverso una chiamata ad accept().
        printf("Accepting a connection...");
        bzero(&peer, sizeof(peer));
        connected_fd = accept(listen_fd, &peer, &addr_len);
        if ( connected_fd < 0 )
            err_sys("accept error at iteration %d", current_iteration);

        // Dopo l’accept() il descrittore connected_fd ha la quintupla tutta impostata, ed è pronto ad essere utilizzato.
        // La quintupla è (protocol, local-addr, local-process, foreign-addr, foreign-process)

        if ( fork() == 0 ) {
            close(listen_fd); /* child */

            server(connected_fd, peer, addr_len); /* process the request */

            close(connected_fd);
            exit(0);
        }

        // Chiude una socket e rilascia le risorse ad essa associate.
        close(connected_fd); /* parent */
        current_iteration++;
    }
}