# Implementation of a Binary Search Tree (BST) for Managing Hotel Reservations

This C program implements a **Binary Search Tree (BST)** for managing hotel reservations at various hotels. Below are the descriptions of the operations executed at the commented points in the `main()` function:

## Preorder Traversal:
- The tree is traversed in **preorder** (root, left, right), displaying the information of each node. This traversal method prints the root before its descendant nodes.

## Total Payment Calculation:
- The total amount due for all reservations belonging to a specific client, identified as **"Popescu Razvan"**, is calculated.
  The entire tree is traversed, and the `paymentAmount` value from each node with the matching `client_name` is summed.

## Number of Nodes per Level:
- The number of nodes present at each level of the tree is calculated.
  The result is stored in a vector, which is then displayed. The function `NumberOfNodesPerLevel` constructs the vector, while `displayVector` prints it.

## Deleting the Minimum Node:
- The node with the **minimum value** in the tree (the leftmost node that has no left child) is removed.
  After deletion, the tree is displayed again to reflect the new structure.
