# Algorithm Analysis

This repository contains homework assignments for the Algorithm Analysis course. Each assignment demonstrates a different algorithmic concept implemented in C.

---

## HW1 – Hybrid Hash Table

**Folder:** `HW1-Hybrid-Hash-Table/`

### Overview

Implements a **hybrid collision-resolution hash table** that combines two strategies:

1. **Linear Probing** – When inserting an element, up to `k` consecutive slots are probed.
2. **Separate Chaining** – If all `k` probes fail (slots are occupied), the element is appended to a linked list attached to the original hash index.

The hash function used is:

```
h(x) = x mod m
```

where `m` is the smallest prime number greater than or equal to `n / 10` (n = total number of user IDs).

### Features

- Reads user IDs from an input file (`input.txt` / `input2.txt`)
- Tests multiple `k` values: `1, 5, 10, 25, 100, 500, 1000`
- Reports **minimum**, **average**, and **maximum** probe counts for each `k`

### How to Compile & Run

```bash
cd HW1-Hybrid-Hash-Table
gcc -o hash 23011503.c
./hash
```

You will be prompted to enter a `k` value. Make sure `input2.txt` is present in the same directory.

---

## HW2 – Server Allocation with Backtracking

**Folder:** `HW2-Server-Allocation-Backtracking/`

### Overview

Implements a **backtracking algorithm** to allocate servers into cabins under the following constraints:

1. **Power Capacity** – The total wattage of servers in a cabin must not exceed `maxCapacity`.
2. **Heat Ratio** – The ratio of total heat to total power in each cabin must not exceed a threshold `k`.
3. **Conflict Constraint** – Certain pairs of servers cannot be placed in the same cabin (defined by a conflict matrix).

The algorithm explores all possible placements recursively and stops at the first valid solution found.

### Features

- Interactive input: number of servers, cabins, capacity, heat threshold, server specs, and conflict pairs
- Displays the conflict matrix before searching
- Prints the cabin assignment for each server when a valid solution is found

### How to Compile & Run

```bash
cd HW2-Server-Allocation-Backtracking
gcc -o server_alloc 23011503.c
./server_alloc
```

**Example interaction:**

```
SERVERS ALLOCATION
Servers number: 4
Cabin number: 2
Cabin watt Capacity: 120
k value: 2.0

Server infos:
 Server[1] Power: 60  Heat: 120
 Server[2] Power: 50  Heat: 40
 Server[3] Power: 40  Heat: 30
 Server[4] Power: 30  Heat: 60

--- Conflict info ---
Conflict num: 1
Conflict 1 (S1 S2): 1 3
```

---

## Repository Structure

```
Algorithm-Analysis/
├── HW1-Hybrid-Hash-Table/
│   ├── 23011503.c       # Hybrid hash table implementation
│   ├── 23011503.pdf     # Assignment report
│   ├── input.txt        # Sample input
│   └── input2.txt       # Sample input (larger dataset)
├── HW2-Server-Allocation-Backtracking/
│   ├── 23011503.c       # Backtracking server allocation implementation
│   └── 23011503.pdf     # Assignment report
└── README.md
```

---

## Author

Student ID: **23011503**
