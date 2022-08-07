#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <limits.h>
#include <time.h>
#define UMAX (int64_t)(INT_MAX/94)

typedef struct string{
    char *str;
    int skip,hash,index;
}String;

int skip_cmp(char *s1,char *s2,int skip)
{
    for(int i=0;s1[i]!='\0' && s2[i]!='\0';i++){
        if(i!=skip){
            if(s1[i]!=s2[i]) return s1[i]-s2[i];
        }
    }
    return 0;
}

void fZero_ls(int k,int l,String **s)
{
    for(int i=0;i<k-1;i++){
        if(s[i]->index!=s[i+1]->index){
            if(s[i]->hash==s[i+1]->hash){
                if(s[i]->skip==s[i+1]->skip){
                    if(skip_cmp(s[i]->str,s[i+1]->str,s[i]->skip)==0){
                        printf("Yes\n%d %d\n",s[i]->index,s[i+1]->index);
                        return;
                    }
                }
            }
        }
    }
    printf("No\n");
    return;
}

void fOne_ls(int k,int l,String **s)
{
    int64_t extra=0LL;
    int64_t total=0LL;
    for(int i=0;i<k-1;i++){
        int64_t acc=1;
        while(i<k-1 && s[i]->hash==s[i+1]->hash && s[i]->skip==s[i+1]->skip && s[i]->skip==-1 && skip_cmp(s[i]->str,s[i+1]->str,s[i]->skip)==0){
            acc++;
            i++;
        }
        extra+=(acc-1LL)*acc/2LL;
    }
    for(int i=0;i<k-1;i++){
        int64_t acc=1;
        while(i<k-1 && s[i]->hash==s[i+1]->hash && s[i]->skip==s[i+1]->skip && skip_cmp(s[i]->str,s[i+1]->str,s[i]->skip)==0){
            acc++;
            i++;
        }
        total+=(acc-1LL)*acc/2LL;
    }
    if(total-extra*(int64_t)l>0LL) printf("Yes\n%lld\n",total-extra*(int64_t)l);
    else printf("No\n");
    return;
}

void fZero_ks(int k,int l,char **str,int *hash)
{
    for(int i=0;i<k-1;i++){
        for(int j=i+1;j<k;j++){
            if(hash[i]==hash[j]){
                printf("Yes\n%d %d\n",i,j);
                return;
            }else{
                int diff=0;
                for(int x=0;x<l && diff<=1;x++){
                    diff+= (str[i][x]!=str[j][x]);
                }
                if(diff<=1){
                    printf("Yes\n%d %d\n",i,j);
                    return;
                }
            }
        }
    }
    printf("No\n");
    return;
}

void fOne_ks(int k,int l,char **str,int *hash)
{   
    int64_t total=0LL;
    for(int i=0;i<k-1;i++){
        for(int j=i+1;j<k;j++){
            if(hash[i]==hash[j]){
                total++;
            }else{
                int diff=0;
                for(int x=0;x<l && diff<=1;x++){
                    diff+= (str[i][x]!=str[j][x]);
                }
                total+= (int64_t)(diff<=1);
            }
        }
    }
    if(total>0LL) printf("Yes\n%lld\n",total);
    else printf("No\n");
    return;
}

int cmp(const void *data1,const void *data2)
{
    String *s1=*(String**)data1;
    String *s2=*(String**)data2;
    if(s1->hash==s2->hash){
        if(s1->skip!=s2->skip) return s1->skip-s2->skip;
        else return skip_cmp(s1->str,s2->str,s1->skip);
    }else return s1->hash-s2->hash;
}

int modifyHash(int64_t a,int64_t b,int64_t factor)
{
    return (int)(((a-(b-0x21LL)*factor)%UMAX+UMAX)%UMAX);
}

String *strMaker(char *str,int skip,int hash,int index)
{
    String *s=malloc(sizeof(String));
    s->str=str;
    s->skip=skip;
    s->hash=hash;
    s->index=index;
    return s;
}

String **preprocess_ls(int k,int l)
{
    char **str=malloc(k*sizeof(char*));
    String **s=malloc(k*(l+1)*sizeof(String*));
    int factor[l];
    factor[l-1]=1;
    for(int i=l-2;i>=0;i--){
        factor[i]=(int)(((int64_t)factor[i+1]*94LL)%UMAX);
    }

    int j=0;
    for(int i=0;i<k;i++){
        str[i]=malloc((l+1)*sizeof(char));
        scanf("%s",str[i]);

        s[j]=strMaker(str[i],-1,0,i);
        for(int x=0;x<l;x++)
            s[j]->hash=(int)((int64_t)s[j]->hash*94LL+(int64_t)str[i][x]-0x21LL)%UMAX;
        int std=s[j]->hash;
        j++;

        for(int x=0;x<l;x++,j++){
            s[j]=strMaker(str[i],x,modifyHash((int64_t)std,(int64_t)str[i][x],(int64_t)factor[x]),i);
        }
    }
    qsort(s,j,sizeof(String*),cmp);
    return s;
}

void preprocess_ks(int k,int l,char **str,int *hash)
{
    for(int i=0;i<k;i++){
        str[i]=malloc((l+1)*sizeof(char));
        scanf("%s",str[i]);
        for(int j=0;j<l;j++)
            hash[i]=(int)((int64_t)hash[i]*94LL+(int64_t)str[i][j]-0x21)%UMAX;
    }
}

int main()
{
#ifdef TIME
    clock_t c1=clock();
#endif
    int k,l,f;
    scanf("%d%d%d",&k,&l,&f);
    if(k==1){
        printf("No\n");
        return 0;
    }

    if(f==0){
        if(l<500){
            String **s=preprocess_ls(k,l);
            fZero_ls(k*(l+1),l,s);
        }else{
            char **str=malloc(k*sizeof(char*));
            int *hash=calloc(k,sizeof(int));
            preprocess_ks(k,l,str,hash);
            fZero_ks(k,l,str,hash);
        }
    }else if(f==1){
        if(700>l){
            String **s=preprocess_ls(k,l);
            fOne_ls(k*(l+1),l,s);
        }else{
            char **str=malloc(k*sizeof(char*));
            int *hash=calloc(k,sizeof(int));
            preprocess_ks(k,l,str,hash);
            fOne_ks(k,l,str,hash);
        }
    }
#ifdef TIME
    clock_t c2=clock();
    printf("time spent:%lf",(double)(c2-c1)/CLOCKS_PER_SEC);
#endif
    return 0;
}