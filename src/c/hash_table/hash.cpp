#include <cstdlib>
#include <iostream>
#include <string>
#include "hash.h"

using namespace std;

// Constructor for table size, initialize a hash table
hash_obj::hash_obj()
{
    // Fill out the table size
    for (int i = 0; i < table_size; i++)
    {
        // For each slot in the hash table, create a new item with name, drinks, and pointer
        hash_table[i] = new item;
        hash_table[i]->name = "empty";
        hash_table[i]->drink = "empty";
        hash_table[i]->next = NULL;
    }
}

void hash_obj::add_item(string name, string drink)
{
    int index = hash_func(name); // index in the hash table

    // If the first item in the hash table has the name empty, then overwrite that item
    if (hash_table[index]->name == "empty")
    {
        hash_table[index]->name = name;
        hash_table[index]->drink = drink;
    }
    else
    {
        item *ptr = hash_table[index]; // point at the beginning of the index, but we will add linked list to this index later

        // initialize new item
        item *n = new item;
        n->name = name;
        n->drink = drink;
        n->next = NULL;

        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }

        ptr->next = n;
    }
}

int hash_obj::number_of_item_in_index(int index)
{
    int count = 0;

    // If we don't havve anthing to this bucket yet                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           ....
    if (hash_table[index]->name == "empty")
    {
        return count;
    }
    else
    {
        count++;
        item *ptr = hash_table[index];
        while (ptr->next != NULL)
        {
            count++;
            ptr = ptr->next;
        }
        return count;
    }
}

// Traverse through the table and print out everything
void hash_obj::print_table()
{
    int number;
    for (int i = 0; i < table_size; i++)
    {
        number = number_of_item_in_index(i);
        cout << "---------------------\n";
        cout << "Index = " << i << endl;
        cout << "Name = " << hash_table[i]->name << endl;
        cout << "Drink = " << hash_table[i]->drink << endl;
        cout << "# of items in the bucket = " << number << endl;
        cout << "---------------------\n";
    }
}

void hash_obj::print_items_in_index(int index)
{
    item *ptr = hash_table[index];
    if (ptr->name == "empty")
    {
        cout << "Index = " << index << " is empty";
    }
    else
    {
        cout << "Index = " << index << " contains the following item\n";
        while (ptr != NULL)
        {
            cout << "---------------------\n";
            cout << "Name = " << ptr->name << endl;
            cout << "Drink = " << ptr->drink << endl;
            cout << "---------------------\n";
            ptr = ptr->next;
        }
    }
}

void hash_obj::find_drink(string name)
{
    // find the bucket
    int index = hash_func(name); // the bucket
    bool found_name = false;
    string drink;

    item *ptr = hash_table[index];
    while (ptr != NULL)
    {
        if (ptr->name == name)
        {
            found_name = true;
            drink = ptr->drink;
        }
        ptr = ptr->next;
    }
    // Check if find name or not
    if (found_name == true)
    {
        cout << "Favorite drink = " << drink << endl;
    }
    else
    {
        cout << name << "'s info was not found in the hash table." << endl;
    }
}

/**
 * Case 0: The bucket is empty
 * Case 1: There is only 1 item contained in the bucket and that item has matching name
 * Case 2: The item matched is located in the first item in the bucket but there are more item in the bucket
 * Case 3: The bucket contain item but the first item is not a match
 * Case 3a: Look through all item but no match
 * Case 3b: Look trhough all item but do find the match
 */
void hash_obj::remove_item(string name)
{
    int index = hash_func(name); // return the index in the hash table array

    item *del_ptr;
    item *ptr_1;
    item *ptr_2;

    // Case 0
    if (hash_table[index]->name == "empty" && hash_table[index]->drink == "empty")
    {
        cout << "Case 0: " << name << " was not found in the hash table\n";
    }
    // Case 1
    else if (hash_table[index]->name == name && hash_table[index]->next == NULL)
    {
        hash_table[index]->name = "empty";
        hash_table[index]->drink = "empty";
        cout << "Case 1: " << name << " was removed from the hash table"
             << endl;
    }
    // Case 2
    else if (hash_table[index]->name == name)
    {
        del_ptr = hash_table[index];
        hash_table[index] = hash_table[index]->next;
        delete del_ptr;
        cout << "Case 2: " << name << " was removed from the hash table"
             << endl;
    }
    // Case 3
    else
    {
        ptr_1 = hash_table[index]->next;
        ptr_2 = hash_table[index];

        while (ptr_1 != NULL && ptr_1->name != name)
        {
            ptr_2 = ptr_1;
            ptr_1 = ptr_1->next;
        }
        // Case 3.1
        if (ptr_1 == NULL)
        {
            cout << "Case 3.1: " << name << " was not found in the hash table"
                 << endl;
        }
        // Case 3.2
        else
        {
            del_ptr = ptr_1;
            ptr_1 = ptr_1->next;
            ptr_2->next = ptr_1;

            delete del_ptr;
            cout << "Case 3.2: " << name << " was removed from the hash table"
                 << endl;
        }
    }
}

// Hash Function convert a string to a position in the hash table array
int hash_obj::hash_func(string key)
{
    int hash_value = 0;
    int index;

    for (int i = 0; i < key.length(); i++)
    {
        hash_value = hash_value + (int)key[i];
    }

    // get the remainder of the hash_value
    index = hash_value % table_size;

    return index;
}