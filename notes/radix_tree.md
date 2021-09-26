# Radix Tree Implementation in C


# Radix Tree Implementation in Linux Kernel
```
/* Initialize the radix tree */
RADIX_TREE(radtree, GFP_KERNEL);

struct radix_tree
{
	int val;
};

/* Store value for the Radix Tree */
static int store_radix_tree(int val)
{
	// Initialize the index and return value
	int *index = kmalloc(sizeof(int), GFP_KERNEL);
	int radix_ret;
	
	// If was able to allocate memory for the new index
	if (index != NULL)
	{
		// Set value and insert provide the return value for the radix tree
		*index = val;
		radix_ret = radix_tree_insert(&radtree, (long)val, index);
		if (radix_ret != 0)
		{
			printk(KERN_INFO "%d", radix_ret);
		}
		return 0;
	}
	return -ENOMEM;
}

/* Function destroys the dynamic pointers and free allocated memory in the radix tree */
static void destroy_radix_tree_and_free(void)
{
	int key = 1;
	int *value;
	
	// Go through all the nodes that is not null in the tree, delete the node, and deallocate memory
	// Note: radix_tree_delete() is the function in the radix tree Linux API
	while ((value = radix_tree_delete(&radtree, key)) != NULL)
	{
		key++;
	}
}

/* Test function for the radix tree
 * by iterate through all elements stored in the tree "radtree" and print out all values order in both dmesg and /proc
 */
static void test_radix_tree(struct seq_file *sf)
{
	int key = 1;
	int *value;
	printk(KERN_INFO "Radix tree: ");
	seq_printf(sf, "Radix tree: ");

	// While still able to look up value with key
	while (radix_tree_lookup(&radtree, key) != NULL)
	{
		// Get the value. Note: radix_tree_lookup() is the function in the radix-tree Linux API
		value = radix_tree_lookup(&radtree, (long)key);
		printk(KERN_CONT "%d, ", *value);
		seq_printf(sf, "%d, ", *value);

		// Move to the next key
		key++;
	}
	seq_printf(sf, "\n");
}
```
