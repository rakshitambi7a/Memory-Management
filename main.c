#include <stdio.h>

#define MEMORY_SIZE 1000
#define MAX_PROCESSES 10

struct MemoryBlock {
    int size;
    int allocated;
};

struct Process {
    int size;
    int allocated_block;
};

// Function to initialize memory blocks
void initializeMemory(struct MemoryBlock memory[], int size) {
    for (int i = 0; i < size; i++) {
        memory[i].size = MEMORY_SIZE / size;
        memory[i].allocated = 0;
    }
}

// Function to perform First-Fit memory allocation for processes
void firstFit(struct MemoryBlock memory[], int num_blocks, struct Process processes[], int num_processes) {
    for (int i = 0; i < num_processes; i++) {
        for (int j = 0; j < num_blocks; j++) {
            if (!memory[j].allocated && memory[j].size >= processes[i].size) {
                memory[j].allocated = 1;
                processes[i].allocated_block = j;
                memory[j].size -= processes[i].size;
                break;
            }
        }
    }
}

// Function to deallocate memory blocks for completed processes
void deallocateMemory(struct MemoryBlock memory[], struct Process processes[], int num_processes) {
    for (int i = 0; i < num_processes; i++) {
        int block = processes[i].allocated_block;
        if (block != -1) {
            memory[block].allocated = 0;
            memory[block].size += processes[i].size;
            processes[i].allocated_block = -1;
        }
    }
}

int main() {
    struct MemoryBlock memory[MAX_PROCESSES];
    struct Process processes[MAX_PROCESSES];
    int num_blocks, num_processes, simulation_time;

    // User input: number of memory blocks, processes, and simulation time
    printf("Enter the number of memory blocks: ");
    scanf("%d", &num_blocks);
    printf("Enter the number of processes: ");
    scanf("%d", &num_processes);
    printf("Enter the simulation time (in time units): ");
    scanf("%d", &simulation_time);

    // Check for maximum limit of blocks and processes
    if (num_blocks > MAX_PROCESSES || num_processes > MAX_PROCESSES) {
        printf("Too many blocks or processes. Increase MAX_PROCESSES.\n");
        return 1;
    }

    // Initialize memory blocks
    initializeMemory(memory, num_blocks);

    int total_fragmentation = 0;
    int total_wasted_memory = 0;

    // Simulation loop for the specified time units
    for (int t = 1; t <= simulation_time; t++) {
        printf("\nTime Unit %d\n", t);
        // User input: size of processes
        for (int i = 0; i < num_processes; i++) {
            printf("Enter the size of process %d: ", i + 1);
            scanf("%d", &processes[i].size);
            processes[i].allocated_block = -1;
        }

        // Allocate memory for processes using First-Fit algorithm
        firstFit(memory, num_blocks, processes, num_processes);

        printf("\nMemory Allocation:\n");
        // Display allocated blocks for processes
        for (int i = 0; i < num_processes; i++) {
            printf("Process %d -> Block %d\n", i + 1, processes[i].allocated_block);
        }

        // Deallocate memory for completed processes
        deallocateMemory(memory, processes, num_processes);

        printf("\nMemory Deallocation:\n");
        // Display deallocated blocks for processes
        for (int i = 0; i < num_processes; i++) {
            printf("Process %d -> Block %d (Deallocated)\n", i + 1, processes[i].allocated_block);
        }

        // Calculate fragmentation and wasted memory
        int fragmentation = 0;
        int wasted_memory = 0;

        // Calculate fragmentation (gaps between allocated blocks)
        for (int i = 0; i < num_blocks; i++) {
            if (!memory[i].allocated) {
                wasted_memory += memory[i].size;
            }
        }

        // Calculate wasted memory (unallocated blocks)
        for (int i = 0; i < num_blocks; i++) {
            if (memory[i].allocated && i < num_blocks - 1 && !memory[i + 1].allocated) {
                fragmentation += memory[i].size;
            }
        }

        total_fragmentation += fragmentation;
        total_wasted_memory += wasted_memory;

        printf("\nFragmentation: %d\n", fragmentation);
        printf("Wasted Memory: %d\n", wasted_memory);
    }

    // Display average fragmentation and wasted memory over the simulation time
    printf("\nAverage Fragmentation: %.2f\n", (float)total_fragmentation / simulation_time);
    printf("Average Wasted Memory: %.2f\n", (float)total_wasted_memory / simulation_time);

    return 0;
}
