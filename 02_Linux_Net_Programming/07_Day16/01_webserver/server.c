#include <headers.h>
#include <sys/epoll.h>
#include <dirent.h>
#include <signal.h>

#include "pub.h"

#define SIZE 1024
#define MAXSIZE 4096

void fun(int signo)
{
	printf("in cb fun\n");
}
int send_header(int connfd, int code, char* status, char* file_type, int file_length)
{

	int ret = -1;
	char buffer[MAXSIZE];

	//debuf code:
	printf("\033[31m<<Step in send_header: fd = \033[0m %d\n\n",connfd);
	//end of debuf code

	bzero(buffer, MAXSIZE);
	sprintf(buffer, "HTTP1.1 %d %s\r\n", code, status );
	ret = send(connfd, buffer, strlen(buffer), 0);
	if(-1 == ret)
	{
		perror("send");
		goto err0;
	}

	bzero(buffer, MAXSIZE);
	sprintf(buffer, "Content-Type: %s\r\n", file_type);
	ret = send(connfd, buffer, strlen(buffer), 0);
	if(-1 == ret)
	{
		perror("send");
		goto err0;
	}

	bzero(buffer, MAXSIZE);
	sprintf(buffer, "Content-Length %d\r\n", file_length);
	ret = send(connfd, buffer, strlen(buffer), 0);
	if(-1 == ret)
	{
		perror("send");
		goto err0;
	}

	bzero(buffer, MAXSIZE);
	sprintf(buffer, "Connection: close\r\n" );
	ret = send(connfd, buffer, strlen(buffer), 0);
	if(-1 == ret)
	{
		perror("send");
		goto err0;
	}

	bzero(buffer, MAXSIZE);
	sprintf(buffer, "\r\n");
	ret = send(connfd, buffer, strlen(buffer), 0);
	if(-1 == ret)
	{
		perror("send");
		goto err0;
	}


	//debuf code:
	printf("\033[31m Step out send_header:>>\033[0m\n\n");
	//end of debuf code

	return 0;
err0:
	return 1;
}

int send_file(int connfd, char* file_name)
{
	int ret = -1;
	int fd = -1;
   
	//debuf code:
	printf("\033[31m<<Step in send_file: fd =\033[0m %d\n\n", connfd);
	//end of debuf code

	printf(" ~~~~~~ this time is to send %s\n", file_name);
	char buffer[MAXSIZE];

	fd = open(file_name, O_RDONLY);
	if(-1 == fd)
	{
		perror("open");
	}
	while(1)
	{
		bzero(buffer, MAXSIZE);
		ret = read(fd, buffer, MAXSIZE );
		if(ret <=0)
		{
			perror("read");
			break;
		}
		printf("\033[35m Here is send file:\nContent to send is :\n\033[0m %s\n", buffer);
		ret = send(connfd, buffer, ret, 0 );
		if(ret <= 0)
		{
			perror("send");
			break;
		}
	}

	close(fd);

	//debuf code:
	printf("\033[31m Step out send_file:>>\033[0m\n\n");
	//end of debuf code
	return 0;
}

int send_dir(int connfd, char* file_name)
{
	int ret = -1;
	char buffer[MAXSIZE];

	//debuf code:
	printf("\033[31m<<Step in send_dir:\033[0m %d\n\n", connfd);
	//end of debuf code
	printf("--------------here is send dir()------------@file_name = %s\n", file_name);
	
	struct dirent* d = NULL;
	DIR* dir = NULL;

	dir = opendir(file_name);
	if(NULL == dir)
	{
		perror("opendir");
		return 1;
	}

	while(1)
	{
		d = readdir(dir);
		if(NULL == d)
		{
			break;
		}

		bzero(buffer, MAXSIZE);
		if(d->d_type ==  DT_REG)
		{
			printf("d->d_type = \033[35mDT_REG\033[0m\n");
			sprintf(buffer, "<li><a href = '%s'> %s</a></li>", d->d_name, d->d_name);
			printf("----------\033[31m buffer: \033[0m%s\n", buffer);
		}
		else if(d->d_type == DT_DIR)
		{
			printf("d->d_type =\033[36m DT_DIR\033[0m\n");
			sprintf(buffer, "<li><a href = '%s/'> %s</a></li>", d->d_name, d->d_name);
			printf("----------\033[31m buffer: \033[0m%s\n", buffer);
		}
		ret = send(connfd, buffer, strlen(buffer), 0);
		if(ret <= 0)
		{
			printf("***%s***\n", d->d_name);
			perror("send");
		}
	}

	closedir(dir);

	//debuf code:
	printf("\033[31m Step out send_dir:>>\033[0m\n\n");
	//end of debuf code

	return 0;
}

int http_request(int connfd)
{
	//debuf code:
	printf("\033[31m <<Step in http_request:\033[0m\n\n");
	//end of debuf code

	int ret = -1;
	char buffer[MAXSIZE];
	char str[MAXSIZE];

	char method[32];
	char url[1024];
	char version[32];
	char status[32];

	struct stat s;
	char* pfile = NULL;

	bzero(buffer, MAXSIZE);
	ret = get_line(connfd, str, MAXSIZE);
	if(-1 == ret)
	{
		printf("get_line failed\n");
		return 1;
	}

	printf("buffer: %s", str);

	sscanf(str, "%s %s %s\r\n", method, url, version);

	//debug code
	printf("\033[31mmethod:\033[0m %s, \033[31murl: \033[0m%s, \033[31mversion: \033[0m%s\n", method, url, version);
	//end of debug code

	strdecode(url, url);
	//debug code
	printf("\033[31mmethod:\033[0m %s, \033[31murl: \033[0m%s, \033[31mversion: \033[0m%s\n", method, url, version);
	//end of debug code

	while(1)
	{
		bzero(buffer, MAXSIZE);
		ret = get_line(connfd, buffer, MAXSIZE);
		if(ret <= 0)
		{
			perror("recv");
			break;
		}
		printf("%s", buffer);
	}

	if(strncmp(method, "GET", 3) != 0)
	{
		printf("This is not a GET req\n");
		return 1;
	}

	if(strcmp(url, "/") == 0)
	{
		pfile = "./";
		//debuf code:
		printf("\033[31mpfile = \033[0m%s\n", pfile);
		//end of debuf code
	}
	else
	{
		pfile = url + 1 ;
	}
	

	//debuf code:
	printf("\033[31mpfile = \033[0m%s\n", pfile);
	//end of debuf code

	bzero(&s, sizeof(s));
	ret = stat(pfile, &s);
	if(-1 == ret)
	{
		perror("stat");
		send_header(connfd, 404, "NOT FOUND", ".html", -1);
		send_file(connfd, "./01_Lessons/02_Linux_Net_Programming/07_Day16/01_webserver/404.html");
		return 1;
	}

	if(S_ISREG(s.st_mode))
	{
		//debuf code:
		printf("\033[31mtype = \033[0m%s\n", get_mime_type(pfile));
		//end of debuf code

		send_header(connfd, 200, "OK", get_mime_type(pfile), s.st_size);
		send_file(connfd, pfile);
		close(connfd);
	}
	else if(S_ISDIR(s.st_mode))
	{
		send_header(connfd, 200, "OK", "*.html", -1);
		send_file(connfd, "./01_Lessons/02_Linux_Net_Programming/07_Day16/01_webserver/upper.html");
		send_dir(connfd, pfile);
		send_file(connfd, "./01_Lessons/02_Linux_Net_Programming/07_Day16/01_webserver/lower.html");
		close(connfd);
	}

	//debuf code
	printf("\033[31m Step out http_request:>>\033[0m\n\n");
	//end of debuf code
	return 0;
}

int main(int argc, char* argv[])
{
	int ret = -1;
	int sockfd = -1;
	int connfd = -1;
	int epollfd = -1;
	int i = 1;

	int flag = 0;

	int count = 0;

	char ip_buf[16];

	struct sockaddr_in addr;
	struct sockaddr_in from;
	struct epoll_event e;
	struct epoll_event re[128];

	socklen_t len = 0;

	if(2 != argc)
	{
		printf("usage: server port ");
		goto err0;
	}

	signal(SIGPIPE, fun);

	ret = chdir(getenv("HOME"));
	if(-1 == ret)
	{
		perror("chdir");
		goto err0;
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sockfd)
	{
		perror("socket");
		goto err0;
	}

	ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &i, sizeof(i));
	if(-1 == ret)
	{
		perror("setsockopt");
		goto err1;
	}

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[1]));
	ret = inet_pton(AF_INET, "192.168.73.132", &addr.sin_addr.s_addr);
	if(-1 == ret)
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

	epollfd = epoll_create(1);
	if(-1 == epollfd)
	{
		perror("epoll_create");
		goto err1;
	}

	e.events = EPOLLIN;
	e.data.fd = sockfd;

	ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &e);
	if(-1 == ret)
	{
		perror("epoll_ctl");
		goto err2;
	}

	i = 0;
	while(1)
	{
		printf("---------- step in epoll loop in main   -----i = %d \n", i++);
		bzero(re, sizeof(re));
		ret = epoll_wait(epollfd, re, 128, 300000);
		if(-1 == ret)
		{
			perror("epoll_wait");
			break;
		}//epoll_wait goes wrong!
		if(0 == ret)
		{
			/* do nothing */;
		}//epoll timo
		else
		{
			count = ret;
			for(int i = 0; i<count; i++)
			{
				if(re[i].events & EPOLLIN)
				{
					if(re[i].data.fd == sockfd)
					{
						len = sizeof(from);
						bzero(&from, len);
						connfd = accept(sockfd, (void*)&from, &len);
						printf("\033[32mClient \033[0m%s:%d \033[32mconnected, fd = \033[0m%d\n", inet_ntoa(from.sin_addr), ntohs(from.sin_port), connfd);
						if(-1 == connfd)
						{
							perror("accept");
							continue;
						}
						flag = fcntl(connfd, F_GETFL);
						if(-1 == flag)
						{
							perror("fcntl");
							break;
						}
						flag |= O_NONBLOCK;
						ret = fcntl(connfd, F_SETFL, flag);
						if(-1 == ret)
						{
							perror("fcntl");
							break;
						}

						e.events = EPOLLIN | EPOLLET;
						e.data.fd = connfd;

						ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &e);
						if(-1 == ret)
						{
							perror("epoll_ctl");
							close(connfd);
							continue;
						}
					}
					else
					{
						http_request(re[i].data.fd);
					}
						printf("\033[31mstep out epoll loop in main   -----i = %d >>\033[0m\n", i++);
				}
			}
		}//else
	}//while(1)
    return 0;

err2:
	printf("---------------------2000-------------------\n");
	close(epollfd);
err1:
	printf("---------------------1000-------------------\n");
	close(sockfd);
err0:
	printf("---------------------0000-------------------\n");
	return 1;

}

