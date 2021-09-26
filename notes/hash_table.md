# Hash Table Implementation in C

# Hash Table Implementation in Linux Kernel
```
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
```