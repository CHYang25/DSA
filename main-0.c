#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#define MAXDIGIT 257

typedef struct bigint{
    int numD;
    int arr[MAXDIGIT];
}bigInt;

void printer(bigInt *a)
{
    for(int i=a->numD-1;i>=0;i--)
        printf("%d",a->arr[i]);
    printf("\n");
}

void reverse(bigInt *a)
{
    for(int i=0;i<a->numD/2;i++){
        int temp=a->arr[i];
        a->arr[i]=a->arr[a->numD-i-1];
        a->arr[a->numD-i-1]=temp;
    }
    return;
}

int compare(bigInt *a,bigInt *b)
{
    if(a->numD > b->numD) return 1;
    else if(a->numD == b->numD){
        for(int i=a->numD-1;i>=0;i--)
            if(a->arr[i]>b->arr[i]) return 1;
            else if(a->arr[i]<b->arr[i]) return 0;
        return 0;
    }else return 0;
}

void subtract(bigInt *a,bigInt *b)
{
    assert(a->numD >= b->numD);
    for(int i=0;i<a->numD;i++){
        if(a->arr[i]<b->arr[i]){
            a->arr[i]=a->arr[i]+10-b->arr[i];
            a->arr[i+1]--;
        }else a->arr[i]=a->arr[i]-b->arr[i];
    }
    while(a->numD>=1 && a->arr[a->numD-1]==0) a->numD--;
    return;
}

void divide2(bigInt *a)
{
    int x=0;
    for(int i=a->numD;i>=0;i--){
        if((a->arr[i]+10*x)%2!=0){
            a->arr[i]=(a->arr[i]+10*x)/2;
            x=1;
        }else{
            a->arr[i]=(a->arr[i]+10*x)/2;
            x=0;
        }
    }
    while(a->numD>=1 && a->arr[a->numD-1]==0) a->numD--;
    return;
}

void multi2(bigInt *a,int k)
{  
    for(int j=0;j<k;j++){
        bool nextPlus=false;
        for(int i=0;i<=a->numD;i++){
            a->arr[i]*=2;
            if(nextPlus) a->arr[i]++;
            nextPlus=(a->arr[i]/10>0);
            a->arr[i]%=10;
        }
        if(a->arr[a->numD] > 0) a->numD++;
    }
    return;
}

bigInt *gcd(bigInt *a,bigInt *b)
{
    bigInt *n=(!compare(a,b))? a:b;
    bigInt *m=(compare(a,b))? a:b;
    int ansK=0;
    while(n->numD!=0 && m->numD!=0){
        if(n->arr[0]%2==0 && m->arr[0]%2==0){
            ansK++;
            divide2(n);
            divide2(m);
        }
        else if(n->arr[0]%2==0) divide2(n);
        else if(m->arr[0]%2==0) divide2(m);

        if(compare(n,m)){
            bigInt *temp=n;
            n=m;
            m=temp;
        }
        subtract(m,n);
    }
    multi2(n,ansK);
    return n;
}

int main()
{
    bigInt a={0,{0}},b={0,{0}};
    while(scanf("%c",&a.arr[a.numD])==1){
        if(a.arr[a.numD]==' ') break;
        a.arr[a.numD]-='0';
        a.numD++;
    }
    a.arr[a.numD]=0;
    while(scanf("%c",&b.arr[b.numD])==1){
        if(b.arr[b.numD]=='\n') break;
        b.arr[b.numD]-='0';
        b.numD++;
    }
    b.arr[b.numD]=0;
    reverse(&a);
    reverse(&b);
#ifndef UNCHECK
    bigInt *ans=gcd(&a,&b);
    printer(ans);
#endif
#ifdef CHECKINPUT
    printer(a);
    printer(b);
#endif
#ifdef CHECKCOMP
    if(compare(&a,&b)) printf("a is bigger than b\n");
    else printf("a isn't bigger than b\n");
#endif
#ifdef CHECKSUB
    subtract(&a,&b);
    printer(&a);
#endif
#ifdef CHECKDIV2
    divide2(&a);
    printer(&a);
#endif
#ifdef CHECKMUL2
    multi2(&a,10);
    printer(&a);
#endif
    return 0;
} 

