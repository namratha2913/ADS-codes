// C / C++ program for Prim's MST for adjacency list representation of graph

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>



struct node{
  int n,m;
  int degree;
  struct node* parent;
  struct node* child;
  struct node* sibling;
};
struct node* MAKE_BINOMIAL_HEAP();
int BINOMIAL_LINK(struct node*,struct node*);
struct node* CREATE_NODE(int,int);
struct node* BINOMIAL_HEAP_UNION(struct node*,struct node*);
struct node* BINOMIAL_HEAP_INSERT(struct node*,struct node*);
struct node* BINOMIAL_HEAP_MERGE(struct node*,struct node*);
struct node* BINOMIAL_HEAP_EXTRACT_MIN(struct node*);
int REVERT_LIST(struct node*);
int DISPLAY(struct node*);
struct node* H=NULL;
struct node* Hr=NULL;
struct node* FIND_NODE(struct node*,int);
int BINOMIAL_HEAP_DECREASE_KEY(struct node*,int,int);
int BINOMIAL_HEAP_DELETE(struct node*,int);

int size=0;
int count = 1;

struct node* MAKE_BINOMIAL_HEAP()
{
    struct node* np;
    np=NULL;
    return np;
}
  
int BINOMIAL_LINK(struct node* y,struct node* z)
{
    y->parent=z;
    y->sibling=z->child;
    z->child=y;
    z->degree=z->degree+1;
}
  
struct node* CREATE_NODE(int k,int l)
{
    struct node* p;//new node; 
    p=(struct node*)malloc(sizeof(struct node));  
    p->n=k;
    p->m=l;

    return p;
}
  
struct node* BINOMIAL_HEAP_INSERT(struct node* H,struct node* x)
{ 
    struct node* H1=MAKE_BINOMIAL_HEAP();
    x->parent=NULL;
    x->child=NULL;
    x->sibling=NULL;
    x->degree=0;
    H1=x;
    H=BINOMIAL_HEAP_UNION(H,H1);
    return H;
}
  
struct node* BINOMIAL_HEAP_UNION(struct node* H1,struct node* H2)
{
    struct node* H=MAKE_BINOMIAL_HEAP();
    H=BINOMIAL_HEAP_MERGE(H1,H2);//the objects H1 H2 have not been freed
    if(H==NULL)
      return H;
    struct node* prev_x;
    struct node* next_x;
    struct node* x;
    prev_x=NULL;
    x=H;
    next_x=x->sibling;
    while(next_x!=NULL)
      { 
	if((x->degree!=next_x->degree)||((next_x->sibling!=NULL)&&(next_x->sibling)->degree==x->degree))
	    {
	      prev_x=x;
	      x=next_x;
	    }
	else
	    {
	     if(x->n<=next_x->n)
	      {
		x->sibling=next_x->sibling;
		BINOMIAL_LINK(next_x,x);
	      }
	     else
	      { 
		if(prev_x==NULL)
		  H=next_x;
		else
		  prev_x->sibling=next_x;
		BINOMIAL_LINK(x,next_x);
		x=next_x;
	      }
	    }
	next_x=x->sibling;    
      }
   return H;   
}
  
struct node* BINOMIAL_HEAP_MERGE(struct node* H1,struct node* H2)
{
    struct node* H=MAKE_BINOMIAL_HEAP();
    struct node* y;
    struct node* z;
    struct node* a;
    struct node* b;
    y=H1;
    z=H2;
    if(y!=NULL)
      {
	if(z!=NULL&&y->degree<=z->degree)
	  H=y;
	else if(z!=NULL&&y->degree>z->degree)//need some modificationss here;the first and the else conditions can be merged together!!!!
	  H=z; 
	else
	  H=y;
      }
    else
      H=z;
    while(y!=NULL&&z!=NULL)
      {
	if(y->degree<z->degree)
	  {
	    y=y->sibling;
	  }
	else if(y->degree==z->degree)
	  {
	    a=y->sibling;
	    y->sibling=z;
	    y=a;
	  }
        else
	  {
	    b=z->sibling;
	    z->sibling=y;
	    z=b;
	  }	  
      }  
    return H;
    
}
  
int DISPLAY(struct node* H)
{ 
    if(H==NULL)
      {
      printf("\nHEAP EMPTY!!!\n");
      return 0;
      }
    printf("\nTHE ROOT NODES ARE:-\n");
    struct node* p;
    p=H;
    while(p!=NULL)
      {
	printf("%d",p->n);
	if(p->sibling!=NULL)
	  printf("-->");p=p->sibling;
      }
    printf("\n");  
}
  
struct node* BINOMIAL_HEAP_EXTRACT_MIN(struct node* H1)
{ 
    //printf("main node=%d\n",H1->n);
    Hr=NULL;
    struct node* t=NULL;
    struct node* x=H1;
    if(x==NULL)
      {
      //printf("\nNOTHING TO EXTRACT!!!!!!\n");
      return x; 
      }   
    int min=x->m;
    struct node* p=x;
    while(p->sibling!=NULL)
      {
	if((p->sibling)->m<min)
	  {
	    min=(p->sibling)->m;
	    t=p;
	    x=p->sibling;
	  }
	p=p->sibling;
          
      }
    if(t==NULL&&x->sibling==NULL)
      H1=NULL;
    else if(t==NULL)
      H1=x->sibling;
    else if(t->sibling==NULL)
      t=NULL;
    else
      t->sibling=x->sibling;
    if(x->child!=NULL)
      {
       // printf("Child:%d\n",x->child->n);
	REVERT_LIST(x->child);
	(x->child)->sibling=NULL;
      }
    H=BINOMIAL_HEAP_UNION(H1,Hr);
    //printf("After rem min:\n");
    //DISPLAY(H);
    return x;
}

int REVERT_LIST(struct node* y)
{

    y->parent=NULL;
    if(y->sibling!=NULL)
      {
	REVERT_LIST(y->sibling);
	(y->sibling)->sibling=y;
      }
    else
      {
	Hr=y;
      }
}
  
struct node* FIND_NODE(struct node* H,int k)
{ 

    //printf("**\n");
    struct node* x=H;
    struct node* p=NULL;
    if(x->n==k)
      {
	p=x;
	return p;
      }
    if(x->child!=NULL&&p==NULL)
      {
      p=FIND_NODE(x->child,k);
      }
    if(x->sibling!=NULL&&p==NULL)
      {
      p=FIND_NODE(x->sibling,k);
      }
    return p; 
}
 
int BINOMIAL_HEAP_DECREASE_KEY(struct node* H,int i,int k)
{
    int t;
    struct node* p;
    struct node* y;
    struct node* z;
    
    //printf("\n%d node before redn has %d\n",p->n,p->m);
    if(H==NULL)
      {
	printf("\nINVALID CHOICE OF KEY TO BE REDUCED!!!");
	return 0;
      }
     p=FIND_NODE(H,i);
     if(p==NULL)
       return 0;
     if(k>p->m)
      {
      printf("\nERROR!!!!!THE NEW KEY IS GREATER THAN CURRENT ONE!!!");
      //return 0;
      }
    p->m=k;
    //printf("\n%d node reduced to %d\n",p->n,p->m);  
    y=p;
    z=p->parent;
    //printf("p:%d--->c:%d\n",z->n,y->n);
    //DISPLAY(z);
    while(z!=NULL&&y->m<z->m)
      {
        //printf("p:%d--->c:%d\n",z->n,y->n);
        //DISPLAY(z);
	t=y->n;
	y->n=z->n;
	z->n=t;
        t=y->m;
	y->m=z->m;
	z->m=t;
        
	y=z;
	z=z->parent;
      }
    //printf("After reduc min:\n");
    //DISPLAY(H);
}
  
int BINOMIAL_HEAP_DELETE(struct node* H,int k)
{
    struct node* np;
    if(H==NULL)
      {
      printf("\nHEAP EMPTY!!!!!");
      return 0;
      }
    BINOMIAL_HEAP_DECREASE_KEY(H,k,-1000);
    np=BINOMIAL_HEAP_EXTRACT_MIN(H);
    if(np!=NULL)
    printf("\nNODE DELETED SUCCESSFULLY!!!");
}     



int isEmpty(struct node* H)
{
    //printf("*\n");
    if(H==NULL)
    return 1;

    else
    return 0;
}

int isInMinHeap( int v)
{

   //printf("***\n");
   struct node* p;
   if (H== NULL) {
         //printf("\nkey not found!!");
         return 0;
   }
   p = FIND_NODE(H, v);
   if (p== NULL) {
         //printf("\nkey not found!!");
         return 0;
   }
   if (p->n < size)
     return 1;
   return 0;
}

 
struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode* next;
};
 
struct AdjList
{
    struct AdjListNode *head;  
};
 
struct Graph
{
    int V;
    struct AdjList* array;
};
 
//create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest, int weight)
{
    struct AdjListNode* newNode =
            (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}
 
//creates a graph of V vertices
struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
 
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));
 
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;
 
    return graph;
}
 
// Adds an edge to graph
void addEdge(struct Graph* graph, int src, int dest, int weight)
{
    
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
 
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}



 

//print the constructed MST
void printArr(int arr[], int n)
{
    for (int i = 1; i < n; i++)
        printf("%d - %d\n", arr[i], i);
}
 

// Minimum Spanning Tree (MST) using Prim's algorithm
void PrimMST(struct Graph* graph)
{
    int V = graph->V;// Get the number of vertices in graph
    int parent[V];   // Array to store constructed MST
    int key[V];      // Key values used to pick minimum weight edge in cut
 
     struct node* np;
  
    size=V;
 
    key[0] = 0;
    np = CREATE_NODE(0,key[0]);
    H = BINOMIAL_HEAP_INSERT(H, np);
    // Initialize min heap with all vertices. Key value of all vertices (except 0th vertex) is initially infinite
    for (int v = 1; v < V; v++)
    {
        parent[v] = -1;
        key[v] = INT_MAX;
        np = CREATE_NODE(v,key[v]);
        H = BINOMIAL_HEAP_INSERT(H, np);
    }
 
    //DISPLAY(H);
 
     
    // In the followin loop, min heap contains all nodes not yet added to MST.
    while (!isEmpty(H))
    {
        // Extract the vertex with minimum key value
        struct node* p= BINOMIAL_HEAP_EXTRACT_MIN(H);
        int u = p->n; // Store the extracted vertex number
        printf("\nmin=%d  ---of----%d node\n",p->m,u);

        // Traverse through all adjacent vertices of u (the extracted vertex) and update their key values
        struct AdjListNode* move = graph->array[u].head;
        while (move != NULL)
        {
            int v = move->dest;
            printf("node no:%d of wt=%d and key=%d \n",v,move->weight,key[v]);
           
            // If v is not yet included in MST and weight of u-v is less than key value of v, then update key value and parent of v
            if (isInMinHeap(v) && move->weight < key[v])
            {
                printf("\nDoing!!!\n");
                key[v] = move->weight;
                parent[v] = u;
                BINOMIAL_HEAP_DECREASE_KEY(H, v, key[v]);
            }
            move = move->next;
        }
    }
 
    // print edges of MST
    printArr(parent, V);
}
 

int main()
{
    //reading graph from file

    int n,a,b,c,i;
    FILE *fp;
    fp=fopen("ip_graph2.txt","r");
    fscanf (fp, "%d", &n); 
    int V = n;
    struct Graph* graph = createGraph(V);   
    while (!feof (fp))
    {
	fscanf (fp, "%d", &a); 
        fscanf (fp, "%d", &b);
        fscanf (fp, "%d", &c);
	addEdge(graph, a, b, c);     
    }
    fclose(fp);
 
    PrimMST(graph);
 
    return 0;
}
