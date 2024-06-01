#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <math.h>
#include <ctype.h>
#define EPSILON 1e-10
#define MAX_SIZE 50
#define MAX_ITER 150
void menu(int *yontem);
void degerAl(double* a, double* b, char* fonksiyon, double* hata );
void NRdegerAl(double* a, char* fonksiyon, double* hata );
void integralDegerAl(double* a, double* b,int* n, char* fonksiyon);
//matris i�in fonksiyonlar
void getInputMatrix(double **matrix, int n, int m);// matrisin elemanlar�n� almak i�in fonksiyon 
void birimMatris(double **matrix, int n);// birimMatris olu�turmak i�in fonksiyon
void printMatrix(double **matrix, int n, int m);
void swapRows(double **matrix, int row1, int row2, int n);
void multiplyRow(double **matrix, int row, double scalar, int n);
void addMultipleOfRow(double **matrix, int sourceRow, int destinationRow, double multiple, int n);
double** findMaxRow(double **mat,int n);// Gauss Seidelde max katsay�lar� olanlar� k��egene almak i�in
double** yerAc(int n, int m);
double **matrisTersi(double **matrix, int n);
void freeMemory(double **matrix, int m);
//bittiler
void sonucYazdir(double*cevap, int n);
void gaussEliminasyon(double **matrix, int n, double*cevap);
int findMax(double a, double b);
void gaussSeidel(double **mat, int n, double epsilon);
struct Data { //structto keep inputs (x and y values)
    double x;
    double y;
};
// Function to get inputs (x and y values)
void getInputsforGN(struct Data data[], int *n, double * xi);
// Farklari hesaplar
double calculateDifference(struct Data data[], int n, int i, int j);
void gregoryNewton(struct Data data[], int n, double xi);
/////infix to postfix and evaluation icin 
struct Stack // Structure to represent a stack
{
    int top;
    int totalSize;
    char *arrayStack;
};
// Function prototypes for infixtopostfix operations
void initializeStack(struct Stack *stack, int size);
void destroyStack(struct Stack *stack);
int isEmpty(struct Stack *stack);
int isFull(struct Stack *stack);
char peek(struct Stack *stack);
char pop(struct Stack *stack);
void push(char element, struct Stack *stack);
int precedence(char ch);
int isOperand(char element);
int isInteger(char element);
char* infixToPostfix(char *expressionArray, struct Stack *stack);
//for evaluation 
typedef struct {
    int top;
    int totalSize;
    double *stack;
} stack2;
void initializeStack2(stack2 *s, int size);
void push2(stack2 *s, double item);
double pop2(stack2 *s);
int is_operator(char symbol);
double evaluate(char* expression, double x);
double evaluateExpression(char* postfix, double x);
double regulaFalsi(char* postfix, double a, double b, double hata);
double bisectionMethod(char* postfix, double a, double b, double hata); 
void getInputforST(char *fonksiyon, double *x, double *h, int *method);
double SayisalTurev(char *postfix, double x, double h, int method);
double Trapez(char *postfix, double a,double b, double n);
double simpson_1(char* postfix, double a,double b, int n);
double simpson_2(char* postfix, double a,double b, int n);
double newtonRaphson(char* postfix, double a, double hata);

int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "turkish");
	int yontem;
	int i,n,j, simpson; // i ve j for d�ng�leri i�in n matris boyutu ve integral i�in par�a say�s�
	double a,b; // integral aral�klar� || k�k�n bulundu�u aral�k
	double hata, xi;
	char fonksiyon[MAX_SIZE];
	char* postfix;
	struct Data data[MAX_SIZE];// GregoryNewton i�in de�erleri tuttu�umuz veri yap�s�
	struct Stack stack;
	double x,h,turev,root;	// say�sal t�rev hesaplarken kullan�lan de�i�kenler
	int fark;
	menu(&yontem);
	switch(yontem) {
    case 1:
        // Bisection Y�ntemi
    degerAl(&a,&b,fonksiyon,&hata);
    postfix = infixToPostfix(fonksiyon, &stack);
    root= bisectionMethod(postfix,a,b,hata);
    printf("Bulunan yakla��k k�k de�eri= %.4lf", root);
    free(postfix);
        break;
    case 2:
        // Regula-Falsi Y�ntemi
        degerAl(&a,&b,fonksiyon, &hata);
        postfix = infixToPostfix(fonksiyon, &stack);
	root=regulaFalsi(postfix, a, b, hata);
 	printf("Bulunan yakla��k k�k de�eri= %.4lf", root);
 	free(postfix);
        break;
    case 3:
        // Newton-Rapshon Y�ntemi
        NRdegerAl(&a,fonksiyon,&hata);
        postfix = infixToPostfix(fonksiyon, &stack);
        root= newtonRaphson(postfix,a,hata);
        printf("Bulunan yakla��k k�k de�eri= %.4lf", root);
        break;
    case 4:
        // NxN'lik Bir Matrisin Tersini Bulma
        printf("Matris boyutunu girin: ");
    	scanf("%d", &n);
    // n x n boyutunda iki matris i�in memory allocation
    double **matrix= yerAc(n,n);
    double **inverse= yerAc(n,n);
    getInputMatrix(matrix, n,n);
    birimMatris(inverse, n);
    printf("Verilen Matris:\n");
	printMatrix(matrix, n,n);
	inverse=matrisTersi(matrix,n);
    printf("Matrisin Tersi:\n");
    printMatrix(inverse,n,n);
	freeMemory(matrix,n);
	freeMemory(inverse,n);
        break;
    case 5:
        // Gauss Eliminasyon Y�ntemi
        printf("Ka� tane deklem girece�inizi yaz�n�z.\n");
        scanf("%d",&n);
        double **GEmatrix= yerAc(n,n+1);
        double* cevap= calloc(n, sizeof(double));
        getInputMatrix(GEmatrix, n,n+1);
        gaussEliminasyon(GEmatrix, n, cevap);
        freeMemory(GEmatrix,n+1);
        break;
    case 6:
        // Gauss Seidal Y�ntemi
        printf("Ka� tane deklem girece�inizi yaz�n�z.\n");
        scanf("%d",&n);
        double **GSmatrix= yerAc(n,n+1);
  	 	printf(" Denklem katsay�lar�n� ve fonksiyonun e�itlendi�i de�eri s�ras�yla giriniz:\n\n");
  	 	printf("Matrisin [][%d]. eleman� �ok de�i�kenli fonksiyonun e�itlendi�i de�erdir.\n",n+1);
  	 	getInputMatrix(GSmatrix, n, n+1);
  	 	GSmatrix=findMaxRow(GSmatrix,n);
  	 	printMatrix(GSmatrix,n, n+1);
		double epsilon;
    	printf("Hata de�erini girin: ");
    	scanf("%lf", &epsilon);
	    gaussSeidel(GSmatrix, n, epsilon);
	    freeMemory(GSmatrix,n+1);
	    
        break;
    case 7:
    getInputforST(fonksiyon, &x, &h, &fark);
    postfix = infixToPostfix(fonksiyon, &stack);
    turev = SayisalTurev(postfix, x, h, fark);
	printf("%.4lf de�eri i�in t�rev %.4lf bulundu.\n", x, turev);  
	    break;
    case 8:
    	// Simpson Y�ntemi
    	integralDegerAl(&a,&b,&n,fonksiyon);
		postfix = infixToPostfix(fonksiyon, &stack);
		printf("1/3(1. se�enek) ya da 3/8(2. se�enek) y�ntemini se�iniz:");
		scanf("%d", &simpson);
		if(simpson==1){
		root=simpson_1(postfix, a,b,n);
		}else{
		root=simpson_2(postfix, a,b,n);
		} printf("�ntegral de�eri %lf olarak bulundu.\n", root);
		
        break;
    case 9:
        // Trapez Y�ntemi
    	integralDegerAl(&a,&b,&n,fonksiyon);
		postfix = infixToPostfix(fonksiyon, &stack);
		root=Trapez(postfix, a, b, n);
		printf("�ntegral de�eri %lf olarak bulundu.\n", root);
        break;
    case 10:
        // De�i�ken d�n���ms�z Gregory Newton Enterpolasyonu
        getInputsforGN(data, &n, &xi);      
        gregoryNewton(data, n, xi);
        break;
    default:
        printf("Ge�ersiz y�ntem numaras�\n");
}
	return 0;
}

void initializeStack(struct Stack *stack, int size) {  		//infixtopostfix i�in
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
    initializeStack(stack, strlen(expressionArray));   // Initialize stack
    int postfixSize = strlen(expressionArray) * 2;    // Initial estimate of postfix size
    char *postfix = (char *)malloc(postfixSize * sizeof(char)); // Allocate memory for postfix expression
    int postfixIndex = 0; // Index for postfix expression
    int currIndex = 0; 

    while (expressionArray[currIndex] != '\0') {
        if (isOperand(expressionArray[currIndex]) || isInteger(expressionArray[currIndex])){
            postfix[postfixIndex++] = expressionArray[currIndex++];
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


void initializeStack2(stack2 *s, int size) {	//evaluation i�in
    s->top = -1;
    s->totalSize = size;
    s->stack = (double *)calloc(s->totalSize, sizeof(double));
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

double evaluate(char* expression, double x) { 
    stack2 s;
    initializeStack2(&s, strlen(expression));
    double operand1, operand2, result;
    char *token = strtok(expression, " ");
    while (token!= NULL) {
        if (isdigit(*token)) {
            // If it's a number, push it onto the stack
            push2(&s, atof(token));
        } else if (*token == 'x') {
            // If it's 'x', push the value of x onto the stack
            push2(&s, x);
        } else if(*token == 'e'){// if it is e  push the yakla��k de�er 
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
    char* postfix_copy = strdup(postfix); // Make a copy of postfix cuz strtok ruins the string
    if (!postfix_copy) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    double result = evaluate(postfix_copy, x);
    free(postfix_copy); // Free the copied postfix expression
    return result;
}
//y�ntemler
double regulaFalsi(char* postfix, double a, double b, double hata){
	double c;
	 int iter = 0;
	 if (evaluateExpression(postfix, b) * evaluateExpression(postfix, a) > 0) {
        printf("Ba�lang�� de�erleri uygun de�il. f(a) ve f(b) ayn� i�aretli olmamal�d�r.\n");
        return NAN; // Not-A-Number (NaN) de�eri d�nd�rme
    }
    double fa=1;
   	double fc,fb=1;
     while ((b - a) / pow(2, iter) >= hata && (fa*fb )!=0){
     	fa=evaluateExpression(postfix,a);
   		fb=evaluateExpression(postfix,b);
      	c= ((b*fa)-(a*fb))/ (fa-fb);
   		fc=evaluateExpression(postfix,c);
     	 if(fa*fc<0){
     	 	b=c;
		  }else {
		  	a=c;
		  }
		iter++;
	 }
    if((fa*fb)!=0){
    	return c;
	}else if(fa==0){
		return a;
	}else return b;
}
double bisectionMethod(char* postfix, double a, double b, double hata){
    double c;    // orta nokta 
    int iter = 0;
    if (evaluateExpression(postfix, b) * evaluateExpression(postfix, a) > 0) {
        printf("Ba�lang�� de�erleri uygun de�il. f(a) ve f(b) ayn� i�aretli olmamal�d�r.\n");
        return NAN; // Not-A-Number (NaN) de�eri d�nd�rme
    }

    while (((b - a) / pow(2, iter) >= hata ) && iter<MAX_ITER) { //durma kosulu
        c = (a + b) / 2.0;		//orta nokta say�l�r
        double c_degeri = evaluateExpression(postfix, c);
        printf("\nSinirlarimiz : [%lf, %lf]", a, b);
        printf("\nOrta Noktamiz : %lf\n", c);
        printf("Hata=%lf\n", (b - a) / pow(2, iter));
        
        if (c_degeri == 0.0) {
            printf("K�k bulundu.\n");
            return c;
        } else if (evaluateExpression(postfix, b) * c_degeri > 0) {
            b = c;
        } else {
            a = c;
        }
        iter++;
    }
    printf("�terasyon sayisi: %d\n", iter); 
    return c;
}
double newtonRaphson(char* postfix, double a, double hata){
	double x = a;
    double fx, dfx;
    double x1;
    int flag=1;
    while (flag) {
    fx = evaluateExpression(postfix, x);
    double h = 1e-7; // k���k bir sayi
    dfx = SayisalTurev(postfix,x,h,2);    // merkezi fark hesabi
        x1 = x - fx / dfx;			 	 // formul uygulamas�
        if (fabs(x1 - x) < hata) {		//durma kosulu
            flag=0;
        }
     x = x1;
    }
    free(postfix); 
    return x1;
}
void getInputforST(char *fonksiyon, double *x, double *h, int *method) {
    printf("T�revini bulmak istedi�iniz fonksiyonu ve de�erlerini giriniz:\n");  
    printf("T�revini bulmak istedi�iniz fonksiyonu giriniz\n");
    scanf("%s", fonksiyon);
    getchar(); //  scanf sonraki ad�mda do�ru �al��abilsin diye
    
    printf("T�revini bulmak istedi�iniz de�erini giriniz:");
    scanf("%lf", x);
    getchar(); //  scanf sonraki ad�mda do�ru �al��abilsin diye
    
    printf("h de�erini giriniz: ");
    scanf("%lf", h);
    getchar(); //  scanf sonraki ad�mda do�ru �al��abilsin diye
    
    printf("Hangi y�ntemi kullanaca��n�z� se�iniz.\n");
    printf("1. �leri farkla\n");
    printf("2. Geri farkla\n");
    printf("3. Merkezi farkla\n");
    scanf("%d", method);
    getchar(); // scanf sonraki ad�mda do�ru �al��abilsin diye
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
            printf("Ge�ersiz se�enek. Varsay�lan olarak merkezi fark kullan�lacak.\n");
            turev = (evaluateExpression(postfix, x + h) - evaluateExpression(postfix, x - h)) / (2 * h);
            break;
    }
    free(postfix);
    return turev;
}
double simpson_1 (char* postfix, double a, double b,int n){
	double h= (b-a)/n;
	double c=a; int i;
	double fc, fa= evaluateExpression(postfix, a);
	double fb= evaluateExpression(postfix,b);
	double sum=fa+fb;
	for(i=1;i<n;i++){
		c+=h;
		fc=evaluateExpression(postfix,c);
		if(i%2==1){
			fc*=4;
		}else{
			fc*=2;
		}
		sum+=fc;	
	}
	sum*= (h/3);
	 free(postfix); 
	return sum;	
}
double simpson_2 (char* postfix, double a, double b,int n){
	double h=(b-a)/n; 
	double h2=h/3;		
	int i,j;
	double a1=a;
	double b1,c;
	double sum1=0.0,sum=0.0;
	for(i=0;i<n;i++){
		a1=a1+ i*h;	
		b1=a1+h;		
	double fa= evaluateExpression(postfix, a1);		
	double fc= evaluateExpression(postfix, a1+h2);	
	double fd= evaluateExpression(postfix, b1-h2);	
	double fb= evaluateExpression(postfix,b1);		
	 sum=fa+3*fc+3*fd+fb;  
	 sum1+=sum;
	}
	sum1= sum1*(h/8);
	free(postfix); 
	return sum1;	
	
}
void menu(int *yontem){
	printf("Kullanaca��n�z y�ntemi se�iniz.\n");
	printf("1.Bisection Y�ntemi\n");
	printf("2.Regula-Falsi Y�ntemi\n");
	printf("3.Newton-Rapshon Y�ntemi\n");
	printf("4.NxN'lik Bir Matrisin Tersini Bulma\n");
	printf("5.Gauss Eliminasyon Y�ntemi\n");
	printf("6.Gauss Seidal Y�ntemi\n");
	printf("7.Say�sal T�rev Y�ntemi\n");
	printf("8.Simpson  Y�ntemi\n");
	printf("9.Trapez Y�ntemi\n");
	printf("10.De�i�ken d�n���ms�z Gregory Newton Enterpolasyonu\n");
	printf("Y�ntem:");
	scanf("%d",&(*yontem));
}
void degerAl(double* a, double* b, char* fonksiyon, double* hata ){
  printf("K�k bulmak istediiniz fonksiyonu giriniz.\n");
        scanf("%s",fonksiyon);       
        	printf("K�k�n bulundu�u aral���n de�erlerini giriniz.\n");
        	scanf("%lf %lf", a, b);
        	printf("Hata de�erini giriniz:");
        	scanf("%lf", hata);
}
void NRdegerAl(double* a, char* fonksiyon, double* hata ){
  printf("K�k bulmak istedi�iniz fonksiyonu giriniz.\n");
        	scanf("%s",fonksiyon);       
        	printf("Ba�lang�� de�erini giriniz.\n");
        	scanf("%lf", a);
        	getchar();
        	printf("Hata de�erini giriniz:");
        	scanf("%lf", hata);
	
}
void integralDegerAl(double* a, double* b,int* n, char* fonksiyon){
		printf("�ntegralini almak istedi�iniz fonksiyonu giriniz.\n");
    	scanf("%s", fonksiyon);
    	printf("�ntegralini almak istedi�iniz aral��� (a ve b olarak)giriniz .\n");
    	scanf("%lf %lf", a, b);
    	printf("Ka� par�ayla hesap yapmak istedi�inizi giriniz.\nSimpson 1/3 i�in �ift say� olmal�d�r.\n");
		scanf("%d", n);
}
//matris i�in fonksiyonlar
void getInputMatrix(double **matrix, int n, int m) {
    int i,j;
	printf("Matrisin elemanlar�n� giriniz:\n");
    for ( i = 0; i < n; i++) {
        for ( j = 0; j < m; j++) {
            printf("[%d][%d]eleman� giriniz: ", i+1, j+1);
            scanf("%lf", &matrix[i][j]);
        }
    }
}// matrisin elemanlar�n� almak i�in fonksiyon 
void birimMatris(double **matrix, int n){
	int i,j;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(i==j){		//k��egen mi kontrol�
				*(*(matrix+i)+j)=1;
			}else *(*(matrix+i)+j)=0;
		}
	}
}// birimMatris olu�turmak i�in fonksiyon
void printMatrix(double **matrix, int n, int m){
		int i,j;
	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			printf("%lf ", matrix[i][j]);
		}printf("\n");
}
}
void swapRows(double **matrix, int row1, int row2, int n) {
    int i;
	for ( i = 0; i < n; i++) {
        double temp = matrix[row1][i];
        matrix[row1][i] = matrix[row2][i];
        matrix[row2][i] = temp;
    }
}
void multiplyRow(double **matrix, int row, double scalar, int n) {
	int i;
    for ( i = 0; i < n; i++)
        matrix[row][i] *= scalar;
}
void addMultipleOfRow(double **matrix, int sourceRow, int destinationRow, double multiple, int n) {
   int i;
    for ( i = 0; i < n; i++) 
        matrix[destinationRow][i] += multiple * matrix[sourceRow][i]; 
}
double** findMaxRow(double **mat,int n){
	int i,j;
	double temp;
	double max; 
	for(i=0;i<n;i++){
		max=mat[i][i];
		int row=i;
		for(j=i+1;j<n;j++){
			temp=mat[j][i];
			if (findMax(temp,max)){
				row=j;}
		} swapRows(mat, i, row, n+1);
	}
	return mat;
}// Gauss Seidelde max katsay�lar� olanlar� k��egene almak i�in
double** yerAc(int n, int m){
	int i;
	double **matrix = (double **)calloc(n ,sizeof(double *));
    if (matrix == NULL) {
        printf("Bellek tahsis edilemedi.\n");
        exit(1);
    }
    for ( i = 0; i < n; i++) {
        matrix[i] = (double *)calloc(m ,sizeof(double));
        if (matrix[i] == NULL) {
            printf("Bellek tahsis edilemedi.\n");
            exit(1);
        }
    }
    return matrix;
}
double **matrisTersi(double **matrix, int n) {
    //birim matris i�in yer a�ma ve olusturma
    double **inverseMatrix = yerAc(n,n);
    birimMatris(inverseMatrix, n);
    // Girilen matrisi birim matrise cevirmek icin elementer satir islemleri kullanma
    int singular = 0;	// tersi var mi diye kontrol etmek icin
    int i = 0;
    while (i < n && !singular) {
        // Find the pivot
        int foundPivot = 0;
        int j = i;
        while (j < n && matrix[j][i] == 0) {// sat�rda 0 olmayan ilk eleman� buluyoruz
            j++;
        }
        if (j < n) {	//satirdaki hepsi 0 ise else statementa gider sing�ler matristir
            if (j != i) {		// satirda 0 olmayan ilk elemani bulduk
                swapRows(matrix, i, j, n);		  // birim matrise benzesin diye satir degisiyor
                swapRows(inverseMatrix, i, j, n);// ayni islemi hem verilen matrise hem de birim matrise yapariz
            }
            // kosegendeki eleman� 1 yapal�m
            double scalar = 1.0f / matrix[i][i];
            multiplyRow(matrix, i, scalar, n);
            multiplyRow(inverseMatrix, i, scalar, n);
            for (j = 0; j < n; j++) {  // K��egenin alt�nda ve �st�nde kalan t�m elemanlar 0'A e�itlenir
                if (j != i) {
                    double multiple = -matrix[j][i];
                    addMultipleOfRow(matrix, i, j, multiple, n);
                    addMultipleOfRow(inverseMatrix, i, j, multiple, n);
                }
            }
        } else { //satirdaki hepsi 0 ise matrisin determinant 0 olur tersi yoktur
            singular = 1;
        }
        i++;
    }
    if (singular) {
        printf("Matris sing�lerdir, tersi yoktur.\n");
        return NULL;
    }
    return inverseMatrix;
}
void freeMemory(double **matrix, int m){
            int j;
         for ( j = 0; j < m; j++) {
                free(matrix[j]);
            }
            free(matrix);
}
//bittiler
void sonucYazdir(double*cevap, int n){	// GaussEliminasyon ve gaussSeidel i�in sonu� yazd�rma fonksiyonu
	int i;
	printf("��z�m:\n");
	for(i=0;i<n;i++){
		printf("x%d: %.3f\n", i+1, cevap[i]);
	}
}
void gaussEliminasyon(double **matrix, int n, double*cevap) {
    // Girilen matrisi ust ucgen matrise cevirmek icin elementer satir islemleri kullanma
    int i = 0,j;
    while (i < n) {  
            // kosegendeki eleman� 1 yapal�m
            double scalar = 1.0f / matrix[i][i];
            multiplyRow(matrix, i, scalar, n+1);
            // Eliminate non-zero elements below the diagonal
            for (j = i+1; j < n; j++) {
                if (matrix[j][i]!=0) {
                    double multiple = (-1)*matrix[j][i];
                    addMultipleOfRow(matrix, i, j, multiple, n+1);
                }
            }
        i++;
    }
    printMatrix(matrix, n, n+1);
     for (i = n - 1; i >= 0; i--) {
        cevap[i] = matrix[i][n];
        for (j = i + 1; j < n; j++) {
            cevap[i] -= matrix[i][j] * cevap[j];
        }

    }
    sonucYazdir(cevap, n);
}
int findMax(double a, double b){          
	if(a>b)
	return 1;
	return 0;
}
void gaussSeidel(double **mat, int n, double epsilon) {
    double *x = (double *)malloc(n * sizeof(double));
    double *prevX = (double *)malloc(n * sizeof(double));
	int i,j;
    printf("Ba�lang�� de�erlerini girin:\n");
    for ( i = 0; i < n; i++) {
        printf("x%d: ", i + 1);
        scanf("%lf", &x[i]);
        prevX[i] = x[i];
    }
    double error;
    int iter = 0;
    do {
        for ( i = 0; i < n; i++) {
            prevX[i] = x[i];		// hata hesab� i�in eski de�erleri sakl�yoruz
        }
        for ( i = 0; i < n; i++) {
            double sum = 0.0;
            for ( j = 0; j < n; j++) {
                if (j != i) {
                    sum += mat[i][j] * x[j];	// x1 hesaplan�rkenmesela a2*x2+a3*x3 hesab� yap�l�yor
                }
            }
            x[i] = (mat[i][n] - sum) / mat[i][i];// bulunan toplam istenilen xnin katsay�s�na b�l�n�yor xn hesab� bitiyor
        }
        error = 0.0;
        for ( i = 0; i < n; i++) {
            error += fabs(x[i] - prevX[i]); //t�m xnler i�in hata oran� toplan�r
        }
        iter++;

        printf("�terasyon %d, Hata = %lf\n", iter, error); // Her iterasyon i�in hata de�erini yazd�r
    } while (error > epsilon && iter < MAX_ITER); 

    if (iter == MAX_ITER) {
        printf("Yeterli yak�nsama sa�lanamad�.\n");
    }else{
        printf("Gauss-Seidel y�ntemiyle %d iterasyonda sonuca ula��ld�.\n", iter);
        sonucYazdir(x,n);
    }
    free(x);
    free(prevX);
}
double Trapez(char *postfix, double a,double b, double n){
	double h= (b-a)/n;
	int i;
	double fa= evaluateExpression(postfix,a);
	double fb= evaluateExpression(postfix,b);
	double sum=fa+fb;
	double c=a;
	for(i=1;i<n;i++){
		c+=h;
		sum+= 2*(evaluateExpression(postfix,c));
	}
	sum*= (h/2);
	free(postfix);
	return sum;
}
// Function to get inputs (x and y values)
void getInputsforGN(struct Data data[], int *n, double * xi) {
    printf("Ka� tane veri girece�inizi belirtiniz: ");
    scanf("%d", n);
int i;
    printf("\nDe�erleri giriniz:\n");
    for ( i = 0; i < *n; i++) {
        printf("x[%d] = ", i);
        scanf("%lf", &data[i].x);
        printf("y[%d] = ", i);
        scanf("%lf", &data[i].y);
    }
    printf("Enterpolasyon hesaplamas� yap�lacak de�eri giriniz:");
        scanf("%lf", xi);
}
// Farklari hesaplar
double calculateDifference(struct Data data[], int n, int i, int j) {
    if (j == 0)
        return data[i].y;
    else
        return (calculateDifference(data, n, i + 1, j - 1) - calculateDifference(data, n, i, j - 1)) / (data[i + j].x - data[i].x);
}
void gregoryNewton(struct Data data[], int n, double xi) {
    double result = 0;
    int i,j;
    for (i = 0; i < n; i++) {
        double term = calculateDifference(data, n, 0, i);
        for ( j = 0; j < i; j++) {
            term *= (xi - data[j].x);
        }
        result += term;
    }
    printf("%.2lf i�in hesaplanan enterpolasyon de�eri= %.4lf", xi,result);
}
