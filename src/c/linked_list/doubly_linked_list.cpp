// Doubly Linked List allow to move forward and backward

#include <stdlib.h>
#include <stdio.h>

/* Define a node */
typedef struct node
{
    int data;
    struct node *next;
    struct node *prev;
} node;

/* Define a linked list */
typedef struct linked_list
{
    struct node *head;
} linked_list;

/* Function make new node with initial data */
node *create_node(int data)
{
    node *z;
    z = (node *)malloc(sizeof(struct node));
    z->data = data;
    z->next = NULL;
    z->prev = NULL;
}

/* Function make ne linked list */
linked_list *create_linked_list(int data)
{
    node *a;
    linked_list *L = (linked_list *)malloc(sizeof(struct linked_list));
    L->head = a;

    return L;
}

/* Function traverse through the linked list and print out all data of the linked list */
void traverse(linked_list *L)
{
    // Set temp node for traversal
    node *temp = L->head;

    while (temp != NULL)
    {
        printf("Data: %d\t", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

/* Insert new node to the front of the linked list */
void add_at_front(linked_list *L, node *n)
{
    // Set connection
    n->next = L->head;
    L->head->prev = n;

    // Set the head pointed to the actual node
    L->head = n;
}

/* Insert new node to the end of the linked list */
void add_at_end(linked_list *L, node *n)
{
    // Traverse till the end of the linked list
    node *temp = L->head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    // If the next node is NULL then we know that we are at the end of the linked list
    temp->next = n;
    n->prev = temp;
}

/* Insert a new node after any node
 * Node n = node that we want to add
 * Node a = node that already in the linked list */
void add_after_node(node *n, node *a)
{
    // Traverse till the current node is a
    n->next = a->next;
    a->next->prev = n;
    a->next = n;
    n->prev = a;
}

/* Delete a Node
 * First check if it is a head node or not by prev pointer of the node is null
 * If not the head node then point the next pointer of the node previous to a to the next of a */
void delete_node(linked_list *L, node *a)
{
    // If the node to be deleted is not a head node
    // These two conditions deal with a only itself
    if (a->prev != NULL)
    {
        a->prev->next = a->next;
    }
    else
    {
        L->head = a->next;
    }

    // point the prev pointer of the node next to a to be the node previous to a
    if (a->next != NULL)
    {
        a->next->prev = a->prev;
    }

    free(a);
}

int main()
{
    printf("Doubly Linked List \n");

    // Intialize the linked list with the head pointed to the node stored data = 10
    linked_list *L = create_linked_list(10);
    node *a, *b, *c, *z;

    a = create_node(20);
    b = create_node(50);
    c = create_node(60);

    // Add the new node to the linked list after
    L->head->next = a;
    a->next = b;
    b->next = c;

    traverse(L);

    z = create_node(0);
    add_at_front(L, z);
    z = create_node(-10);
    add_at_front(L, z);

    z = create_node(100);
    add_at_end(L, z);

    z = create_node(30);
    add_after_node(z, a);
    z = create_node(40);
    add_after_node(z, a->next);
    z = create_node(500);
    add_after_node(z, a->next->next);

    traverse(L);

    delete_node(L, L->head);
    delete_node(L, z);
    traverse(L);

    return 0;
}