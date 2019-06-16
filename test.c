#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h> 
#include <headers.h>

#define SIZE 128

 int main(void)
 {
     int fd = -1;
     static char c = '0';
     int epfd,nfds;
     struct epoll_event ev,events[5];
      
      int ret = -1;
      char buffer[SIZE];

     epfd = epoll_create(1);
     ev.data.fd = STDIN_FILENO;
     ev.events = EPOLLIN|EPOLLET;
     epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);
     for(;;)
     {
         nfds = epoll_wait(epfd, events, 5, -1);
         for(int i = 0; i < nfds; i++)
         {
             if(events[i].data.fd==STDIN_FILENO)
             {
                 printf("welcome to epoll's word!");
                 fflush(stdout);
                 buffer[i] = c;
                 fd = open("log", O_CREAT | O_APPEND, 0644);
                 write(fd, buffer, strlen(buffer) );
                 close(fd);
                 ev.data.fd = STDIN_FILENO;
				 ev.events = EPOLLIN|EPOLLET;
//                 epoll_ctl(epfd, EPOLL_CTL_MOD, STDIN_FILENO, &ev);
             }            
         }
     }
 }
