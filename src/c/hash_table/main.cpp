// To compile this on the terminal do: g++ main.cpp hash.cpp -o main

#include <cstdlib>
#include <iostream>
#include <string>
#include "hash.h"

using namespace std;

int main(int argc, char **argv)
{
    hash_obj hashy; // create a new hash_object and called a constructor

    string name = "";

    // Add people in the hash table with name as key and drink as data
    hashy.add_item("Paul", "Locha");
    hashy.add_item("Kim", "Iced Mocha");
    hashy.add_item("Emma", "Strawberry Smoothy");
    hashy.add_item("Annie", "Hot Chocolate");
    hashy.add_item("Sarah", "Passion Tea");
    hashy.add_item("Pepper", "Caramel Mocha");
    hashy.add_item("Mike", "Chai Tea");
    hashy.add_item("Steve", "Apple Cider");
    hashy.add_item("Bill", "Root Beer");
    hashy.add_item("Marie", "Skinny Latte");
    hashy.add_item("Susan", "Water");
    hashy.add_item("Joe", "Green Tea");

    hashy.print_table();

    // Test Search
    // while(name != "exit"){
    //     cout << "Search for: (or exit): ";
    //     cin >> name;
    //     if(name != "exit"){
    //         hashy.find_drink(name);
    //     }
    // }

    // Test Remove
    while (name != "exit")
    {
        cout << "Remove: (or exit): ";
        cin >> name;
        if (name != "exit")
        {
            hashy.remove_item(name);
        }
    }

    hashy.print_table();

    return 0;
}