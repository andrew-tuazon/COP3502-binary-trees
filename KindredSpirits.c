// Andrew Tuazon
// an623310
// COP 3502, Spring 2020

#include <stdio.h>
#include <stdlib.h>
#include "KindredSpirits.h"
#include <limits.h>
#define EMPTY_STACK_ERR INT_MIN

// stack-related functions and struct borrowed from Stacks notes on Webcourses
typedef struct Stack
{
	int *array;
	// current array size
	int size;
	// array capacity
	int capacity;
} Stack;

Stack *createStack(int capacity)
{
	Stack *s;
	// avoid segfaulting when making stack pointer
	if ((s = malloc(sizeof(Stack))) == NULL)
		return NULL;
	// if array allocation fails return NULL
	if ((s->array = malloc(sizeof(int) * capacity)) == NULL)
	{
		free(s);
		return NULL;
	}
	//always set size to 0
	s->size = 0;
	s->capacity = capacity;
	return s;
}

// destroy stack; coded defensively to avoid segfaults
Stack *destroyStack(Stack *s)
{
	if (s == NULL)
		return NULL;
	free(s->array);
	free(s);
	return NULL;
}

// return 1 if the stack is empty, 0 otherwise
int isEmpty(Stack *s)
{
	return (s == NULL || s->size == 0);
}

// return 1 if the stack is full, 0 otherwise
int isFull(Stack *s)
{
	return (s == NULL || s->size == s->capacity);
}

// push a new element onto the top of the stack
void push(Stack *s, int data)
{
	if (isFull(s))
		return;
	// post-increment
	s->array[s->size++] = data;
}

// remove the top element from the stack and return its value
int pop(Stack *s)
{
	if (isEmpty(s))
		// return dummy value if stack is empty 
		return EMPTY_STACK_ERR;
	// pre-decrement
	return s->array[--s->size];
}

// returns the number of nodes in a (possibly empty) binary tree
int count_nodes(node *root)
{	
	// if root is NULL, it is an empty tree
	if (root == NULL)
		return 0;
	// recursively add each node
	return 1 + count_nodes(root->left) + count_nodes(root->right);
}

// recursive preorder traversal that fills a stack
void preorder_recursive(Stack *s, node *root)
{
	if (root == NULL)
		return;
	
	push(s, root->data);
	preorder_recursive(s,root->left);
	preorder_recursive(s,root->right);
}

// recursive postorder traversal that fills a stack
void postorder_recursive(Stack *s, node *root)
{
	if (root == NULL)
		return;
	
	postorder_recursive(s, root->left);
	postorder_recursive(s, root->right);
	push(s, root->data);
}

// determines if a root has children
int hasChild(node *root)
{
	// return 0 if no children
	if (root->left == NULL && root->right == NULL)
		return 0;
	// return 1 for left child
	else if (root->right == NULL)
		return 1;
	// return 2 for right child
	else if (root-> left == NULL)
		return 2;
	// return 3 for root and two children
	else 
		return 3;
}

// node creation function borrowed bst.c code in Webcourses
node *createNode(int data)
{
	// calloc() initializes the 'left' and 'right' pointers to NULL
	node *n = calloc(1, sizeof(node));
	n->data = data;
	return n;
}

// takes two roots and returns 1 if both are reflections of each other, 0 otherwise
int isReflection(node *a, node *b)
{
	// empty trees are reflections
	if (a == NULL && b == NULL)
		return 1;
	// if one root exists and the other is empty return 0
	if (a == NULL || b == NULL)
		return 0;
	// recursively check if the data at one node matches the data in the mirrored node
	return a->data == b->data && isReflection(a->left, b->right) && isReflection(b->left, a->right);
}

// returns the root of a reflection tree
node *makeReflection(node *root)
{
	node *new_root = NULL;
	
	// empty tree reflection is an empty tree
	if (root == NULL)
		return NULL;
	// the reflection of a single root is itself
	if (root->left == NULL && root->right == NULL)
		return root;
	
	new_root = createNode(root->data);
	//recursively create a mirrored version of the binary tree
	new_root->left = makeReflection(root->right);
	new_root->right = makeReflection(root->left);
	return new_root;
}

// helper function returns 1 if stacks correspond, 0 otherwise
int stacksEqual(Stack *a, Stack *b)
{
	while (!isEmpty(a))
	{
		if (pop(a)!= pop(b))\
		{
			// destroy stacks once we know they do not correspond
			destroyStack(a);
			destroyStack(b);
			return 0;
		}
	}
	return 1;
}

// returns 1 if two trees are kindred spirits, 0 otherwise
int kindredSpirits(node *a, node *b)
{
	int nodesA = count_nodes(a), nodesB = count_nodes(b);
	
	// use stacks to store traversals 
	Stack *stack1 = createStack(nodesA);
	Stack *stack2 = createStack(nodesB);
	
	// empty trees are kindred spirits
	if (a == NULL && b == NULL)
		return 1;
	
	// two nodes with the same data are kindred spirits
	if (hasChild(a) == 0 && hasChild(b) == 0)
		return a->data == b->data ? 1 : 0;
	
	// if node amounts differ the trees are not kindred spirits
	if (nodesA != nodesB)
		return 0;
	
	// preorder traverse one tree a and postorder traverse tree b
	preorder_recursive(stack1, a);
	postorder_recursive(stack2, b);
	
	// if stacks correspond they are kindred spirits
	if (stacksEqual(stack1, stack2))
	{
		// free stacks
		destroyStack(stack1);
		destroyStack(stack2);
		return 1;
	}
	
	stack1 = createStack(nodesB);
	stack2 = createStack(nodesA);
	
	// preorder traverse one tree b and postorder traverse tree a
	preorder_recursive(stack1, b);
	postorder_recursive(stack2, a);
	
	// if stacks correspond they are kindred spirits
	if (stacksEqual(stack1, stack2))
	{
		// free stacks
		destroyStack(stack1);
		destroyStack(stack2);
		return 1;
	}
}

double difficultyRating(void)
{
	return 4;
}

double hoursSpent(void)
{
	return 21.0;
}