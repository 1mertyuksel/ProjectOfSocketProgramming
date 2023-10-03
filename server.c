// gcc server10.c -o server -lpthread -lsctp
// ./server


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/sctp.h>
#include <pthread.h>

#define SERVER_IP "10.20.207.166"
#define SERVER_PORT 12357
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

struct ClientInfo {
    int socket;
    int client_num;
};

struct ClientInfo clients[MAX_CLIENTS];
int client_count = 1;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

void *client_thread(void *arg) {
    int clientSocket = *((int *)arg);
    char buffer[BUFFER_SIZE];

    pthread_mutex_lock(&count_mutex);
    int current_client_num = client_count++;
    pthread_mutex_unlock(&count_mutex);

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_received = sctp_recvmsg(clientSocket, buffer, sizeof(buffer), NULL, 0, 0, 0);

        if (bytes_received == -1) {
            perror("Mesaj alma hatası");
            close(clientSocket);
            break;
        } else if (bytes_received == 0) {
            printf("İstemci %d bağlantısı kapatıldı.\n", current_client_num);
            close(clientSocket);
            break;
        }

        printf("İstemci %d'den gelen mesaj: %s\n", current_client_num, buffer);

        for (int i = 1; i < client_count; i++) {
            if (i != current_client_num) {
                if (sctp_sendmsg(clients[i].socket, buffer, strlen(buffer), NULL, 0, 0, 0, 0, 0, 0) == -1) {
                    perror("Cevap gönderme hatası");
                    close(clients[i].socket);
                    break;
                }
            }
        }
    }

    pthread_mutex_lock(&count_mutex);
    client_count--;
    pthread_mutex_unlock(&count_mutex);

    pthread_exit(NULL);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (serverSocket == -1) {
        perror("Socket oluşturma hatası");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Bağlama hatası");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        perror("Dinleme hatası");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    printf("Sunucu başlatıldı. İstemcileri bekliyor...\n");

    while (1) {
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket < 0) {
            perror("Kabul etme hatası");
            close(serverSocket);
            exit(EXIT_FAILURE);
        }

        int client_index = -1;
        for (int i = 1; i < MAX_CLIENTS; i++) {
            if (clients[i].socket == 0) {
                client_index = i;
                break;
            }
        }

        if (client_index >= 0) {
            clients[client_index].socket = clientSocket;
            clients[client_index].client_num = client_index;

            pthread_t client_thread_id;
            pthread_create(&client_thread_id, NULL, client_thread, (void *)&clients[client_index].socket);
            pthread_detach(client_thread_id);
        } else {
            printf("Maksimum istemci sayısına ulaşıldı, bağlantı reddedildi.\n");
            close(clientSocket);
        }
    }

    return 0;
}
