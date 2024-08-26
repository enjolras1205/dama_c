#include <stdio.h>
#include <stdlib.h>

int main() {
    const char* token = getenv("token"); 
    if (token) {
        while(1) {
            printf("The token is: %s\n", token);
        }
    } else {
        printf("Environment variable 'TOKEN' is not set.\n");
    }
    return 0;
}
