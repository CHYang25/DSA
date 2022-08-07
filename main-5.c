#include "price.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
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

uint64_t removeMin(Heap *h)
{
    uint64_t tmp=h->arr[0].val;
    h->arr[0]=h->arr[--h->len];
    heapify(1,h);
    return tmp;
}

int cmp(const void *data1,const void *data2)
{
    uint64_t dat1=*(uint64_t*)data1;
    uint64_t dat2=*(uint64_t*)data2;
    if(dat1>dat2) return 1;
    else if(dat1<dat2) return -1;
    else return 0;
}

int smallkval(uint64_t *arr,int mid,int n,uint64_t s,int k)
{
    if(mid-n<0){
        for(int i=0,j=1;i<mid+n+1;i++,j++){
            arr[i]=price(s,(uint64_t)j);
        }
        qsort(arr,mid+n+1,sizeof(uint64_t),cmp);
        return mid;
    }else{
        for(int i=0,j=mid-n+1;i<2*n+1;i++,j++){
            arr[i]=price(s,(uint64_t)j);
        }
        qsort(arr,2*n+1,sizeof(uint64_t),cmp);
        return n;
    }
}

uint64_t findbalance(uint64_t *result,uint64_t s,int h,int t,int k,int n)
{
    int mid1=(h+t)/2;
    int mid2=k-mid1-1;
    uint64_t pri=price(s,(uint64_t)mid2+1ULL);
    if(t-h<1){
        uint64_t *arr=malloc((4*n+2)*sizeof(uint64_t));
        int index=smallkval(arr,mid2,n,s,k);
        int start=(mid1-n>=0)? mid1-n:0;
        int end=(mid1+n<HLEN)? mid1+n:HLEN-1;
        int aim=(mid2-n<0)? k-start-1 : k-start-mid2+n-1;
        int x=0,i=0,j=start;
        for(;i<index+n+1 && j<=end && x<aim;){
            if(arr[i]<result[j]){i++;x++;}
            else{j++;x++;}
        }
        if(arr[i]<result[j]) return arr[i];
        else return result[j];
    }else if(pri>result[mid1]){
        return findbalance(result,s,mid1+1,t,k,n);
    }else{
        return findbalance(result,s,h,mid1-1,k,n);
    }
}

int main()
{
    int a,q,n;
    scanf("%d%d%d",&a,&q,&n);
    uint64_t stock[a];
    for(int i=0;i<a;i++)
        scanf("%llu",stock+i);

    Heap *ad=malloc(sizeof(Heap));
    ad->len=0;
    ad->arr=malloc(a*n*sizeof(Pair));
    for(int i=0;i<a;i++){
        for(uint64_t t=1;t<=n;t++,ad->len++){
            ad->arr[ad->len].val=price(stock[i],t);
            ad->arr[ad->len].stk=i;
            ad->arr[ad->len].t=t;
        }
    }
    for(int i=ad->len/2;i>0;i--){
        heapify(i,ad);
    }
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
    return 0;
}