#ifndef bst_generic__
#define bst_generic__

struct node ;
void insertfinal(void *data,struct node**head,double(*compar)(const void*,const void*));
void printtreepre(struct node*currentno);
void printtreein(struct node*currentno);
void printtreepost(struct node*currentno);
void printpre(struct node**head);
void printin(struct node**head);
void printpost(struct node**head);
struct node *find(struct node*head,void *key1,double(*compar)(const void*,const void*));
void deletetree(void *key1,struct node**head,double(*compar)(const void*,const void*));

#endif

