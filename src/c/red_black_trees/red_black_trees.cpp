// Red-Black Trees is a self-balanced binary search tree.
// A "perfect balanced" bst is basically a complete tree.
// However, we can't recuild the entire tree as a complete tree after inserting or deleting
// any node because the buiding process is very expensive and then there won't be any use of inserting and deleting in O(logN) time.

// Rules of RB-Trees:
// 1. Every node is colored either red or black
// 2. Root of the tree is is black
// 3. All leaves are black
// 4. Both children of a red node are black ie, there can't be consecutive red nodes
// 5. All the simple paths from a node to descendant leaves contain the same number of black nodes

// Black Height = the number of BLACK NODE on any simpple path from a node x (not including it) to a leaf. Black height of any node x is represented by bh(x)

#include <stdio.h>
#include <stdlib.h>

// Binary marks red or black of each node
enum COLOR
{
    Red,
    Black
};

// Create a node data type.
typedef struct tree_node
{
    int data;
    struct tree_node *right;
    struct tree_node *left;
    struct tree_node *parent;
    enum COLOR color;
} tree_node;

// Create a rb-tree datatype
typedef struct red_black_tree
{
    tree_node *root;
    tree_node *NIL;
} rb_tree;

// Function creates a new tree node with data
tree_node *new_tree_node(int data)
{
    tree_node *n = (tree_node *)malloc(sizeof(tree_node));
    n->left = NULL;
    n->right = NULL;
    n->parent = NULL;
    n->data = data;
    n->color = Red;

    return n;
}

// Function creates a new rb tree
rb_tree *new_rb_tree()
{
    rb_tree *t = (rb_tree *)malloc(sizeof(rb_tree));
    tree_node *nil_node = (tree_node *)malloc(sizeof(tree_node));
    nil_node->left = NULL;
    nil_node->right = NULL;
    nil_node->parent = NULL;
    nil_node->color = Black;
    nil_node->data = 0;
    t->NIL = nil_node;
    t->root = t->NIL;

    return t;
}

// Function rotate the tree to the left for self-balance
void left_rotate(red_black_tree *t, tree_node *x)
{
    tree_node *y = x->right;
    x->right = y->left;
    if (y->left != t->NIL)
    {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == t->NIL)
    { //x is root
        t->root = y;
    }
    else if (x == x->parent->left)
    { //x is left child
        x->parent->left = y;
    }
    else
    { //x is right child
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

// Function rotate the tree to the right for self-balance
void right_rotate(red_black_tree *t, tree_node *x)
{
    tree_node *y = x->left;
    x->left = y->right;
    if (y->right != t->NIL)
    {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == t->NIL)
    { //x is root
        t->root = y;
    }
    else if (x == x->parent->right)
    { //x is left child
        x->parent->right = y;
    }
    else
    { //x is right child
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

// Function insert the new node to tree with rotation.
void insertion_fixup(red_black_tree *t, tree_node *z)
{
    while (z->parent->color == Red)
    {
        if (z->parent == z->parent->parent->left)
        { //z.parent is the left child

            tree_node *y = z->parent->parent->right; //uncle of z

            if (y->color == Red)
            { //case 1: node to be insert is in the last level
                z->parent->color = Black;
                y->color = Black;
                z->parent->parent->color = Red;
                z = z->parent->parent;
            }
            else
            { //case2 or case3: node to be insert is either in middle level right or left
                if (z == z->parent->right)
                {                  //case2
                    z = z->parent; //marked z.parent as new z
                    left_rotate(t, z);
                }
                //case3
                z->parent->color = Black;       //made parent black
                z->parent->parent->color = Red; //made parent red
                right_rotate(t, z->parent->parent);
            }
        }
        else
        {                                           //z.parent is the right child
            tree_node *y = z->parent->parent->left; //uncle of z

            if (y->color == Red)
            {
                z->parent->color = Black;
                y->color = Black;
                z->parent->parent->color = Red;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent; //marked z.parent as new z
                    right_rotate(t, z);
                }
                z->parent->color = Black;       //made parent black
                z->parent->parent->color = Red; //made parent red
                left_rotate(t, z->parent->parent);
            }
        }
    }
    t->root->color = Black;
}

// Final insert function of fix-up and non-fix-up cases.
void insert_node(red_black_tree *t, tree_node *z)
{
    tree_node *y = t->NIL; //variable for the parent of the added node
    tree_node *temp = t->root;

    while (temp != t->NIL)
    {
        y = temp;
        if (z->data < temp->data)
            temp = temp->left;
        else
            temp = temp->right;
    }
    z->parent = y;

    if (y == t->NIL)
    { //newly added node is root
        t->root = z;
    }
    else if (z->data < y->data) //data of child is less than its parent, left child
        y->left = z;
    else
        y->right = z;

    z->right = t->NIL;
    z->left = t->NIL;

    insertion_fixup(t, z);
}

// Helper Function transplants for deletion
void rb_transplant(red_black_tree *t, tree_node *u, tree_node *v)
{
    if (u->parent == t->NIL)
        t->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

// Function find the smallest element in the BST
tree_node *minimum(red_black_tree *t, tree_node *x)
{
    while (x->left != t->NIL)
        x = x->left;
    return x;
}

// Function deletes the new node from tree with rotation.
void rb_delete_fixup(red_black_tree *t, tree_node *x)
{
    while (x != t->root && x->color == Black)
    {
        if (x == x->parent->left)
        {
            tree_node *w = x->parent->right;
            if (w->color == Red)
            {
                w->color = Black;
                x->parent->color = Red;
                left_rotate(t, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == Black && w->right->color == Black)
            {
                w->color = Red;
                x = x->parent;
            }
            else
            {
                if (w->right->color == Black)
                {
                    w->left->color = Black;
                    w->color = Red;
                    right_rotate(t, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = Black;
                w->right->color = Black;
                left_rotate(t, x->parent);
                x = t->root;
            }
        }
        else
        {
            tree_node *w = x->parent->left;
            if (w->color == Red)
            {
                w->color = Black;
                x->parent->color = Red;
                right_rotate(t, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == Black && w->left->color == Black)
            {
                w->color = Red;
                x = x->parent;
            }
            else
            {
                if (w->left->color == Black)
                {
                    w->right->color = Black;
                    w->color = Red;
                    left_rotate(t, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = Black;
                w->left->color = Black;
                right_rotate(t, x->parent);
                x = t->root;
            }
        }
    }
    x->color = Black;
}

// Final delete function of fix-up and non-fix-up cases.
void rb_delete(red_black_tree *t, tree_node *z)
{
    tree_node *y = z;
    tree_node *x;
    enum COLOR y_orignal_color = y->color;
    if (z->left == t->NIL)
    {
        x = z->right;
        rb_transplant(t, z, z->right);
    }
    else if (z->right == t->NIL)
    {
        x = z->left;
        rb_transplant(t, z, z->left);
    }
    else
    {
        y = minimum(t, z->right);
        y_orignal_color = y->color;
        x = y->right;
        if (y->parent == z)
        {
            x->parent = z;
        }
        else
        {
            rb_transplant(t, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        rb_transplant(t, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (y_orignal_color == Black)
        rb_delete_fixup(t, x);
}

// Function traverse all the node and print out all the node in order
void traversal_inorder(red_black_tree *t, tree_node *n)
{
    if (n != t->NIL)
    {
        traversal_inorder(t, n->left);
        printf("%d\n", n->data);
        traversal_inorder(t, n->right);
    }
}

int main()
{
    // Create a red-black tree with empty nodes
    rb_tree *t = new_rb_tree();
    tree_node *a, *b, *c, *d, *e, *f, *g, *h, *i, *j, *k, *l, *m;

    // Add data to the empty nodes
    a = new_tree_node(10);
    b = new_tree_node(20);
    c = new_tree_node(30);
    d = new_tree_node(100);
    e = new_tree_node(90);
    f = new_tree_node(40);
    g = new_tree_node(50);
    h = new_tree_node(60);
    i = new_tree_node(70);
    j = new_tree_node(80);
    k = new_tree_node(150);
    l = new_tree_node(110);
    m = new_tree_node(120);

    // Insert the nodes to the empty red-black tree
    insert_node(t, a);
    insert_node(t, b);
    insert_node(t, c);
    insert_node(t, d);
    insert_node(t, e);
    insert_node(t, f);
    insert_node(t, g);
    insert_node(t, h);
    insert_node(t, i);
    insert_node(t, j);
    insert_node(t, k);
    insert_node(t, l);
    insert_node(t, m);

    // Delete the nodes from the empty red-black tree
    rb_delete(t, a);
    rb_delete(t, m);

    traversal_inorder(t, t->root);

    return 0;
}