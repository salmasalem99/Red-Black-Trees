#include <stdio.h>
#include<string.h>
#include <stdlib.h>
typedef struct Node{
    char val[40];
    struct Node* left;
    struct Node* right;
    struct Node* parent;
    int b;
}Node;


void inorder(Node* node) {
    if(!node) return;

    inorder(node->left);
    printf("[%s %d] ",node->val, node->b);
    inorder(node->right);
}
Node *newRBTNode(char string[])
{
    Node *node =(Node*)malloc(sizeof(Node));
    strcpy(node->val,string);
    node->parent = NULL;
    node->left = node->right = NULL;
    node->b=0;
    return node;
}
Node* traverse(Node* root, Node* p)
{
    if(root==NULL)
    {
        root=p;
        p->parent=NULL;
        return p;
    }
    else if(strcasecmp(p->val,root->val)<0)
    {
        if(root->left == NULL)
        {
            root->left=p;
            p->parent=root;
            return p;
        }
       traverse(root->left,p);
    }
    else if(strcasecmp(p->val,root->val)>0)
    {
        if(root->right == NULL)
        {
            root->right=p;
            p->parent=root;
            return p;
        }
        traverse(root->right,p);
    }
    return p;
}
void Rrotate(Node* x, Node** root)
{
    Node* y=x->left;
    x->left=y->right;
    if(y->right!=NULL)
        x->left->parent=x;
    y->parent=x->parent;
    if(x->parent==NULL)
    {
        *root=y;
        (*root)->parent=NULL;
    }
    else if(x==x->parent->left)
        x->parent->left=y;
    else
        x->parent->right=y;
    y->right=x;
    x->parent=y;
}
void Lrotate(Node* x, Node** root)
{
    Node* y= x->right;
    x->right=y->left;
    if(y->left!=NULL)
        x->right->parent=x;
    y->parent=x->parent;
    if(x->parent==NULL)
    {
        *root=y;
        (*root)->parent=NULL;
    }
    else if(x==x->parent->left)
        x->parent->left=y;
    else
        x->parent->right=y;
    y->left=x;
    x->parent=y;
}

void validate(Node* node, Node** root)
{
    if(node==*root )
    {
           node->parent = NULL;
           node->b=1;
           return;
    }
    if(node->parent==*root)
        return;
    while(node!=*root && node->parent->b!=1 )
    {
         if(node->parent->b==0)
        {
            int black=1;
            Node* uncle=NULL;
            if(node->parent==node->parent->parent->right)
            {
                uncle=node->parent->parent->left;
            }
            else
            {
                uncle=node->parent->parent->right;
            }
            if(uncle!=NULL)
            {
                black=uncle->b;
            }
            if(black==0)
            {
                node->parent->b=1;
                uncle->b=1;
                node->parent->parent->b=0;
                node=node->parent->parent;
                validate(node,root);
                return;
            }
            else if(black==1)
            {
                Node*p=node->parent;
                Node*gp=node->parent->parent;

                //LEFT LEFT CASE
                if(node->parent->left==node&&node->parent->parent->left==node->parent)
                {
                    p->b=1;
                    gp->b=0;
                    Rrotate(gp,root);
                    return;
                }
                // LEFT RIGHT
                else if(node->parent->right==node&&node->parent->parent->left==node->parent)
                {
                    node->b=1;
                    gp->b=0;
                    Lrotate(p,root);
                    Rrotate(gp,root);
                    return;
                }
                // RIGHT RIGHT
                else if(node->parent->right==node&&node->parent->parent->right==node->parent)
                {
                    p->b=1;
                    gp->b=0;
                    Lrotate(gp,root);
                    return;
                }
                //RIGHT LEFT
                else if(node->parent->left==node&&node->parent->parent->right==node->parent)
                {
                    node->b=1;
                    gp->b=0;
                    Rrotate(p,root);
                    Lrotate(gp,root);
                    return;
                }
            }
        }
    }
    (*root)->b=1;
    return;

}

void insert(char word[],Node** root)
{
    Node* p= newRBTNode(word);
    if(*root==NULL)
    {
        *root=p;
        (*root)->b=1;
        (*root)->parent=NULL;
        return ;
    }
    else
    {
        p=traverse(*root,p);
        //printf("parent:%s\n",p->parent->val);
        validate(p,root);
        (*root)->parent=NULL;
        (*root)->b=1;
    }
   return ;
}

Node* openfile(char filename[40], Node* root)
{
    FILE* f = fopen(filename,"r");
    char word[40];
    while (!feof(f))
    {
        fgets(word,40,f);
        insert(word,&root);
        root->b=1;
        root->parent=NULL;
    }
    fclose(f);
    return root;
}
int max(int a, int b)
{
    return a>b? a:b;
}
int PrintHeight(Node* root)
{
    if(root==NULL)
        return -1;
    else
        return 1+max(PrintHeight(root->left),PrintHeight(root->right));
}
int search(Node*root, char word[])
{
    if(root==NULL)
        return 0;
    else
    {
        if(strcasecmp(root->val,word)==0)
            return 1;
        else if(strcasecmp(root->val,word)>0)
            return search(root->left,word);
        else if(strcasecmp(root->val,word)<0)
            return search(root->right,word);
    }

}
int main(int argc, char**argv)
{

    if(argc!=2) {
		printf("Usage: %s filename\n", argv[0]);
		exit(1);
	}
    Node* root=NULL;
    root=openfile(argv[1], root);
    int x=PrintHeight(root);
    printf("Height:%d\n",x);
    char key[40];
    while(1)
    {
        printf("Enter a word to insert, press E to exit:");
        gets(key);
        if(strcasecmp(key,"e")==0)
            break;
        if(search(root,key)==1)
            printf("ERROR: Word already in dictionary!\n");
        else insert(key,&root);
        printf("Enter a word to look up, press E to exit:");
        gets(key);
        if(strcasecmp(key,"e")==0)
            break;
        if(search(root,key)==1)
            printf("YES\n");
        else printf("NO\n");
    }

    return 0;
}
