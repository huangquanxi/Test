#include "mysock.h"
#include <mysql/mysql.h>
#include <stdlib.h>
#include <string.h>

int count = 0;

int orderRegister(NODE *pnode)
{
	char sql[1024];
	MYSQL db,*pdb;

	mysql_init(&db);
	pdb = mysql_real_connect(&db, "localhost", "root", "123", "orderfood",0,NULL,0);
	if(pdb == NULL)
	{
		printf("mysql connect failed\n");
	}
	else
	{
		printf("mysql connect success\n");
	}
	sprintf(sql, "insert into accountTable(name, passwd, type) values(\'%s\',\'%s\',%d)",pnode->msghead.account,pnode->msghead.passwd,0);
	if(mysql_query(&db, sql))
	{
		printf("insert failed\n");
		return -1;
	}
	mysql_close(&db);
	return 0;
}

int orderLogin(NODE *pnode)
{
	char sql[1024];
	MYSQL db,*pdb;

	mysql_init(&db);
	pdb = mysql_real_connect(&db, "localhost", "root", "123", "orderfood",0,NULL,0);
	if(pdb == NULL)
	{
		printf("mysql connect failed\n");
	}
	else
	{
		printf("mysql connect success\n");
	}
	sprintf(sql,"select passwd from accountTable where name = \'%s\';",pnode->msghead.account);
	if(mysql_query(&db, sql))
	{
		printf("select failed\n");
		return -1;
	}
	MYSQL_FIELD *field;
	int row;
	MYSQL_RES *ptr;
	MYSQL_ROW result;

	ptr = mysql_store_result(&db);

	row = mysql_num_rows(ptr);
	if(row <= 0)
	{
		mysql_close(&db);
		return -1;
	}

	result = mysql_fetch_row(ptr);
	if(strcmp(pnode->msghead.passwd, result[0]) != 0)
	{
		mysql_close(&db);
		return -1;
	}

	mysql_close(&db);
	return 0;
}

int orderRestaurant(NODE *pnode, char * pbuff)
{
	char sql[1024];
	MYSQL db,*pdb;
	int i;
	char str[10];

	mysql_init(&db);
	pdb = mysql_real_connect(&db, "localhost", "root", "123", "orderfood",0,NULL,0);
	if(pdb == NULL)
	{
		printf("mysql connect failed\n");
	}
	else
	{
		printf("mysql connect success\n");
	}
	sprintf(sql,"select name from restaurant");
	if(mysql_query(&db, sql))
	{
		printf("select failed\n");
		return -1;
	}
	MYSQL_FIELD *field;
	int row;
	MYSQL_RES *ptr;
	MYSQL_ROW result;

	ptr = mysql_store_result(&db);

	row = mysql_num_rows(ptr);
	if(row <= 0)
	{
		mysql_close(&db);
		return -1;
	}


	strcat(pbuff, "\n");
	strcat(pbuff,"restaurant info:\n");
    for(i =0; i<row; i++)
	{
		result =mysql_fetch_row(ptr);
        sprintf(str,"%d", i+1);
		strcat(pbuff,str);
		strcat(pbuff,". ");
		strcat(pbuff,result[0]);
		strcat(pbuff,"\n");

	}

	mysql_close(&db);
	return 0;
}


int orderMenu(NODE *pnode, char * pbuff)
{
	char sql[1024];
	MYSQL db,*pdb;
	int i,j;
	char str[10];

	mysql_init(&db);
	pdb = mysql_real_connect(&db, "localhost", "root", "123", "orderfood",0,NULL,0);
	if(pdb == NULL)
	{
		printf("mysql connect failed\n");
	}
	else
	{
		printf("mysql connect success\n");
	}
	sprintf(sql,"select * from menu where restaurantName = \'%s\';",pnode->msghead.restaurant);
	if(mysql_query(&db, sql))
	{
		printf("select failed\n");
		return -1;
	}
	MYSQL_FIELD *field;
	int row,column;
	MYSQL_RES *ptr;
	MYSQL_ROW result;

	ptr = mysql_store_result(&db);
	column =mysql_num_fields(ptr);


	row = mysql_num_rows(ptr);
	if(row <= 0)
	{
		mysql_close(&db);
		return -1;
	}


	strcat(pbuff, "\n");
    for(i = 0; field = mysql_fetch_field(ptr); i++)
	{
		sprintf(pbuff,"%s%15s",pbuff,field->name);
		//strcat(pbuff, field->name);
		//srtcat(pbuff, "  ")
	}
	strcat(pbuff,"\n");
	//strcat(pbuff,"restaurant info:\n")
    for(i =0; i<row; i++)
	{
		result =mysql_fetch_row(ptr);
		for(j = 0; j< column; j++)
		{
			sprintf(pbuff,"%s%15s",pbuff,result[j]);
		//strcat(pbuff,result[0]);
		//srtcat(pbuff,"  ");
		}
		strcat(pbuff,"\n");

	}

	mysql_close(&db);
	return 0;
}

int orderOrder(NODE *pnode)
{
	char sql[1024];
	MYSQL db,*pdb;
	int i,j;
	char str[10];

	mysql_init(&db);
	pdb = mysql_real_connect(&db, "localhost", "root", "123", "orderfood",0,NULL,0);
	if(pdb == NULL)
	{
		printf("mysql connect failed\n");
	}
	else
	{
		printf("mysql connect success\n");
	}
	sprintf(sql,"select * from menu where id = %d;",pnode->msghead.id);
	if(mysql_query(&db, sql))
	{
		printf("select failed\n");
		return -1;
	}
	MYSQL_FIELD *field;
	int row,column;
	MYSQL_RES *ptr;
	MYSQL_ROW result;

	ptr = mysql_store_result(&db);
	column =mysql_num_fields(ptr);


	row = mysql_num_rows(ptr);
	if(row <= 0)
	{
		mysql_close(&db);
		return -1;
	}

	result =mysql_fetch_row(ptr);
 
	sprintf(sql,"insert into orderlist (restaurant, name, price, status,account) values (\'%s\',\'%s\',%f,\'%s\',\'%s\');",result[1],result[2],atof(result[3]),"order",pnode->msghead.account);
	//printf("sql = %s\n",sql);
	if(mysql_query(&db, sql))
	{
		printf("insert failed\n");
		return -1;
	}

	mysql_close(&db);
	return 0;
}

int orderQuery(NODE *pnode, char * pbuff)
{
	char sql[1024];
	MYSQL db,*pdb;
	int i,j;
	char str[10];

	mysql_init(&db);
	pdb = mysql_real_connect(&db, "localhost", "root", "123", "orderfood",0,NULL,0);
	if(pdb == NULL)
	{
		printf("mysql connect failed\n");
	}
	else
	{
		printf("mysql connect success\n");
	}
	sprintf(sql,"select * from orderlist where account = \'%s\';",pnode->msghead.account);
	if(mysql_query(&db, sql))
	{
		printf("select failed\n");
		return -1;
	}
	MYSQL_FIELD *field;
	int row,column;
	MYSQL_RES *ptr;
	MYSQL_ROW result;

	ptr = mysql_store_result(&db);
	column =mysql_num_fields(ptr);


	row = mysql_num_rows(ptr);
	if(row <= 0)
	{
		mysql_close(&db);
		return -1;
	}


	strcat(pbuff, "\n");
    for(i = 0; field = mysql_fetch_field(ptr); i++)
	{
		sprintf(pbuff,"%s%15s",pbuff,field->name);
		//strcat(pbuff, field->name);
		//srtcat(pbuff, "  ")
	}
	strcat(pbuff,"\n");
	//strcat(pbuff,"restaurant info:\n")
    for(i =0; i<row; i++)
	{
		result =mysql_fetch_row(ptr);
		for(j = 0; j< column; j++)
		{
			sprintf(pbuff,"%s%15s",pbuff,result[j]);
		//strcat(pbuff,result[0]);
		//srtcat(pbuff,"  ");
		}
		strcat(pbuff,"\n");

	}

	mysql_close(&db);
	return 0;
}

int orderDelete(NODE *pnode)
{
	char sql[1024];
	MYSQL db,*pdb;

	mysql_init(&db);
	pdb = mysql_real_connect(&db, "localhost", "root", "123", "orderfood",0,NULL,0);
	if(pdb == NULL)
	{
		printf("mysql connect failed\n");
	}
	else
	{
		printf("mysql connect success\n");
	}
	sprintf(sql, "update orderlist set status = 'delete' where id = %d;",pnode->msghead.id);
	printf("sql = %s\n",sql);
	if(mysql_query(&db, sql))
	{
		printf("update failed\n");
		return -1;
	}
	mysql_close(&db);
	return 0;
}
void *handler(void *arg)
{
	NODE *pnode;
	MSGRES msgres;
	int ret;

	while(1)
	{
		if(head.next != NULL)
		{
			pnode = head.next;
			head.next = head.next->next;

			if(pnode->msghead.msgtype == MSG_ORDER_REGISTER)
			{
				memset(&msgres, 0, sizeof(msgres));
				ret = orderRegister(pnode);
				if(ret == 0)
				{
					msgres.msglen = sizeof(msgres);
					msgres.msgtype = MSG_ORDER_REGISTER_RES;
					msgres.result = 0;
//					strcpy(msgres.buff, "register ok!");

					msgres.msglen = htonl(msgres.msglen);
					msgres.msgtype = htonl(msgres.msgtype);

					ret = write(pnode->fd, &msgres, sizeof(msgres));
					if(ret < 0)
					{
						printf("write error\n");
						return NULL;
					}
				}
				else
				{

					msgres.msglen = sizeof(msgres);
					msgres.msgtype = MSG_ORDER_REGISTER_RES;
					msgres.result = 1;
					strcpy(msgres.buff, "query restaurant error!\n");

					msgres.msglen = htonl(msgres.msglen);
					msgres.msgtype = htonl(msgres.msgtype);
					ret = write(pnode->fd, &msgres, sizeof(msgres));
					if(ret < 0)
					{
						printf("write error\n");
						return NULL;
					}
				}


			}
			else if(pnode->msghead.msgtype == MSG_ORDER_LOGIN)
			{

				memset(&msgres, 0, sizeof(msgres));
				ret = orderLogin(pnode);
				if(ret == 0)
				{
					msgres.msglen = sizeof(msgres);
					msgres.msgtype = MSG_ORDER_LOGIN_RES;
					msgres.result = 0;
					strcpy(msgres.buff, "Login ok!");

					msgres.msglen = htonl(msgres.msglen);
					msgres.msgtype = htonl(msgres.msgtype);

					ret = write(pnode->fd, &msgres, sizeof(msgres));
					if(ret < 0)
					{
						printf("write error\n");
						return NULL;
					}
				}
				else
				{

					msgres.msglen = sizeof(msgres);
					msgres.msgtype = MSG_ORDER_LOGIN_RES;
					msgres.result = 1;
					strcpy(msgres.buff, "Login failed!");

					msgres.msglen = htonl(msgres.msglen);
					msgres.msgtype = htonl(msgres.msgtype);
					ret = write(pnode->fd, &msgres, sizeof(msgres));
					if(ret < 0)
					{
						printf("write error\n");
						return NULL;
					}
				}

			}
			else if(pnode->msghead.msgtype == MSG_ORDER_RESTAURANT)
			{

				memset(&msgres, 0, sizeof(msgres));
				ret = orderRestaurant(pnode, msgres.buff);
				if(ret == 0)
				{
					msgres.msglen = sizeof(msgres);
					msgres.msgtype = MSG_ORDER_RESTAURANT_RES;
					msgres.result = 0;
		//			strcpy(msgres.buff, "Login ok!");

					msgres.msglen = htonl(msgres.msglen);
					msgres.msgtype = htonl(msgres.msgtype);

					ret = write(pnode->fd, &msgres, sizeof(msgres));
					if(ret < 0)
					{
						printf("write error\n");
						return NULL;
					}
				}
				else
				{

					msgres.msglen = sizeof(msgres);
					msgres.msgtype = MSG_ORDER_LOGIN_RES;
					msgres.result = 1;
					strcpy(msgres.buff, "query restaurant failed!");

					msgres.msglen = htonl(msgres.msglen);
					msgres.msgtype = htonl(msgres.msgtype);
					ret = write(pnode->fd, &msgres, sizeof(msgres));
					if(ret < 0)
					{
						printf("write error\n");
						return NULL;
					}
				}

			}
			else if(pnode->msghead.msgtype == MSG_ORDER_MENU)
			{

				memset(&msgres, 0, sizeof(msgres));
				ret = orderMenu(pnode, msgres.buff);
				if(ret == 0)
				{
					msgres.msglen = sizeof(msgres);
					msgres.msgtype = MSG_ORDER_MENU_RES;
					msgres.result = 0;
		//			strcpy(msgres.buff, "Login ok!");

					msgres.msglen = htonl(msgres.msglen);
					msgres.msgtype = htonl(msgres.msgtype);

					ret = write(pnode->fd, &msgres, sizeof(msgres));
					if(ret < 0)
					{
						printf("write error\n");
						return NULL;
					}
				}
				else
				{

					msgres.msglen = sizeof(msgres);
					msgres.msgtype = MSG_ORDER_MENU_RES;
					msgres.result = 1;
					strcpy(msgres.buff, "query menu failed!");

					msgres.msglen = htonl(msgres.msglen);
					msgres.msgtype = htonl(msgres.msgtype);
					ret = write(pnode->fd, &msgres, sizeof(msgres));
					if(ret < 0)
					{
						printf("write error\n");
						return NULL;
					}
				}

			}
			else if(pnode->msghead.msgtype == MSG_ORDER_ORDER)
			{

				memset(&msgres, 0, sizeof(msgres));
				ret = orderOrder(pnode);
				if(ret == 0)
				{
					msgres.msglen = sizeof(msgres);
					msgres.msgtype = MSG_ORDER_ORDER_RES;
					msgres.result = 0;
					strcpy(msgres.buff, "order ok!");

					msgres.msglen = htonl(msgres.msglen);
					msgres.msgtype = htonl(msgres.msgtype);

					ret = write(pnode->fd, &msgres, sizeof(msgres));
					if(ret < 0)
					{
						printf("write error\n");
						return NULL;
					}
				}
				else
				{

					msgres.msglen = sizeof(msgres);
					msgres.msgtype = MSG_ORDER_ORDER_RES;
					msgres.result = 1;
					strcpy(msgres.buff, "order failed!");

					msgres.msglen = htonl(msgres.msglen);
					msgres.msgtype = htonl(msgres.msgtype);
					ret = write(pnode->fd, &msgres, sizeof(msgres));
					if(ret < 0)
					{
						printf("write error\n");
						return NULL;
					}
				}

			}
			else if(pnode->msghead.msgtype == MSG_ORDER_QUERY)
			{

				memset(&msgres, 0, sizeof(msgres));
				ret = orderQuery(pnode, msgres.buff);
				if(ret == 0)
				{
					msgres.msglen = sizeof(msgres);
					msgres.msgtype = MSG_ORDER_ORDER_RES;
					msgres.result = 0;
					//strcpy(msgres.buff, "order ok!");

					msgres.msglen = htonl(msgres.msglen);
					msgres.msgtype = htonl(msgres.msgtype);

					ret = write(pnode->fd, &msgres, sizeof(msgres));
					if(ret < 0)
					{
						printf("write error\n");
						return NULL;
					}
				}
				else
				{

					msgres.msglen = sizeof(msgres);
					msgres.msgtype = MSG_ORDER_ORDER_RES;
					msgres.result = 1;
					strcpy(msgres.buff, "query order failed!");

					msgres.msglen = htonl(msgres.msglen);
					msgres.msgtype = htonl(msgres.msgtype);
					ret = write(pnode->fd, &msgres, sizeof(msgres));
					if(ret < 0)
					{
						printf("write error\n");
						return NULL;
					}
				}

			}
			else if(pnode->msghead.msgtype == MSG_ORDER_DELETE)
			{

				memset(&msgres, 0, sizeof(msgres));
				ret = orderDelete(pnode);
				if(ret == 0)
				{
					msgres.msglen = sizeof(msgres);
					msgres.msgtype = MSG_ORDER_DELETE_RES;
					msgres.result = 0;
					strcpy(msgres.buff, "delete order ok!");

					msgres.msglen = htonl(msgres.msglen);
					msgres.msgtype = htonl(msgres.msgtype);

					ret = write(pnode->fd, &msgres, sizeof(msgres));
					if(ret < 0)
					{
						printf("write error\n");
						return NULL;
					}
				}
				else
				{

					msgres.msglen = sizeof(msgres);
					msgres.msgtype = MSG_ORDER_DELETE_RES;
					msgres.result = 1;
					strcpy(msgres.buff, "delete order failed!");

					msgres.msglen = htonl(msgres.msglen);
					msgres.msgtype = htonl(msgres.msgtype);
					ret = write(pnode->fd, &msgres, sizeof(msgres));
					if(ret < 0)
					{
						printf("write error\n");
						return NULL;
					}
				}

			}
		}
	}
	return NULL;
}
void *createServerHandler(void *arg)
{
	int ret;

	printf("arg = %s\n",(char *)arg);
	ret = creatServer(atoi((char *)arg));

	return NULL;
}


int main(int argc, char ** argv)
{
	int ret;
	pthread_t pth_id;
	pthread_t pth_id1;
    
	if(argc != 2)
	{
		printf("para error\n");
		return -1;
	}
	memset(&head, 0, sizeof(head));

	ret = pthread_create(&pth_id, NULL, handler, NULL);
	if(ret < 0)
	{
		printf("create error\n");
		return -1;
	}

	printf("*********\n");
	ret = pthread_create(&pth_id1, NULL,createServerHandler,argv[1]);
	if (ret <0)
	{
		printf("create error\n");
		return -1;
	}

	pthread_join(pth_id,NULL);
	pthread_join(pth_id1,NULL);
	return 0;
}
