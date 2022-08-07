#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <stdint.h>
#include <limits.h>

// 這裡用到treap線段樹 跟 吉如一線段樹
typedef struct node{ // a treap node
    int t,pri,size;
    uint64_t sum;
    int max,sec,maxn,mtag;
    bool tag;
    struct node *left,*right;
}Node;

Node *genNode(int t,int size,Node *left,Node *right)
{
    Node *n=malloc(sizeof(Node));
    n->t=t;
    n->pri=rand();
    n->size=size;
    n->sum=(uint64_t)t;
    n->max=t;
    n->sec=0;
    n->maxn=1;
    n->mtag=0;
    n->tag=0;
    n->left=left;
    n->right=right;
    return n;
}

void preorder_trav(Node *root)
{
    if(root==NULL) return;
    printf("%d",root->t);
    printf("(");
    preorder_trav(root->left);
    printf(" , ");
    preorder_trav(root->right);
    printf(")");
    return;
}

void inorder_trav(Node *root)
{
    if(root==NULL) return;
    inorder_trav(root->left);
    printf("%d %lu %d %d %d|",root->t,root->sum,root->max,root->sec,root->mtag);
    inorder_trav(root->right);
    return;
}

void inorder(Node *root)
{
    preorder_trav(root);
    printf("\n");
    inorder_trav(root);
    printf("\n\n");
}

void swap(Node *root)
{
    if(root==NULL) return;
    if(!root->tag) return;   
    else{
        Node *tmp=root->right;
        root->right=root->left;
        root->left=tmp;
        root->tag=0;
        if(root->left) root->left->tag=(!root->left->tag);
        if(root->right) root->right->tag=(!root->right->tag);
    }
    return;
}

Node *recalc_node(Node *root)
{
    int size=1;
    uint64_t sum=(uint64_t) root->t;
    if(root->left){
        size+=root->left->size;
        sum+=root->left->sum;
    }
    if(root->right){
        size+=root->right->size;
        sum+=root->right->sum;
    }
    root->size=size;
    root->sum=sum;
    int cmp[5]={-1};
    if(root->left){
        cmp[0]=root->left->max;
        cmp[1]=root->left->sec;
    }
    if(root->right){
        cmp[2]=root->right->max;
        cmp[3]=root->right->sec;
    }
    cmp[4]=root->t;
    int max=-1,sec=-1,maxn=0;
    for(int i=0;i<5;i++){
        if(cmp[i]>max){
            sec=max;
            max=cmp[i];
        }else if(cmp[i]<max && cmp[i]>sec){
            sec=cmp[i];
        }
    }
    if(cmp[0]==max) maxn+=root->left->maxn;
    if(cmp[2]==max) maxn+=root->right->maxn;
    if(cmp[4]==max) maxn+=1;
    root->max=max;
    root->maxn=maxn;
    root->sec=sec;
    return root;
}

void upgrade(Node *root)
{
    if(root==NULL) return;
    if(root->mtag==0) return;
    else{
        if(root->mtag>=root->max){
            root->mtag=0;
            return;
        }else if(root->mtag>root->sec && root->mtag<root->max){
            if(root->t>root->mtag) root->t=root->mtag;
            uint64_t diff=(uint64_t)(root->max-root->mtag);
            root->max=root->mtag;
            root->sum-=(uint64_t)((uint64_t)(root->maxn)*diff);
            if(root->left) root->left->mtag=root->mtag;
            if(root->right) root->right->mtag=root->mtag;
            root->mtag=0;
        }else{
            if(root->left){
                root->left->mtag=root->mtag;
                swap(root->left);
                upgrade(root->left);
            }
            if(root->right){
                root->right->mtag=root->mtag;
                swap(root->right);
                upgrade(root->right);
            }
            if(root->t>root->mtag) root->t=root->mtag;
            root->mtag=0;
            root=recalc_node(root);
        }
    }
    return;
}



Node *merge(Node *a,Node *b)
{
    if(!a && !b) return NULL;
    if(!a){
        swap(b);
        upgrade(b);
        return b;
    }
    if(!b){
        swap(a);
        upgrade(a);
        return a;
    }
    if(a->pri>b->pri){
        swap(a);
        upgrade(a);
        a->right=merge(a->right,b);
        upgrade(a->left);
        upgrade(a->right);
        a=recalc_node(a);
        return a;
    }else{
        swap(b);
        upgrade(b);
        b->left=merge(a,b->left);
        upgrade(b->left);
        upgrade(b->right);
        b=recalc_node(b);
        return b;
    }
}

void split(Node *root,Node **a,Node **b,int index,int cur_index)
{
    if(root==NULL){
        (*a)=(*b)=NULL;
        return;
    }
    swap(root);
    upgrade(root);
    if(cur_index<=index){
        (*a)=root;
        cur_index++;
        if(root){swap(root->right);upgrade(root->right);}
        if(root && root->right){swap(root->right->left);upgrade(root->right->left);}
        if(root->right && root->right->left) cur_index+=root->right->left->size;
        split(root->right,&((*a)->right),b,index,cur_index);
        upgrade((*a)->left);
        upgrade((*a)->right);
        (*a)=recalc_node((*a));
    }else{
        (*b)=root;
        cur_index--;
        if(root){swap(root->left);upgrade(root->left);}
        if(root && root->left){swap(root->left->right);upgrade(root->left->right);}
        if(root->left && root->left->right) cur_index-=root->left->right->size;
        split(root->left,a,&((*b)->left),index,cur_index);
        upgrade((*b)->left);
        upgrade((*b)->right);
        (*b)=recalc_node((*b));
    }
}

Node *swap_marker(Node *root,int l,int r)
{   
    Node *a,*b,*c,*tmp;
    split(root,&tmp,&c,r,(root && root->left)? root->left->size:0);
    split(tmp,&a,&b,l-1,(tmp && tmp->left)? tmp->left->size:0);
    b->tag=(!b->tag);
    return merge(merge(a,b),c);  
}

Node *insert(Node *root,int time,int index,int cur_index)
{
    Node *a,*b;
    split(root,&a,&b,index-1,cur_index);
    Node *new=genNode(time,1,NULL,NULL);
    return merge(merge(a,new),b);
}

Node *reboot_Time(Node *root,int l,int r,uint64_t *sum)
{
    Node *a,*b,*c,*tmp;
    split(root,&tmp,&c,r,(root && root->left)? root->left->size:0);
    split(tmp,&a,&b,l-1,(tmp && tmp->left)? tmp->left->size:0);
    (*sum)=b->sum;
    return merge(merge(a,b),c);
}

Node *delete(Node *root,int index,int cur_index)
{
    Node *a,*b,*c,*tmp;
    split(root,&tmp,&c,index,cur_index);
    split(tmp,&a,&b,index-1,(tmp && tmp->left)? tmp->left->size:0);
    free(b);
    return merge(a,c);
}

Node *swap_blocks(Node *root,int l1,int r1,int l2,int r2)
{
    Node *a,*b,*c,*d,*e,*tmp;
    split(root,&tmp,&e,r2,(root && root->left)? root->left->size:0);
    Node *tmp2;
    split(tmp,&tmp2,&d,l2-1,(tmp && tmp->left)? tmp->left->size:0);
    Node *tmp3;
    split(tmp2,&tmp3,&c,r1,(tmp2 && tmp2->left)? tmp2->left->size:0);
    split(tmp3,&a,&b,l1-1,(tmp3 && tmp3->left)? tmp3->left->size:0);
    return merge(merge(merge(merge(a,d),c),b),e);
}

Node *update(Node *root,int l,int r,int k)
{
    Node *a,*b,*c,*tmp;
    split(root,&tmp,&c,r,(root && root->left)? root->left->size:0);
    split(tmp,&a,&b,l-1,(tmp && tmp->left)? tmp->left->size:0);
    if(b->mtag==0 || (b->mtag!=0 && k<b->mtag)) b->mtag=k;
    return merge(merge(a,b),c);
}

int main()
{
    int N,Q;
    scanf("%d%d",&N,&Q);
    Node *root=NULL;
    for(int i=0;i<N;i++){
        int time;
        scanf("%d",&time);
        if(root && root->left) root=insert(root,time,i,root->left->size);
        else root=insert(root,time,i,0);
    }
    for(int i=0;i<Q;i++){
        int cmd;
        scanf("%d",&cmd);
        if(cmd==1){
            int p,time;
            scanf("%d%d",&p,&time);
            if(root && root->left) root=insert(root,time,p,root->left->size);
            else root=insert(root,time,p,0);
        }else if(cmd==2){
            int p;
            scanf("%d",&p);
            if(root && root->left) root=delete(root,--p,root->left->size);
            else root=delete(root,--p,0);
        }else if(cmd==3){
            int l,r;
            scanf("%d%d",&l,&r);
            root=swap_marker(root,--l,--r);
        }else if(cmd==4){
            int l,r,x,y;
            scanf("%d%d%d%d",&l,&r,&x,&y);
            root=swap_blocks(root,--l,--r,--x,--y);
        }else if(cmd==5){
            int l,r,k;
            scanf("%d%d%d",&l,&r,&k);
            root=update(root,--l,--r,k);
        }else if(cmd==6){
            int l,r;
            scanf("%d%d",&l,&r);
            uint64_t sum;
            root=reboot_Time(root,--l,--r,&sum);
            printf("%lu\n",sum);

        }
    }
    return 0;
}