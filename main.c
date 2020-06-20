#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bstgeneric.h"

void insertfinal(void *data,struct node**head,double(*compar)(const void*,const void*));
double comparId(const void*x,const void*y)
{
	int key1,key2;
	key1=*((int*)x);
	key2=*((int*)y);
	return (key1-key2);
}	

int main()
{
	struct node*head=NULL;
	srand(time(NULL));
	insertfinal((int*)34,&head,comparId);
	printin(&head);
	return 0;
}	

