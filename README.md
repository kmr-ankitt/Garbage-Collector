# Simple C-Based Garbage Collector

This project implements a basic garbage collector in C to automate memory management, improving efficiency and reducing the risk of leaks.

## Features

- Automatic memory reclamation for dynamically allocated objects.
- Reduced memory consumption by freeing unused memory.
- Lower risk of memory leaks compared to manual management.

## Getting Started

### Prerequisites:

- A C compiler (e.g., GCC)

### Build Instructions:

1. Clone this repository.
2. Navigate to the project directory in your terminal.
3. Compile the source code:
    ```bash
    cd Garbage-collector
    g++ src/main.c -o main
    ```
    Use code with caution.
4. Run the program:
    ```bash
    ./main.out
    ```
    Use code with caution.

## Example Usage

The provided `main.c` demonstrates basic usage of the garbage collector. It allocates memory dynamically and triggers garbage collection when necessary.

## Implementation Details

This garbage collector employs a mark-and-sweep algorithm:

- **Marking:** It traverses the program's execution state (stack) to identify reachable objects (objects directly or indirectly referenced by the program).
- **Sweeping:** It iterates through all allocated objects. Unmarked objects, considered unreachable, are freed.

### Further Enhancements:

- Integration with standard memory allocation functions (`malloc` and `free`).
- Support for a wider range of data structures.
- Error handling for memory allocation failures.
- Multithreading considerations (if applicable).

## Author

Ankit Kumar Sahu<br>
Thanks to Robert Nystrom !!

**Note:**
This is a basic implementation for educational purposes. For production use, consider well-tested and mature garbage collection libraries.
