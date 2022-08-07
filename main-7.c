#include "price.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#define MAXLEN 2048
#define HLEN 1000000

typedef struct pair{
    uint64_t val;
    uint64_t stk;
    uint64_t t;
}Pair;
typedef struct heap{
    Pair *arr;
    int len;
}Heap;

void swap(Pair *a,Pair *b)
{
    Pair tmp=(*a);
    (*a)=(*b);
    (*b)=tmp;
}

void heapify(int x,Heap *h)
{
    while(x<=h->len){
        bool l=(x<<1)<=h->len && h->arr[x-1].val>h->arr[(x<<1)-1].val;
        bool r=(x<<1)+1<=h->len && h->arr[x-1].val>h->arr[x<<1].val;
        if(r && l){
            if(h->arr[(x<<1)-1].val<h->arr[x<<1].val){
                swap(h->arr+x-1,h->arr+(x<<1)-1);
                x<<=1;
            }else{
                swap(h->arr+x-1,h->arr+(x<<1));
                x<<=1;
                x++;
            }
        }else if(l){
            swap(h->arr+x-1,h->arr+(x<<1)-1);
            x<<=1;
        }else if(r){
            swap(h->arr+x-1,h->arr+(x<<1));
            x<<=1;
            x++;
        }else break;
    }
}

void construct(Heap *h)
{
    for(int i=h->len/2;i>0;i--){
        heapify(i,h);
    }
    return;
}

uint64_t removeMin(Heap *h)
{
    uint64_t tmp=h->arr[0].val;
    h->arr[0]=h->arr[--h->len];
    heapify(1,h);
    return tmp;
}

uint64_t cmp(const void *data1,const void *data2)
{
    return (*(uint64_t*)data1)-(*(uint64_t*)data2);
}

uint64_t smallkval(uint64_t *arr,int mid,int n,uint64_t s,int k)
{
    for(int i=0,j=mid-n+2;i<2*n-1;i++,j++){
        arr[i]=price(s,j);
    }
    qsort(arr,2*n-1,sizeof(uint64_t),cmp);
    return arr[n-1];
}

uint64_t findbalance(uint64_t *result,uint64_t s,int h,int t,int k,int n)
{
    int mid1=(h+t)/2;
    int mid2=k-mid1-1;
    uint64_t *arr=malloc((2*n)*sizeof(uint64_t));
    uint64_t tmp=smallkval(arr,mid2,n,s,k);
#ifdef DEBUG
    printf("mid1:%llu mid2:%llu num1:%llu num2:%llu delta:%lld arrf:%llu arrb:%llu\n",mid1,mid2,result[mid1],tmp,(int64_t)result[mid1]-(int64_t)tmp,arr[0],arr[2*n-2]);
#endif
    if(arr[n-2]<=result[mid1] && arr[n]>=result[mid1]){
        //printf("did\n");
        if(arr[n-1]>result[mid1]) return result[mid1];
        else return arr[n-1];
    }else if(h==t){
        arr[2*n-1]=result[mid1];
        qsort(arr,2*n,sizeof(uint64_t),cmp);
        return arr[n-1];
    }else if(tmp>result[mid1]){
        free(arr);
        return findbalance(result,s,mid1+1,t,k,n);
    }else if(tmp<result[mid1]){
        free(arr);
        return findbalance(result,s,h,mid1-1,k,n);
    }
}

int main()
{
    //clock_t c1=clock();
    int a,q,n;
    scanf("%d%d%d",&a,&q,&n);
    uint64_t stock[a];
    for(int i=0;i<a;i++)
        scanf("%llu",stock+i);
    // uint64_t s[q],k[q],ns[q];
    // for(int i=0;i<q;i++){
    //     scanf("%llu%llu",s+i,k+i);
    //     ns[i]=i;
    // }

    Heap *ad=malloc(sizeof(Heap));
    ad->len=0;
    ad->arr=malloc(a*n*sizeof(Pair));
    for(int i=0;i<a;i++){
        for(int t=1;t<=n;t++,ad->len++){
            ad->arr[ad->len].val=price(stock[i],(uint64_t)t);
            ad->arr[ad->len].stk=i;
            ad->arr[ad->len].t=(uint64_t)t;
        }
    }
    construct(ad);
    uint64_t *result=malloc(HLEN*sizeof(uint64_t));
    for(int i=0;i<HLEN;i++){
        result[i]=ad->arr[0].val;
        ad->arr[0].t+=n;
        ad->arr[0].val=price(stock[ad->arr[0].stk],ad->arr[0].t);
        heapify(1,ad);
    }
    for(int i=0;i<q;i++){ 
        uint64_t s,k;
        scanf("%llu%llu",&s,&k);
        if(s==0){
            printf("%llu\n",result[k-1]);
        }else{
            printf("%llu\n",findbalance(result,s,0,k-1,k,n));
        }
    }
    free(ad);
    free(result);
    //clock_t c2=clock();
    //printf("%f\n",(double)(c2-c1));
    return 0;
}