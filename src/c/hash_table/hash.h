#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

#ifndef HASH_H
#define HASH_H
class hash_obj
{
private:
    // Size of the table for the hash table. Define the size of the table and use right away
    static const int table_size = 10;

    // Similar to the node in linked list
    struct item
    {
        string name;
        string drink;
        item *next;
    };

    // Create 1 hash table array contain a lot of item
    item *hash_table[table_size];

public:
    // Constructor
    hash_obj();

    // Hash Function convert a string to a position in the hash table array
    int hash_func(string key);

    // Add item to the hash table
    void add_item(string name, string drink);

    // Whatever index is, look at the bucket and count number of item in the linked list
    int number_of_item_in_index(int index);

    // Print function
    void print_table();

    // Print all items in bucket 3
    void print_items_in_index(int index);

    // Search for drink by name
    void find_drink(string name);

    // Remove
    void remove_item(string name);
};

#endif