#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct node{
    int id;
    struct node *next[2];
}Node;

typedef struct group{
    int g;
    struct node *head;
    struct node *tail;
    int trav;
    struct group *prev;
    struct group *next;
}Group;

typedef struct list{
    Group *head;
    Group *tail;
    Group **tags;
    int np,nn;
}List;

Node *genNode(int id,Node *prev,Node *next)
{
    Node *node=malloc(sizeof(Node));
    node->id=id;
    node->next[0]=prev;
    node->next[1]=next;
    return node;
}

Group *genGroup(Node *head,Node *tail,Group *prev,Group *next,int g)
{
    Group *group=malloc(sizeof(Group));
    group->g=g;
    group->head=head;
    group->tail=tail;
    group->prev=prev;
    group->next=next;
    group->trav=0;
    return group;
}

void printer(List *ls[],int b)
{
    for(int i=0;i<b;i++){
        Group *runner=ls[i]->head;
        while(runner!=NULL){
            if(runner->trav%2==0){
                Node *traveler=runner->head;
                while(traveler!=NULL){
                    printf("%d ",traveler->id);
                    traveler=traveler->next[1];
                }
            }else{
                Node *traveler=runner->tail;
                while(traveler!=NULL){
                    printf("%d ",traveler->id);
                    traveler=traveler->next[0];
                }
            }
            //printf(" | ");
            runner=runner->next;
        }
        printf("\n");
    }
}

void enter(List *ls,int group,int id)
{
    if(ls->head==NULL){
        Node *tmp=genNode(id,NULL,NULL);
        ls->head=genGroup(tmp,tmp,NULL,NULL,group);
        ls->tail=ls->head;
        ls->tags[group]=ls->head;
    }else if(ls->tags[group]==NULL){
        Node *tmp=genNode(id,NULL,NULL);
        ls->tail->next=genGroup(tmp,tmp,ls->tail,NULL,group);
        ls->tail=ls->tail->next;
        ls->tags[group]=ls->tail;
    }else{
        int trav=ls->tags[group]->trav%2;
        if(trav==0){
            ls->tags[group]->tail->next[trav+1]=genNode(id,ls->tags[group]->tail,NULL);
            ls->tags[group]->tail=ls->tags[group]->tail->next[trav+1];
        }else{
            ls->tags[group]->head->next[(trav+1)%2]=genNode(id,NULL,ls->tags[group]->head);
            ls->tags[group]->head=ls->tags[group]->head->next[(trav+1)%2];
        }
    }   
}

void leave(List *ls)
{
    if(ls->tail->head==ls->tail->tail){
        if(ls->head==ls->tail){
            ls->tags[ls->head->g]=NULL;
            free(ls->head);
            ls->head=NULL;
            ls->tail=NULL;
        }else{
            if(ls->tail->prev->g==ls->tail->g) ls->tags[ls->tail->g]=ls->tail->prev;
            else ls->tags[ls->tail->g]=NULL;
            ls->tail=ls->tail->prev;
            free(ls->tail->next);
            ls->tail->next=NULL;
        }
    }else{
        if(ls->tail->trav%2==0){
            ls->tail->tail=ls->tail->tail->next[0];
            free(ls->tail->tail->next[1]);
            ls->tail->tail->next[1]=NULL;
            if(ls->tags[ls->tail->g]==ls->tail)
                ls->tags[ls->tail->g]->tail=ls->tail->tail;
        }else{
            ls->tail->head=ls->tail->head->next[1];
            free(ls->tail->head->next[0]);
            ls->tail->head->next[0]=NULL;
            if(ls->tags[ls->tail->g]==ls->tail)
                ls->tags[ls->tail->g]->head=ls->tail->head;
        }
    }
}

void go(List *ls)
{
    if(ls->head->head==ls->head->tail){
        if(ls->head==ls->tail){
            ls->tags[ls->head->g]=NULL;
            free(ls->head);
            ls->head=NULL;
            ls->tail=NULL;
        }else{
            if(ls->head==ls->tags[ls->head->g]) ls->tags[ls->head->g]=NULL;
            ls->head=ls->head->next;
            free(ls->head->prev);
            ls->head->prev=NULL;
        }
    }else{
        if(ls->head->trav%2==0){
            ls->head->head=ls->head->head->next[1];
            free(ls->head->head->next[0]);
            ls->head->head->next[0]=NULL;
            if(ls->tags[ls->head->g]==ls->head)
                ls->tags[ls->head->g]->head=ls->head->head;
        }else{
            ls->head->tail=ls->head->tail->next[0];
            free(ls->head->tail->next[1]);
            ls->head->tail->next[1]=NULL;
            if(ls->tags[ls->head->g]==ls->head)
                ls->tags[ls->head->g]->tail=ls->head->tail;
        }
    }
}

void append(List **ls,int line,int closer,int b)
{   
    while(ls[line]->tail!=NULL){
        Group *tmp=ls[line]->tail;
        tmp->trav++;
        ls[line]->tail=ls[line]->tail->prev;
        if(ls[line]->tail!=NULL) ls[line]->tail->next=NULL;
        if(ls[closer]->head==NULL){
            tmp->prev=NULL;
            tmp->next=NULL;
            ls[closer]->head=tmp;
            ls[closer]->tail=tmp;
            ls[closer]->tags[tmp->g]=tmp;
        }else if(ls[closer]->tags[tmp->g]==NULL || (ls[closer]->tags[tmp->g]!=NULL && ls[closer]->tags[tmp->g]->next==NULL)){
            tmp->prev=ls[closer]->tail;
            tmp->next=NULL;
            ls[closer]->tail->next=tmp;
            ls[closer]->tags[tmp->g]=tmp;
            ls[closer]->tail=ls[closer]->tail->next;
        }else{
            tmp->prev=ls[closer]->tags[tmp->g];
            tmp->next=ls[closer]->tags[tmp->g]->next;
            ls[closer]->tags[tmp->g]->next->prev=tmp;
            ls[closer]->tags[tmp->g]->next=tmp;
            ls[closer]->tags[tmp->g]=tmp;
        }
        
    }
    ls[line]->head=NULL;
    ls[line]->tail=NULL;
    ls[ls[line]->nn]->np=ls[line]->np;
    ls[ls[line]->np]->nn=ls[line]->nn;
    return;
}

int main()
{
    List **ls;
    int b,n,g;
    scanf("%d%d%d",&b,&n,&g);
    ls=malloc(b*sizeof(List*));
    for(int i=0;i<b;i++){      
        ls[i]=malloc(sizeof(List));
        ls[i]->head=ls[i]->tail=NULL;
        ls[i]->tags=malloc(g*sizeof(Group*));
        ls[i]->np=i-1;
        ls[i]->nn=i+1;
    }
    ls[0]->np=b-1;
    ls[b-1]->nn=0;
    for(int i=0;i<n;i++){
        char cmd[10];
        scanf("%s",cmd);
        int line;
        if(cmd[0]=='e'){
            int group,id;
            scanf("%d%d%d",&group,&id,&line);
            enter(ls[line],group,id);
        }else if(cmd[0]=='l'){
            scanf("%d",&line);
            leave(ls[line]);
        }else if(cmd[0]=='g'){
            scanf("%d",&line);
            go(ls[line]);
        }else if(cmd[0]=='c'){
            scanf("%d",&line);
            append(ls,line,ls[line]->np,b);
        }
        //printer(ls,b);
    }
    printer(ls,b);
    return 0;
}