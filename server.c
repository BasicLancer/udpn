#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Socket bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP server running on port %d\n", PORT);

    while (1) {
        // Receive message from client
        int msg_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &addr_len);
        if (msg_len < 0) {
            perror("Message receive failed");
            exit(EXIT_FAILURE);
        }

        buffer[msg_len] = '\0';
        printf("Received message: %s\n", buffer);

        // Send response to client
        if (sendto(sockfd, buffer, msg_len, 0, (struct sockaddr*)&client_addr, addr_len) < 0) {
            perror("Message send failed");
            exit(EXIT_FAILURE);
        }
        printf("Response sent.\n");
    }

    // Close the socket
    close(sockfd);
    return 0;
}
