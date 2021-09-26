// Link: https://www.codesdope.com/c-introduction/

#include <iostream>
#include <stdio.h>
using namespace std;

/* Struct node contains a data that node store and a pointer to the next node */
typedef struct node
{
    int data;
    struct node *next;
} node;

/* Linked list object create only the head for that linked list to point to the node */
typedef struct linked_list
{
    struct node *head;
} linked_list;

/* Function create a new node in the heap with any data */
node *new_node(int data)
{
    node *z; // declare a pointer
    z = (node *)malloc(sizeof(struct node));

    z->data = data;
    z->next = NULL;

    return z;
}

/* Function create a new linked list in the heap by create a first node then point the head of the linked list to the node */
linked_list *new_linked_list(int data)
{
    node *a = new_node(data);
    linked_list *L = (linked_list *)malloc(sizeof(struct linked_list));

    L->head = a;
    return L;
}

/* Function go through all the node in the linked list then print out all the data
 * If the node->next == NULL then stop */
void traverse(linked_list *L)
{
    // Create a temp node point to the head that allow us to traverse
    node *temp = L->head;
    while (temp != NULL)
    {
        printf("Data: %d \t", temp->data);

        // Move to the next node
        temp = temp->next;
    }
    printf("\n");
}

/* Function add the node at the beginning of the linked list
 * Set new node->next = L->head
 * Set the L->head = n */
void add_begin(linked_list *L, node *n)
{
    n->next = L->head;
    L->head = n;
}

/* Function add the node at the end of the linked list */
void add_end(linked_list *L, node *n)
{
    // Traverset through the linked list, then set last_node->next to n
    node *temp = L->head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = n;
}

/* Function add the node after any node
 * node n is the node to be added
 * node a is the node before n */
void add_after_node(node *n, node *a)
{
    n->next = a->next;
    a->next = n;
}

/* Function delete any node in the linked list
 * Case 1: at the begin of the linked list
 * Case 2: at the end of the linked list */
void delete_node(linked_list *L, node *d)
{
    // Node temp for traversal
    node *temp = L->head;

    // If delete the head node
    if (temp == d)
    {
        // Move the head of the linked list, the free that node d
        L->head = d->next; // or temp->next
        free(d);
    }
    else
    {
        // while the current node has data
        while (temp != NULL)
        {
            // If the next node is the one that we want to delete
            if (temp->next == d)
            {
                // Skip that node
                temp->next = d->next;
                free(d);
                break; // break out of the loop after delete the node
            }

            temp = temp->next;
        }
    }
}

int main()
{
    printf("Linked List\n");

    // Create a linked list with the head node with data 10
    linked_list *L = new_linked_list(10);

    node *a, *b, *c, *z;
    a = new_node(20);
    b = new_node(50);
    c = new_node(60);

    // Add a,b,c to the linked list
    L->head->next = a;
    a->next = b;
    b->next = c;

    // Print out value of Linked List
    printf("Add node to Linked List: \n");
    traverse(L);

    // Insert at the beginning
    z = new_node(0);
    add_begin(L, z);
    z = new_node(-10);
    add_begin(L, z);

    // Insert at the end
    z = new_node(100);
    add_end(L, z);

    // Insert after a node
    z = new_node(30);
    add_after_node(z, a);
    z = new_node(40);
    add_after_node(z, a->next);
    z = new_node(500);
    add_after_node(z, a->next->next);

    printf("Add node after node to Linked List: \n");
    traverse(L);

    printf("Delete to Linked List: \n");
    delete_node(L, L->head);
    delete_node(L, z);
    traverse(L);

    return 0;
}