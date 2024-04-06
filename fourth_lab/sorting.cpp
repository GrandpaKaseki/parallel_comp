#include <iostream>
#include <omp.h>
using namespace std;


struct Node {
	int data;
	struct Node* next;
};



int length(struct Node* current)
{
	int count = 0;
	while (current != NULL) {
		current = current->next;
		count++;
	}
	return count;
}


void merge(struct Node** start1, struct Node** end1,
	struct Node** start2, struct Node** end2)
{

	
	struct Node* temp = NULL;
	if ((*start1)->data > (*start2)->data) {
		swap(*start1, *start2);
		swap(*end1, *end2);
	}

	struct Node* astart = *start1, * aend = *end1;
	struct Node* bstart = *start2, * bend = *end2;
	struct Node* bendnext = (*end2)->next;
	while (astart != aend && bstart != bendnext) {
		if (astart->next->data > bstart->data) {
			temp = bstart->next;
			bstart->next = astart->next;
			astart->next = bstart;
			bstart = temp;
		}
		astart = astart->next;
	}
	if (astart == aend)
		astart->next = bstart;
	else
		*end2 = *end1;
}

void mergeSort(struct Node** head)
{
	if (*head == NULL)
		return;
	struct Node* start1 = NULL, * end1 = NULL;
	struct Node* start2 = NULL, * end2 = NULL;
	struct Node* prevend = NULL;
	int len = length(*head);
	#pragma omp parallel for
	for (int gap = 1; gap < len; gap = gap * 2) {
		start1 = *head;
		while (start1) {

			bool isFirstIter = 0;
			if (start1 == *head)
				isFirstIter = 1;

			int counter = gap;
			end1 = start1;
			while (--counter && end1->next)
				end1 = end1->next;

			start2 = end1->next;
			if (!start2)
				break;
			counter = gap;
			end2 = start2;
			while (--counter && end2->next)
				end2 = end2->next;

			
			Node* temp = end2->next;

			
			merge(&start1, &end1, &start2, &end2);

			if (isFirstIter)
				*head = start1;
			else
				prevend->next = start1;

			prevend = end2;
			start1 = temp;
		}
		prevend->next = start1;
	}
}


void print(struct Node** head)
{
	if ((*head) == NULL)
		return;
	struct Node* temp = *head;
	while (temp != NULL) {
		printf("%d ", temp->data);
		temp = temp->next;
	}
	printf("\n");
}



void push(struct Node** head_ref,
	int new_data)
{
	struct Node* new_node = new Node;
	new_node->data = new_data;
	new_node->next = (*head_ref);
	(*head_ref) = new_node;
}

int main()
{
	struct Node* head = NULL;

	// create linked list 
	// 1->2->3->4->5->6->7 
	push(&head, 9);
	push(&head, 6);
	push(&head, 5);
	push(&head, 4);
	push(&head, 13);
	push(&head, 2);
	push(&head, 1);

	mergeSort(&head);

	print(&head);
}
