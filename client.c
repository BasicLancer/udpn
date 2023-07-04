#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &(server_addr.sin_addr)) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    // Send message to server
    const char* message = "Hello, server!";
    if (sendto(sockfd, message, strlen(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Message send failed");
        exit(EXIT_FAILURE);
    }
    printf("Message sent.\n");

    // Receive response from server
    int msg_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
    if (msg_len < 0) {
        perror("Response receive failed");
        exit(EXIT_FAILURE);
    }

    buffer[msg_len] = '\0';
    printf("Response from server: %s\n", buffer);

    // Close the socket
    close(sockfd);
    return 0;
}
