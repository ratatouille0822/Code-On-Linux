#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <event2/buffer.h>
#include <event2/event.h>
#include <fcntl.h>
#include <dirent.h>

#include "pub.h"

#define SIZE 4096

void send_dir(struct evbuffer* outbuffer, char* file_name)
{
    //debug code
    printf("\033[34m<<Step in send_dir:\n\033[0m\n");
    printf("\033[34m FILE_NAME = \033[0m\"%s\033[34m\"\033[0m\n", file_name);
    //end debug code

    int ret = -1;
    char buffer[128];

    DIR* dir = NULL;
    struct dirent* d = NULL;

    dir = opendir(file_name);
    if(NULL == dir)
    {
        perror("opendir");
        return;
    }
    while(1)
    {
        d = readdir(dir);
        if(NULL == d)
        {
            perror("readdir");
            break;
        }

        if(DT_DIR == d->d_type)
        {
            evbuffer_add_printf(outbuffer, "<li><a href = '%s/'>%s</a></li>", d->d_name, d->d_name );
        }
        if(DT_REG == d->d_type)
        {
            evbuffer_add_printf(outbuffer, "<li><a href = '%s'>%s</a></li>", d->d_name, d->d_name );
        }
    }

    //debug code
    //ret = evbuffer_copyout(outbuffer, (void*)buffer, 128);
    //if( ret <= 0)
    //{
        //printf( "copyed nothing");
    //}
    //printf("--->%s\n", buffer);

    printf("\033[34mStep out send_dir>>\n\033[0m\n");
    //end debug code
}

void send_file(struct evbuffer* outbuffer, char* file_name)
{
    //debug code
    printf("\033[34m<<Step in send_file:\n\033[0m\n");
    printf("\033[34m FILE_NAME = \033[0m\"%s\033[34m\"\033[0m\n", file_name);
    //end debug code

    int cnt = 0;
    int ret = -1;
    int fd = -1;
    char buffer[SIZE];

    fd = open(file_name, O_RDONLY );
    if(-1== fd)
    {
        perror("open");
        return;
    }

    printf("-------------------------------------------------------------------------------------------\n");
    while(1)
    {
        bzero(buffer, SIZE);
        cnt = read(fd, buffer, SIZE);
        if(-1 == cnt)
        {
            perror("read");
            break;
        }
        else if( 0 == cnt)
        {
            printf("read done...\n");
            break;
        }
        ret =  evbuffer_add(outbuffer,  buffer, cnt);
        //evbuffer_copyout(outbuffer, (void*)buffer, cnt-1);
        //printf("%s", buffer);
        if(0 !=  ret)
        {
            printf("evbuffer_add failed...\n");
            break;
        }
        else if(0 == ret)
        {
            /*do nothing */
        }
    }
    close(fd);

    //debug code
    printf("\033[34mStep out send_file>>\n\033[0m\n");
    //end debug code
}

void http_send_header_lines(struct evbuffer* outbuf, int code, char* status, char* type, size_t size)
{
    int ret = -1;

    ret = evbuffer_add_printf(outbuf, "HTTP/1.1 %d %s\r\n", code, status);
    if(-1 == ret)
    {
        printf("evbuffer_add_printf  failed...\n");
        return;
    }
    if(0 == ret)
    {
        printf("Send over...\n");
    }

    ret = evbuffer_add_printf(outbuf, "Content-Type: %s\r\n", type);
    if(-1 == ret)
    {
        printf("evbuffer_add_printf  failed...\n");
        return;
    }
    if(0 == ret)
    {
        printf("Send over...\n");
    }

    ret = evbuffer_add_printf(outbuf, "Content-Length: %ld\r\n", size);
    if(-1 == ret)
    {
        printf("evbuffer_add_printf  failed...\n");
        return;
    }
    if(0 == ret)
    {
        printf("Send over...\n");
    }

    ret = evbuffer_add_printf(outbuf, "Connection: close\r\n");
    if(-1 == ret)
    {
        printf("evbuffer_add_printf  failed...\n");
        return;
    }
    if(0 == ret)
    {
        printf("Send over...\n");
    }

    ret = evbuffer_add_printf(outbuf, "\r\n");
    if(-1 == ret)
    {
        printf("evbuffer_add_printf  failed...\n");
        return;
    }
    if(0 == ret)
    {
        printf("Send over...\n");
    }
}



static void bev_read_cb(struct bufferevent* bev, void* cbargs)
{
    int ret = -1;
    int connfd ;

    char version[32];
    char url[512];
    char method[32];

    char* buffer;
    char* pfile;

    struct stat s;
    struct evbuffer* inbuf = bufferevent_get_input(bev);
    struct evbuffer* outbuf = bufferevent_get_output(bev);

    connfd = (evutil_socket_t)(long)cbargs;

    buffer = evbuffer_readln(inbuf, NULL, EVBUFFER_EOL_CRLF_STRICT );
    sscanf(buffer, "%s %s %s\r\n", method, url, version);
    printf("\033[31mMethod: \033[0m %s -- \033[31mUrl: \033[0m %s -- \033[31mVersion: \033[0m %s\n", method, url, version);

    if(strncmp(method, "GET", 3 ) != 0)
    {
        printf(" this is not a GET request...\n");
    }

    strdecode(url, url);
    pfile = url + 1;
    if('\0' == *pfile)
    {
        pfile = "./";
    }

    ret = stat(pfile, &s);
    if(-1 == ret)
    {
        perror("stat");
        http_send_header_lines(outbuf, 404, "NOT FOUND", get_mime_type(".html"), -1);
        send_file(outbuf, "01_Lessons/02_Linux_Net_Programming/07_Day16/02_Libevent_Server/404.html");
        return;
    }

    if(S_ISREG(s.st_mode))
    {
        http_send_header_lines(outbuf, 200, "OK", get_mime_type(pfile), s.st_size);
        send_file(outbuf, pfile);
    }
    else if(S_ISDIR(s.st_mode))
    {
        http_send_header_lines(outbuf, 200, "OK", get_mime_type(".html") , s.st_size);
        send_file(outbuf, "01_Lessons/02_Linux_Net_Programming/07_Day16/02_Libevent_Server/upper.html");
        send_dir(outbuf, pfile);
        send_file(outbuf, "01_Lessons/02_Linux_Net_Programming/07_Day16/02_Libevent_Server/lower.html");
    }

    //while(1)
    //{
        //buffer = evbuffer_readln(inbuf, NULL, EVBUFFER_EOL_CRLF_STRICT);
        //if(NULL == buffer)
        //{
            //printf("-- Le Fin --\n");
            //break;
        //}
        //printf("buffer: %s\n", buffer );
    //}

    //evbuffer_free(inbuf);
    //evbuffer_free(outbuf);
}

static void bev_event_cb(struct bufferevent* bev, short events, void* cbargs )
{
    if(events & BEV_EVENT_ERROR)
    {
        printf("bufferent has something wrong\n");
    }
    if(events & (BEV_EVENT_EOF | BEV_EVENT_ERROR))
    {
        printf("Connection Closed\n");
        bufferevent_free(bev);
    }
}

static void accept_cb(struct evconnlistener* listener, evutil_socket_t sockfd, struct sockaddr* addr, int len, void* ctx)
{
    printf("100---------------%d\n", sockfd);
    struct event_base* base = evconnlistener_get_base(listener);
    struct bufferevent* bev = bufferevent_socket_new(base, sockfd, BEV_OPT_CLOSE_ON_FREE);
    printf("200---------------%d\n", sockfd);

    bufferevent_setcb(bev, bev_read_cb, NULL, bev_event_cb,  (void*)(long)sockfd);
    bufferevent_enable(bev, EV_READ | EV_WRITE);

    //debug code
    printf("\033[35m NEW FD IS : \033[0m %d\n", sockfd);
    //end debug code
}

int main(int argc, char* argv[])
{
    int ret = -1;
    struct event_base * base = NULL;
    struct evconnlistener* listener = NULL;
    struct sockaddr_in addr;

    if(2 != argc)
    {
        printf("usage: server port\n");
        goto err0;
    }

    ret = chdir(getenv("HOME"));
    if( -1 == ret)
    {
        perror("chdir");
        goto err0;
    }

    //debug code
    printf("\n");
    system("pwd");
    printf("\n");
    //end of debug code

    base = event_base_new();
    if(NULL == base)
    {
        printf("event_base_new failed...\n");
        goto err0;
    }

    bzero(&addr, sizeof(addr));
    ret = inet_pton(AF_INET, "192.168.73.132", &addr.sin_addr.s_addr);
    if(-1 == ret)
    {
        perror("inet_pton");
        goto err0;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));
    
    listener = evconnlistener_new_bind(base, accept_cb, NULL, LEV_OPT_CLOSE_ON_EXEC|LEV_OPT_REUSEABLE_PORT,10, (void*)&addr, sizeof(addr)) ;
    if(NULL == listener)
    {
        printf("evconnlistener_new_bind failed...\n");
        goto err0;
    }

    event_base_dispatch(base);
    return 0;
err0:
    return 1;
}
