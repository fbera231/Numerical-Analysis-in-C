#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <locale.h>
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
    stack->arrayStack = (char *)calloc(stack->totalSize , sizeof(char));
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

//for evaluation 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_SIZE 100 // Define MAX_SIZE as a constant

typedef struct {
    int top;
    int totalSize;
    double *stack; // Changed data type to double pointer
} stack2;

void displayStack(stack2* s) {
    int i = 0;
    while (s->top > i) {
        printf("%.2lf ", s->stack[i]);
        i++;
    }
    printf("stack ended\n");
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

double evaluate(char* expression,  double* x) {
    stack2 s;
	initializeStack2(&s, strlen(expression));
    double operand1, operand2, result;
    char *token = strtok(expression, " ");
    while (token != NULL) {
        if (isdigit(*token) || (*token == '-' && isdigit(*(token + 1)))) {
            // If it's a number, push it onto the stack
            push2(&s, atof(token));
        } else if (*token == 'x') {
            // If it's 'x', push the value of x onto the stack
            push2(&s, *x);
        } else if (is_operator(*token)) {
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

double evaluateExpression(char* postfix, double* x) {
    char* postfix_copy = strdup(postfix); // Make a copy of postfix
    if (!postfix_copy) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    double result = evaluate(postfix_copy, x);
    free(postfix_copy); // Free the copied postfix expression
    return result;
}



void degerAl(double* a, double* b, char* fonksiyon,double* hata ){
  printf("Kök bulmak istediiniz fonksiyonu giriniz.\n");
        scanf("%s",fonksiyon);
        
        	printf("Kökün bulunduðu aralýðý([a,b] olarak) giriniz.\n");
        	scanf("%lf %lf", a, b);
        	printf("Hata deðerini giriniz:");
        	scanf("%lf", hata);

}
    
//-------------------------------------------------------------------------------------------//

int main() {
	setlocale(LC_ALL, "turkish");
	char fonksiyon[MAX_SIZE];
    char expressionArray[40];
    struct Stack stack;
    double x;
		//	stack2 s;
			double a,b,hata,root;
	      	degerAl(&a,&b,fonksiyon, &hata);
	      	char *postfix = infixToPostfix(fonksiyon, &stack);
			printf("Postfix Expression: %s\n", postfix);

	double mid;
	double orta;
	int iterasyon=0;
	double alt=evaluateExpression(postfix, &a);
	double ust=evaluateExpression(postfix, &b);
	printf("f(a)= %.3lf f(b)=%.3lf \n", alt, ust);
	 if (alt * ust >= 0) {
        printf("f(a) and f(b) zýt iþaretli olmadýðý için Bisection metodu çalýþmaz.\n");
        return 0;
    }
   
	while ((b - a) / 2.0 > hata) {
        mid = (a + b) / 2.0;
         alt=evaluateExpression(postfix, &a);
		 ust=evaluateExpression(postfix, &b);
  		 orta=evaluateExpression(postfix,&mid);
  		 printf("%d. iterasyonda alt=%.2lf orta=%.2lf ust=%.2lf\n\n", iterasyon, alt, orta, ust);
        iterasyon++;
        if (orta == 0.0) {
            // Root found exactly
                printf("Kök=%.4lf %d. iterasyonda bulundu. \n", mid, iterasyon);

        } else if (orta * alt < 0) {
            b = mid;
        } else {
            a = mid;
        }
    }
    printf("Kök=%.4lf %d. iterasyonda bulundu. \n", mid, iterasyon);
    
    return 0;
}

