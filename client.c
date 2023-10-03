// gcc client10.c -o client -lpthread -lsctp
// ./client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/sctp.h>

#define SERVER_IP "10.20.207.166"
#define SERVER_PORT 12357
#define BUFFER_SIZE 1024

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (clientSocket == -1) {
        perror("Socket oluşturma hatası");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(SERVER_PORT);

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Bağlantı hatası");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    printf("Server'a bağlandı. Mesaj gönderin (Çıkış için 'exit' yazın):\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);

        if (strncmp(buffer, "exit", 4) == 0) {
            // İstemci sadece kendi bağlantısını kapatır
            close(clientSocket);
            printf("Bağlantı kapatıldı.\n");
            break;
        }

        if (sctp_sendmsg(clientSocket, buffer, strlen(buffer), NULL, 0, 0, 0, 0, 0, 0) == -1) {
            perror("Mesaj gönderme hatası");
            close(clientSocket);
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
