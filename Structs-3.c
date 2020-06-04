#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "RBTree.h"
#include "Structs.h"


/**
 * check str length
 * @param str: a String its length require.
 * @return str length.
 */
unsigned int stringLength(const char * str)
{
    unsigned int count = 0;
    while ((char) str[count] != '\0')
    {
        count++;
    }
    return count;
}


/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
    return (a == NULL || b == NULL) ? false : strcmp((const char *) a, (const char *) b);
}


/**
 * ForEach function that concatenates the given word and \n to pConcatenated. pConcatenated is
 * already allocated with enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{
    if (word == NULL || pConcatenated == NULL)
    {
        return false;
    }
    strcat((char *) pConcatenated, (const char *) word);
    strcat((char *) pConcatenated, (const char *) "\n");
    return true;
}


/**
 * FreeFunc for strings
 */
void freeString(void *s)
{
    if (s != NULL)
    {
        free(s);
    }
}


/**
 * compares element by element in the vectors.
 * @param v1: a Vector object to compare to the other Vector object.
 * @param v2: a Vector object to compare to the other Vector object.
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
double compareVectors(const Vector * v1, const Vector * v2)
{
    unsigned int minimalLength = v1->len > v2->len ? v2->len : v1->len;
    for (unsigned int i = 0; i < minimalLength; ++i)
    {
        if (v1->vector[i] != v2->vector[i])
        {
            return (v1->vector[i] - v2->vector[i]);
        }
    }
    return v1->len - v2->len;
}



/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
    double result = compareVectors((Vector *) a, (Vector *) b);
    if (-1 < result && result < 1)
    {
        return result > 0 ? 1 : (result < 0 ? -1 : 0);
    }
    return (int) result;
}


/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector)
{
    free(((Vector *) pVector)->vector);
    free(pVector);
}


/**
 * calculate the square of norm of v.
 * @param v: a Vector its norm required.
 * @return the square of norm of v.
 */
long double normCalculator(Vector * v)
{
    long double sum = 0;
    for (int i = 0; i < v->len; ++i)
    {
        sum += v->vector[i] * v->vector[i];
    }
    return sum;
}


/**
 * create a copy of vector 1 in vector 2.
 * @param pVector1: a pointer to a Vector its copy required.
 * @param pVector2: a pointer to a Vector to put the the copy of vector 1 in.
 */
void deepcopyVector(const Vector *pVector1, Vector *pVector2)
{
    if (pVector1 == NULL || pVector2 == NULL)
    {
        return;
    }
    pVector2->len = pVector1->len;
    free(pVector2->vector);
    pVector2->vector = realloc(pVector1->vector, pVector1->len * sizeof(double));
}


/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector->vector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector that will hold a copy of the data of pVector.
 * @return 1 on success, 0 on failure (if pVector == NULL || pMaxVector==NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
    if (pVector == NULL || pMaxVector == NULL)
    {
        return false;
    }
    long double pVectorNorm = normCalculator((Vector *) pVector);
    long double pMaxVectorNorm = normCalculator((Vector *) pMaxVector);
    if (pVectorNorm > pMaxVectorNorm)
    {
        deepcopyVector(pVector, pMaxVector);
    }
    return true;
}


/**
 * This function allocates memory it does not free.
 * @param tree a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm), NULL on failure.
 */
Vector * findMaxNormVectorInTree(RBTree *tree)
{
    if (tree == NULL || tree->root == NULL)
    {
        return NULL;
    }
    Vector *resVector = NULL;
    resVector = (Vector *) malloc(sizeof(Vector));
    if (resVector == NULL)
    {
        return NULL;
    }
    resVector->len = 0;
    resVector->vector = NULL;
    forEachRBTree(tree, copyIfNormIsLarger, resVector);
    return resVector;
}
