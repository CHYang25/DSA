#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define MAXLEN 100

typedef struct stack{
    char stk[MAXLEN];
    int prior[MAXLEN];
    int len;
}Stack;

void push(Stack *stk,char in,int pri)
{
    stk->stk[stk->len]=in;
    stk->prior[stk->len++]=pri;
}

char pop(Stack *stk)
{
    char tmp=stk->stk[stk->len-1];
    stk->stk[stk->len-1]=0;
    stk->prior[--stk->len]=0;
    return tmp;
}

int seqNum(char c,int level)
{
    switch(c){
        case '+':
        case '-':
            return 0+2*level;
        case '*':
        case '/':
        case '%':
            return 1+2*level;
    }
}

int64_t arithmatic(char c,int64_t a,int64_t b)
{
    switch(c){
        case '+': return a+b;
        case '-': return a-b;
        case '*': return a*b;
        case '/':
            if(a<0) return a/b-1;
            else return a/b;
        case '%': return a%b;
    }
}

int main()
{
    Stack *stkO=(Stack*)malloc(sizeof(Stack));
    for(int i=0;i<MAXLEN;i++){
        stkO->stk[i]=0;
        stkO->prior[i]=0;
    }
    stkO->len=0;
    int64_t stkN[MAXLEN]={0LL};
    int lenN=0;
    int level=0;
    char c;
    scanf("%c",&c);
    while(c!='\n'){
        if(c>='0' && c<='9'){
            stkN[lenN]=stkN[lenN]*10+c-'0';
        }else if(c=='='){
            lenN++;
            while(lenN>1 && stkO->len>0 && stkO->prior[stkO->len-1]>=level*2){
                stkN[lenN-2]=arithmatic(pop(stkO),stkN[lenN-2],stkN[lenN-1]);
                stkN[--lenN]=0LL;
            }
            printf("Print: %lld\n",stkN[--lenN]);
        }else if(c=='('){
            level++;
        }else if(c==')'){
            lenN++;
            while(lenN>1 && stkO->len>0 && stkO->prior[stkO->len-1]>=level*2){
                stkN[lenN-2]=arithmatic(pop(stkO),stkN[lenN-2],stkN[lenN-1]);
                stkN[--lenN]=0LL;
            }
            level--;
            lenN--;
        }else{
            lenN++;
            int pri=seqNum(c,level);
            if(stkO->len>0 && pri>stkO->prior[stkO->len-1]){
                push(stkO,c,pri);
            }else{
                while(lenN>1 && stkO->len>0 && stkO->prior[stkO->len-1]>=pri){
                    stkN[lenN-2]=arithmatic(pop(stkO),stkN[lenN-2],stkN[lenN-1]);
                    stkN[--lenN]=0LL;
                }
                push(stkO,c,pri);
            }
        }
        scanf("%c",&c);
    }
    return 0;
}