/**
 * Project 2 - LKP: Important Data Structures in Linux Kernel Programming
 * - Linked List
 * - Hash Table
 * - Red Black Tree
 * - Radix Tree
 * - XArray
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/hashtable.h>
#include <linux/rbtree.h>
#include <linux/radix-tree.h>
#include <linux/xarray.h>
#include <linux/seq_file.h>

static char *int_str;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Minh T. Nguyen");
MODULE_DESCRIPTION("LKP Project 2");

/* Module take in values int_str from the command line and pass to this module */
module_param(int_str, charp, S_IRUSR | S_IRGRP | S_IROTH);

/* For documentation purposes */
MODULE_PARM_DESC(int_str, "Important Data Structures in Linux Kernel in Processed File Systems");

/* DATA STRUCTURE 1: Linked List ------------------------------------------------------------------------------------- */
/* Initialize the head of the list */
static LIST_HEAD(mylist);

/* Initialize the entry node for linked list */
struct entry
{
	int val;
	struct list_head next;
};

/* Store value to the tail of linked list */
static int store_linked_list(int val)
{
	// Initialize temporary node for new value
	struct entry *temp_node = kmalloc(sizeof(struct entry), GFP_KERNEL);

	// If able to allocate memory for the new node
	if (temp_node != NULL)
	{
		temp_node->val = val;

		// Set connect with the head node and add to the tail of the linked list
		INIT_LIST_HEAD(&temp_node->next);
		list_add_tail(&temp_node->next, &mylist);
		return 0;
	}
	return -ENOMEM;
}

/* Function destroys the dynamic pointers and free allocated memory in the linked list */
static void destroy_linked_list_and_free(void)
{
	struct entry *cursor, *temp;
	// Use the cursor node to traverse and delete each node in linked list
	// Note: list_for_each_entry_safe(), list_del(), and kfree() are functions in the linked-list Linux API
	list_for_each_entry_safe(cursor, temp, &mylist, next)
	{
		list_del(&cursor->next);
		kfree(cursor);
	}
}

/* Test function for linked list implementation 
 * by iterate through all elements in the stored linked-list "mylist" and print out all values order in both dmesg and /proc
 */
static void test_linked_list(struct seq_file *sf)
{
	struct entry *temp;

	printk(KERN_INFO "Linked list: ");
	seq_printf(sf, "Linked list: ");
	
	// list_for_each_entry() is a function in the linked-list Linux API
	list_for_each_entry(temp, &mylist, next)
	{
		printk(KERN_CONT " %d, ", temp->val);
		seq_printf(sf, "%d, ", temp->val);
	}
	
	seq_printf(sf, "\n");
}

/* DATA STRUCTURE 2: Hash Table  ------------------------------------------------------------------------------------- */
/* Initialize the hash table with size of 10 */
static DEFINE_HASHTABLE(table, 10);

/* Initialize the node of the hash table */
struct hash_node
{
	int val;
	struct hlist_node next; // hlist_node is a struct in linux APIT
};

/* Initialize hash function */
static int hash_func(int key)
{
	int index;
	// Basic hash calculation with the remainder between key and table size
	index = key % sizeof(table);
	return index;
}

/* Store value for hash table */
static int store_hash_table(int val)
{
	// Initialize the first node in the table and the key
	struct hash_node *n = kmalloc(sizeof(struct hash_node), GFP_KERNEL);
	int key_n;
	
	// If was able to allocate memory for new node
	if (n != NULL)
	{
		n->val = val;

		// Calculate the key value from node n to return key value to store in the table
		key_n = hash_func(n->val);

		// hash_add() is the function in the hash table Linux API. Solve Collision with linked list
		hash_add(table, &n->next, key_n);
		return 0;
	}
	return -ENOMEM;
}

/* Function destroys the dynamic pointers and free allocated memory in the hash table */
static void destroy_hash_table_and_free(void)
{
	struct hash_node *del_node;
	struct hlist_node *temp;
	unsigned bkt;

	// Go through each node in the hash table, delete, and free memory
	// Note: hash_for_each_safe(), hash_del(), kfree() are functions in the hash-table Linux API
	hash_for_each_safe(table, bkt, temp, del_node, next)
	{
		hash_del(&del_node->next);
		kfree(del_node);
	}
}

/* Test function for the hash table 
 * by iterate through all elements stored in the table "table" and print out all values order in both dmesg and /proc
 */
static void test_hash_table(struct seq_file *sf)
{
	struct hash_node *curr_node;
	unsigned int bkt; //integer in hash_for_each to use as bucket loop cursor
	printk(KERN_INFO "Hash table: ");
	seq_printf(sf, "Hash table: ");

	// hash_for_each() is a function in the hash table Linux API
	hash_for_each(table, bkt, curr_node, next)
	{
		printk(KERN_CONT "%d, ", curr_node->val);
		seq_printf(sf, "%d, ", curr_node->val);
	}
	seq_printf(sf, "\n");
}

/* DATA STRUCTURE 3: Red-Black Tree  ------------------------------------------------------------------------------------- */
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

/* DATA STRUCTURE 4: Radix Tree  ------------------------------------------------------------------------------------- */
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

/* DATA STRUCTURE 5: XArray  ------------------------------------------------------------------------------------- */
DEFINE_XARRAY(xarray);

/* Store temp for the X Array */
static int store_xarray(int val)
{
	// Initialize the value
	int *temp = kmalloc(sizeof(int), GFP_KERNEL);

	// If was able to allocate memory for the new temp
	if (temp != NULL)
	{
		// Store value in the xarray
		*temp = val;

		// xa_store() is the function in the xarray Linux API
		xa_store(&xarray, val, temp, GFP_KERNEL);
		return 0;
	}
	return -ENOMEM;
}

/* Function destroys the dynamic pointers and free allocated memory in the XArray */
static void destroy_xarray_and_free(void)
{
	int key = 1;
	int *value;
	
	// Go through all the data that is not null in the radix tree, delete the data, and deallocate memory
	while ((value = xa_erase(&xarray, key)) != NULL)
	{
		key++;
	}
}

/* Test function for the XArray
 * by iterate through all elements stored in the XArray "xarray" and print out all values order in both dmesg and /proc
 */
static void test_xarray(struct seq_file *sf)
{
	int key = 1;
	int *value;
	printk(KERN_INFO "XArray: ");
	seq_printf(sf, "XArray: ");

	// While still be able to load the value in the array with key
	// xa_load() is the function in the xarray Linux API
	while (xa_load(&xarray, key) != NULL)
	{
		// Get the value
		value = xa_load(&xarray, key);
		printk(KERN_CONT "%d, ", *value);
		seq_printf(sf, "%d, ", *value);

		// Move to the next key
		key++;
	}
	seq_printf(sf, "\n");
}


/* PARAMETER PARSER  ------------------------------------------------------------------------------------- */
static int parse_params(void)
{
	int val, err = 0;
	char *p, *orig, *params;

	/*
	 * kstrdup allocates space for and copy an existing int_str string.
	 * GFP_KERNEL is the GFP mask used in the kmalloc call when allocating memory.
	 * If not able to allocate for and copy a string int_str, then return corresponding error code in error.h.
	 * Else set the copied string to new variable called orig.
	 */
	params = kstrdup(int_str, GFP_KERNEL);
	if (!params)
		return -ENOMEM;
	orig = params;

	/*
	 * strsep mean to split a string params into tokens with "," is a character to search for.
	 * While the first string/char before "," is not null, then proceed the if condition.
	 * If the pointer p was not able to be created, then continue to the next p (or the next round in while loop).
	 */
	while ((p = strsep(&params, ",")) != NULL)
	{
		if (!*p)
			continue;
		/* 
		 * kstrtoint convert string to in with int kstrtoint (	const char * p, unsigned int 0, int &val);
		 * Then assign the converted value to new variable called err. If success, return 0.
		 * If 1 or not able to convert, then break / escape from the while loop.
		 */
		err = kstrtoint(p, 0, &val);
		if (err)
			break;

		/*
		 * Execute function store_linked_list defined above with int val named "val". If success, then return 0
		 * If not able to allocate space or add to the tail of mylist (aka, return 1), then break from the while loop
		 */
		err = store_linked_list(val);
		if (err)
			break;

		/*
		 * Execute function store_hash_table defined above with int val named "val". If success, then return 0
		 * If not able to allocate space (aka, return 1), then break from the while loop
		 */
		err = store_hash_table(val);
		if (err)
			break;

		/*
		 * Execute function store_rb_tree defined above with int val named "val". If success, then return 0
		 * If not able to allocate space (aka, return 1), then break from the while loop
		 */
		err = store_rb_tree(&my_rb_tree, val);
		if (err)
			break;

		/*
		 * Execute function store_radix_tree defined above with int val named "val". If success, then return 0
		 * If not able to allocate space (aka, return 1), then break from the while loop
		 */
		err = store_radix_tree(val);
		if (err)
			break;

		/*
		 * Execute function store_xarray defined above with int val named "val". If success, then return 0
		 * If not able to allocate space (aka, return 1), then break from the while loop
		 */
		err = store_xarray(val);
		if (err)
			break;
	}

	/* 
	 * kfree() free previously allocated memory, specifically orig which is a pointer returned by kmalloc.
	 * return the value stored in variable err
	 */
	kfree(orig);
	return err;
}

/* TEST DATA STRUCTURES & DESTROY  ------------------------------------------------------------------------------------- */
/*
 * Execute the function test_linked_list() which print out all entry in mylist linked list.
 * We have this function called in a static function since we want to restrict access to a function. aka hide the client code.
 */
static void run_tests(struct seq_file *sf)
{
	test_linked_list(sf);
	test_hash_table(sf);
	test_rb_tree(sf);
	test_radix_tree(sf);
	test_xarray(sf);
}

/* 
 * Print out the string "Cleaning up..." then free all entry in linked-list mylist
 * We can think of this as deallocate the memory in dynamic programming which helps to reduce wastage of memory by freeing it.
 * We have this function called in a static function since we want to restrict access to a function. aka hide the client code.
 */
static void cleanup(void)
{
	printk(KERN_INFO "\nCleaning up...\n");
	destroy_linked_list_and_free();
	destroy_hash_table_and_free();
	destroy_rb_tree_and_free();
	destroy_radix_tree_and_free();
	destroy_xarray_and_free();
}

/* PROC FILE SYSTEMS  ------------------------------------------------------------------------------------- */
static int project_2_init(struct seq_file *sf, void *v)
{
	int err = 0;

	/*
	 * Note: The __init macro causes the init function to be discarded and its memory freed once the init function finishes for built-in drivers, but not loadable modules. 
	 * If the string int_str is not create or exist, then print the error message and return -1.
	 */
	if (!int_str)
	{
		printk(KERN_INFO "Missing \'int_str\' parameter, exiting\n");
		return -1;
	}

	/*
	 * The goto statement comes in handy when a function exits from multiple locations and some common work such as cleanup has to be done.
	 * Execute the function parse_params() which allocate space, make a copy of int_str and parse it by ",".
	 * If not able to convert string to int or store the parsed value to the tail of mylist linked list, then go to the out section below
	 */
	err = parse_params();
	if (err)
		goto out;

	/*
	 * Execute the static function run_tests(). We have this function called in a static function since we want to restrict access to a function. aka hide the client code.
	 */
	run_tests(sf);
out:
	/* 
	 * Execute the static function cleanup(). We have this function called in a static function since we want to restrict access to a function. aka hide the client code.
	 * Then return the value stored in the err variable
	 */
	cleanup();
	return err;
}

/* Functions is the open callback, called when the proc file is opened. */
static int project_2_proc_open(struct inode *inode, struct file *file)
{
	// single_open() means that all the data is output at once for the sequence file
	return single_open(file, project_2_init, NULL);
}

/* Function is the file operations structure used to define file manipulation call backs for the pseudo file*/
static const struct proc_ops project_2_proc_fops = {
	.proc_open = project_2_proc_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

/* Function creates proc file "proj2" */
static int __init project_2_proc_init(void)
{
	// proc_create creates "proj2" with 0 is the permission_bits parametes means defaults 0444 permission for file
	// NULL in the parent_dir mean that the file is located at /proc
	proc_create("proj2", 0, NULL, &project_2_proc_fops);
	return 0;
}

/* Function used to deallocate memory and remove the pseudo file */
static void __exit project_2_proc_exit(void)
{
	cleanup();
	remove_proc_entry("proj2", NULL);
}

/* NOTE:  Using the module_init() and module_exit() macros it is easy to write code without #ifdefs which can operate both as a module or built into the kernel. */
module_init(project_2_proc_init);
module_exit(project_2_proc_exit);
