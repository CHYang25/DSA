#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAXLEN 26

bool checkRow(int x,int c,int one,int rowReq[MAXLEN],int rrNum)
{
    if(rrNum==0){
        if(x==0) return true;
        else return false;
    }
    int len[MAXLEN]={0};
    int ri=0;
    for(;one>0;ri++){
        while(one>0 && (x&one)==0) one>>=1;
        while(one>0 && (x&one)==one){
            len[ri]++;
            one>>=1;
        }
    }
    if(ri!=rrNum && len[rrNum]!=0) return false;
    for(int i=0;i<rrNum;i++){
        if(len[i]!=rowReq[i]) return false;
    }
    return true;
}

bool checkColumn(int one,int r,int puzBin[],int colReq[MAXLEN],int crNum)
{
    if(crNum==0){
        for(int i=0;i<r;i++){
            if((one&puzBin[i])!=0) return false;
        }
        return true;
    }
    int len[MAXLEN]={0};
    int ci=0;
    int i=0;
    for(;i<r;ci++){
        while(i<r && (puzBin[i]&one)==0) i++;
        while(i<r && (puzBin[i]&one)==one){
            len[ci]++;
            i++;
        }
    }
    if(ci!=crNum && len[crNum]!=0) return false;
    for(int j=0;j<crNum;j++){
        if(len[j]!=colReq[j]) return false;
    }
    return true;
}

void printer(int r,int c,int puzBin[],bool reversed)
{
    if(reversed){
        int one=1<<(c-1);
        for(int i=0;i<c;i++){
            for(int j=0;j<r;j++){
                printf("%c",((puzBin[j]&one)==one)? 'o':'_');
            }
            one>>=1;
            printf("\n");
        }
    }else{
        for(int i=0;i<r;i++){
            int one=1<<(c-1);
            for(int j=0;j<c;j++){
                printf("%c",((puzBin[i]&one)==one)? 'o':'_');
                one>>=1;
            }
            printf("\n");
        }
    }
    return;
}

bool findPattern(int r,int c,int nowr,int wid,int puzBin[],int rowReq[MAXLEN][MAXLEN],
                    int colReq[MAXLEN][MAXLEN],int rrNum[MAXLEN],int crNum[MAXLEN],bool reversed)
{
    if(nowr==r){
        for(int i=0;i<c;i++){
            if(!checkColumn(wid>>(i+1),r,puzBin,colReq[i],crNum[i])) return false;
        }
        printer(r,c,puzBin,reversed);
        return true;
    }
    for(int i=0;i<wid;i++){
        if(checkRow(i,c,wid>>1,rowReq[nowr],rrNum[nowr])){
            puzBin[nowr]=i;
            if(findPattern(r,c,nowr+1,wid,puzBin,rowReq,colReq,rrNum,crNum,reversed)) return true;
        }
    }
    return false;
}

int main()
{
    int r,c;
    scanf("%d%d",&r,&c);
    if(r*c==0) return 0;
    int puzBin[MAXLEN]={0};
    int rowReq[MAXLEN][MAXLEN]={{0}};
    int colReq[MAXLEN][MAXLEN]={{0}};
    int rrNum[MAXLEN]={0};
    int crNum[MAXLEN]={0};
    for(int i=0;i<r;i++){
        scanf("%d",&rrNum[i]);
        for(int j=0;j<rrNum[i];j++){
            scanf("%d",&rowReq[i][j]);
        }
    }
    for(int i=0;i<c;i++){
        scanf("%d",&crNum[i]);
        for(int j=0;j<crNum[i];j++){
            scanf("%d",&colReq[i][j]);
        }
    }
    bool reversed= c>r;
    if(c>r){
        int tmp=r;
        r=c;
        c=tmp;
        for(int i=0;i<MAXLEN;i++){
            for(int j=0;j<MAXLEN;j++){
                int temp=rowReq[i][j];
                rowReq[i][j]=colReq[i][j];
                colReq[i][j]=temp;
            }
            int temp=rrNum[i];
            rrNum[i]=crNum[i];
            crNum[i]=temp;
        }
    }
    findPattern(r,c,0,1<<c,puzBin,rowReq,colReq,rrNum,crNum,reversed);
    return 0;
}