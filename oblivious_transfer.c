#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
// #include "ecdsa.h"
#include "sha256.h"
#include "curves.h"
#include "secp256k1.h"

#define int long long



char* encrypt_decrypt(char *key, char *input) {
    char *output = (char*)malloc(strlen(input) + 1); 
    
    for(int i = 0; i < strlen(input); i++) {
        output[i] = input[i] ^ key[i % strlen(key)]; 
    }
    output[strlen(input)] = '\0'; 
    return output;
}

char *int_to_string(int num) {
    int size = snprintf(NULL, 0, "%d", num);
    if (size < 0) {
        return NULL;
    }

    char *str = (char *)malloc(size + 1);
    if (str == NULL) {
        return NULL; 
    }
    snprintf(str, size + 1, "%d", num);
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




int main(){
    // ecdsa_curve curve;
    // const ecdsa_curve *curve = &secp256k1;
    
    int g=108121;
    int p=1e9+7;
    int a=rand()%1000;
    int b=rand()%1000;
    int alice=(int)poww(g,a,p);
    int c=1;
    int m0=112,m1=118;

    printf("Alice : %d\na: %d\nb: %d\n",alice,a,b);
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

    printf("m0_str : %s\n",m0_str);
    printf("m1_str : %s\n",m1_str);
    printf("encryptM0 : %s\n",encryptM0);
    printf("encryptM1 : %s\n",encryptM1);
    printf("decryptM0 : %s\n",decryptM0);
    printf("decryptM1 : %s\n",decryptM1);
    return 0;
}