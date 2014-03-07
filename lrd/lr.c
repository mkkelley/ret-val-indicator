#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#define MAX 100

int main(int argc, char **argv) {
    int socket_fd, n;
    socklen_t len;
    char sline[MAX], rline[MAX+1];
    struct sockaddr_in saddr;

    if (argc != 2) {
        printf("Usage: %s ipaddress\n", argv[0]);
        return -1;
    }

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &saddr.sin_addr);
    saddr.sin_port = htons(2910);

    printf("Client running\n");
    while (fgets(sline, MAX, stdin) != NULL) {
        len = sizeof(saddr);
        sendto(socket_fd, sline, strlen(sline), 0, (struct sockaddr *)&saddr, len);
        n = recvfrom(socket_fd, rline, MAX, 0, NULL, NULL);
        rline[n] = 0;
        fputs(rline, stdout);
    }
    return 0;
}
