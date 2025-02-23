# Implementation of a Balanced Binary Search Tree (BST)

The presented C code implements a balanced binary search tree (BST) for managing data structures representing warehouses, containing information such as name, location, number of employees, salaries, and seniority. At the marked points in the `main` function, the following operations are performed:

## Creating and Inserting into the BST:
- Data is read from a file, and `DepositInfo` structures are created and then inserted into the binary search tree.

## Warehouse Vector at a Specific Level:
- A vector is constructed containing all warehouses at a specific level of the tree (level 2 in the given example) and displayed.

## Path Vector for a Specific Warehouse:
- A vector is generated and displayed containing all nodes (warehouses) on the path to a node with a specific key (in this case, a certain number of employees).

## Deleting Leaf Nodes:
- All leaf nodes in the tree are deleted.

## Memory Deallocation:
- All allocated memory for the data structures in the program, including the tree and created vectors, is freed.

## Display After Deallocation:
- The structures are displayed after deallocation for verification.
