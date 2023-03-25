#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int binaryToDecimal(char *binary);
char *decimalToBinary(int input);
char *takes2sComplement(char *in);
char *boothMultiply(char* n1, char* n2);
void readMultiplicationOperation(FILE* pInfile, char** pN1, char** pN2);
int shift(char *input);
void add(char *A, char *M);

int main(void) {
    printf("\nBooth's Signed Multiplication Algorithm\n");
    
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) {
        printf("Unable to find input.txt, exiting...\n");
        return 1;
    }

    int countMult;
    char *n1, *n2;

    fscanf(fp, "%d", &countMult); // scan in count of multiplications from file
    for (int i = 0; i < countMult; i++) {
        readMultiplicationOperation(fp, &n1, &n2);
    }

    fclose(fp); // close file
    
    fp = NULL;
    n1 = NULL;
    n2 = NULL;

    return 0;
}

int binaryToDecimal(char *binary) {
    int binaryLength = strlen(binary); // get how long the binary string is
    int sum = 0; // initalize the sum variable

    /* for loop logic: iterate through 0 (the first digit starting from the left) all the way
    to the ending digit, which would be binaryLength - 1, since strlen doesnt start at 0.
    then, if we see that the binary digit isnt 0, then we can use power to do 2 ^ (digit index) */
    if (binary[0] == '1') { // negative number
        char *negativeBinary = takes2sComplement(binary);

        for (int i = 0; i <= binaryLength - 1; i++) {
            if (negativeBinary[i] != '0')
                sum += pow(2, binaryLength - 1 - i);
        }
        free(negativeBinary);
        return -sum;
    }
    else {
        for (int i = 0; i <= binaryLength - 1; i++)
            if (binary[i] != '0')
                sum += pow(2, binaryLength - 1 - i);
        return sum;
    }
}

char *decimalToBinary(int input) {
    int isNegative = -1;
    if (input < 0) {
        isNegative *= -1;
        input *= -1;
    }

    // a general formula to find how many digits will be in the binary version
    // of a decimal is log2(number) + 1
    int findNumDigits = (int) log2(input) + 1;
    if ((findNumDigits + 1) % 2 != 0)
        findNumDigits++;
    
    int binaryLength = (input == 0) ? 2 : (findNumDigits + 2); // get length of the binary representation of input
    char *binary = (char *) malloc(sizeof(char) * binaryLength); // malloc a string that is the size of how long the binary will be
    for (int i = binaryLength - 2; i >= 0; i--) { // work backwards from the end of the string to the beginning
        if (input % 2 == 0)
            binary[i] = '0'; // place a 0 if the input is divisible by 2 with no remainder
        else if (input % 2 == 1)
            binary[i] = '1'; // place a 1 if the input has a remainder
        input /= 2; // divide the input by two
    }

    if (isNegative == 1)
        return takes2sComplement(binary);
    return binary; // return the binary string of the input
}

char *takes2sComplement(char *in) {
    int binaryLength, digit, carry;
    char *binary = (char *) malloc(sizeof(char) * (strlen(in) + 1));
    strcpy(binary, ""); // to stop valgrind from saying i have uninitalized values

    // take ones complement first
    digit = 0;
    while (in[digit] != '\0') {
        if (in[digit] == '1')
            strcat(binary, "0");
        else
            strcat(binary, "1");

        digit++;
    }

    binaryLength = strlen(binary);
    carry = 0;
    // add one to the binary number
    for (int i = binaryLength - 1; i >= 0; i--) {
        if (binary[i] == '0' && carry == 0) {
            binary[i] = '1';
            break;
        }
        else if (binary[i] == '0' && carry == 1) {
            binary[i] = '1';
            carry = 0;
            break;
        }
        else if (binary[i] == '1' && carry == 1) {
            binary[i] = '0';
        }
        else if (binary[i] == '1' && carry == 0) {
            binary[i] = '0';
            carry = 1; 
        }
    }
    return binary;
}

int shift(char *input) {
    int binaryLength;
    int carryoutFlag = 0;
    int isNegative = 0;

    binaryLength = strlen(input);

    if (input[binaryLength - 1] == '1') // check if we will have a carryout
        carryoutFlag = 1;

    for (int i = binaryLength - 1; i > 0; i--)
        input[i] = input[i - 1]; // shift all digits to the right

    if (carryoutFlag == 1)
        return 1;

    return 0;
}

void add(char *A, char *M) {
    int binaryLength, carry;
    binaryLength = strlen(A);
    carry = 0;

    for (int i = binaryLength - 1; i >= 0; i--) {
        if (A[i] == '0' && M[i] == '1') { // adding 0 + 1
            if (carry) {
                carry = 1;
            }
            else {
                A[i] = '1';
            }
        }
        else if (A[i] == '1' && M[i] == '1') { // adding 1 + 1
            if (carry) {
                A[i] = '1';
                carry = 1;                
            }
            else {
                A[i] = '0';
                carry = 1;
            }
        }
        else if (A[i] == '1' && M[i] == '0') { // adding 1 + 0
            if (carry) {
                A[i] = '0';
                carry = 1;
            }
            else {
                A[i] = '1';
            }
        }
        else if (A[i] == '0' && M[i] == '0') { // adding 0 + 0
            if (carry) {
                A[i] = '1';
                carry = 0;
            }
        }
    }
}

void printProduct(char *product) {
    int productLength = strlen(product);
    for (int i = 0; i < productLength; i++) {
        if (i == productLength / 2) // split the binary at the halfway point
            printf(" ");
        printf("%c", product[i]);
    }
}

char *boothMultiply(char *n1, char *n2) {
    int numofOperations, resultLength, copyUpTo;
    int q;
    char *result, *temp, *n1twoComp;
    
    // create new string that will hold
    // the eventual resultant binary
    copyUpTo = strlen(n1);
    resultLength = strlen(n1) + strlen(n2) + 1;
    result = (char *) malloc(sizeof(char) * resultLength);
    result[resultLength - 1] = '\0';

    n1twoComp = takes2sComplement(n1);

    /* some rules:
        - 01: add (A = A + M)
        - 10: subtract (A = A - M)
            (this is the same as adding but with twos complement)
        - 00/11: shift right 1
    */
    
    q = 0;
    numofOperations = strlen(n2);
    
    printf("\nMultiply %s and %s\n\n", n1, n2);

    // copy n2 into new array, and fill 
    for (int i = 0; i < resultLength; i++) {
        if (i >= numofOperations)
            result[i] = n2[i - numofOperations];
        else
            result[i] = '0';
    }

    printProduct(result);
    printf("\t%d\t%s\t%s\tPopulate Data\n", q, n1, n1twoComp);

    while (numofOperations > 0) {
        if (result[resultLength - 2] == '1' && q == 0) {
            // subtract
            temp = (char *) malloc(sizeof(char) * (copyUpTo + 1));
            temp[copyUpTo] = '\0';
            strncpy(temp, result, copyUpTo);
            add(temp, n1twoComp);
            strncpy(result, temp, copyUpTo);
            printProduct(result);
            printf("\t%d\t%s\t%s\tA = A - M\n", q, n1, n1twoComp);
            q = shift(result);
            printProduct(result);
            printf("\t%d\t%s\t%s\tShift\n", q, n1, n1twoComp);
            free(temp);
            numofOperations--;
        }
        else if (result[resultLength - 2] == '0' && q == 1) {
            // add
            temp = (char *) malloc(sizeof(char) * (copyUpTo + 1));
            temp[copyUpTo] = '\0';
            strncpy(temp, result, copyUpTo);
            add(temp, n1);
            strncpy(result, temp, copyUpTo);
            printProduct(result);
            printf("\t%d\t%s\t%s\tA = A + M\n", q, n1, n1twoComp);
            q = shift(result);
            printProduct(result);
            printf("\t%d\t%s\t%s\tShift\n", q, n1, n1twoComp);
            free(temp);
            numofOperations--;
        }
        else if ((result[resultLength - 2] == '1' && q == 1) || (result[resultLength - 2] == '0' && q == 0)) {
            // shift
            q = shift(result);
            printProduct(result);
            printf("\t%d\t%s\t%s\tShift\n", q, n1, n1twoComp);
            numofOperations--;
        }
    }
    printf("\n%s * %s = %s\n", n1, n2, result);
    printf("\n%d * %d = %d\n", binaryToDecimal(n1), binaryToDecimal(n2), binaryToDecimal(result));

    free(result);
    free(n1twoComp);
}

void readMultiplicationOperation(FILE *pInfile, char **pN1, char **pN2) {
    int numChars;

    fscanf(pInfile, "%d", &numChars); // number of digits in binary number

    // allocate space for both binary numbers
    *pN1 = (char *) malloc(sizeof(char) * (numChars + 1));
    *pN2 = (char *) malloc(sizeof(char) * (numChars + 1));

    fscanf(pInfile, " %s", *pN1); // scan in first binary number of line
    fscanf(pInfile, " %s", *pN2); // scan in second binary number of line

    boothMultiply(*pN1, *pN2);

    // free allocated memory to use for next iteration
    free(*pN1);
    free(*pN2);
}

