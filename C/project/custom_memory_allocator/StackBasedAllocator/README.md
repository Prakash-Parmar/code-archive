# Stack-Based Memory Allocator

This folder contains a **simple stack (linear) memory allocator** implemented in C.  
Memory is allocated in **Last-In-First-Out (LIFO)** order from a pre-allocated buffer.


## Stack Allocator Free Behavior

* Follows LIFO (Last-In-First-Out) model.
* Users are expected to free only the most recent allocation.
* `stack_free` does **not** check explicitly if the pointer is the last allocation.
* Freeing a middle allocation may corrupt or overwrite memory allocated after it.
* This design keeps the allocator **simple and efficient**.

## Stack Allocator Resize Behavior

* Any allocation can be resized, not necessarily the last allocation.
* Resize process:
    1. Allocate a new block of the requested size.
    2. Copy old data (up to min(old_size, new_size)) into the new block.
    3. Return the new pointer.
* Original memory is **not freed** unless it's the last allocation.
* This may lead to **fragmentation** if many reallocations occur.
* True in-place resizing is only possible if resizing the last allocation (LIFO).
* This design trades simplicity and flexibility for potential memory inefficiency.

