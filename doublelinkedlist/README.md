# Train Wagon Reservation Management System

This C program implements a **train wagon reservation management system**, using a data structure that combines a **doubly linked list** and a **priority queue**.

## Displaying the Doubly Linked List:
- The list is displayed **from head to tail and vice versa**, showing details about each wagon, such as **wagon number, company, number of tickets sold, and capacity**.

## Deleting the Wagon with the Fewest Tickets Sold:
- The wagon with the **smallest number of tickets sold** is removed from the list.

## Converting the List into a Priority Queue:
- The **doubly linked list** is converted into a **priority queue**, where the order of elements is determined by a specific criterion, such as **the number of tickets sold or occupancy rate**.

## Modifying the Number of Tickets and Displaying the Priority Queue:
- The **number of tickets sold** for a specific wagon is updated, and the **updated queue** is displayed, reflecting the new priority order.

## Dequeue (Extraction) and Displaying the Extracted Wagon:
- The wagon with the **highest priority** (the one with the **fewest tickets sold** or the **highest occupancy rate**) is extracted from the queue, its details are displayed, followed by the **remaining priority queue** after extraction.
