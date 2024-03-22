#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
// #include "ecdsa.h"
#include "sha256.h"
#include "curves.h"
#include "secp256k1.h"

#define int long long

int p=1e9+7;

char* encrypt_decrypt(char *key, char *input) {
    char *output = (char*)malloc(strlen(input) + 1); 
    
    for(int i = 0; i < strlen(input); i++) {
        output[i] = input[i] ^ key[i % strlen(key)]; 
    }
    output[strlen(input)] = '\0'; 
    return output;
}

char *int_to_string(int num) {
    int size = snprintf(NULL, 0, "%lld", num);
    if (size < 0) {
        return NULL;
    }

    char *str = (char *)malloc(size + 1);
    if (str == NULL) {
        return NULL; 
    }
    snprintf(str, size + 1, "%lld", num);
    return str;
}

unsigned int poww(int x,int y,int p){
	unsigned int res=1;
	while(y>0){
		if(y&1)res=(res*x)%p;
		y/=2;
		x=(x*x)%p;
	}
	return res;
}

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

int charArrayToInt(char *charArray) {
    int result = 0;
    int length=strlen(charArray);
    for (int i = 0; i < length; i++) {
        result = result * 10 + (charArray[i] - '0');
    }
    return result;
}


int oblivious_transfer(int g,int m0,int m1,int c){
    int a=rand()%1000;
    int b=rand()%1000;
    int alice=(int)poww(g,a,p);

    // printf("Alice : %d\na: %d\nb: %d\n",alice,a,b);
    int bob;
    if(c==0){
        bob=(int)poww(g,b,p);
    }else{
        bob=alice*((int)poww(g,b,p));
    }

    char alice_key0[SHA256_HEX_SIZE];
    char alice_key1[SHA256_HEX_SIZE];

    const char* alice_val1 = int_to_string((int)poww(bob,a,p));
    const char* alice_val2 = int_to_string((int)poww(bob/alice,a,p));

    sha256_hex(alice_val1,strlen(alice_val1),alice_key0); // alice's key0
    sha256_hex(alice_val2,strlen(alice_val2),alice_key1);  // alice's key1

    char* m0_str=int_to_string(m0);
    char* m1_str=int_to_string(m1);

    char* encryptM0=encrypt_decrypt(alice_key0,m0_str);
    char* encryptM1=encrypt_decrypt(alice_key1,m1_str);
    
    char bob_key[SHA256_HEX_SIZE];
    const char* bob_val = int_to_string((int)poww(alice,b,p));
    sha256_hex(bob_val,strlen(bob_val),bob_key); // bob's key0

    char* decryptM0=encrypt_decrypt(bob_key,encryptM0);
    char* decryptM1=encrypt_decrypt(bob_key,encryptM1);
    if(strcmp(decryptM0,m0_str)==0){
        int val=charArrayToInt(m0_str);
        return val;
    }else{
        int val=charArrayToInt(m1_str);
        return val;
    }
}


int main(){
    int x, y, g;
    x=10001,y=90010,g=10002; // your multiplicative inputs x and y and g->determined by alice and bob
    int x_bits[32];
    int y_bits[32]; 
    decimalToBinary(x,x_bits);
    decimalToBinary(y,y_bits);
    

    int u=0,v=0; // additive shares
    int curr_pow=1;
    for(int i=0;i<32;++i){
        int u_i=rand()%10+1;
        int m0=u_i;
        int m1=u_i+x;
        int c=y_bits[i];
        int mci= oblivious_transfer(g,m0,m1,c);
        u=u-(curr_pow*u_i);
        v=v+(curr_pow*mci);
        // if(c==0){
        //     v=v+(curr_pow*m0);
        // }else{
        //     v=v+(curr_pow*m1);
        // }
        curr_pow*=2;
    }
    printf("x: %lld  y : %lld",x,y);
    printf("\n");
    printf("u : %lld v : %lld",u,v);
    return 0;
}