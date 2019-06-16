#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include<sys/stat.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<dirent.h>
#include<unistd.h>
#include<fcntl.h>
#include "pub.h"

#define SRV_PORT 10000
#define MAXEVENT 1024	
#define BUFSIZE 128

//发送http协议头
void send_header(int connfd,int code, char* msg, char* fileType, int fileSize)
{
	char buf[BUFSIZE];
	
	//发送响应行
	bzero(buf,BUFSIZE);
	sprintf(buf,"HTTP/1.1 %d %s\r\n",code,msg);
	send(connfd,buf,strlen(buf),0);

	//发送响应头
	bzero(buf,BUFSIZE);
	sprintf(buf,"Content-Type:%s\r\n",fileType);
	send(connfd,buf,strlen(buf),0);

	bzero(buf,BUFSIZE);
	sprintf(buf,"Content-Length:%d\r\n",fileSize);
	send(connfd,buf,strlen(buf),0);

	bzero(buf,BUFSIZE);
	sprintf(buf,"Connection:close\r\n");
	send(connfd,buf,strlen(buf),0);

	//发送空行
	bzero(buf,BUFSIZE);
	sprintf(buf,"\r\n");
	send(connfd,buf,strlen(buf),0);
}

//发送文件
void send_file(int connfd,char* fileName)
{
	printf("进行发送文件，文件名：%s\n",fileName);
	int ret;
	int fd;
	char buf[4096];
	//打开文件
	fd=open(fileName,O_RDONLY);
	if(-1==fd)
	{
		perror("open");
		return;
	}
	while(1)
	{
		ret = read(fd,buf,4096);
		if(ret==0)
		{
			break;
		}
		ret = send(connfd,buf,ret,0);
		if(ret<=0)
		{
			perror("send");
			break;
		}
	}
	close(fd);
	return;
}

//发送目录
void send_dir(int connfd,char* fileName)
{
	printf("进行发送目录...\n");
	int ret=-1;
	DIR *dir=NULL;
	struct dirent *rdir;
	char buf[1024];
	int i = 0;

	dir = opendir(fileName);
	if(NULL==dir)
   {
		perror("opendir");
		return;
	}
	printf("成功打开目录...\n");
	
	
	while(1)
	{
		i++;
		rdir = readdir(dir);
		if(rdir==NULL)
		{
			printf("------------------>%d\n",__LINE__);
			close(connfd);
			break;
			
		}
		printf("------------100 i = %d------------\n", i);
		memset(buf,0,1024);
		printf("------------150 i = %d------------\n", i * 10);
		//system("ls");
		printf("读取目录:%s...\n",rdir->d_name);
		if(rdir->d_type==DT_REG)
		{
				printf("%d\n",__LINE__);
			sprintf(buf,"<li><a href=\"%s\">%s</a></li>",rdir->d_name,rdir->d_name);
			ret = send(connfd,buf,strlen(buf),0);
			if(ret<=0)
			{
				printf("%d\n",__LINE__);
				perror("send");
				break;
			}
		}
		else if(rdir->d_type==DT_DIR)
		{
			printf("%d\n",__LINE__);
			ret = sprintf(buf,"<li><a href=\"%s/\">%s</a></li>",rdir->d_name,rdir->d_name);
			printf("sprintf ret:%d\n",ret);
			if(-1==ret)
			{
				perror("sprintf");
				break;
			}
			printf("ret data :%sconnectfd :%d\n",buf,connfd);

			ret = send(connfd,buf,strlen(buf), 0);
			printf("send ret:%d\n",ret);
			if(ret<=0)
			{
				printf("%d\n",__LINE__);
				perror("send");
				break;
			}
		}
	}
	closedir(dir);
	printf("目录发送完成\n");
	return;
}

//发送目录html头
void send_dir_htmlhead(int connfd)
{
	char buf[BUFSIZE];
	bzero(buf,BUFSIZE);
	sprintf(buf,"<!DOCTYPE html><html><head><title>文件</title></head><body><ul>");
	send(connfd,buf,strlen(buf),0);
}

//发送目录html尾
void send_dir_htmltail(int connfd)
{
	char buf[BUFSIZE];
	bzero(buf,BUFSIZE);
	sprintf(buf,"</ul></body></html>");
	send(connfd,buf,strlen(buf),0);
}

//进行web请求处理
void http_request(int epfd,int connfd)
{
	char line[BUFSIZE];
	char buf[BUFSIZE];

	char method[32];
	char url[32];
	char version[32];
	char *pfile=NULL;

	struct stat s;

	int ret=-1;
	
	printf("读取请求中...\n");

	//读取请求行
	memset(line,0,BUFSIZE);
	ret=get_line(connfd,line,BUFSIZE);
	printf("%s",line);

	//读取请求头标
	while(1)
	{
		memset(buf,0,BUFSIZE);
		ret = get_line(connfd,buf,BUFSIZE);
		if(ret<=0)
		{
			perror("recv");
			break;
		}
		printf("%s",buf);
	}
	printf("请求读取成功...\n");

	
	//分割出方法，url地址，版本
	bzero(method,32);
	bzero(url,32);
	bzero(version,32);
	
	sscanf(line,"%s %s %s",method,url,version);
	printf("要进行的方法：%s,资源地址：%s\n",method,url);

	//如果是get请求
	if(strncmp(method,"GET",3)==0)
	{
		if(strcmp(url,"/")==0)
			pfile="./";
		else{
			pfile=url+1;
		}

		printf("文件名：%s\n",pfile);
		//获取文件相关信息
		ret=stat(pfile,&s);
		printf("ret:%d\n",ret);
		if(-1==ret)
		{
			if(errno==ENOENT)//如果请求资源不存在
			{
				printf("进行发送404错误页面\n");
				send_header(connfd,404,"err",".html",-1);
			}
			perror("stat");
			return;
		}
		if((s.st_mode&S_IFMT)==S_IFREG)//普通文件
		{
			send_header(connfd,200,"ok",get_mime_type(pfile),s.st_size);
			send_file(connfd,pfile);
		}
		else if((s.st_mode&S_IFMT)==S_IFDIR)//目录文件
		{
			send_header(connfd,200,"ok",".html",-1);
			send_dir_htmlhead(connfd);
			send_dir(connfd,pfile);
			send_dir_htmltail(connfd);
		}
		epoll_ctl(epfd, EPOLL_CTL_DEL, connfd, NULL);

	}
}

int main(int argc, char** argv)
{
	int listenfd,connfd,epfd;
	int ret=-1;
	int i,nready;
	struct sockaddr_in srvaddr,cltaddr;
	socklen_t cltaddrlen;

	struct epoll_event rev[MAXEVENT],ev;

	//1、创建套接字
	listenfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==listenfd)
	{
		perror("socket");
		goto err0;
	}

	//2、绑定桃姐字
	srvaddr.sin_family=AF_INET;
	srvaddr.sin_port=htons(atoi(argv[1]));
	srvaddr.sin_addr.s_addr=INADDR_ANY;
	ret=bind(listenfd,(void*)&srvaddr,sizeof(srvaddr));
	if(-1==ret)
	{
		perror("bind");
		goto err1;
	}

	//3、设置端口复用
	i=1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, &i, sizeof(i));
	
	//4、设置监听
	ret=listen(listenfd,100);
	if(-1==ret)
	{
		perror("listen");
		goto err1;
	}

	//5、创建epoll专用文件描述符
	epfd=epoll_create(MAXEVENT);
	if(-1==epfd)
	{
		perror("epoll_create");
		goto err1;
	}

	//6、将监听文件描述符添加到epoll监听事件中
	ev.events=EPOLLIN;
	ev.data.fd=listenfd;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
	if(-1==ret)
	{
		perror("epoll_ctl");
		goto err2;
	}
	printf("服务器处于监听状态中\n");

	//7、循环监听
	while(1)
	{
		nready=epoll_wait(epfd,rev,MAXEVENT,3000000);
		if(-1==nready)
		{
			perror("epoll_wait");
			break;
		}
		else if(0==nready)
		{
			printf("3 sec timeout...\n");
			continue;
		}
		else
		{
			for(i=0;i<nready;i++)
			{
				if(rev[i].events&EPOLLIN)
				{
					if(rev[i].data.fd==listenfd)//有新客户端连接
					{
						connfd=accept(listenfd,(void*)&cltaddr,&cltaddrlen);
						if(-1==connfd)
						{
							continue;
						}
						
						//设置为非阻塞
						ret = fcntl(connfd,F_GETFL);
						ret|=O_NONBLOCK;
						fcntl(connfd,F_SETFL,ret);

						//添加最新文件描述符到此集合中
						ev.events=EPOLLIN|EPOLLET;
						ev.data.fd=connfd;
						epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
						printf("有新的客户端连接成功%s:%d...\n",inet_ntoa(cltaddr.sin_addr),ntohs(cltaddr.sin_port));
					}
					else//有新的web请求
					{
						http_request(epfd,rev[i].data.fd);
						printf("响应web请求成功\n");
					}
				}
			}
		}
	}


	return 0;

err2:
	close(epfd);
err1:
	close(listenfd);
err0:
	return 1;
}


