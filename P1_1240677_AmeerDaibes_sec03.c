//Ameer Daibes
//1240677

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode
{
    char data[20];
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct EqNode
{
    char equation[100];
    char postfix[100];
    int result;
    struct EqNode* next;
} EqNode;

EqNode* head = NULL;
int count = 0;
char stack[100];
int top = -1;

void print_menu();
void read_input_file();
int check_validity(char equation[], char error[]);
void push(char value);
char pop();
int isEmpty();
void In2PostFix(char infix[], char postfix[]);
int evaluate_postfix(char postfix[]);
void print_invalid_equations();
TreeNode* createNode(char value[]);
TreeNode* buildExpressionTree(char postfix[]);
void inorder(TreeNode* root);
void preorder(TreeNode* root);
void postorder(TreeNode* root);
int isOperator(char c);
int priority(char op);
void print_output_file();
EqNode* getNode(int index);
int string_to_int(char token[]);

int main()
{
    print_menu();

    int option;
    scanf("%d", &option);

    while (option != 0)
    {
        switch (option)
        {
            case 1:
                printf("Selected option 1: Read Input file\n");
                read_input_file();
                break;

            case 2:
                printf("Selected option 2: Check Validity\n");
                {
                    // traverse the linked list and check validity of each equation
                    EqNode* curr = head;
                    int i = 0;
                    char error[100];

                    while (curr != NULL)
                    {
                        if (check_validity(curr->equation, error))
                            printf("Equation No. %d -> valid\n", i + 1);
                        else
                            printf("Equation No. %d -> invalid: %s\n", i + 1, error);

                        curr = curr->next;
                        i++;
                    }
                }
                break;

            case 3:
                printf("Selected option 3: Convert From infix to postfix\n");
                {
                    EqNode* curr = head;
                    int i = 0;
                    char error[100];

                    while (curr != NULL)
                    {
                        if (check_validity(curr->equation, error))
                        {
                            In2PostFix(curr->equation, curr->postfix);
                            printf("Equation No. %d postfix: %s\n", i + 1, curr->postfix);
                        }
                        else
                        {
                            printf("Equation No. %d invalid: %s\n", i + 1, error);
                        }

                        curr = curr->next;
                        i++;
                    }
                }
                break;

            case 4:
                printf("Selected option 4: Evaluate Postfix Expression\n");
                {
                    EqNode* curr = head;
                    int i = 0;
                    char error[100];

                    while (curr != NULL)
                    {
                        if (check_validity(curr->equation, error))
                        {
                            In2PostFix(curr->equation, curr->postfix);
                            curr->result = evaluate_postfix(curr->postfix);
                            printf("Equation No. %d result: %d\n", i + 1, curr->result);
                        }
                        else
                        {
                            printf("Equation No. %d invalid: %s\n", i + 1, error);
                        }

                        curr = curr->next;
                        i++;
                    }
                }
                break;

            case 5:
                printf("Selected option 5: Print Invalid Equations\n");
                print_invalid_equations();
                break;

            case 6:
            {
                printf("Selected option 6: Expression tree\n");

                int eqNum;
                char error[100];

                printf("Enter equation number: ");
                scanf("%d", &eqNum);

                EqNode* curr = getNode(eqNum - 1);

                if (eqNum < 1 || eqNum > count)
                {
                    printf("Invalid equation number.\n");
                }
                else if (!check_validity(curr->equation, error))
                {
                    printf("Equation is invalid: %s\n", error);
                }
                else
                {
                    In2PostFix(curr->equation, curr->postfix);

                    TreeNode* root;
                    root = buildExpressionTree(curr->postfix);

                    printf("Inorder: ");
                    inorder(root);

                    printf("\nPostorder: ");
                    postorder(root);

                    printf("\nPreorder: ");
                    preorder(root);

                    printf("\n");
                }

                break;
            }

            case 7:
                printf("Selected option 7: Print Output File\n");
                print_output_file();
                break;

            default:
                printf("wrong option. Please select an option (1-7).\n");
        }

        print_menu();
        scanf("%d", &option);
    }

    printf("Exiting the program. Goodbye!\n");

    return 0;
}

void print_menu()
{
    printf("Menu:\n");
    printf("1. Read Input file \n");
    printf("2. Check Validity\n");
    printf("3. Convert From infix to postfix\n");
    printf("4. Evaluate Postfix Expression\n");
    printf("5. Print Invalid Equations\n");
    printf("6. Expression tree\n");
    printf("7. Print Output File\n");
    printf("0. Exit\n");
    printf(" --Select an option (1-7):-- \n");
}

// reads equations from the input file and stores them in a linked list
void read_input_file()
{
    FILE* inputFile;
    char Fname[100];
    char temp[100];

    printf("Enter the filename to read: ");
    scanf("%s", Fname);

    inputFile = fopen(Fname, "r");

    if (inputFile == NULL)
    {
        printf("Error opening file,try again.\n");
        return;
    }

    while (fscanf(inputFile, "%s", temp) == 1)
    {
        EqNode* newNode = (EqNode*)malloc(sizeof(EqNode));

        // copy the equation read from the file into the new node
        strcpy(newNode->equation, temp);

        // initialize postfix string, result value, and next pointer
        newNode->postfix[0] = '\0';
        newNode->result = 0;
        newNode->next = NULL;

        // insert the new node at the end of the linked list
        if (head == NULL)
        {
            head = newNode;
        }
        else
        {
            EqNode* curr = head;

            while (curr->next != NULL)
            {
                curr = curr->next;
            }

            curr->next = newNode;
        }

        printf("Equation read: %s\n", temp);
        count++;
    }

    printf("Total equations read: %d\n", count);
    printf("File loaded successfully.\n");

    fclose(inputFile);
}

// return the node at the given index from the linked list
EqNode* getNode(int index)
{
    EqNode* curr = head;
    int i = 0;

    while (curr != NULL && i < index)
    {
        curr = curr->next;
        i++;
    }

    return curr;
}

// push a character onto the stack
void push(char value)
{
    top++;
    stack[top] = value;
}

// remove and return the top character from the stack
char pop()
{
    char value = stack[top];
    top--;
    return value;
}

// check if the stack is empty
int isEmpty()
{
    if (top == -1)
        return 1;

    return 0;
}

// validate the equation by checking operators, brackets, and syntax errors
int check_validity(char equation[], char error[])
{
    int i = 0;
    char prev = '\0';
    char prevprev = '\0';

    // reset stack and previous character before validation
    top = -1;

    while (equation[i] != '\0')
    {
        char c = equation[i];

        // skip spaces in the equation
        if (c == ' ')
        {
            i++;
            continue;
        }

        // return invalid if the character is not a number, operator, or bracket
        if (!((c >= '0' && c <= '9') || isOperator(c) || c == '(' || c == ')' || c == '[' || c == ']'))
        {
            sprintf(error, "invalid character %c", c);
            return 0;
        }

        // check for invalid placement of * or / operators
        if ((c == '*' || c == '/') && (prev == '\0' || prev == '(' || prev == '['))
        {
            sprintf(error, "%c is in invalid position", c);
            return 0;
        }

        // prevent invalid consecutive operators except for negative numbers
        if (isOperator(c) && isOperator(prev))
        {
            if (!(c == '-' && (prev == '+' || prev == '-' || prev == '*' || prev == '/')))
            {
                sprintf(error, "there is no operator between %c%c%c", prevprev, prev, c);   
                return 0;
            }
        }

        // prevent missing operator before opening brackets
        if ((c == '(' || c == '[') && ((prev >= '0' && prev <= '9') || prev == ')' || prev == ']'))
        {
            sprintf(error, "there is no operator before %c", c);
            return 0;
        }

        // prevent missing operator after closing brackets
        if ((c >= '0' && c <= '9') && (prev == ')' || prev == ']'))
        {
            sprintf(error, "there is no operator between %c%c%c", prevprev, prev, c);
            return 0;
        }

        // push opening brackets onto the stack
        if (c == '(' || c == '[')
            push(c);

        else if (c == ')')
        {
            // check for matching opening parenthesis
            if (isEmpty() || pop() != '(')
            {
                strcpy(error, "( is not closed");
                return 0;
            }
        }

        else if (c == ']')
        {
            // check for matching opening square bracket
            if (isEmpty() || pop() != '[')
            {
                strcpy(error, "( is not closed");
                return 0;
            }
        }

        // update previous character and move to next character
        prevprev = prev;
        prev = c;
        i++;
    }

    // ensure the equation does not end with an operator
    if (isOperator(prev))
    {
        strcpy(error, "equation ends with an operator");
        return 0;
    }

    // ensure all brackets are closed
    if (!isEmpty())
    {
        if (stack[top] == '(')
            strcpy(error, "( is not closed");
        else if (stack[top] == '[')
            strcpy(error, "[ is not closed");

        return 0;
    }

    strcpy(error, "valid");
    return 1;
}

// convert a valid infix equation to postfix notation using a stack
void In2PostFix(char infix[], char postfix[])
{
    int i = 0;
    int j = 0;
    top = -1;

    // loop through the infix expression
    while (infix[i] != '\0')
    {
        // skip spaces in the expression
        if (infix[i] == ' ')
        {
            i++;
        }

        // copy numbers, including negative numbers, to the postfix expression
        else if ((infix[i] >= '0' && infix[i] <= '9') ||
                (infix[i] == '-' && infix[i + 1] >= '0' && infix[i + 1] <= '9' &&
                (i == 0 || isOperator(infix[i - 1]) || infix[i - 1] == '(' || infix[i - 1] == '[')))
        {
            if (infix[i] == '-')
            {
                // add the current character to the postfix expression
                postfix[j] = infix[i];
                j++;
                i++;
            }

            // copy all digits of the number to the postfix expression
            while (infix[i] >= '0' && infix[i] <= '9')
            {
                postfix[j] = infix[i];
                j++;
                i++;
            }

            // add a space after the number in postfix notation
            postfix[j] = ' ';
            j++;
        }

        // push opening brackets onto the stack
        else if (infix[i] == '(' || infix[i] == '[')
        {
            push(infix[i]);
            i++;
        }

        // pop operators from the stack until the matching opening bracket is found
        else if (infix[i] == ')' || infix[i] == ']')
        {
            while (!isEmpty() && stack[top] != '(' && stack[top] != '[')
            {
                postfix[j] = pop();
                j++;
                postfix[j] = ' ';
                j++;
            }

            pop();
            i++;
        }

        // pop higher or equal priority operators, then push the current operator
        else if (isOperator(infix[i]))
        {
            while (!isEmpty() && priority(stack[top]) >= priority(infix[i]))
            {
                postfix[j] = pop();
                j++;
                postfix[j] = ' ';
                j++;
            }

            push(infix[i]);
            i++;
        }
        else
        {
            i++;
        }
    }

    while (!isEmpty())
    {
        postfix[j] = pop();
        j++;
        postfix[j] = ' ';
        j++;
    }

    postfix[j] = '\0';
}

// convert a string number into an integer value
int string_to_int(char token[])
{
    int i = 0;
    int sign = 1;
    int number = 0;

    if (token[i] == '-')
    {
        sign = -1;
        i++;
    }

    while (token[i] != '\0')
    {
        // build the integer value digit by digit
        number = number * 10 + (token[i] - '0');
        i++;
    }

    return number * sign;
}

// evaluate the postfix expression using a number stack
int evaluate_postfix(char postfix[])
{
    int numStack[100];
    int numTop = -1;
    int i = 0;
    int j;
    char token[20];

    while (postfix[i] != '\0')
    {
        if (postfix[i] == ' ')
        {
            i++;
        }

        // read numbers from the postfix expression and push them onto the stack
        else if ((postfix[i] >= '0' && postfix[i] <= '9') ||
                (postfix[i] == '-' && postfix[i + 1] >= '0' && postfix[i + 1] <= '9'))
        {
            j = 0;

            if (postfix[i] == '-')
            {
                token[j] = postfix[i];
                j++;
                i++;
            }

            while (postfix[i] >= '0' && postfix[i] <= '9')
            {
                token[j] = postfix[i];
                j++;
                i++;
            }

            token[j] = '\0';
            numStack[++numTop] = string_to_int(token);
        }

        // apply the operator to the top two numbers in the stack
        else if (isOperator(postfix[i]))
        {
            int right = numStack[numTop--];
            int left = numStack[numTop--];

            switch (postfix[i])
            {
                case '+':
                    numStack[++numTop] = left + right;
                    break;

                case '-':
                    numStack[++numTop] = left - right;
                    break;

                case '*':
                    numStack[++numTop] = left * right;
                    break;

                case '/':
                    numStack[++numTop] = left / right;
                    break;
            }

            i++;
        }
        else
        {
            i++;
        }
    }

    return numStack[numTop];
}

// print all invalid equations from the linked list
void print_invalid_equations()
{
    printf("Invalid equations:\n");

    // start from the first node in the linked list
    EqNode* curr = head;
    char error[100];

    // loop through the linked list
    while (curr != NULL)
    {
        // if the equation is invalid, print it and move to the next node
        if (!check_validity(curr->equation, error))
        {
            printf("%s -> %s\n", curr->equation, error);
        }

        curr = curr->next;
    }
}

// create and initialize a new tree node
TreeNode* createNode(char value[])
{
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));

    // copy the value into the tree node
    strcpy(newNode->data, value);

    // initialize left and right child pointers
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

// build an expression tree from the postfix expression
TreeNode* buildExpressionTree(char postfix[])
{
    TreeNode* nodeStack[100];
    int nodeTop = -1;

    char token[20];
    int i = 0, j;

    while (postfix[i] != '\0')
    {
        if (postfix[i] == ' ')
        {
            i++;
        }
        else
        {
            j = 0;

            while (postfix[i] != ' ' && postfix[i] != '\0')
            {
                token[j] = postfix[i];
                j++;
                i++;
            }

            token[j] = '\0';

            // create a tree node for numbers and push it onto the stack
            if ((token[0] >= '0' && token[0] <= '9') ||
                (token[0] == '-' && token[1] >= '0' && token[1] <= '9'))
            {
                nodeStack[++nodeTop] = createNode(token);
            }

            // create an operator node and connect its left and right children
            else if (isOperator(token[0]))
            {
                TreeNode* newNode = createNode(token);

                newNode->right = nodeStack[nodeTop--];
                newNode->left = nodeStack[nodeTop--];

                nodeStack[++nodeTop] = newNode;
            }
        }
    }

    return nodeStack[nodeTop];
}

// print the expression tree in inorder form
void inorder(TreeNode* root)
{
    if (root != NULL)
    {
        if (isOperator(root->data[0]))
            printf("(");

        inorder(root->left);

        printf("%s", root->data);

        inorder(root->right);

        if (isOperator(root->data[0]))
            printf(")");
    }
}

// print the expression tree in preorder form
void preorder(TreeNode* root)
{
    if (root != NULL)
    {
        printf("%s", root->data);
        preorder(root->left);
        preorder(root->right);
    }
}

// print the expression tree in postorder form
void postorder(TreeNode* root)
{
    if (root != NULL)
    {
        postorder(root->left);
        postorder(root->right);
        printf("%s", root->data);
    }
}

// write all equations, validity status, postfix expressions, and results to output.txt
void print_output_file()
{
    FILE *outputFile;

    outputFile = fopen("output.txt", "w");

    if (outputFile == NULL)
    {
        printf("Error opening output file.\n");
        return;
    }

    // start from the first equation and initialize the counter
    EqNode* curr = head;
    int i = 0;
    char error[100];

    // loop through all equations and write their information to the output file
    while (curr != NULL)
    {
        fprintf(outputFile, "Equation No. %d: %s\n", i + 1, curr->equation);

        if (check_validity(curr->equation, error))
        {
            In2PostFix(curr->equation, curr->postfix);
            curr->result = evaluate_postfix(curr->postfix);

            fprintf(outputFile, "Status: valid\n");
            fprintf(outputFile, "Postfix: %s\n", curr->postfix);
            fprintf(outputFile, "Result: %d\n", curr->result);
        }
        else
        {
            fprintf(outputFile, "Status: invalid: %s\n", error);
        }

        fprintf(outputFile, "\n");

        curr = curr->next;
        i++;
    }

    fclose(outputFile);

    printf("Output written to a txt file successfully.\n");
}

// check if the character is an arithmetic operator
int isOperator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// return the priority level of an operator
int priority(char op)
{
    if (op == '*' || op == '/')
        return 2;

    if (op == '+' || op == '-')
        return 1;

    return 0;
}