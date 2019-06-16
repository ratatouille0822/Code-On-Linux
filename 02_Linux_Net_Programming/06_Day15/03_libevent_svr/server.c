#include <headers.h>
#include <event.h>

#define SIZE 128

void read_connfd(evutil_socket_t connfd, short what, void* arg)
{
    int ret = -1;

    char buffer[SIZE];
    bzero(buffer, SIZE);

    ret = recv(connfd, buffer, sizeof(buffer), 0);
    if(ret <= 0)
    {
        perror("recv");
        return;
    }
    if(buffer[strlen(buffer) - 1] == '\n')
    {
        buffer[strlen(buffer) - 1] = '\0';
    }

    printf("\033[31mClient: \033[0m%s\n", buffer );

/*
    ret = send(connfd, buffer, strlen(buffer), 0);
    if(-1 == ret)
    {
        perror("send");
        return;
    }
*/
}

void read_stdin(evutil_socket_t fd, short what, void* arg)
{
    int connfd = (int)(long)arg;
    int ret = -1;
    char buffer[SIZE];

    fgets(buffer, SIZE, stdin);
    /*
    if(buffer[strlen(buffer) - 1] == '\n')
    {
        buffer[strlen(buffer) - 1] = '\0';
    }
    */

    printf("\033[31mClient: \033[0m%s\n", buffer );

    ret = send(connfd, buffer, strlen(buffer), 0);
    if(-1 == ret)
    {
        perror("send");
        return;
    }
}

void mainloop( int connfd)
{
    int ret = -1;

    struct event_base* base;
    struct event* ev1 = NULL;
    struct event* ev2 = NULL;

    base = event_base_new();
    if(NULL == base)
    {
        printf("event_base_new failed ...\n");
        return ;
    }

    ev1 = event_new(base, connfd, EV_READ | EV_PERSIST, read_connfd, NULL);
    if(NULL == ev1)
    {
        printf("event_new failed ...\n");
        return ;
    }

    ev2 = event_new(base, STDIN_FILENO, EV_READ | EV_PERSIST, read_stdin, (void*)(long)connfd );
    if(NULL == ev1)
    {
        printf("event_new failed ...\n");
        return ;
    }

    ret = event_add(ev1, NULL);
    if(-1 == ret)
    {
        printf("event_add failed \n");
        return ;
    }

    ret = event_add(ev2, NULL);
    if(-1 == ret)
    {
        printf("event_add failed \n");
        return ;
    }

    ret = event_base_dispatch(base);
    if(-1 == ret)
    {
        printf("event_base_dispatch failed ...\n");
        return ;
    }

    event_free(ev1);
    event_base_free(base);

}

int main(int argc, char* argv[])
{
    int ret = -1;
    int sockfd = -1;
    int connfd = -1;

    struct sockaddr_in addr;
    struct sockaddr_in from;

    char buffer[SIZE];
    char ip_buffer[16];

    socklen_t len = 0;

    if(3 != argc)
    {
        printf("usage: server x.x.x.x port\n");
        return 1;
    }
    memset(&addr, 0, sizeof(addr));
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
    {
        perror("socket");
        goto err0;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    ret = inet_pton(AF_INET, argv[1], &addr.sin_addr.s_addr);
    if( -1 == ret)
    {
        perror("inet_pton");
        goto err1;
    }

    ret = bind(sockfd, (void*)&addr, sizeof(addr));
    if(-1 == ret)
    {
        perror("bind");
        goto err1;
    }

    ret = listen(sockfd, 10);
    if(-1 == ret)
    {
        perror("listen");
        goto err1;
    }

    len = sizeof(from);
    bzero(&from, len);
    connfd = accept(sockfd, (void*)&from, &len);
    if(-1 == connfd)
    {
        perror("accept");
        goto err2;
    }

    mainloop(connfd);

    close(sockfd);
    close(connfd);
    return 0;

err2:
    close(connfd);
err1:
    close(sockfd);
err0:
    return 1;
}