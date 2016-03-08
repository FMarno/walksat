#ifndef LIST_H
#define LIST_H

typedef struct ListNodeName {
	int value;
	struct ListNodeName * next;	
} ListNode;

typedef struct {
	int length;
	ListNode * root;
} List;

List* createList();

void add(List* l, int value);
void addNode(ListNode* node, int value);

void removeValue(List* l, int value);
int removeNode(ListNode* node, int value);

int contains(List* l, int value);
int nodeContains(ListNode* node, int value);

int lengthOfList(List* l);

void printList(List* l);
void printNode(ListNode* n);
void freeList(List* l);
void freeNode(ListNode* node);


#endif