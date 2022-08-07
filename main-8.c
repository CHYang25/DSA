#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 10000001

void reverse(char *rev,char *str,int len)
{
    for(int i=0;i<len;i++){
        rev[i]=str[len-1-i];
    }
    rev[len]='\0';
}

int prefix_func(char *s,int len)
{
    int *arr=malloc((len+1)*sizeof(int));
    arr[0]=0;
    arr[1]=0;
    int k=0;
    for(int i=2;i<=len;i++){
        while(k>0 && s[k]!=s[i-1])
            k=arr[k];
        if(s[k]==s[i-1])
            k++;
        arr[i]=k;
    }
    return arr[len-1];
}

int findFront(char *rev,char *str,int slen)
{
    char *s=malloc((slen*2+1)*sizeof(char));
    strcpy(s,str);
    strcat(s,rev);
    //printf("%s\n",s);
    return prefix_func(s,slen*2+1);
}

int findBack(char *rev,char *str,int slen)
{
    char *s=malloc((slen*2+1)*sizeof(char));
    strcpy(s,rev);
    strcat(s,str);
    //printf("%s\n",s);
    return prefix_func(s,slen*2+1);
}

int main()
{
    char *str=malloc(MAXLEN*sizeof(char));
    scanf("%s",str);
    int slen=strlen(str);
    char *rev=malloc((slen+1)*sizeof(char));
    reverse(rev,str,slen);
    int catF=findFront(rev,str,slen); // catF is concat Front
    int catT=findBack(rev,str,slen);
    //printf("%d %d\n",catF,catT);
    int extra;
    if(catF>catT){
        extra=slen-catF;
        if(extra==0) printf("0\n%s\n",str);
        else{
            char *cat=malloc((extra+1)*sizeof(char));
            strncpy(cat,rev,extra);
            printf("%d\n%s%s\n",extra,cat,str);
        }
    }else if(catF<catT){
        extra=slen-catT;
        if(extra==0) printf("0\n%s\n",str);
        else{
            char *cat=malloc((extra+1)*sizeof(char));
            strcpy(cat,rev+catT);
            printf("%d\n%s%s\n",extra,str,cat);
        }
    }else{
        extra=slen-catF;
        if(extra==0) printf("0\n%s\n",str);
        else{
            char *cat=malloc((extra+1)*sizeof(char));
            strncpy(cat,rev,extra);
            printf("%d\n%s%s\n",extra,cat,str);
            strcpy(cat,rev+catT);
            printf("%s%s\n",str,cat);
        }
    }
    return 0;
}