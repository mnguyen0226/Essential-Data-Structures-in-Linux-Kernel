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