#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include<locale.h>
#define MAX_SIZE 100

// Structure to represent a stack
struct Stack
{
    int top;
    int totalSize;
    char *arrayStack;
};
//for evaluation 
typedef struct {
    int top;
    int totalSize;
    double *stack;
} stack2;
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
void initializeStack2(stack2 *s, int size);
void push2(stack2 *s, double item);
double pop2(stack2 *s);
int is_operator(char symbol);
double evaluate(char* expression, double x);
double evaluateExpression(char* postfix, double x);

void getInputforST(char *fonksiyon, double *x, double *h, int *method);
double SayisalTurev(char *postfix, double x, double h, int method);
int main() {
    setlocale(LC_ALL, "turkish");
    char fonksiyon[MAX_SIZE];
    double x, h, turev;
    int fark;
	struct Stack stack;
    getInputforST(fonksiyon, &x, &h, &fark);   
	char* postfix=infixToPostfix(fonksiyon,&stack);    
    turev = SayisalTurev(postfix, x, h, fark);
	printf("%.4lf deðeri için türev %.4lf bulundu.\n", x, turev);
    free(postfix);
    return 0;
}
double SayisalTurev(char *postfix, double x, double h, int method) {
    double turev;
    switch(method) {
        case 1:
            // ileri fark
            turev = (evaluateExpression(postfix, x + h) - evaluateExpression(postfix, x)) / h;
            break;
        case 2:
            // geri fark
            turev = (evaluateExpression(postfix, x) - evaluateExpression(postfix, x - h)) / h;
            break;
        case 3:
            // merkezi fark
            turev = (evaluateExpression(postfix, x + h) - evaluateExpression(postfix, x - h)) / (2 * h);
            break;
        default:
            printf("Geçersiz seçenek. Varsayýlan olarak merkezi fark kullanýlacak.\n");
            turev = (evaluateExpression(postfix, x + h) - evaluateExpression(postfix, x - h)) / (2 * h);
            break;
    }
    return turev;
}
void getInputforST(char *fonksiyon, double *x, double *h, int *method) {
    printf("Türevini bulmak istediðiniz fonksiyonu ve deðerlerini giriniz:\n");
    
    // Get the function name
    printf("Türevini bulmak istediðiniz fonksiyonu giriniz\n");
    scanf("%s", fonksiyon);
    getchar(); // Consume the newline character left by scanf
    
    // Get the derivative value
    printf("Türevini bulmak istediðiniz deðerini giriniz:");
    scanf("%lf", x);
    getchar(); // Consume the newline character left by scanf
    
    // Get the h value
    printf("h deðerini giriniz: ");

    scanf("%lf", h);
    getchar(); // Consume the newline character left by scanf
    
    // Select the method
    printf("Hangi yöntemi kullanacaðýnýzý seçiniz.\n");
    printf("1. Ýleri farkla\n");
    printf("2. Geri farkla\n");
    printf("3. Merkezi farkla\n");
    scanf("%d", method);
    getchar(); // Consume the newline character left by scanf
}
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
            while (!isEmpty(stack) && precedence(peek(stack)) >= precedence(expressionArray[currIndex])){
            	 postfix[postfixIndex++]= pop(stack);
                postfix[postfixIndex++]=' ';
			}
               
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
void initializeStack2(stack2 *s, int size) {
    s->top = -1;
    s->totalSize = size;
    s->stack = (double *)calloc(s->totalSize , sizeof(double));
    if (s->stack == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
}

void push2(stack2 *s, double item) {
    if (s->top >= s->totalSize - 1) {
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
    return (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/' || symbol == '^');
}

double evaluate(char* expression, double x) { // Modified to take double instead of double*
    stack2 s;
    initializeStack2(&s, strlen(expression));
    double operand1, operand2, result;
    char *token = strtok(expression, " ");
    while (token!= NULL) {
        if (isdigit(*token) || (*token == '-' && isdigit(*(token + 1)))) {
            // If it's a number, push it onto the stack
            push2(&s, atof(token));
        } else if (*token == 'x') {
            // If it's 'x', push the value of x onto the stack
            push2(&s, x);
        } else if(*token == 'e'){
        	push2(&s, 2.718281828459);
        }  else if (is_operator(*token)) {
            // If it's an operator, pop operands from the stack, perform the operation, and push the result onto the stack
            operand2 = pop2(&s);
            operand1 = pop2(&s);
            switch (*token) {
                case '+': result = operand1 + operand2; break;
                case '-': result = operand1 - operand2; break;
                case '*': result = operand1 * operand2; break;
                case '/': if (operand2 == 0) {
                                printf("Division by zero error\n");
                                exit(EXIT_FAILURE);
                            }
                            result = operand1 / operand2; break;
                case '^': result = pow(operand1, operand2); break;
                default: printf("Unknown operator: %c\n", *token); exit(EXIT_FAILURE);
            }
            push2(&s, result);
        } else {
            printf("Unknown token: %s\n", token);
            exit(EXIT_FAILURE);
        }
        token = strtok(NULL, " ");
    }
    return result;
}

double evaluateExpression(char* postfix, double x) {
    char* postfix_copy = strdup(postfix); // Make a copy of postfix because strtok modifies the original string
    if (!postfix_copy) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    double result = evaluate(postfix_copy, x); // Pass the double value directly
    free(postfix_copy); // Free the copied postfix expression
    return result;
}

