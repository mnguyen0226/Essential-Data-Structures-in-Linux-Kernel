```

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

```
