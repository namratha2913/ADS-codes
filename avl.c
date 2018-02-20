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


struct avl_node* insert(struct avl_node*root,int data);
struct avl_node* delnode(struct avl_node*root,int data);
struct avl_node* rotleft(struct avl_node* node);
struct avl_node* rotright(struct avl_node* node);
int height(struct avl_node* node);
int balance( struct avl_node* node);

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

struct avl_node* delnode(struct avl_node* root,int data)
{
	struct avl_node *newnode,*temp;
	int t;
	temp=root;
	
	if(root==NULL)
	return root;

	if(data<root->data)
	root->left=delnode(root->left,data);
	else if(data>root->data)
	root->right=delnode(root->right,data);

	else
	{
		if(!root->left&&!root->right)
		{
			root=NULL;
		}
		else if(temp->left)
		{
			temp=temp->left;
			while(temp->right!=NULL)
			{temp=temp->right;}
			t=temp->data;
			temp->data=root->data;
			root->data=t;
			root->left=delnode(root->left,data);
		}
		else if(temp->right)
		{
			temp=temp->right;
			while(temp->left!=NULL)
			{temp=temp->left;}
			t=temp->data;
			temp->data=root->data;
			root->data=t;
			root->right=delnode(root->right,data);
		}			
	}

	if(root){
		int bal = balance(root);
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

void print(struct avl_node* node)
{
	if (node == NULL)
        return ;
	static int i=0;
	//printf("NODE: \n");
	printf(" %d  ", node->data);
	if(node->left)
     	//printf("Left Child: %d ",node->left->data);
	if(node->right)
     	//printf("Right Child: %d\n",node->right->data);
	printf("\n");
	print(node->left); 
	print(node->right);
}


int main()
{
	int n;
	FILE *fp;
	struct avl_node* root=NULL;
	fp=fopen("addnode.txt","r");
	start = clock();
	fscanf (fp, "%d", &n);    
	while (!feof (fp))
	{
		//printf ("%d inserting\n", n);  
		root=insert(root,n);
		fscanf (fp, "%d", &n);      
	}
	fclose(fp);
	end = clock();
	cpu_time_used = ((double) (end - start));
	printf("time of execution of INSERTION:%f\n",cpu_time_used/CLOCKS_PER_SEC);
	//print(root);
	printf("\n----Deletion----\n");
	fp=fopen("delnode.txt","r");
	start = clock();
	fscanf (fp, "%d", &n); 
	while (!feof (fp))
	{  
		//printf ("deleting ---%d\n ", n);
		root=delnode(root,n);
		fscanf (fp, "%d", &n);
	      
	}
	fclose(fp);
	end = clock();
	cpu_time_used = ((double) (end - start));
	printf("time of execution of DELETION:%f\n",cpu_time_used/CLOCKS_PER_SEC);
	//print(root);
	printf("\n");
}
	
	
	
