#include<stdio.h>
#include<stdlib.h>
#include <time.h>
     
clock_t start, end;
double cpu_time_used;

struct avl_node{
	int data;
	struct avl_node* left;
	struct avl_node* right;
};


struct trees{
	struct avl_node *t1,*t2;
	int d;
};

struct avl_node* insert(struct avl_node*root,int data);
struct avl_node* delnode(struct avl_node*root,int data);
struct avl_node* rotleft(struct avl_node* node);
struct avl_node* rotright(struct avl_node* node);
struct avl_node* balance_tree(struct avl_node* node);
int height(struct avl_node* node);
int balance( struct avl_node* node);
struct trees split(struct avl_node* node,int k);
struct avl_node* join(struct avl_node* l,struct avl_node* r,int d);

int height(struct avl_node* node)
{
	if (node == NULL) 
	return 0;
	int htl = height(node->left);
	int htr = height(node->right);

	if (htl > htr) 
           return(htl + 1);
	else 
           return(htr + 1);	
}

int balance( struct avl_node* node)
{

	int bal=height( node->left )-height( node->right );
	return bal;
}

struct avl_node* rotright(struct avl_node* node)
{
	//printf("rotating left\n");
	struct avl_node* x=node->left;
	struct avl_node* y=x->right;
	
	x->right=node;
	node->left=y;

	return x;
}


struct avl_node* rotleft(struct avl_node* node)
{
	//printf("rotating right\n");
	struct avl_node* x=node->right;
	struct avl_node* y=x->left;
	
	x->left=node;
	node->right=y;

	return x;
}


struct avl_node* insert(struct avl_node* node,int data)
{

	
	struct avl_node *newnode,*temp;
	newnode=(struct avl_node*)malloc(sizeof(struct avl_node*));
	newnode->data=data;
	newnode->left=NULL;
	newnode->right=NULL;

	if (node == NULL)
	{
		node=newnode;
        	return node;
	}
 
	if (data < node->data)
	{	
		//printf("L%d\n\n",node->data);
		node->left  = insert(node->left, data);
	}
	else if (data > node->data)
	{	
		//printf("%dR\n\n",node->data);
		node->right = insert(node->right, data);
	}
	
  
	int bal = balance(node);
	//printf("%d--bal=%d\n\n",node->data,bal);
    
	if (bal > 1 && data < node->left->data)
	return rotright(node);
 
    
	if (bal < -1 && data > node->right->data)
        return rotleft(node);
 
    
	if (bal > 1 && data > node->left->data)
	{	
        	node->left =  rotleft(node->left);
        	return rotright(node);
    	}
 
	if (bal < -1 && data < node->right->data)
	{
	        node->right = rotright(node->right);
	        return rotleft(node);
	}
	return node;
}

void print(struct avl_node* node)
{


	//printf("printing\n");
	if (node == NULL)
        return ;
	static int i=0;
	printf("NODE: \n");
	printf("Data: %d \n", node->data);
	if(node->left)
     	printf("Left Child: %d ",node->left->data);
	if(node->right)
     	printf("Right Child: %d\n",node->right->data);
	printf("\n");
	print(node->left); 
	print(node->right);
}

struct avl_node* balance_tree(struct avl_node* root)
{	
	if(root)
	{int bal = balance(root);
		//printf("%d--bal=%d\n\n",root->data,bal);
	    
		if (bal > 1 && balance(root->left) > 0)
		return rotright(root);
	 
	    
		if (bal < -1 &&  balance(root->right) < 0)
		return rotleft(root);
	 
	    
		if (bal > 1 && balance(root->left) < 0)
		{	
			root->left =  rotleft(root->left);
			return rotright(root);
	    	}
	 
		if (bal < -1 &&  balance(root->right) > 0)
		{
			root->right = rotright(root->right);
			return rotleft(root);
		}
	}
	return root;
}


struct avl_node* join(struct avl_node* l,struct avl_node* n,int dir)
{
	if(dir==0)
	n->left=l;

	else
	n->right=l;
	
	return n; 
}

struct trees split(struct avl_node* node ,int key)
{
	struct trees res;

	if(!node)
	{	res.t1=res.t2=NULL;
		res.d=0;
		return res;	
	}

	
		else
	{
		
		if(key<node->data)
		{
			res=split(node->left,key);
			res.t2=join(res.t2,node,0);
			res.d=0;
			res.t2=balance_tree(res.t2);
			res.t1=balance_tree(res.t1);
			return res;
						
		}
		else if(key==node->data)
		{	
			res.t1=node->left;
			res.t2=node->right;
			res.d=node->data;
			res.t2=balance_tree(res.t2);
			res.t1=balance_tree(res.t1);
			//printf("%d!!!\n",res.d);
			return res;
		}
		else
		{
			res=split(node->right,key);
			res.t1=join(res.t1,node,1);
			res.d=0;
			res.t2=balance_tree(res.t2);
			res.t1=balance_tree(res.t1);
			return res;
		}	
		
	}

}






int main()
{
	int n;
	FILE *fp;
	struct avl_node* root=NULL;
	struct trees T;
	fp=fopen("addnode.txt","r");
	
	fscanf (fp, "%d", &n);    
	while (!feof (fp))
	{
		//printf ("%d inserting\n", n);  
		root=insert(root,n);
		fscanf (fp, "%d", &n);      
	}
	fclose(fp);
	//print(root);
	
	printf("\n Enter Key to split tree:");
	scanf("%d",&n);
	start = clock();
	T=split(root,n);
	end = clock();
	cpu_time_used = ((double) (end - start));
	//printf("\n----T1----\n");
	//print(T.t1);
	//printf("\n key found? :%d \n",T.d);
	//printf("\n----T2----\n");
	//print(T.t2);

	printf("time of execution:%f\n",cpu_time_used/CLOCKS_PER_SEC);

	free(root);

}
	
	
	
