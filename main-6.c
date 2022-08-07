#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

typedef struct node{
    int dat;
    struct node *next;
}Node;

Node *genNode(int dat,Node *next)
{
    Node *node=malloc(sizeof(Node));
    node->dat=dat;
    node->next=next;
    return node;
}

void printer(Node *root)
{
    while(root!=NULL){
        printf("%d ",root->dat+1);
        root=root->next;
    }
    printf("\n");
    return;
}

void construct(Node **roots,Node **city,int now,Node *last)
{
    roots[now]=genNode(now,last);
    Node *runner=city[now];
    while(runner!=NULL){
        if(last==NULL || (last!=NULL && runner->dat!=last->dat))
            construct(roots,city,runner->dat,roots[now]);
        runner=runner->next;
    }
    return;
}

int compare(Node *runS,Node *runR,int *rem,int index)
{
    if(rem[index]!=-1) return rem[index];
    if(runS->next==NULL || runS->next->dat!=runR->next->dat){
        return runS->dat;
    }
    rem[index]=compare(runS->next,runR->next,rem,runS->next->dat);
    return rem[index];
}

int main()
{
    int n,q,s,r;
    scanf("%d%d%d%d",&n,&q,&s,&r);
    s--;r--;
    Node **city=malloc(n*sizeof(Node*));
    for(int i=0;i<n-1;i++){
        int c1,c2;
        scanf("%d%d",&c1,&c2);
        c1--;c2--;
        city[c1]=genNode(c2,city[c1]);
        city[c2]=genNode(c1,city[c2]);
    }
#ifdef DEBUG
    for(int i=0;i<n;i++)
        printer(city[i]);
#endif
#ifndef DEBUG
    Node **rootS=malloc(n*sizeof(Node*));
    Node **rootR=malloc(n*sizeof(Node*));
    int *rem=malloc(n*sizeof(int));
    for(int i=0;i<n;i++)
        rem[i]=-1;
    construct(rootS,city,s,NULL);
    construct(rootR,city,r,NULL);
    Node *runner=rootS[r];
    while(runner!=NULL){
        rem[runner->dat]=runner->dat;
        runner=runner->next;
    }
    for(int i=0;i<n;i++){
        if(rem[i]==-1){
            rem[i]=compare(rootS[i],rootR[i],rem,i);
        }
    }
    for(int i=0;i<q;i++){
        int c;
        scanf("%d",&c);
        printf("%d\n",rem[c-1]+1);
    }
#endif
    return 0;
}