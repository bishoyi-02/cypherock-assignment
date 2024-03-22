#include<stdio.h>
#include <stdlib.h>  


int* decimalToBinary(int num,int binary[32]) {
    int i = 0;
    
    while (num > 0) {
        binary[i] = num & 1;
        num = num >> 1;
        i++;
    }
    for(;i<32;++i){
        binary[i]=0;
    }

    return binary;
}


int main(){
    int x, y;
    printf("Enter two integers: ");
    scanf("%d %d", &x, &y);
     int x_bits[32];
    int y_bits[32];

    decimalToBinary(x,x_bits);
    decimalToBinary(y,y_bits);
    
    for(int i=31;i>=0;--i){
        printf("%d",x_bits[i]);
    }
    printf("\n");
    
    for(int i=31;i>=0;--i){
        printf("%d",y_bits[i]);
    }
    printf("\n");
    

    int u=0,v=0;
    int curr_pow=1;
    for(int i=0;i<32;++i){
        int u_i=rand()%10+1;
        int m0=u_i;
        int m1=u_i+x;
        int c=y_bits[i];
        
        u=u-(curr_pow*u_i);
        if(c==0){
            v=v+(curr_pow*m0);
        }else{
            v=v+(curr_pow*m1);
        }
        curr_pow*=2;
    }
    printf("x: %d  y : %d",x,y);
    printf("\n");
    printf("u : %d v : %d",u,v);
    return 0;
}