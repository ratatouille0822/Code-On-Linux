#include <headers.h>

#include <event.h>

int main()
{
	int i = 0;
	struct event_base* eb = NULL;
	const char ** str;

	char method[10];
	 
	eb = event_base_new();
	str = event_get_supported_methods();
	while(NULL != str[i])
	{
		printf("%s \n", str[i++]);
	}

	strncpy(method, event_base_get_method(eb), strlen(event_base_get_method(eb)));

	if(NULL == method)
	{
		printf("event_bae_get_method failed\n");
		return 1;
	}

	printf("Method: %s\n", method);
	
	event_base_free(eb);


    return 0;
}

