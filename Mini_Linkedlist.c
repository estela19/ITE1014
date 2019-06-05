#include<stdio.h>
#include<stdlib.h>

void print(void);
void insert(int data);
void find(int data);
void del(int data);

typedef struct Node {
	int data;
	struct Node* next;
}node;

node* Head = NULL;
node* Tail = NULL;

int main(void) {
	insert(10);
	insert(10);
	insert(20);
	insert(20);
	del(20);
	insert(30);
	insert(40);
	print();
	find(20);
	return 0;
}

void print(void) {
	int count = 0;
	node* Current = Head;
	while (Current != NULL) {
		printf("%d ->", Current->data);
		count++;
		Current = Current->next;
	}
	if (count == 0)  printf("Empty!\n");
	else printf("\nNumber of Nodes : %d\n", count);
}

void insert(int data) {
	node* New;
	New = (node*)malloc(sizeof(node));
	New->next = NULL;
	New->data = data;
	if (Head == NULL) {
		Head = New;
		Tail = New;
	}
	else {
		Tail->next = New; 
		Tail = New;
	}
}

void del(int data) {
	node* Current = Head;
	node* Pre = Head;
	while (Current != NULL) {
		if (Current->data == data) {
			Pre->next = Current->next;
			free(Current);
			break;
		}
		Pre = Current;
		Current = Current->next;
	}
}

void find(int data) {
	int find = 0;
	node* Current = Head;
	while (Current != NULL) {
		if (Current->data == data) {
			printf("%d node found\n", find);
			find++;
		}
		else {
			printf("%d node not found\n", find);
			find++;
		}
		Current = Current->next;
	}
}
