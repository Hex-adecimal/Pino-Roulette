 
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

gcc main.c -l sqlite3

- signupv2 non va
*/

#define PORT 50000
#define BUFFER_SIZE 1024

#define MAX_QUERY_LEN 500
#define MAX_QUEUE_LEN 100
#define MAX_USERNAME_LEN 100
#define MAX_GROUP_NAME_LEN 100
#define MAX_PASSWORD_LEN 100
#define MAX_MESSAGE_LEN 1000
#define MAX_DATETIME_LEN 20

#define DB_NAME "pino-roulet.db"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>

#include <netinet/in.h>

#include <sqlite3.h>

void err_sys(const char* x) {
    perror(x);
    exit(1);
}

void db_error(sqlite3 *db) {
    fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
}

void my_recv(int connected_fd, char *buffer) {
    bzero(buffer, BUFFER_SIZE);
    int bytes_recived = read(connected_fd, buffer, BUFFER_SIZE);
    printf("\tReceived %s\n", buffer); fflush(stdout);
}

void my_send(int connected_fd, char *buffer) {
    printf("\tSending %s\n", buffer); fflush(stdout);
    int len = strlen(buffer);
    write(connected_fd, buffer, strlen(buffer));
}

int nameToGroupId(char *group_name) {
    //Dato il nome del gruppo restituisce il suo id
    sqlite3 *db;
    sqlite3_stmt *result;

    char *query = malloc(sizeof(char) * MAX_QUERY_LEN);
    sprintf(query, "SELECT id FROM group_t WHERE group_name = '%s';", group_name);

    if ( sqlite3_open(DB_NAME, &db) != SQLITE_OK ) db_error(db);

    if ( sqlite3_prepare_v2(db, query, -1, &result, 0) != SQLITE_OK ) db_error(db);

    if ( sqlite3_step(result) != SQLITE_ROW ) db_error(db);

    int ret = (int)sqlite3_column_int(result, 0);

    sqlite3_finalize(result);
    sqlite3_close(db);
    free(query);

    return ret;
}

int login(char *username, char *password) {
    sqlite3 *db;
    sqlite3_stmt *result;

    char *query = malloc(sizeof(char) * MAX_QUERY_LEN);
    sprintf(query, "SELECT COUNT(*) FROM user_t WHERE username = '%s' AND pswd = '%s';", username, password);

    if ( sqlite3_open(DB_NAME, &db) != SQLITE_OK ) db_error(db);

    if ( sqlite3_prepare_v2(db, query, -1, &result, 0) != SQLITE_OK ) db_error(db);

    if ( sqlite3_step(result) != SQLITE_ROW ) db_error(db);

    int ret = (int)sqlite3_column_int(result, 0);

    sqlite3_finalize(result);
    sqlite3_close(db);
    free(query);

    return ret;
}

int check_signup(char* username){
    sqlite3 *db;
    sqlite3_stmt *result;

    char *query = malloc(sizeof(char) * MAX_QUERY_LEN);
    sprintf(query, "SELECT COUNT(*) FROM user_t WHERE username='%s';", username);

    if ( sqlite3_open(DB_NAME, &db) != SQLITE_OK ) db_error(db);

    if ( sqlite3_prepare_v2(db, query, -1, &result, 0) != SQLITE_OK ) db_error(db);

    if ( sqlite3_step(result) != SQLITE_ROW ) db_error(db);

    int ret = (int)sqlite3_column_int(result, 0);

    sqlite3_finalize(result);
    sqlite3_close(db);
    free(query);

    return ret;
}

int signup_v2(char* username,char *password){
    sqlite3 *db;
    sqlite3_stmt *result;
    int ret = 0;

    if (check_signup(username) == 0){
        char *query = malloc(sizeof(char) * MAX_QUERY_LEN);
        sprintf(query, "INSERT INTO user_t VALUES ('%s', '%s');", username, password);
    
        if ( sqlite3_open(DB_NAME, &db) != SQLITE_OK ) db_error(db);

        char *err_msg = NULL;
        if ( sqlite3_exec(db, query, 0, 0, &err_msg) != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", err_msg);

            sqlite3_free(err_msg);
            sqlite3_close(db);
        }

        ret = sqlite3_last_insert_rowid(db);
    
        sqlite3_close(db);
        free(query);
    }
    return ret;
}

int createGroup(char *name, char *username) {
    sqlite3 *db;

    char *query = malloc(sizeof(char) * MAX_QUERY_LEN);
    sprintf(query, "INSERT INTO group_t(group_name, admin_username) VALUES ('%s', '%s');", name, username);

    if ( sqlite3_open(DB_NAME, &db) != SQLITE_OK ) db_error(db);

    char *err_msg = NULL;
    if ( sqlite3_exec(db, query, 0, 0, &err_msg) != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
    }

    int ret = sqlite3_last_insert_rowid(db);
    
    sqlite3_close(db);
    free(query);

    return ret;
}

int add_in_Pending_list(char *username, int group_id) {
    sqlite3 *db;

    char *query = malloc(sizeof(char) * MAX_QUERY_LEN);
    sprintf(query, "INSERT INTO queue_for_group VALUES ('%s', %d);", username, group_id);

    if ( sqlite3_open(DB_NAME, &db) != SQLITE_OK ) db_error(db);

    char *err_msg;
    if ( sqlite3_exec(db, query, 0, 0, &err_msg) != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
    }

    int ret = sqlite3_last_insert_rowid(db);

    sqlite3_close(db);
    free(query);

    return ret;
}

void showUsers(int connected_fd,int group_id , char* buffer) {
    // Show every one queued to that group
    sqlite3 *db;
    sqlite3_stmt *result;

    char *query = malloc(sizeof(char) * MAX_QUERY_LEN);
    sprintf(query,"SELECT user FROM queue_for_group WHERE id_group = %d;", group_id);

    if ( sqlite3_open(DB_NAME, &db) != SQLITE_OK ) db_error(db);

    if ( sqlite3_prepare_v2(db, query, -1, &result, 0) != SQLITE_OK ) db_error(db);

    while (sqlite3_step(result) != SQLITE_DONE) {
        my_recv(connected_fd, buffer);
        sprintf(buffer, "%s\n", sqlite3_column_text(result, 0));
        my_send(connected_fd, buffer);
    }

    sqlite3_finalize(result);
    sqlite3_close(db);
    free(query);
}

int Remove_from_Pending_List(char* username, int group_id){
    sqlite3 *db;

    char *query = malloc(sizeof(char) * MAX_QUERY_LEN);
    sprintf(query,"DELETE FROM queue_for_group WHERE user = '%s' AND id_group = %d;",username,group_id);

     if ( sqlite3_open(DB_NAME, &db) != SQLITE_OK ) db_error(db);

    char *err_msg;
    if ( sqlite3_exec(db, query, 0, 0, &err_msg) != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
    }

    int ret = sqlite3_last_insert_rowid(db);

    sqlite3_close(db);
    free(query);

    return ret;
}

int addToGroup(char* username, int group_id){
    sqlite3 *db;

    char *query = malloc(sizeof(char) * MAX_QUERY_LEN);
    sprintf(query,"INSERT INTO relation_group_user(group_id, username) VALUES (%d,'%s');", group_id, username);

    if ( sqlite3_open(DB_NAME, &db) != SQLITE_OK ) db_error(db);

    char *err_msg = NULL;
    if ( sqlite3_exec(db, query, 0, 0, &err_msg) != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
    }

    Remove_from_Pending_List(username, group_id);
    
    int ret = sqlite3_last_insert_rowid(db);

    sqlite3_close(db);
    free(query);

    return ret;
}

int sendMessage(char *group_name, char *sender, char *actual_message, char *time) {
    int group_id = nameToGroupId(group_name);

    sqlite3 *db;
    sqlite3_stmt *result;

    char *query = malloc(sizeof(char) * MAX_QUERY_LEN);
    sprintf(query, "INSERT INTO message VALUES ('%s', %d, '%s', '%s');", sender, group_id, time, actual_message);

    char *err_msg = NULL;

    if ( sqlite3_open(DB_NAME, &db) != SQLITE_OK ) db_error(db);

    if ( sqlite3_exec(db, query, 0, 0, &err_msg) != SQLITE_OK ) {
        sqlite3_free(err_msg);
        db_error(db);
    }

    int ret = sqlite3_last_insert_rowid(db);

    sqlite3_close(db);
    free(query);

    return ret;
}

void recvMessage(int connected_fd, char *group_name, char *buffer) {
    sqlite3 *db;
    sqlite3_stmt *result;

    char *query = malloc(sizeof(char) * MAX_QUERY_LEN);
    sprintf(query, "SELECT message.sender, message.sent_time, message.actual_message FROM group_t INNER JOIN message ON group_t.id = message.group_id WHERE group_t.group_name = '%s' ORDER BY sent_time;", group_name);

    if ( sqlite3_open(DB_NAME, &db) != SQLITE_OK ) db_error(db);

    if ( sqlite3_prepare_v2(db, query, -1, &result, 0) != SQLITE_OK ) db_error(db);

    while (sqlite3_step(result) != SQLITE_DONE) {
        my_recv(connected_fd, buffer);
        sprintf(buffer, "%s\n", sqlite3_column_text(result, 0));
        my_send(connected_fd, buffer);

        my_recv(connected_fd, buffer);
        sprintf(buffer, "%s\n", sqlite3_column_text(result, 1));
        my_send(connected_fd, buffer);

        my_recv(connected_fd, buffer);
        sprintf(buffer, "%s\n", sqlite3_column_text(result, 2));
        my_send(connected_fd, buffer);
    }

    sqlite3_finalize(result);
    sqlite3_close(db);
    free(query);
}

int howManyGroups(char *username) {
    sqlite3 *db;
    sqlite3_stmt *result;

    char *query = malloc(sizeof(char) * MAX_QUERY_LEN);
    sprintf(query, "SELECT COUNT(*) FROM relation_group_user WHERE username = '%s';", username);

    if ( sqlite3_open(DB_NAME, &db) != SQLITE_OK ) db_error(db);

    if ( sqlite3_prepare_v2(db, query, -1, &result, 0) != SQLITE_OK ) db_error(db);

    if ( sqlite3_step(result) != SQLITE_ROW ) db_error(db);

    int ret = (int)sqlite3_column_int(result, 0);

    sqlite3_finalize(result);
    sqlite3_close(db);
    free(query);

    return ret;
}

int howManyMessages(char *group_name) {
    sqlite3 *db;
    sqlite3_stmt *result;

    char *query = malloc(sizeof(char) * MAX_QUERY_LEN);
    sprintf(query, "SELECT COUNT(*) FROM group_t INNER JOIN message ON group_t.id = message.group_id WHERE group_t.group_name = '%s' ORDER BY sent_time;", group_name);

    if ( sqlite3_open(DB_NAME, &db) != SQLITE_OK ) db_error(db);

    if ( sqlite3_prepare_v2(db, query, -1, &result, 0) != SQLITE_OK ) db_error(db);

    if ( sqlite3_step(result) != SQLITE_ROW ) db_error(db);

    int ret = (int)sqlite3_column_int(result, 0);
    
    sqlite3_finalize(result);
    sqlite3_close(db);
    free(query);

    return ret;
}

int howManyAdminGroups(char *username) {
    sqlite3 *db;
    sqlite3_stmt *result;

    char *query = malloc(sizeof(char) * MAX_QUERY_LEN);
    sprintf(query, "SELECT COUNT(*) FROM group_t WHERE admin_username = '%s';", username);

    if ( sqlite3_open(DB_NAME, &db) != SQLITE_OK ) db_error(db);

    if ( sqlite3_prepare_v2(db, query, -1, &result, 0) != SQLITE_OK ) db_error(db);

    if ( sqlite3_step(result) != SQLITE_ROW ) db_error(db);

    int ret = (int)sqlite3_column_int(result, 0);
    
    sqlite3_finalize(result);
    sqlite3_close(db);
    free(query);

    return ret;
}

int howManyPending(int group_id) {
    sqlite3 *db;
    sqlite3_stmt *result;

    char *query = malloc(sizeof(char) * MAX_QUERY_LEN);
    sprintf(query, "SELECT COUNT(*) FROM queue_for_group WHERE id_group = %d;", group_id);

    if ( sqlite3_open(DB_NAME, &db) != SQLITE_OK ) db_error(db);

    if ( sqlite3_prepare_v2(db, query, -1, &result, 0) != SQLITE_OK ) db_error(db);

    if ( sqlite3_step(result) != SQLITE_ROW ) db_error(db);

    int ret = (int)sqlite3_column_int(result, 0);
    
    sqlite3_finalize(result);
    sqlite3_close(db);
    free(query);

    return ret;
}

void getGroups(int connected_fd, char *username, char *buffer) {
    sqlite3 *db;
    sqlite3_stmt *result;

    char *query = malloc(sizeof(char) * MAX_QUERY_LEN);
    sprintf(query, "SELECT group_t.group_name FROM group_t INNER JOIN relation_group_user ON group_t.id = relation_group_user.group_id WHERE relation_group_user.username = '%s';", username);

    if ( sqlite3_open(DB_NAME, &db) != SQLITE_OK ) db_error(db);

    if ( sqlite3_prepare_v2(db, query, -1, &result, 0) != SQLITE_OK ) db_error(db);

    while (sqlite3_step(result) != SQLITE_DONE) {
        my_recv(connected_fd, buffer);
        sprintf(buffer, "%s\n", sqlite3_column_text(result, 0));
        my_send(connected_fd, buffer);
    }

    sqlite3_finalize(result);
    sqlite3_close(db);
    free(query);
}

void getAdminGroups (int connected_fd, char* username, char* buffer){
    sqlite3 *db;
    sqlite3_stmt *result;

    char *query = malloc(sizeof(char) * MAX_QUERY_LEN);
    sprintf(query, "SELECT group_name FROM group_t WHERE admin_username = '%s';", username);

    if ( sqlite3_open(DB_NAME, &db) != SQLITE_OK ) db_error(db);

    if ( sqlite3_prepare_v2(db, query, -1, &result, 0) != SQLITE_OK ) db_error(db);

    while (sqlite3_step(result) != SQLITE_DONE) {
        my_recv(connected_fd, buffer);
        sprintf(buffer, "%s\n", sqlite3_column_text(result, 0));
        my_send(connected_fd, buffer);
    }

    sqlite3_finalize(result);
    sqlite3_close(db);
    free(query);
}

void server(int connected_fd, struct sockaddr peer, unsigned int addr_len) {
    char buffer[BUFFER_SIZE];

    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    char group_name[MAX_GROUP_NAME_LEN];
    char message[MAX_MESSAGE_LEN];
    char datetime[MAX_DATETIME_LEN];

    printf("\n --- Server is serving --- \n"); fflush(stdout);

    my_recv(connected_fd, buffer);
    my_send(connected_fd, buffer);

    if ( strcmp(buffer, "login\n") == 0 ) {
        printf("\tExecuting login\n"); fflush(stdout);

        my_recv(connected_fd, buffer);
        strcpy(username, buffer);
        username[strlen(username)-1] = '\0';
        my_send(connected_fd, "username ok\n");

        my_recv(connected_fd, buffer);
        strcpy(password, buffer);
        password[strlen(password)-1] = '\0';
        my_send(connected_fd, "password ok\n");

        if ( login(username, password) ) {
            my_recv(connected_fd, buffer);
            my_send(connected_fd, "login ok\n");
        } else {
            my_recv(connected_fd, buffer);
            my_send(connected_fd, "login not ok\n");
        }
    }

    if ( strcmp(buffer, "signup\n") == 0 ) {  // XXX
        printf("\tExecuting signup\n"); fflush(stdout);

        my_recv(connected_fd, buffer);
        strcpy(username, buffer);
        username[strlen(username)-1] = '\0';
        my_send(connected_fd, "username ok\n");

        my_recv(connected_fd, buffer);
        strcpy(password, buffer);
        password[strlen(password)-1] = '\0';
        my_send(connected_fd, "password ok\n");

        if ( signup_v2(username, password) ) {
            my_recv(connected_fd, buffer);
            my_send(connected_fd, "signup ok\n");
        } else {
            my_recv(connected_fd, buffer);
            my_send(connected_fd, "signup not ok\n");
        }
    }

    if ( strcmp(buffer, "getGroups\n") == 0 ) {
        printf("\tExecuting getGroups\n"); fflush(stdout);

        my_recv(connected_fd, buffer);
        strcpy(username, buffer);
        username[strlen(username)-1] = '\0';

        int x = howManyGroups(username);
        sprintf(buffer, "%d\n", x);
        my_send(connected_fd, buffer);

        getGroups(connected_fd, username, buffer);
    }

    if (strcmp(buffer,"getAdminGroups\n") == 0){
        printf("\tExecuting getAdminGroups\n"); fflush(stdout);

        my_recv(connected_fd, buffer);
        strcpy(username, buffer);
        username[strlen(username)-1] = '\0';

        int x = howManyAdminGroups(username);
        sprintf(buffer, "%d\n", x);
        my_send(connected_fd, buffer);

        getAdminGroups(connected_fd, username, buffer);
    }

    if( strcmp(buffer,"getPendingList\n") == 0 ){
        printf("\tExecuting getPendingList\n"); fflush(stdout);

        my_recv(connected_fd, buffer);
        strcpy(group_name,buffer);
        group_name[strlen(group_name) - 1] = '\0';

        sprintf(buffer,"%d\n", howManyPending(nameToGroupId(group_name)));
        my_send(connected_fd,buffer);

        showUsers(connected_fd, nameToGroupId(group_name), buffer);        
    }

    if ( strcmp(buffer, "createGroup\n") == 0 ) {
        printf("\tExecuting createGroup\n"); fflush(stdout);

        my_recv(connected_fd, buffer);
        strcpy(group_name, buffer);
        group_name[strlen(group_name) - 1]='\0';
        my_send(connected_fd, "newGroupName ok\n");

        my_recv(connected_fd, buffer);
        strcpy(username, buffer);
        username[strlen(username) - 1] = '\0';
        my_send(connected_fd, "username ok\n");

        my_recv(connected_fd, buffer);
        if (createGroup(group_name, username) != 0) {
            addToGroup(username, nameToGroupId(group_name));
            my_send(connected_fd, "creation ok\n");
        } else {
            my_send(connected_fd, "creation ko\n");
        }
    }

    if ( strcmp(buffer, "joinGroup\n") == 0 ) {
        printf("\tExecuting joinGroup\n"); fflush(stdout);
        
        my_recv(connected_fd,buffer);
        strcpy(group_name,buffer);
        group_name[strlen(group_name) - 1]='\0';
        my_send(connected_fd, "groupName ok\n");

        my_recv(connected_fd, buffer);
        strcpy(username, buffer);
        username[strlen(username) - 1] = '\0';
        my_send(connected_fd, "username ok\n");

        if (add_in_Pending_list(username, nameToGroupId(group_name)) != 0) {
            my_recv(connected_fd, buffer);
            my_send(connected_fd, "join ok\n");
        } else {
            my_recv(connected_fd, buffer);
            my_send(connected_fd, "join ko\n");
        }
    }

    if(strcmp(buffer,"deleteUser\n") == 0){
        printf("\tExecuting deleteUser\n"); fflush(stdout);

        my_recv(connected_fd, buffer);
        strcpy(username, buffer);
        username[strlen(username)-1] = '\0';
        my_send(connected_fd, "username ok\n");

        my_recv(connected_fd, buffer);
        strcpy(group_name, buffer);
        group_name[strlen(group_name)-1] = '\0';
        my_send(connected_fd, "group name ok\n");

        if (Remove_from_Pending_List(username, nameToGroupId(group_name)) != 0) {
            my_recv(connected_fd, buffer);
            my_send(connected_fd, "delete ok\n");
        } else {
            my_recv(connected_fd, buffer);
            my_send(connected_fd, "delete ko\n");
        }
    }

    if ( strcmp(buffer, "acceptUser\n") == 0 ) {
        printf("\tExecuting acceptUser\n"); fflush(stdout);

        my_recv(connected_fd, buffer);
        strcpy(username, buffer);
        username[strlen(username)-1] = '\0';
        my_send(connected_fd, "username ok\n");

        my_recv(connected_fd, buffer);
        strcpy(group_name, buffer);
        group_name[strlen(group_name)-1] = '\0';
        my_send(connected_fd, "group name ok\n");

        if (addToGroup(username, nameToGroupId(group_name)) != 0) {
            my_recv(connected_fd, buffer);
            my_send(connected_fd, "accepted ok\n");
        } else {
            my_recv(connected_fd, buffer);
            my_send(connected_fd, "accepted ko\n");
        }
    }

    if ( strcmp(buffer, "sendMessage\n") == 0 ) {
        printf("\tExecuting sendMessage\n"); fflush(stdout);

        my_recv(connected_fd, buffer);
        strcpy(group_name, buffer);
        group_name[strlen(group_name)-1] = '\0';
        my_send(connected_fd, "group name ok\n");

        my_recv(connected_fd, buffer);
        strcpy(username, buffer);
        username[strlen(username)-1] = '\0';
        my_send(connected_fd, "username ok\n");

        my_recv(connected_fd, buffer);
        strcpy(message, buffer);
        message[strlen(message)-1] = '\0';
        my_send(connected_fd, "message ok\n");

        my_recv(connected_fd, buffer); 
        buffer[strlen(buffer)-1] = '\0';
        my_send(connected_fd, "time ok\n");

        sendMessage(group_name, username, message, buffer);
    }

    if ( strcmp(buffer, "recvMessage\n") == 0 ) {
        printf("\tExecuting recvMessage\n"); fflush(stdout);

        my_recv(connected_fd, buffer);
        buffer[strlen(buffer)-1] = '\0';
        strcpy(group_name, buffer); 

        int nMessages = howManyMessages(group_name);
        sprintf(buffer, "%d\n", nMessages);
        my_send(connected_fd, buffer);

        recvMessage(connected_fd, group_name, buffer);
    }
    printf("\n --- Server accomplished the task --- \n"); fflush(stdout);
}

int main(int argc, char *argv[]){
    int listen_fd, connected_fd;
    unsigned int addr_len;
    struct sockaddr_in servaddr;
    struct sockaddr peer;

    // Questa system-call serve ad instanziare un nuovo descrittore di socket.
    printf("Creating the socket...\n"); fflush(stdout);
    if ( (listen_fd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 )
        err_sys("socket error");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    printf("Binding the socket...\n"); fflush(stdout);
    // Questa system-call serve ad assegnare un indirizzo locale (name) ad una socket.
    if ( bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 )
        err_sys("bind error");

    printf("Listening...\n"); fflush(stdout);
    // Attraverso la chiamata listen(), un server manifesta la sua volontà di apprestarsi a ricevere connessioni.
    if ( listen(listen_fd, MAX_QUEUE_LEN) < 0 )
        err_sys("listen error");

    int current_iteration = 0;
    while (1) {
        // Dopo la chiamata listen(), un server si mette realmente in attesa di connessioni attraverso una chiamata ad accept().
        ///////////////printf("Accepting a connection...\n"); fflush(stdout);
        bzero(&peer, sizeof(peer));
        connected_fd = accept(listen_fd, &peer, &addr_len);
        if ( connected_fd < 0 )
            err_sys("accept error");
        //////////////printf("Accepted a connection\n"); fflush(stdout);
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