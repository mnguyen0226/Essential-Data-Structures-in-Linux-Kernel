```
/* Initialize the rb tree */
struct rb_tree
{
	int val;
	struct rb_node next;
};

/* Initialize the root of rb tree */
struct rb_root my_rb_tree = RB_ROOT;

/* Store value for rb Tree */
static int store_rb_tree(struct rb_root *root, int val)
{
	// Initialize the temp node, linked node, and parent node
	struct rb_tree *temp = kmalloc(sizeof(struct rb_tree), GFP_KERNEL);
	struct rb_node **link_node = &root->rb_node;
	struct rb_node *parent_node = NULL;
	struct rb_tree *src_node;

	// If was able to allocate memory for the new node
	if (temp != NULL)
	{
		temp->val = val;
		// While link node exist, set left or right based on the mechanism of its value
		while (*link_node)
		{
			parent_node = *link_node;
			src_node = rb_entry(parent_node, struct rb_tree, next);
			if (src_node->val > val)
			{
				link_node = &(*link_node)->rb_left;
			}
			else
			{
				link_node = &(*link_node)->rb_right;
			}
		}

		// rb_link_node() and rb_insert_color() are function in the rbtree Linux API
		rb_link_node(&temp->next, parent_node, link_node);
		rb_insert_color(&temp->next, root);
		return 0;
	}
	return -ENOMEM;
}

/* Function destroys the dynamic pointers and free allocated memory in the red-black tree */
static void destroy_rb_tree_and_free(void)
{
	struct rb_tree *tree;
	struct rb_node *n = rb_last(&my_rb_tree);

	// Go through all the nodes that is not null in the tree, delete the node, and deallocate memory
	while (n != NULL)
	{
		// Note: rb_entry(), rb_prev(), and rb_erase(), kfree() are function in the red-black tree API
		tree = rb_entry(n, struct rb_tree, next);
		n = rb_prev(n);
		rb_erase(&tree->next, &my_rb_tree);
		kfree(tree);
	}
}

/* Test function for the red-black tree
 * by iterate through all elements stored in the tree "my_rb_tree" and print out all values order in both dmesg and /proc
 */
static void test_rb_tree(struct seq_file *sf)
{
	struct rb_node *curr_node = rb_first(&my_rb_tree);
	struct rb_tree *traverse_node;
	printk(KERN_INFO "Red-black tree: ");
	seq_printf(sf, "Red-black tree: ");

	// While the current node still store a value
	while (curr_node != NULL)
	{
		// Get value from traversal node and print out
		traverse_node = rb_entry(curr_node, struct rb_tree, next);
		printk(KERN_CONT "%d, ", traverse_node->val);
		seq_printf(sf, "%d, ", traverse_node->val);

		// Keep traversing
		curr_node = rb_next(curr_node);
	}
	seq_printf(sf, "\n");
}
```