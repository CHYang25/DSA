#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define SLEN 13

typedef struct disjointSet{
    int size,label,head;
} DisjointSet;

DisjointSet ds[1 << 24];
bool set[1 << 24] = {};

int c2i(char c) { 
    if ('0' <= c && c <= '9') return c - '0';
    else if ('a' <= c && c <= 'z') return c - 'a' + 10;
    else if ('A' <= c && c <= 'Z') return c - 'A' + 36;
    return -1;
}

int hash(const char* s) {  
    int ret = 0;
    int mask = (1<<24)-1;
    int len = strlen(s);
    for (int i = 0; i < len; i++)
        ret = mask & (ret << 4) | c2i(s[i]);
    return ret;
}

void makeset(const char* s){
    int i=hash(s);
    ds[i].size=1;
    ds[i].head=i;
    ds[i].label=i;
    return;
}

inline void static init(const char* s) {
    int i = hash(s);
    if (!set[i]) {
        makeset(s);
        set[i] = 1;
    }
}

int find_set(const char* s) {
    init(s);
    int i = hash(s);
    int tmp=i;
    while(ds[i].head!=i){
        i=ds[i].head;
    }
    while(ds[tmp].head!=tmp){
        int tmp2=ds[tmp].head;
        ds[tmp].head=i;
        tmp=tmp2;
    }
    return ds[i].label;
}

void group(const char *ra, const char *rb) {
    int a = find_set(ra), b = find_set(rb);
    int t_size=ds[a].size+ds[b].size;
    if(ds[a].size>ds[b].size){
        ds[b].head=a;
        ds[a].size=t_size;
    }else{
        ds[a].head=b;
        ds[b].size=t_size;
    }   
    return;
}

bool same_set(const char*a, const char* b) {
    int na = find_set(a), nb = find_set(b);
    return na==nb;
}

int main() {
    int n;
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        char str[SLEN],n1[SLEN],n2[SLEN];
        scanf("%s%s%s",str,n1,n2);
        if(str[0]=='g'){
            group(n1,n2);
        }else if(str[0]=='t'){
            if(same_set(n1,n2)){
                printf("Positive\n");
            }else{
                printf("Negative\n");
            }
        }
    }
    return 0;
}