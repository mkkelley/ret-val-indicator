#include <stdio.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>

int main(void) {
    int socket_fd, n;
    socklen_t len;
    char line[128];
    struct sockaddr_in saddr, caddr;

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(2910);

    bind(socket_fd, (struct sockaddr *)&saddr, sizeof(saddr));

    printf("Server running\n");
    while(1) {
        len = sizeof(caddr);
        n = recvfrom(socket_fd, line, 128, 0, (struct sockaddr *)&caddr, &len);
        sendto(socket_fd, line, n, 0, (struct sockaddr *)&caddr, len);
        line[n] = 0;
        fputs(line, stdout);
    }

    return 0;
}
