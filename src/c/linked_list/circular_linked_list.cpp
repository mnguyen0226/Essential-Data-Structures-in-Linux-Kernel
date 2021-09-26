// Circular linked List = simple linked list with the next pointer of the last node connected to the first nodes
// We choose to keep record of the last element and not the first because inserting a new node
// after the last node will be efficient in this case instead of traversing over entire list (which we did when keep record of the first element)
// Note that this is a single linked list

#include <stdlib.h>
#include <stdio.h>

// Define a node
typedef struct node
{
    int data;
    struct node *next;
} node;

// Define the circular list and keep track of the last node
typedef struct circular_list
{
    struct node *last;
} circular_list;

/**
 * Function creates a new node dynamically on the heap.
 * This process is the same for all type of linked list
 */
node *create_node(int data)
{
    node *n = (node *)malloc(sizeof(node));
    n->data = data;
    n->next = NULL;

    return n;
}

/**
 * Function creates a circular list dynamically on the heap
 */
circular_list *create_circular_list(int key)
{
    // Create a single node circular list
    node *n = create_node(key);
    n->next = n;

    circular_list *L = (circular_list *)malloc(sizeof(circular_list));
    L->last = n; // This is similar to set the head

    return L;
}

/**
 * Function adds a node n after a node a
 */
void insert_after(node *n, node *a)
{
    n->next = a->next;
    a->next = n;
}

/**
 * Function adds a node at the end of the circular linked list
 */
void insert_at_last(circular_list *L, node *a)
{
    a->next = L->last->next;
    L->last->next = a;
    L->last = a; // set the last node as a
}

/**
 * Function deletes the node in a list
 */
void delete_node(circular_list *L, node *a)
{
    // Traverst to that node in the list
    node *temp = L->last;
    while (temp->next != a)
    {
        temp = temp->next;
    }

    // At this point, we at the node before a
    // If a last node
    if (a == L->last)
    {
        // If it is the only node in the list
        if (a->next == a)
        {
            L->last == NULL; // L->last is a
        }
        else
        {
            temp->next = a->next;
            L->last = temp; // Update the last node
        }
    }
    // If this is not the last node
    else
    {
        temp->next = a->next;
    }

    free(a);
}

/**
 * Function traverses and print out all the values in the circular linked list
 */
void traverse(circular_list *L)
{
    node *temp = L->last;       // temporary node for traversal
    printf("%d\t", temp->data); // print the "last node"
    temp = temp->next;          // move to the next node

    while (temp != L->last)
    { // Traverse till last node
        printf("%d\t", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

int main()
{
    printf("Circular Linked List\n");

    // Intialize the linked list with the head pointed to the node stored data = 10
    circular_list *L = create_circular_list(10);
    node *a, *b, *c, *z;

    a = create_node(20);
    b = create_node(50);
    c = create_node(60);

    // Add the new node to the linked list after
    L->last->next = a;
    a->next = b;
    b->next = c;
    c->next = L->last;

    traverse(L);

    z = create_node(50);
    insert_after(z, c);
    z = create_node(25);
    insert_after(z, a);
    z = create_node(100);
    insert_at_last(L, z);

    traverse(L);
    delete_node(L, L->last);
    delete_node(L, b);

    traverse(L);

    return 0;
}
