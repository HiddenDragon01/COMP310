#include <stdio.h>
#include <stdlib.h>

int main() {
    int x;
    // char* myName = "Mushfique";
    char* sentence = malloc(sizeof(char)*100);

    //  Print welcome message
    printf("Please Enter Desired Number of Repetitions.\n");
    //  Take in user input
    scanf("%d", &x);

    //  Ask for sentence
    printf("Please Enter Desired Sentence.\n");
    //  Take in user input
    scanf(" %[^\n]%*c", sentence);
    // fgets(sentence);

    //  Print my name for x times
    printf("String %s will be repeated %d times\n", sentence, x);
    for(int i = 0; i < x; i++) {
        printf("%s\n", sentence);
    }

    free(sentence);

    return 0;
}