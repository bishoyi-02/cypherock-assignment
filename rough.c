#include <stdio.h>
#include<string.h>

int charArrayToInt(char *charArray) {
    int result = 0;
    int length=strlen(charArray);
    for (int i = 0; i < length; i++) {
        result = result * 10 + (charArray[i] - '0');
    }
    return result;
}

int main() {
    char charArray[] = {'1', '2'};
    int length = sizeof(charArray) / sizeof(char); // Calculate the length of the array
    int intValue = charArrayToInt(charArray);
    printf("Integer value: %d\n", intValue);
    return 0;
}
