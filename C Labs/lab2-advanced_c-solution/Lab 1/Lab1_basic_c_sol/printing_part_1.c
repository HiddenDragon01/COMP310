#include <stdio.h>
#include <stdlib.h>

int main() {
    int x;
    char* myName = "Mushfique";

    //  Print welcome message
    printf("Please Enter Desired Number of Repetitions.\n");

    //  Take in user input
    scanf("%d", &x);

    //  Print my name for x times
    printf("string %s will be repeated %d times\n", myName, x);
    for(int i = 0; i < x; i++) {
        printf("%s\n", myName);
    }

    return 0;
}