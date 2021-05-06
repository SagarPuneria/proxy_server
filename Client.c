#include<stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<string.h>
int main()
{
	int sockfd,val,ret,size;
	struct sockaddr_in saddr,caddr;
	char buff[2000];

	/*creating the end point of communication*/
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0){
		printf("client:socket creation failed\n");
		return 0;
	}
	
	/*naming the socket(usr/include/linux/in.h)*/
	caddr.sin_family=AF_INET;
	caddr.sin_port=htons(9100);//client port
	caddr.sin_addr.s_addr=inet_addr("10.244.15.9");//client ip
	ret=bind(sockfd,(struct sockaddr *)&caddr,sizeof(caddr));
	if(ret<0)
	{
		printf("client:bind failed\n");
		return 0;
	}

	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(9200);//proxy server port
	saddr.sin_addr.s_addr=inet_addr("10.244.15.13");//proxy server ip
	connect(sockfd,(struct sockaddr *)&saddr,sizeof(saddr));

	size=sizeof(caddr);
	getsockname(sockfd,(struct sockaddr *)&caddr,&size);
	printf("client ip:%s,client port:%d\n",inet_ntoa(caddr.sin_addr),ntohs(caddr.sin_port));

	memset(buff,0,sizeof(buff));
	printf("Enter file name\n");
	scanf("%s",buff);
	write(sockfd,buff,sizeof(buff));

	memset(buff,0,sizeof(buff));
	
	read(sockfd,buff,sizeof(buff));
	printf("Data read\n%s\n",buff);

	close(sockfd);
	return 0;
}
