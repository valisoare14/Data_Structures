# Implementation of a Binary Search Tree for Managing Photo Information

The provided C program implements a **Binary Search Tree (BST)** for managing information about photos.

## Inorder Traversal:
Displays all photos in the binary search tree in sorted order.
Inorder traversal in a BST displays the nodes in ascending order of their keys, using the **photo ID** as the key in this case.

## Searching for Photos in the Same City:
Counts how many photos are recorded in a specified city, in this case, **"Giurgiu"**.
The function `searchForFotosInSameTown` traverses the tree and counts the nodes where the city matches the searched one.

## Modifying Photo Date:
Changes the date of a specified photo by its ID.
The function `modificareData` searches for the node with the specified ID and updates the date of that photo.

## Deallocating the Root:
Removes the root of the binary search tree and adjusts the tree structure so that it remains a valid BST.
The function `dezalocareRadacina` handles this process.

## Converting BST to a Vector:
Transforms the binary search tree into a vector, adding only the photos that meet a specified resolution criterion.
The functions `fromBSTToVector` and `populateVector` are used for this conversion.

## Displaying Photos After a Specific Date:
Displays all photos that have a **more recent date** than a given specified date.
The function `displayFotoOnlyAboveSpecifiedDate` compares dates and displays only the photos that meet the condition.
