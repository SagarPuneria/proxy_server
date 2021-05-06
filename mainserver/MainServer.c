#include<stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<dirent.h>
#include<fcntl.h>
void func(int sig)
{
	int stat;
	printf("before wait\n");
	wait(&stat);
	printf("after wait\n");
}
int main()
{
	pid_t pid;
	int listenfd,ret,size,sessfd;
	struct sockaddr_in saddr,caddr;
	char main[100],proxy[100],buff[2000],buf[100];
	
	int val=1;
	int fd,fd1,fd2;
	
	/*creating the end point of communication*/
	listenfd=socket(AF_INET,SOCK_STREAM,0);
	if(listenfd<0){
		printf("main server:socket creation failed\n");
		return 0;
	}

	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val));
	/*naming the socket(usr/include/linux/in.h)*/
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(9400);//main server port
	saddr.sin_addr.s_addr=inet_addr("10.244.15.12");//main server ip
	ret=bind(listenfd,(struct sockaddr *)&saddr,sizeof(saddr));
	if(ret<0)
	{
		printf("main server:bind failed\n");
		return 0;
	}

	listen(listenfd,5);
	
	signal(SIGCHLD,func);
		
	while(1)
	{
		memset(buff,0,sizeof(buff));
		size=sizeof(caddr);
		//printf("before accept\n");

		sessfd=accept(listenfd,(struct sockaddr *)&caddr,&size);
		if(sessfd<0)
		{
			printf("main server:accept failed\n");
			continue;
		}

		pid=fork();
		if(pid>0)//parent
		{
			close(sessfd);
			continue;
		}
		close(listenfd);
		printf("child executes sessfd\n");
		printf("proxy server ip:%s,proxy server port:%d\n",inet_ntoa(caddr.sin_addr),ntohs(caddr.sin_port)); 
		if(strcmp(inet_ntoa(caddr.sin_addr),"10.244.15.13")==0||strcmp(inet_ntoa(caddr.sin_addr),"10.244.15.14")==0)
		{
			read(sessfd,buf,sizeof(buf));
			printf("data read %s\n",buf);
			if(strcmp(buf,"hello.c")==0||strcmp(buf,"bye.c")==0||strcmp(buf,"p1.htm")==0||strcmp(buf,"p9.htm")==0)
			{
				strcpy(main,"./main/");
				strcat(main,buf);
				fd1=open(main,O_RDONLY,0644);
				if(fd1<0)
				{
					printf("main server:open failed\n");
					return -1;
				}
				memset(buff,0,sizeof(buff));
				read(fd1,buff,sizeof(buff));
				//printf("%s",buff);
				write(sessfd,buff,sizeof(buff));
			}
			else 
				write(sessfd,"File is Blocked",sizeof(buff));
		}
		else
			write(sessfd,"IP is Blocked",sizeof(buff));	
		close(sessfd);
		_exit(0);
	}
	return 0;
}
