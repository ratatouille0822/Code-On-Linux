#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <event.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/http_struct.h>

#define SIZE  4096

void gencb(struct evhttp_request* req, void* arg)
{
    char* s = "---1--- this is a test string";
    evbuffer_add(req->output_buffer, s, strlen(s));
    evhttp_send_reply(req, 200, "OK", NULL);

}

void test_cb(struct evhttp_request* req, void* arg)
{
    char buffer[SIZE];
    int ret = -1;
    int fd = -1;

    fd = open("test_page.html", O_RDONLY);
    if(-1 == fd)
    {
        perror("open");
        return;
    }
    else
    {
        evhttp_send_reply(req, 200, "OK", NULL);
    }
    
    while(1)
    {
        ret = read(fd, buffer, SIZE);
        if(-1 == ret)
        {
            perror("read");
            break;
        }
        if(0 == ret)
        {
            printf("read done...\n");
            break;
        }
        evbuffer_add(req->output_buffer, buffer, ret);
    }
}


int main(int argc, char* argv[])
{
    int ret = -1;
    struct event_base* base = NULL;
    struct evhttp* http_svr = NULL;

    base = event_base_new();
    if(NULL == base)
    {
        printf("event_base_new failed...\n" );
        goto err0;
    }
    http_svr = evhttp_new(base);
    if(NULL == http_svr)
    {
        printf("evhttp_new failed...\n");
        goto err0;
    }

    ret = evhttp_bind_socket(http_svr, "192.168.73.132", atoi(argv[1]));
    if(0 != ret)
    {
        printf("evhttp_bind_socket failed...\n");
        goto err0;
    }

    ret = evhttp_set_cb(http_svr, "/", test_cb, NULL);
    if(-1 == ret)
    {
        printf("evhttp_ser_cb failed...\n");
        goto err0;
    }
    evhttp_set_gencb(http_svr, gencb, NULL);

    ret = event_base_dispatch(base);
    if(-1 == ret)
    {
        printf("event_base_dispatch failed...\n");
        goto err0;
    }

    evhttp_free(http_svr);

    return 0;
err0:
    return 1;
}