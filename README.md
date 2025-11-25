[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/_DBradCD)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-2e0aaae1b6195c2367325f4f02e2d04e9abb55f0b24a779b69b11b9e10269abc.svg)](https://classroom.github.com/online_ide?assignment_repo_id=21821791&assignment_repo_type=AssignmentRepo)
# Week 10 Wed Lab

## Directions

You are going to implement the hash table data structure using the folding technique as the $h$ hash function and open hashing as the CRP. Therefore, your `HashTableOpen` class will rely on the `Dictionary` and `List` ADTs, as well as the `LinkedList` class.

There are 6 methods that you need to implement to complete the data structure:

- `HashTableOpen(int)`
- `~HashTableOpen()`
- `Val find(const Key&) const`
- `void insert(const Key&, const Val&)`
- `void remove(const Key&)`
- `int size() const`

A working `main` function is already provided. Use this to test your hash table implementation.

## Notes

- All elements in the hash table are stored as `Record` instances, as per the search problem definition
    + If you need access an individual record's key or value, use `.k` or `.v` on it, respectively
- We are assuming that there are no duplicate keys in the hash table
- The hash table relies on `ht`, a dynamic array of dynamic linked lists, to store the records
    + You must allocate memory for `ht` in the constructor and release its memory in the destructor
    + Remember, there are two layers of memory here!
- Use the parameter for the constructor to initialize `M`, the size of `ht`
- When searching for a record in the hash table, you must first hash the key using the `hash` function, which then gives you back the slot (index) to visit in `ht`
    + Remember, what's being stored in `ht` are pointers to `LinkedList` objects, so you'll need to use the methods for the `LinkedList` class to complete the search
- `find` and `remove` need to throw errors if they're unable to complete their tasks, like this:
    ```C++
    throw string("message goes here");
    ```
- When inserting a new record to the hash table, be wary of the state of the linked list (empty vs. non-empty)
    + If done right, `insert` will be $\mathcal{O}(1)$
- The size of the hash table is not the same thing as `M`
    + We define the size as the number of elements stored in the hash table
    + You will need to traverse `ht` and count how many records are in each linked list

