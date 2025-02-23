# Implementation of a Hash Table (HashTable)

This C file implements a **Hash Table (HashTable)** for managing project information, and the commented points in the `main()` function illustrate various operations performed on this data structure:

## Displaying the Hash Table:
Each **bucket** of the hash table is traversed, and the projects stored in the **linked list** associated with each bucket are displayed.

## Calculating the Total Budget:
The total **budget sum** for all projects belonging to a specified **beneficiary** (in this case, **"ROMAC"**) is calculated.
This is done by **traversing the linked list** of the bucket where projects with that beneficiary should be found, based on a **hash function**.

## Constructing and Displaying a List of Projects:
A **linked list** is created containing all **projects with a budget greater** than a specified value (**20,000** in this case).  
The projects in this list are then displayed.
