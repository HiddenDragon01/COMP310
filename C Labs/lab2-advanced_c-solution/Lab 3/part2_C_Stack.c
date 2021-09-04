#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//  Creating a new type of variable
typedef struct
{
    char name[30];
    int age;
    double gpa;
} student_t;

const char *readStudent(student_t *myStudent)
{
    if (myStudent == NULL)
        return "readStudent(): NULL argument";

    //  Ask the user for name
    printf("Name:\n> ");
    fgets(myStudent->name, 30, stdin);
    char *lastChar;
    if ((lastChar = strchr(myStudent->name, '\n')) != NULL)
        *lastChar = '\0';
    else
        return "Error: student name is too long";

    //  Ask the user for age and gpa.
    printf("Age:\n> ");
    if (scanf("%d", &(myStudent->age)) != 1)
        return "Error: expecting student age";
    printf("GPA:\n> ");
    if (scanf("%lf", &(myStudent->gpa)) != 1)
        return "Error: expecting student gpa";

    return NULL;
}

//  Defining the Nodes for the linked list
typedef struct stackNodeTag
{
    student_t myStudent;
    struct stackNodeTag *next;
} stackNode_t;

//  Creating the Head Node
stackNode_t *head = NULL;

const char *push(student_t *myStudent)
{
    if (myStudent == NULL)
        return "push(): NULL argument";

    //  Creating a new Node
    stackNode_t *newNode = (stackNode_t *)malloc(sizeof(stackNode_t));
    if (newNode == NULL)
        return "push(): error occurred during memory allocation";

    //  Copying the Student entry to the new node
    memcpy(&(newNode->myStudent), myStudent, sizeof(student_t));
    newNode->next = head;
    head = newNode;

    return NULL;
}

const char *pop(student_t *myStudent)
{
    if (myStudent == NULL)
        return "pop(): NULL argument";

    //  Check if stack is empty
    if (head == NULL)
        return "pop(): popping an empty stack";

    //  Removing the current head and pointing to the next node
    stackNode_t *currentNode = head;
    head = head->next;
    memcpy(myStudent, &(currentNode->myStudent), sizeof(student_t));
    free(currentNode);

    return NULL;
}

bool isEmpty()
{
    return head == NULL;
}

void clear()
{
    //  Clearing the Stack
    while (head != NULL)
    {
        stackNode_t *currentNode = head;
        head = head->next;
        free(currentNode);
    }
}

const char *printStudent(student_t *myStudent)
{
    if (myStudent == NULL)
        return "printStudent(): NULL argument";
    printf("Name: %s\n", myStudent->name);
    printf("Age : %d\n", myStudent->age);
    printf("GPA : %lf\n", myStudent->gpa);
    return NULL;
}

int main()
{
    student_t myStudent;

// Defining a Macro
#define ABORT(msg)           \
    {                        \
        printf("%s\n", msg); \
        clear();             \
        return EXIT_FAILURE; \
    }

    do
    {
        const char *result;
        // Take user input for a student_t
        result = readStudent(&myStudent);
        if (result != NULL)
            ABORT(result);
        // Push the entry on to the Stack
        result = push(&myStudent);
        if (result != NULL)
            ABORT(result);

        //  Consume the remaining character from previous input.
        while (getchar() != '\n')
        {
        };

        //  Ask user if he/she wants to continue
        printf("Continue? [Y/N]: ");
        char choice = getchar();
        if (!(choice == 'y' || choice == 'Y'))
            break;

        //  Consume the remaining character from previous input.
        while (getchar() != '\n')
        {
        };

    } while (true);

    while (!isEmpty())
    {
        const char *result;
        //  Pop the head node of the stack
        result = pop(&myStudent);
        if (result != NULL)
            ABORT(result);
        //  Print the popped node
        result = printStudent(&myStudent);
        if (result != NULL)
            ABORT(result);
    }

#undef ABORT

    return EXIT_SUCCESS;
}