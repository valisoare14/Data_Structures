# Manipulating a Data Structure for Product Warranties

The provided C code defines **structures and functions** for manipulating a **hash table** data structure that manages product warranty information. It also includes **conversion to a doubly linked list**. In the `main` function, several operations are performed, marked with comments at points **.1 to .5**:

1. **Loading Data into the Hash Table:**
   - A **text file (Data.txt)** is opened.
   - For each line read from the file, **warranty details** are extracted.
   - A **GarantieInfo** structure is created.
   - The **hash table** is populated using these warranty structures.

2. **Searching the Hash Table:**
   - The number of **warranties associated with a specific service** (e.g., **"SecureGuard Insurance"**) is searched.
   - The **count is displayed**.

3. **Extending Warranties:**
   - The **hash table is traversed**.
   - For each **electronic warranty** (where the field `isElectronic` is **true/1**), the **warranty duration** is extended by a specified value (**7 months** in this case).

4. **Modifying the Service for a Warranty:**
   - The **service name** for a specific warranty is changed.
   - This involves **updating the structure information**.
   - If the **hash value of the new service** is different, the **node may need to be moved to another bucket**.

5. **Converting the Hash Table to a Doubly Linked List:**
   - The **hash table is converted** into a **doubly linked list**.
   - Only **warranties with a duration below a specified threshold** (**20 months** in this case) are added to the list.
