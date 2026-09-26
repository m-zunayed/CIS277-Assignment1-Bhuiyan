# CIS-277 Assignment 1: Network Packet Buffer Pool

## Student

Md Zunayed Bhuiyan

## Description

This program implements a fixed-size memory pool for storing network packet data. The memory pool is divided into equal-sized blocks, and a custom Stack ADT is used to keep track of the blocks that are currently available.

When a block is needed, allocate() removes a block from the Stack and returns its address. When the block is no longer needed, deallocate() returns it to the Stack so it can be reused.

The program also demonstrates binary data storage, memory reuse, pool exhaustion, and protection against invalid or double deallocation.

## Stack Implementation

I used a dynamic array to implement the Stack.

The Stack stores its elements in a dynamically allocated array. When the array becomes full, a larger array is created and the existing elements are copied into it.

I chose this implementation because it keeps the Stack structure simple while still allowing it to grow when needed.

## How to Compile

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp MemoryPool.cpp -o buffer_pool
```

## How to Run

```bash
./buffer_pool
```

## Analysis Questions

1. Why is a Stack appropriate for managing the free blocks in this memory pool?

A Stack is appropriate because when a block is released, it can be pushed back onto the Stack and quickly reused by the next allocation.

2. What happens when the free-block Stack becomes empty?

When the free-block Stack is empty, there are no available memory blocks left in the pool. The allocate() function returns nullptr.

3. Why must a released block be returned to the Stack?

A released block must be returned to the Stack so that it becomes available for future allocations.

4. What problem could occur if the same block were deallocated twice?

If the same block were deallocated twice, the same memory address could be added to the free Stack more than once. This could cause the same block to be given to multiple allocations and lead to memory problems.

5. What is the Big-O time complexity of allocate()? Explain why.

The time complexity of allocate() is O(1) because it removes one block from the top of the Stack and updates its allocation status without searching through the pool.

6. What is the Big-O time complexity of deallocate()? Explain why.

The time complexity of deallocate() is O(1) because it validates the block, updates its allocation status and pushes it back onto the Stack without searching through all the blocks.
