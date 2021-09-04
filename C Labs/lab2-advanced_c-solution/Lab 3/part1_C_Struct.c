#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  Creating a new type of variable
typedef struct
{
    char name[30];
    int age;
    double gpa;
} student_t;

int main()
{
    size_t arraySize;

    // Asking the user for the size of the array.
    printf("Enter number of students\n> ");
    if (scanf("%zu", &arraySize) != 1)
    {
        printf("Error: Incorrect input format, please enter an Integer!\n");
        return EXIT_FAILURE;
    }

    //  Creating the an array of Students
    student_t *myArray = (student_t *)malloc(arraySize * sizeof(student_t));
    if (myArray == NULL)
    {
        printf("Error: Memory Allocation failed!\n");
        return EXIT_FAILURE;
    }

// Defining a Macro
#define ABORT(msg)           \
    {                        \
        printf(msg "\n");    \
        free(myArray);       \
        return EXIT_FAILURE; \
    }

    //  Asking the user for cell number and verify
    size_t index;
    printf("Enter cell number:\n> ");
    if (scanf("%zu", &index) != 1)
        ABORT("Error: Incorrect input format!");
    if (!(index < arraySize))
        ABORT("Error: Invalid index, array access of out bound!");

    //  Consume the remaining character from previous input.
    while (getchar() != '\n')
    {
    };

    //  Ask the user for name
    printf("Name:\n> ");
    fgets(myArray[index].name, 30, stdin);
    char *lastChar;
    if ((lastChar = strchr(myArray[index].name, '\n')) != NULL)
        *lastChar = '\0';
    else
        ABORT("Error: student name is too long");

    //  Ask the user for age and gpa.
    printf("Age:\n> ");
    if (scanf("%d", &(myArray[index].age)) != 1)
        ABORT("Error: expecting student age");
    printf("GPA:\n> ");
    if (scanf("%lf", &(myArray[index].gpa)) != 1)
        ABORT("Error: expecting student gpa");

#undef ABORT

    printf("Name: %s\n", myArray[index].name);
    printf("Age : %d\n", myArray[index].age);
    printf("GPA : %lf\n", myArray[index].gpa);

    free(myArray);
    return EXIT_SUCCESS;
}