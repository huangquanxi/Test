#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include "mysock.h"

int fd;
int flag = 0;
char account[30];
/*
void setnonblocking(int sock)
{
	int opt;
	opt = fcntl(sock, F_GETFL);
	if(opt <0)
	{
		printf("fcntl error\n");
		return;
	}

	opt = opt | O_NONBLOCK;
	if(fcntl(sock, F_SETFL, opt) < 0)
	{
		printf("fcntl error\n");
		return;
	}

}
*/
void register1()
{
	MSGHEAD msgHead;
	int real_write;

	memset(&msgHead, 0, sizeof(msgHead));
	printf("please enter a new account:\n");
	scanf("%s",msgHead.account);
	printf("please enter your password:\n");
	scanf("%s",msgHead.passwd);
	msgHead.msglen = sizeof(msgHead);
	msgHead.msgtype = MSG_ORDER_REGISTER;

	msgHead.msglen = htonl(msgHead.msglen);
	msgHead.msgtype = htonl(msgHead.msgtype);

	real_write = write(fd, &msgHead, sizeof(msgHead));
	if(real_write < 0)
	{
		printf("write error\n");
		exit(-1);
	}

	return;

}

void login()
{
	MSGHEAD msgHead;
	int real_write;

	memset(&msgHead, 0, sizeof(msgHead));
	printf("please enter your account:\n");
	scanf("%s",msgHead.account);
	strcpy(account, msgHead.account);
	printf("please enter your password:\n");
	scanf("%s",msgHead.passwd);

	msgHead.msglen = sizeof(msgHead);
	msgHead.msgtype = MSG_ORDER_LOGIN;

	msgHead.msglen = htonl(msgHead.msglen);
	msgHead.msgtype = htonl(msgHead.msgtype);
    
	real_write = write(fd, &msgHead, sizeof(msgHead));
	if(real_write < 0)
	{
		printf("write error\n");
		exit(-1);
	}

	return;
}

void queryRestaurant()
{
	MSGHEAD msgHead;
	int real_write;

	memset(&msgHead, 0, sizeof(msgHead));

	msgHead.msglen = sizeof(msgHead);
	msgHead.msgtype = MSG_ORDER_RESTAURANT;

	msgHead.msglen = htonl(msgHead.msglen);
	msgHead.msgtype = htonl(msgHead.msgtype);
    
	real_write = write(fd, &msgHead, sizeof(msgHead));
	if(real_write < 0)
	{
		printf("write error\n");
		exit(-1);
	}

	return;
}

void queryMenu()
{
	MSGHEAD msgHead;
	int real_write;

	memset(&msgHead, 0, sizeof(msgHead));
	printf("\nplease enter a restaurant name:\n");
    scanf("%s",msgHead.restaurant);
	msgHead.msglen = sizeof(msgHead);
	msgHead.msgtype = MSG_ORDER_MENU;

	msgHead.msglen = htonl(msgHead.msglen);
	msgHead.msgtype = htonl(msgHead.msgtype);
    
	real_write = write(fd, &msgHead, sizeof(msgHead));
	if(real_write < 0)
	{
		printf("write error\n");
		exit(-1);
	}

	return;
}

void order()
{
	MSGHEAD msgHead;
	int real_write;

	memset(&msgHead, 0, sizeof(msgHead));
	printf("\nplease enter a id:\n");
    scanf("%d",&(msgHead.id));
	msgHead.msglen = sizeof(msgHead);
	msgHead.msgtype = MSG_ORDER_ORDER;
	strcpy(msgHead.account, account);

	msgHead.msglen = htonl(msgHead.msglen);
	msgHead.msgtype = htonl(msgHead.msgtype);
	msgHead.id = htonl(msgHead.id);
    
	real_write = write(fd, &msgHead, sizeof(msgHead));
	if(real_write < 0)
	{
		printf("write error\n");
		exit(-1);
	}

	return;
}

void queryOrder()
{
	MSGHEAD msgHead;
	int real_write;

	memset(&msgHead, 0, sizeof(msgHead));

	msgHead.msglen = sizeof(msgHead);
	msgHead.msgtype = MSG_ORDER_QUERY;
    strcpy(msgHead.account, account);

	msgHead.msglen = htonl(msgHead.msglen);
	msgHead.msgtype = htonl(msgHead.msgtype);
    
	real_write = write(fd, &msgHead, sizeof(msgHead));
	if(real_write < 0)
	{
		printf("write error\n");
		exit(-1);
	}

	return;
}

void deleteOrder()
{
	MSGHEAD msgHead;
	int real_write;

	memset(&msgHead, 0, sizeof(msgHead));

	printf("\nplease enter a id:\n");
    scanf("%d",&(msgHead.id));
	msgHead.msglen = sizeof(msgHead);
	msgHead.msgtype = MSG_ORDER_DELETE;
    strcpy(msgHead.account, account);

	msgHead.msglen = htonl(msgHead.msglen);
	msgHead.msgtype = htonl(msgHead.msgtype);
    msgHead.id = htonl(msgHead.id);

	real_write = write(fd, &msgHead, sizeof(msgHead));
	if(real_write < 0)
	{
		printf("write error\n");
		exit(-1);
	}

	return;
}

void* handler(void* arg)
{
	int real_write;
	int real_read;
	char buf[1024];
    MSGHEAD msghead1;
	int num = 0;

	while(1)
	{
		flag = 0;
		//memset(buf, 0, 1024);
		printf("\n");
		printf("****************************\n");
		printf("1.login\n");
		printf("2.register a new account\n");
		printf("0.exit\n");
		printf("****************************\n");
		printf("\n");
		printf("please enter your choice:\n");
		scanf("%d", &num);
        
		if(num == 1)
		{
            login();

			while(flag == 0)
			{}
			if(flag == -1)
			{
				continue;
			}
			else if(flag == 1)
			{
				break;

			}
		}
		else if(num == 2)
		{
			register1();
			while(flag == 0)
			{}
			continue;
		
		}
		else if(num == 0)
		{
			flag = 99;
			return NULL;
		}

	}

	while(1)
	{
		flag = 1;

		printf("\n");
		printf("****************************\n");
		printf("1.query restaurant\n");
		printf("2.query menu\n");
		printf("3.order\n");
		printf("4.query order\n");
		printf("5.delete order\n");
		printf("0.exit\n");
		printf("****************************\n");
		printf("\n");
		printf("please enter your choice:\n");
		scanf("%d", &num);

		if(num == 1)
		{
			queryRestaurant();
			while(flag == 1);
			continue;
		}
		else if(num == 2)
		{
			queryMenu();
			while(flag == 1);
			continue;
		}
		else if(num == 3)
		{
			order();
			while(flag == 1);
			continue;
		}
		else if(num == 4)
		{
			queryOrder();
			while(flag == 1);
			continue;
		}
		else if(num == 5)
		{
			deleteOrder();
			while(flag == 1);
			continue;

		}
		else if(num == 0)
		{
			flag = 99;
			return NULL;
		}
	}
}
void handlerServerResult(MSGRES *pmsgres)
{
	pmsgres->msglen = ntohl(pmsgres->msglen);
	pmsgres->msgtype = ntohl(pmsgres->msgtype);
	pmsgres->result = ntohl(pmsgres->result);

	if(pmsgres->msgtype == MSG_ORDER_REGISTER_RES)
	{
		if(pmsgres->result == 0)
		{
			printf("%s\n", pmsgres->buff);
			flag = 1;
		}
		else
		{
			printf("%s\n",pmsgres->buff);
			flag = -1;
		}
	}
	else if(pmsgres->msgtype == MSG_ORDER_LOGIN_RES)
	{
		if(pmsgres->result == 0)
		{
			printf("%s\n", pmsgres->buff);
			flag = 1;
		}
		else
		{
			printf("%s\n",pmsgres->buff);
			flag = -1;
		}
	}
	else if(pmsgres->msgtype == MSG_ORDER_RESTAURANT_RES)
	{
		if(pmsgres->result == 0)
		{
			printf("%s\n", pmsgres->buff);
			flag = 2;
		}
		else
		{
			printf("%s\n",pmsgres->buff);
			flag = -1;
		}
	}
	else if(pmsgres->msgtype == MSG_ORDER_MENU_RES)
	{
		if(pmsgres->result == 0)
		{
			printf("%s\n", pmsgres->buff);
			flag = 2;
		}
		else
		{
			printf("%s\n",pmsgres->buff);
			flag = -1;
		}
	}
	else if(pmsgres->msgtype == MSG_ORDER_ORDER_RES)
	{
		if(pmsgres->result == 0)
		{
			printf("%s\n", pmsgres->buff);
			flag = 2;
		}
		else
		{
			printf("%s\n",pmsgres->buff);
			flag = -1;
		}
	}
	else if(pmsgres->msgtype == MSG_ORDER_QUERY_RES)
	{
		if(pmsgres->result == 0)
		{
			printf("%s\n", pmsgres->buff);
			flag = 2;
		}
		else
		{
			printf("%s\n",pmsgres->buff);
			flag = -1;
		}
	}
	else if(pmsgres->msgtype == MSG_ORDER_DELETE_RES)
	{
		if(pmsgres->result == 0)
		{
			printf("%s\n", pmsgres->buff);
			flag = 2;
		}
		else
		{
			printf("%s\n",pmsgres->buff);
			flag = -1;
		}
	}



}
int main(int argc, char ** argv)
{
	//	int fd;
	int ret;
	struct sockaddr_in clientAddr;

	if(argc != 3)
	{
		printf("para error\n");
		return -1;
	}
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0)
	{
		printf("socket error\n");
		return -1;
	}

	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(atoi(argv[2]));
	clientAddr.sin_addr.s_addr = inet_addr(argv[1]);

	ret = connect(fd,(struct sockaddr *) &clientAddr, sizeof(clientAddr));
	if(ret < 0)
	{
		printf("connect error\n");
		return -1;
	}

	
//    setnonblocking(fd);
	pthread_t pth_id;
	ret = pthread_create(&pth_id, NULL, handler, NULL);

	char buf[4096];
	MSGRES msgres;
	while(flag != 99)
	{
		memset(buf, 0, 4096);
		memset(&msgres, 0, sizeof(msgres));

		ret = read(fd, buf, 4096);
		if(ret > 0)
		{
			memcpy(&msgres, buf, sizeof(msgres));
			handlerServerResult(&msgres);
			//printf("client A recv msg from server:%s\n", buf);
		}
		if(ret == 0)
		{
			printf("client no msg\n");
			sleep(1);
		}
	}

	close(fd);
	pthread_join(pth_id, NULL);
	return 0;

}
