
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    int socket;
    char id[20];
    int active;
} Client;

Client clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcast_message(const char *sender_id, const char *message) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].active) {
            char buffer[BUFFER_SIZE];
            snprintf(buffer, BUFFER_SIZE, "%s: %s", sender_id, message);
            write(clients[i].socket, buffer, strlen(buffer) + 1);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void send_message_to(const char *sender_id, const char *receiver_id, const char *message) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].active && strcmp(clients[i].id, receiver_id) == 0) {
            char buffer[BUFFER_SIZE];
            snprintf(buffer, BUFFER_SIZE, "%s: %s", sender_id, message);
            write(clients[i].socket, buffer, strlen(buffer) + 1);
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int index) {
    close(clients[index].socket);
    clients[index].active = 0;
    memset(clients[index].id, 0, sizeof(clients[index].id));
}

void list_clients(int client_socket) {
    char buffer[BUFFER_SIZE] = "Active clients:\n";

    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active) {
            strcat(buffer, clients[i].id);
            strcat(buffer, "\n");
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    send(client_socket, buffer, strlen(buffer), 0);
}


void *handle_client(void *arg) {
    int index = *((int *)arg);
    free(arg);
    char buffer[BUFFER_SIZE];
    while (1) {
        int read_size = read(clients[index].socket, buffer, BUFFER_SIZE);
        if (read_size <= 0) {
            remove_client(index);
            break;
        }
        buffer[read_size] = '\0';

        if (strncmp(buffer, "LIST", 4) == 0) {
            list_clients(clients[index].socket);
        } else if (strncmp(buffer, "2ALL ", 4) == 0) {
            broadcast_message(clients[index].id, buffer + 5);
        } else if (strncmp(buffer, "2ONE ", 4) == 0) {
            char *receiver_id = strtok(buffer + 5, " ");
            char *message = strtok(NULL, "");
            send_message_to(clients[index].id, receiver_id, message);
        } else if (strncmp(buffer, "STOP", 4) == 0) {
            remove_client(index);
            break;
        }
    }
    return NULL;
}

void *ping_clients(void *arg) {
    while (1) {
        sleep(20);
        printf("PINING CLIENTS\n");
        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (clients[i].active) {
                if (write(clients[i].socket, "ALIVE", 5) <= 0) {
                    remove_client(i);
                }
            }
        }
        pthread_mutex_unlock(&clients_mutex);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <address> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *address = argv[1];
    int port = atoi(argv[2]);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Could not create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(address);
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on %s:%d\n", address, port);

    pthread_t ping_thread;
    pthread_create(&ping_thread, NULL, ping_clients, NULL);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        pthread_mutex_lock(&clients_mutex);
        int index = -1;
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (!clients[i].active) {
                index = i;
                break;
            }
        }
        if (index == -1) {
            char* napis = "Server is full";
            write(client_socket, napis,  strlen(napis));
            close(client_socket);
            pthread_mutex_unlock(&clients_mutex);
            continue;
        }

        clients[index].socket = client_socket;
        read(client_socket, clients[index].id, sizeof(clients[index].id) - 1);
        //CHECK IF NICKNAME IS ALREADY USED
        int exit=0;
        for (int i=0; i<MAX_CLIENTS; i++) {
            if (i != index && strcmp(clients[i].id, clients[index].id) == 0 &&  clients[i].active) {
                char* napis = "Nickname is already used";
                write(client_socket, napis, strlen(napis));
                exit = 1;
                remove_client(index);
                pthread_mutex_unlock(&clients_mutex);
                break;
            }
        }
        if (exit) continue;
        clients[index].active = 1;
        pthread_mutex_unlock(&clients_mutex);

        int* arg = malloc(sizeof(int));
        *arg = index;
        pthread_t client_thread;
        pthread_create(&client_thread, NULL, handle_client, arg);
    }

    close(server_socket);
    return 0;
}
