
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>  
#include <fcntl.h>
#include <strings.h>

int socket_init();
void socket_deinit(int);
void socket_connect(int fd, char* ip, int port);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s < Server IP> < Server Port>\n", argv[0]);
        return -1;
    }

    int sockfd = socket_init();
    socket_connect(sockfd, argv[1], atoi(argv[2]));
    

    printf("Connected to %s\n", argv[1]);
    
    char buf[1024] = " hello world!\n";
    for(int i=0;i<100;++i)
    {
        buf[0] = i + '0';
        write(sockfd,buf,sizeof buf);
        sleep(1);
    }
        
    printf("write finished\n");
    //int len = recv(sockfd, buf, sizeof(buf), 0);
    // if (len >= 0) {
    //     buf[len] = '\0';
    // }
    
    //printf("RECV: %s\n", buf);

    // sleep(5);

    // for(int i=0;i<10;++i)
    // {
    //     buf[0] = i + '0';
    //     write(sockfd,buf,sizeof buf);
    // }
    // sleep(10);
    socket_deinit(sockfd);
    return 0;
}

void socket_connect(int fd, char* ip, int port) {
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    
    if (connect(fd, (struct sockaddr*)&addr, addrlen) < 0) {
        perror("socket connect failed");
        exit(-1);
    }
}

int socket_init() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket create failed.");
        exit(-1);
    }

    return fd;
}

void socket_deinit(int fd) {
    if (fd >= 0) close(fd);
}