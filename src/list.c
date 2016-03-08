#include "list.h"
#include <stdio.h>
#include <stdlib.h>

List* createList(){
	List* base = malloc(sizeof(List));
	base->length = 0;
	base->root = NULL;
	return base;
}

int get(List* l, int index){
	ListNode* node = l->root;
	for(int i = 0; i<index; i++){
		if (node->next != NULL){
			node = node->next;
		} else {
			printf("There aren't that many nodes in the list\n");
			exit(1);
		}
	}
	return node->value;

}

void add(List* l, int value){
	ListNode* node = l-> root;

	if (node == NULL){
		l->root = malloc(sizeof(ListNode));
		l->root->value = value;
		l->root->next = NULL;
	} else {
		while (node->next != NULL){
			node = node->next;
		}
		node->next = malloc(sizeof(ListNode));
		node->next->value = value;
		node->next->next = NULL;
	}
	l->length++;
}


void removeValue(List* l, int value){
	ListNode* node = l->root;
	if (node == NULL){
		printf("List empty.\n");
	} else {
		if (node->value == value){
			ListNode* pt = node;
			l->root = node->next;
			free(pt);
			l->length--;
		} else {
			while(node->next != NULL){
				if (node->next->value == value){
					ListNode* pt = node->next;
					node->next = node->next->next;
					free(pt);	
					l->length--;
					break;
				}
				node = node->next;
			}
		}
		
	}
}

int contains(List* l, int value){
	ListNode* node = l->root;
	if (node->value == value){
		return 1;
	}
	while (node->next != NULL){
		node = node->next;
		if (node->value == value){
			return 1;
		}
	}
	return 0;
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