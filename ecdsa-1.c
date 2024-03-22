#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define PRIME 17 // Size of the finite field (prime number)
#define A_COEFFICIENT 0
#define B_COEFFICIENT 7
#define G_X 6
#define G_Y 3
#define N_ORDER 7
#define H_COFACTOR 1

// Define a structure to represent a point on the elliptic curve
typedef struct {
    int x;
    int y;
    bool infinity; // Indicates if the point is the point at infinity
} ECPoint;

// Function prototypes
int mod(int a, int b);
ECPoint initPoint(int x, int y);
void printPoint(ECPoint point);
ECPoint pointAdd(ECPoint P, ECPoint Q);
ECPoint pointMultiply(ECPoint P, int scalar);

// Function to perform modulo operation for negative numbers
int mod(int a, int b) {
    int result = a % b;
    return (result >= 0) ? result : result + b;
}

// Function to initialize a point on the curve
ECPoint initPoint(int x, int y) {
    ECPoint point;
    point.x = x;
    point.y = y;
    point.infinity = false;
    return point;
}

// Function to print a point on the curve
void printPoint(ECPoint point) {
    if (point.infinity) {
        printf("Point at infinity\n");
    } else {
        printf("(%d, %d)\n", point.x, point.y);
    }
}

// Function to perform modular inverse
int modInverse(int a, int m) {
    a = mod(a, m);
    for (int x = 1; x < m; x++) {
        if (mod((a * x), m) == 1) {
            return x;
        }
    }
    return 1; // Invalid case
}

// Function to perform point addition on the elliptic curve
ECPoint pointAdd(ECPoint P, ECPoint Q) {
    if (P.infinity) {
        return Q;
    }
    if (Q.infinity) {
        return P;
    }
    if (P.x == Q.x && P.y != Q.y) {
        return initPoint(0, 0); // Point at infinity
    }

    int m;
    if (P.x == Q.x && P.y == Q.y) {
        m = mod(((3 * P.x * P.x + A_COEFFICIENT) * modInverse(2 * P.y, PRIME)), PRIME);
    } else {
        m = mod(((Q.y - P.y) * modInverse(Q.x - P.x, PRIME)), PRIME);
    }

    int xR = mod((m * m - P.x - Q.x), PRIME);
    int yR = mod((m * (P.x - xR) - P.y), PRIME);

    return initPoint(xR, yR);
}

// Function to perform scalar multiplication on the elliptic curve
ECPoint pointMultiply(ECPoint P, int scalar) {
    ECPoint R = initPoint(0, 0);
    ECPoint Q = P;
    while (scalar > 0) {
        if (scalar & 1) {
            R = pointAdd(R, Q);
        }
        Q = pointAdd(Q, Q);
        scalar >>= 1;
    }
    return R;
}



int main() {
    // Base point (generator point) G
    ECPoint G = initPoint(G_X, G_Y);

    // Example scalar (private key)
    int privateKey = 2;

    // Compute the public key by scalar multiplication: publicKey = privateKey * G
    ECPoint publicKey = pointMultiply(G, privateKey);

    // Print the public key
    printf("Public Key: ");
    printPoint(publicKey);

    return 0;
}
