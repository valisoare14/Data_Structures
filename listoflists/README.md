# Managing a List of Lists for Human Resources Management

This C file implements the management of a **list of lists** containing employee information, representing a **human resource management system**. Below are explanations of the operations marked in the `main()` function:

## Displaying the List of Lists:
Displays all employee information from the **list of lists** data structure, where employees are grouped by department.

## Creating and Displaying an Employee Vector:
Constructs a **vector of employees** who earn above a specified hourly wage threshold (**160.5** in this case).  
The vector is then displayed using `displayVector()`.

## Determining and Displaying Employees from a Specific Department:
Creates a **queue of employees** (using a **circular list**) who belong to a specific department (**codDepartament = 2**).  
The queue is built and returned by the function `determinareAngajatiDinDepartamentulX()`.

## Creating and Displaying a Vector of Employees with the Lowest Salaries:
Constructs a **vector of employees** who have the **lowest salaries** from a given queue.
