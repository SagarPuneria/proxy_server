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
	int listenfd,ret,size,sessfd,sockfd;
	struct sockaddr_in saddr,saddr1,caddr,caddr1;
	char main[100],proxy[100],buff[2000],buf[100];
	
	int val=1;
	int fd,fd1,fd2;
	
	/*creating the end point of communication*/
	listenfd=socket(AF_INET,SOCK_STREAM,0);
	if(listenfd<0){
		printf("proxy server:socket creation failed\n");
		return 0;
	}

	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val));
	/*naming the socket(usr/include/linux/in.h)*/
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(9200);//proxy server port
	saddr.sin_addr.s_addr=inet_addr("10.244.15.13");//proxy server ip
	ret=bind(listenfd,(struct sockaddr *)&saddr,sizeof(saddr));
	if(ret<0)
	{
		printf("proxy server:bind failed\n");
		return 0;
	}

	listen(listenfd,5);
	
	signal(SIGCHLD,func);
		
	while(1)
	{
		memset(buff,0,sizeof(buff));
		size=sizeof(caddr);
		printf("befro accept \n");
		sessfd=accept(listenfd,(struct sockaddr *)&caddr,&size);
		if(sessfd<0)
		{
			printf("proxy server:accept failed\n");
			continue;
		}
		printf("after accept \n");
		pid=fork();
		if(pid>0)//parent
		{
			close(sessfd);
			continue;
		}
		close(listenfd);
		printf("child executes sessfd\n");
		printf("client ip:%s,client port:%d\n",inet_ntoa(caddr.sin_addr),ntohs(caddr.sin_port)); 
		if(strcmp(inet_ntoa(caddr.sin_addr),"10.244.15.9")==0||strcmp(inet_ntoa(caddr.sin_addr),"10.244.15.10")==0)
		{
			read(sessfd,buf,sizeof(buf));
			printf("data read %s\n",buf);
			if(strcmp(buf,"hello.c")==0||strcmp(buf,"bye.c")==0||strcmp(buf,"p1.htm")==0||strcmp(buf,"p9.htm")==0)
			{
				strcpy(proxy,"./prox/");
				strcat(proxy,buf);
				fd=open(proxy,O_RDONLY,0644);
				if(fd<0)
				{
					
					sockfd=socket(AF_INET,SOCK_STREAM,0);
					if(sockfd<0){
						printf("proxy server:socket creation failed\n");
						return 0;
					}
					caddr1.sin_family=AF_INET;
					caddr1.sin_port=htons(9300);//proxy server port
					caddr1.sin_addr.s_addr=inet_addr("10.244.15.13");//proxy server ip
					ret=bind(sockfd,(struct sockaddr *)&caddr1,sizeof(caddr1));
					if(ret<0)
					{
						printf("proxy server:bind failed\n");
						return 0;
					}
					saddr1.sin_family=AF_INET;
					saddr1.sin_port=htons(9400);//main sever port
					saddr1.sin_addr.s_addr=inet_addr("10.244.15.12");//main sever ip
					connect(sockfd,(struct sockaddr *)&saddr1,sizeof(saddr1));
					size=sizeof(caddr);
					getsockname(sockfd,(struct sockaddr *)&caddr,&size);
					printf("client ip:%s,client port:%d\n",inet_ntoa(caddr.sin_addr),ntohs(caddr.sin_port));
					memset(buff,0,sizeof(buff));
					strcpy(buff,buf);
					write(sockfd,buff,sizeof(buff));
					memset(buff,0,sizeof(buff));
					read(sockfd,buff,sizeof(buff));
					//printf("data read\n %s\n",buff);
					fd2=creat(proxy, 0644);
					if(fd2<0)
					{
						printf("proxy server:creat file in failed\n");
						return -1;
					}
					write(fd2,buff,strlen(buff));
					close(sockfd);
				}
				else
				{
					memset(buff,0,sizeof(buff));
					read(fd,buff,sizeof(buff));
				}
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
