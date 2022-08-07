#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

typedef struct sector{
    int boom_d;
    int base_d;
    int **cmd;
    int len;
}Sector;

typedef struct node{
    int dat;
    struct node *next;
}Node;

typedef struct modified{
    int index,ori;
    struct modified *next;
}Modified;

int find_set(int *set_arr,int a,int N,int M)
{
    int tmp=a;
    while(set_arr[a-1]!=a){
        a=set_arr[a-1];
    }
    if(N>1000 && M>1000){
        while(set_arr[tmp-1]!=a){
            int tmp2=set_arr[tmp-1];
            set_arr[tmp-1]=a;
            tmp=tmp2;
        }
    }
    return a;
}

Modified *genModified(int index,int ori,Modified *next)
{
    Modified *mod=malloc(sizeof(Modified));
    mod->index=index;
    mod->ori=ori;
    mod->next=next;
    return mod;
}

int merge(int *set_arr,int a,int b,Modified **m,int N,int M)
{
    int s1=find_set(set_arr,a,N,M);
    int s2=find_set(set_arr,b,N,M);
    if(s1==s2) return 0;
    else{
        Modified *mod=(*m);
        mod=genModified(s1-1,set_arr[s1-1],mod);
        set_arr[s1-1]=s2;
        (*m)=mod;
        return 1;
    }
}

Sector *init_sector(int boom_d,int base_d,int **cmd,int len)
{
    Sector *s=malloc(sizeof(Sector));
    s->boom_d=boom_d;
    s->base_d=base_d;
    s->cmd=cmd;
    s->len=len;
    return s;
}

Node *genNode(int dat,Node *next)
{
    Node *node=malloc(sizeof(Node));
    node->dat=dat;
    node->next=next;
    return node;
}

void find_Total(Sector **sector,Node **ls,int curS,int sector_ct,int *result,int *cur,int total,int N,int M)
{
    Modified *m=NULL;
    for(int i=0;i<=sector[curS]->len;i++){
        if(sector[curS]->cmd[i][0]=='m'){
            total-=merge(cur,sector[curS]->cmd[i][1],sector[curS]->cmd[i][2],&m,N,M);
        }else if(sector[curS]->cmd[i][0]=='q'){
            result[sector[curS]->base_d+i]=total;
        }
        if(ls[sector[curS]->base_d+i]!=NULL){
            while(ls[sector[curS]->base_d+i]!=NULL){
                int tmp=ls[sector[curS]->base_d+i]->dat;
                ls[sector[curS]->base_d+i]=ls[sector[curS]->base_d+i]->next;
                find_Total(sector,ls,tmp,sector_ct,result,cur,total,N,M);
            }
        }
    }
    while(m!=NULL){
        cur[m->index]=m->ori;
        m=m->next;
    }
    return;
}

int main()
{
    int N,M;
    scanf("%d%d",&N,&M);
    Node **ls=malloc((M+1)*sizeof(Node*));
    int sector_ct=1;
    int **cmd=malloc((M+1)*sizeof(int*));
    int *boom_mark=malloc((M+2)*sizeof(int));
    cmd[0]=malloc(2*sizeof(int));
    cmd[0][0]='b';
    cmd[0][1]=0;
    boom_mark[sector_ct-1]=0;
    for(int d=1;d<=M;d++){
        char str[10];
        scanf("%s",str);
        if(str[0]=='m'){
            cmd[d]=malloc(3*sizeof(int));
            cmd[d][0]='m';
            scanf("%d%d",cmd[d]+1,cmd[d]+2);
        }else if(str[0]=='q'){
            cmd[d]=malloc(sizeof(int));
            cmd[d][0]='q';
        }else if(str[0]=='b'){
            cmd[d]=malloc(2*sizeof(int));
            cmd[d][0]='b';
            scanf("%d",cmd[d]+1);
            sector_ct++;
            boom_mark[sector_ct-1]=d;
            ls[cmd[d][1]]=genNode(sector_ct-1,ls[cmd[d][1]]);
        }
    }
    boom_mark[sector_ct]=M+1;
    Sector **sector=malloc(sector_ct*sizeof(Sector*));
    for(int d=0,i=0;d<M;){
        sector[i]=init_sector(cmd[d][1],d,cmd+boom_mark[i],boom_mark[i+1]-boom_mark[i]-1);
        d+=boom_mark[i+1]-boom_mark[i];
        i++;
    }
#ifdef SECTOR
    for(int i=0;i<sector_ct;i++){
        printf("------sector------\n");
        printf("%d boom_d:%d  base_d:%d  len:%d\n",i,sector[i]->boom_d,sector[i]->base_d,sector[i]->len);
        for(int j=1;j<=sector[i]->len;j++){
            if(sector[i]->cmd[j][0]=='q') printf("query\n");
            else if(sector[i]->cmd[j][0]=='m'){
                printf("merge %d %d\n",sector[i]->cmd[j][1],sector[i]->cmd[j][2]);
            }
        }
    }
#endif
#ifdef LS
    for(int i=0;i<=M;i++){
        Node *runner=ls[i];
        while(runner!=NULL){
            printf("%d %d\n",i,runner->dat);
            runner=runner->next;
        }
        printf("-------\n");
    }
#endif
    int *result=calloc((M+1),sizeof(int));
    int *cur=malloc(N*sizeof(int));
    for(int i=0;i<N;i++){
        cur[i]=i+1;
    }

#ifndef PREPDONE
    find_Total(sector,ls,0,sector_ct,result,cur,N,N,M);
#endif
    for(int i=0;i<=M;i++){
        if(result[i]!=0) printf("%d\n",result[i]);
    }
    return 0;
}