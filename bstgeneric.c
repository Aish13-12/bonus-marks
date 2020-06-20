#include <stdio.h>
#include <stdlib.h>
#include "bstgeneric.h"



struct node
{
	void *key;
	int size;
	struct node*left;
	struct node*right;
	struct node*parent;
};

	
static int ri2=0;
static int le2=0;	
static void numnodes(struct node*currentno)//Returns the number of nodes attached to a particular node
{
	
    if(currentno==NULL)
	return;
	else
	{
		struct node*temp1=currentno->left;
		struct node*temp2=currentno->right;
		numnodes(temp1);
		if(temp1!=NULL)
		ri2++;
		numnodes(temp2);
		if(temp2!=NULL)
		le2++;
	}
	
}
	
static void sizeassign(struct node*head)//Assigning size of the trees attached to each node
{
	if(head==NULL)
	return;
	else
	{
		numnodes(head);
		head->size=ri2+le2;
		ri2=0;
		le2=0;
		struct node*templ=head->left;//Traversing the tree in  preorder
		struct node*tempr=head->right;
		if(tempr!=NULL&&templ!=NULL)
		{
			sizeassign(templ);
			sizeassign(tempr);
		}
		else if(tempr!=NULL&&templ==NULL)
		sizeassign(tempr);
		else if(tempr==NULL&&templ!=NULL)
		sizeassign(templ);	
		else
		return;
	}
}		

static struct node *rightRotate(struct node *y,double(*compar)(const void*,const void*))//To right rotate about a given node
{ 
    struct node *x = y->left; 
    struct node *T2 = x->right; 
  
    x->parent=y->parent;
    if(x->parent!=NULL)
    {
	   double z=(*compar)(x->key,x->parent->key);
       if(z>0)
       x->parent->right=x;
       else
       x->parent->left=x;
     }  
    x->right = y; 
    y->left = T2; 
    if(T2!=NULL)
    T2->parent=y;
    if(y!=NULL)
    y->parent=x;
    return x; 
} 

static struct node *leftRotate(struct node *x,double(*compar)(const void*,const void*)) //To left rotate about a given node
{ 
    struct node *y = x->right; 
    struct node *T2 = y->left; 
  
    y->parent=x->parent;
    
    if(y->parent!=NULL)
    {
	   double z=(*compar)(y->key,y->parent->key);   
       if(z>0)
       y->parent->right=y;
       else
       y->parent->left=y;
     }  
    y->left = x; 
    x->right = T2; 
    if(T2!=NULL)
    T2->parent=x;
    if(x!=NULL)
    x->parent=y;
    
    return y; 
} 

static struct node *insertpart(struct node*fipa,void *key1,double(*compar)(const void*,const void*))//To find the node to which the new node would be attached
{
	struct node*prev=fipa;
	double z=(*compar)(key1,fipa->key);
	if(z>0)
	fipa=fipa->right;
	else
	fipa=fipa->left;
	if(fipa==NULL)
	return prev;
	else
	insertpart(fipa,key1,*compar);
} 

/*To insert a given node in the tree and make it as the root of the tree by rotaions*/
static void insertatroot(void *data,struct node**head,struct node*temp,double(*compar)(const void*,const void*))
{
	struct node*attach=insertpart(*head,data,*compar);
	
	/*Attach the new node to the node it needs to be attached*/
	double z=(*compar)(temp->key,attach->key);
	if(z>0)
	{
		attach->right=temp;
		temp->parent=attach;
	}
	else
	{
		attach->left=temp;
		temp->parent=attach;
	}
	/*Retracing the path went through to insert the new node*/
	while(attach!=NULL)
	{
		/* If the path takes a right turn at node x,then rotate right at x,else rotate left at x*/
		double z1=(*compar)(temp->key,attach->key);
		if(z1>0)
	    attach=leftRotate(attach,*compar);
	    else
	    attach=rightRotate(attach,*compar);
	    temp=attach;
	    attach=attach->parent;
	 }
	 *head=temp;//Assigning the new head
	 
}	    

//To insert a given node
static struct node* inserttree(void *data,struct node*presentnode,struct node**head,struct node*temp,double(*compar)(const void*,const void*))
{
	struct node*prev=presentnode;
	/* temp stores the address of the new node*/
	numnodes(presentnode);
	int size=ri2+le2;
	ri2=0;
	le2=0;
	if(size!=0)/*Size of the tree attached to the node*/
	{
		
       if((double)rand() /RAND_MAX>(1/size))/*Probability of size-1/size*/
       
       {
		  double z=(*compar)(data,presentnode->key);
		  if(z>0)
	      presentnode=presentnode->right;
	      else
	      presentnode=presentnode->left;
	      if(presentnode==NULL)
	      return prev;//The node at which the new node will be attached
	      else
	      inserttree(data,presentnode,head,temp,*compar);/*Insertion through recursion*/
	    }   
	   else
	   {
		  insertatroot(data,head,temp,*compar);/*With probability of 1/size,insertion at root */
		  return NULL;//the new node has been made the root
	    }	 
	  }
	  else
	  return presentnode;  //Returning the node if size is zero
}	

/*Final function for insertion*/	 
void insertfinal(void *data,struct node**head,double(*compar)(const void*,const void*))
{
	 
	struct node*temp;
	struct node*temp2;
	temp=(struct node*)malloc(sizeof(struct node));
	temp->key=data;
	temp->size=0;
	temp->left=NULL;
	temp->right=NULL;
	temp->parent=NULL;
	if(*head==NULL)
	{
		*head=temp;
		sizeassign(*head);
		printf("True");
	}	
	
	else
	{
		temp2=inserttree(data,*head,head,temp,*compar);
		if(temp2!=NULL)//If  the node is inserted recursively
		{
			double z=(*compar)(temp->key,temp2->key);
			if(z>0)
			{
				temp2->right=temp;
				temp->parent=temp2;
			}
			else
			{
				temp2->left=temp;
				temp->parent=temp2;
			}
			sizeassign(*head);
			printf("True");
		}
		else
		{
			sizeassign(*head);//If it is inserted as the root
			printf("True");
		    return;
		   } 
	}
}				  

void printtreepre(struct node*currentno)//To print the sub-tree with a given node in preorder
{
	if(currentno==NULL)
	return;
	else
	{
		printf("%p",currentno->key);
		struct node*templ=currentno;
		struct node*tempr=currentno;
		templ=templ->left;
		tempr=tempr->right;
		if(tempr!=NULL&&templ!=NULL)
		{
			printtreepre(templ);
			printtreepre(tempr);
		}
		else if(tempr!=NULL&&templ==NULL)
		printtreepre(tempr);
		else if(tempr==NULL&&templ!=NULL)
		printtreepre(templ);	
		else
		return;
		
	}
}

//Inorder-first printing the left child,then the parent and then the right child
void printtreein(struct node*currentno)//To print the sub-tree with a given node in inorder
{
	if(currentno==NULL)
	return;
	else 
	{
		struct node*templ=currentno->left;
		struct node*tempr=currentno->right;
		if(tempr!=NULL&&templ!=NULL)
		{
			printtreein(templ);
			printf("%p ",currentno->key);
			printtreein(tempr);
		}
		else if(tempr!=NULL&&templ==NULL)
		{
			printf("%p ",currentno->key);
			printtreein(tempr);
		}	
		
		else if(tempr==NULL&&templ!=NULL)
		{
			printtreein(templ);
			printf("%p ",currentno->key);
		}
		else
		printf("%p ",currentno->key);
	}
}

//Postorder-first printing the childrena and then the parent
void printtreepost(struct node*currentno)//To print the sub-tree with a given node in postorder
{
	if(currentno==NULL)
	return;
	else
	{
		struct node*templ=currentno->left;
		struct node*tempr=currentno->right;
		if(tempr!=NULL&&templ!=NULL)
		{
			printtreepost(templ);
			printtreepost(tempr);
			printf("%p ",currentno->key);
		}
		else if(tempr!=NULL&&templ==NULL)
		{
			printtreepost(tempr);
			printf("%p ",currentno->key);
		}
		else if(tempr==NULL&&templ!=NULL)
		{
			printtreepost(templ);
			printf("%p ",currentno->key);
		}
		else
		printf("%p ",currentno->key);
	}
}	

//To print the whole tree in preorder,inorder,postorder
void printpre(struct node**head)
{
	printtreepre(*head);
}		

void printin(struct node**head)
{
	printtreein(*head);
}		

void printpost(struct node**head)
{
	printtreepost(*head);
}	

static int ri=0;
static int le=0;			
struct node *find(struct node*head,void *key1,double(*compar)(const void*,const void*))//To find the node with a given key and it depth
{
	
	
	if(head==NULL)
	{
		printf("False");
		return head;
	}
	double z=(*compar)(key1,head->key);
	if(z>0)
	{
		ri++;
		find(head->right,key1,*compar);
	}	
	
	else if(z<0)
	{
		le++;
	    find(head->left,key1,*compar);
	}	
	
	else
	{
		
		printf("True,");
		printf("depth = %d",ri+le);
		ri=0;
		le=0;
		return head;
	
	}
	
}			

static struct node *find1(struct node*head,void *key1,double(*compar)(const void*,const void*))//To find the node with a given key 
{
	
	if(head==NULL)
	return head;
	
	double z=(*compar)(key1,head->key);
	if(z>0)
	find1(head->right,key1,*compar);
		
	else if(z<0)
	find1(head->left,key1,*compar);

	else
	return head;
}	

static struct node*join(struct node*left1,struct node*right1)//returns the root of the sub-tree which replaces the deleted node
{
	
	int m,n,r,total;
    numnodes(left1);
    if(left1!=NULL)		
    m=ri2+le2+1;//size of the tree attached to the left node
    else
    m=0;
    ri2=0;
    le2=0;
    numnodes(right1);
    if(right1!=NULL)
    n=ri2+le2+1;//size of the tree attached to right node
    else
    n=0;
    ri2=0;
    le2=0;
    total=m+n;
    
    if(total!=0)
    r=rand()%total;
    else
    return NULL;
		
    if(r<m)/* probability of m/total*/
    {
		
       left1->right=join(left1->right,right1);
       if(left1->right!=NULL)
       left1->right->parent=left1;
       return left1;
       
     }
     else/*probability of n/total*/
     {
		 right1->left=join(left1,right1->left);
		 if(right1->left!=NULL)
		 right1->left->parent=right1;
		 return right1;
		 
	}
}

void deletetree(void *key1,struct node**head,double(*compar)(const void*,const void*))//To delete a node from the tree
{
	struct node*T=find1(*head,key1,*compar);	
	if(T!=NULL)//if the entered node exists
	{
		
	   struct node*y=T->parent;
	   struct node*temp=join(T->left,T->right);
	   void *z=T->key;
	
	   T->left=NULL;
	   T->right=NULL;
	   T->parent=NULL;
	   free(T);
	   T=temp;

	   if(y==NULL)
	   *head=T;
	   else
	   {
		 if(T!=NULL)
		 {
			double j=(*compar)(T->key,y->key);
		    if(j>0)
		    y->right=T;
		    else
		    y->left=T;
		    T->parent=y;
	     }
	     else
	     {
			 double j1=(*compar)(z,y->key);
			 if(j1>0)
			 y->right=NULL;
			 else
			 y->left=NULL;
		  }	
			
	   } 
	   sizeassign(*head); 
	   printf("True");
	 }
	 else//if the node doesn't exist
	 printf("False");  
	  
}		

void imbalance(struct node*head,void *key1,double(*compar)(const void*,const void*))
{
	
	struct node*currentno=find1(head,key1,*compar);//Finding the required node
	
	if(currentno==NULL)
	{
		printf("False");
		return;
	}
	else
	{
			
	   numnodes(currentno->left);//Number of nodes in its left sub-tree
	   int left=ri2+le2;
	   if(currentno->left!=NULL)
	   left=left+1;
	   ri2=0;
	   le2=0;
	   numnodes(currentno->right);//Number of nodes in its right subtree
	   int right=ri2+le2;
	   if(currentno->right!=NULL)
	   right=right+1;
	   if(left>right)
	   printf("Imbalance: %d",left-right);
	   else
	   printf("Imbalance: %d",right-left);
	  } 
	  ri2=0;
	  le2=0;
	  
}	
		
