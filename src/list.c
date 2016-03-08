#include "list.h"
#include <stdio.h>
#include <stdlib.h>

List* createList(){
	List* base = malloc(sizeof(List));
	base->length = 0;
	base->root = NULL;
	return base;
}

void add(List* l, int value){
	if (l->root == NULL){
		l->root = malloc(sizeof(ListNode));
		l->root->value = value;
		l->root->next = NULL;
	} else {
		addNode(l->root, value);
	}
	l->length++;
}

void addNode(ListNode* node, int value){
	if (node->next == NULL){
		node->next = malloc(sizeof(ListNode));
		node->next->value = value;
		node->next->next = NULL;
	} else {
		addNode(node->next, value);
	}
}

void removeValue(List* l, int value){
	if (l->root == NULL){
		printf("List empty.\n");
	} else {
		if (l->root->value == value){
			ListNode* pt = l->root;
			l->root = l->root->next;
			free(pt);
			l->length--;
		} else {
			if (removeNode(l->root, value)){
				l->length--;
			}
		}
		
	}
}

int removeNode(ListNode* node, int value){
	if (node->next->next == NULL && node->next->value != value){
		return 0;
	}
	if (node->next->value == value){
		//remove the next node
		ListNode* pt = node->next;
		node->next = node->next->next;
		free(pt);		
	} else {
		//keep recursing
		removeNode(node->next, value);
	}
	return 1;
}

int contains(List* l, int value){
	if (l->root != NULL){
		return nodeContains(l->root, value);
	}
	return 0;
}

int nodeContains(ListNode* node, int value){
	if (node->value == value){
		return 1;
	}
	if (node->next == NULL){
		return 0;
	}
	return nodeContains(node->next, value);
}

int lengthOfList(List* l){
	return l->length;
}

void printList(List* l){
	if (l->root == NULL){
		printf("List is empty\n");
	} else {
		printNode(l->root);
	}
}

void printNode(ListNode* n){
	printf("%d", n->value);
	if (n->next != NULL){
		printf(" -> "); printNode(n->next);
	} else {
		printf("\n");
	}
}

void freeList(List* l){
	if (l->root != NULL){
		freeNode(l->root);
	}
	free(l);
}

void freeNode(ListNode* node){
	if (node->next != NULL){
		freeNode(node->next);
	}
	free(node);
}