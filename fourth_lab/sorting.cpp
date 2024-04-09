#include <iostream>
#include <omp.h>
using namespace std;

// struct for node of linked list
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

// merging two slices of linked list
void merge(struct Node** start1, struct Node** end1,
	struct Node** start2, struct Node** end2)
{

	// cheking which slice should go to the beginnig
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
			temp = bstart->next; // make step in right (countains bigger values) slice
			bstart->next = astart->next; // big_start -> next element due small_start
			astart->next = bstart; // small_start -> big_start )
			bstart = temp; // smal_start = temp (check code above)
		}
		astart = astart->next; // making next step in left slice;
	}
	if (astart == aend)
		astart->next = bstart; // small_end -> big_start 
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

			#pragma omp critical
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
	// 10000 -> 9999 -> ... -> 0
	for (int i = 10000;i > 0;i--){
		push(&head, i);
	}

	mergeSort(&head);

	print(&head);
}
