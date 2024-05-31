#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define MAX_SIZE 100
//-----------------------------------Declarations--------------------------------------------//
// Structure to represent a stack
struct Stack
{
    int top;
    int totalSize;
    char *arrayStack;
};

// Function prototypes
void initializeStack(struct Stack *stack, int size);
void destroyStack(struct Stack *stack);
int isEmpty(struct Stack *stack);
int isFull(struct Stack *stack);
char peek(struct Stack *stack);
char pop(struct Stack *stack);
void push(char element, struct Stack *stack);
int precedence(char ch);
int isOperand(char element);
char* infixToPostfix(char *expressionArray, struct Stack *stack);

//-------------------------------------------------------------------------------------------//
//-------------------------------Infix to Postfix--------------------------------------------//
void initializeStack(struct Stack *stack, int size) {
    stack->totalSize = size;
    stack->top = -1;
    stack->arrayStack = (char *)malloc(stack->totalSize * sizeof(char));
}

void destroyStack(struct Stack *stack) {
    free(stack->arrayStack);
}

int isEmpty(struct Stack *stack) {
    return stack->top == -1;
}

int isFull(struct Stack *stack) {
    return stack->top == stack->totalSize - 1;
}

char peek(struct Stack *stack) {
    if (isEmpty(stack))
        return -1;
    return stack->arrayStack[stack->top];
}

char pop(struct Stack *stack) {
    if (isEmpty(stack))
        return -1;
    return stack->arrayStack[stack->top--];
}

void push(char element, struct Stack *stack) {
    if (isFull(stack)) {
        printf("Stack is already Full.");
        return;
    }
    stack->arrayStack[++stack->top] = element;
}

int precedence(char ch) {
    switch (ch) {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    }
    return -1;
}

int isOperand(char element) {
    return (element >= 'A' && element <= 'Z') || (element >= 'a' && element <= 'z');
}
int isInteger(char element){
    return (element >= '0' && element <= '9');
}

char* infixToPostfix(char *expressionArray, struct Stack *stack) {
    initializeStack(stack, strlen(expressionArray));  // Initialize stack
    int postfixSize = strlen(expressionArray) * 2;    // Initial estimate of postfix size
    char *postfix = (char *)malloc(postfixSize * sizeof(char)); // Allocate memory for postfix expression
    int postfixIndex = 0; // Index for postfix expression
    int currIndex = 0; 

    while (expressionArray[currIndex] != '\0') {
        if (isOperand(expressionArray[currIndex]) || isInteger(expressionArray[currIndex])) {
            while (isOperand(expressionArray[currIndex]) || isInteger(expressionArray[currIndex])) {
                postfix[postfixIndex++] = expressionArray[currIndex++];
            }
            postfix[postfixIndex++] = ' ';    // Insert space after each operand
        } else if (expressionArray[currIndex] == '(') {
            push(expressionArray[currIndex], stack);
            currIndex++;
        } else if (expressionArray[currIndex] == ')') {
            while (peek(stack) != '('){
                postfix[postfixIndex++] = pop(stack);
                postfix[postfixIndex++]=' ';
            }
                
            pop(stack); // Pop '('
            currIndex++;
        } else {
            while (!isEmpty(stack) && precedence(peek(stack)) >= precedence(expressionArray[currIndex]))
                postfix[postfixIndex++]= pop(stack);
                postfix[postfixIndex++]=' ';
            push(expressionArray[currIndex], stack);            
            currIndex++;
        }
    }

    // Pop remaining operators from stack
    while (!isEmpty(stack)){
         postfix[postfixIndex++] = pop(stack);
         postfix[postfixIndex++]=' ';
    }
       
    postfix[postfixIndex] = '\0'; // Null-terminate the string
    destroyStack(stack); // Deallocate memory used by stack
    return postfix;
}

//for evaluation 
typedef struct {
    int top;
    double stack[MAX_SIZE];
} stack2;

void displayStack(stack2* s){    //used for debugging 
    int i=0;
    while(s->top>i){
        printf("%.2lf ",s->stack[i] ); // Changed %f to %lf
        i++;
    }
    printf("stack ended\n");
}
void initializeStack2(stack2 *s) {
    s->top = -1;
}

void push2(stack2 *s, double item) { 
    if (s->top >= MAX_SIZE - 1) {
        printf("Stack Overflow\n");
        exit(EXIT_FAILURE);
    }
    s->top++;
    s->stack[s->top] = item;
}

double pop2(stack2 *s) { 
    if (s->top < 0) {
        printf("Stack Underflow\n");
        exit(EXIT_FAILURE);
    }
    double item = s->stack[s->top];
    s->top--;
    return item;
}

int is_operator(char symbol) {
    return (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/'|| symbol == '^');
}


double evaluate(char* expression, stack2 *s, double* x) { 
    double operand1, operand2, result;
    char *token = strtok(expression, " ");
    while (token != '\0') {
       if (isdigit(*token)) {
            // If it's a number, push it onto the stack
            push2(s, atof(token));
        } else if (isalpha(*token)) {
            // If it's, push the value of x onto the stack
            push2(s, *x);
        } else {
            // If it's an operator, pop operands from the stack, perform the operation, and push the result onto the stack
            operand2 = pop2(s);
            operand1 = pop2(s);
            switch(*token) {
                case '+': result = operand1 + operand2; break;
                case '-': result = operand1 - operand2; break;
                case '*': result = operand1 * operand2; break;
                case '/': if (operand2 == 0) {
                               printf("Division by zero error\n");
                               exit(1);
                          } result = operand1 / operand2; break;
                case '^': result = pow(operand1, operand2); break; 
            }
            push2(s, result);
        }
        token = strtok(NULL, " ");
    }
	 return result; 
}

//-------------------------------------------------------------------------------------------//

int main() {
    char expressionArray[40];
    struct Stack stack;
    stack2 stack2;
    double x;
    // Take Input of Infix Expression
    printf("Enter an Infix Expression: ");
    scanf("%s", expressionArray);
    printf("Entered Infix Expression: %s\n", expressionArray);

    // Call the function
    char *postfix = infixToPostfix(expressionArray, &stack);
    printf("Postfix Expression: %s\n", postfix);

    // Take input of x value
    printf("Enter the value of x: ");
    scanf("%lf", &x); // Changed format specifier to %lf for double

    // Initialize Struct2 stack
    initializeStack2(&stack2);

    // verilen x degeriyle fonksiyonun degerini hesapla
    double result = evaluate(postfix, &stack2, &x);
    printf("Result= %.2lf\n", result); 

    free(postfix);

    return 0;
}

