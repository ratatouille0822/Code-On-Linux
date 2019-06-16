#include <headers.h>
#include <event.h>

#define SIZE 128

void fun(evutil_socket_t fd, short what, void* arg)
{
	int ret = -1;
	char buffer[SIZE];

	fgets(buffer, SIZE, stdin);

	printf("BUF: %s\n", buffer);
	printf("ARG: %s\n", (char*)arg);
}

int main()
{

	int ret = -1;
	int i = 0;

	struct event_base* base = NULL;
	struct event * ev = NULL;

	base = event_base_new();
	if(NULL == base)
	{
		printf("event_base_new failed...\n");
		return 1;
	}

	ev = event_new(base, STDIN_FILENO , EV_READ|EV_PERSIST, fun, (void*)"hello libevent!\n");
	if(NULL == ev)
	{
		printf("event_new failed...\n");
		return 1;
	}

	ret = event_add(ev, NULL);
	if(-1 == ret)
	{
		printf("event_add failed...\n");
		return 1;
	}

	ret = event_base_dispatch(base);
	if(-1 == ret)
	{
		printf("event_base_dispatch failed ...\n");
		return 1;
	}

	event_base_free(base);
	event_free(ev);

    return 0;
}

