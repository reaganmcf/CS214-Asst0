#include<stdio.h>
#include<stdlib.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#include<unistd.h>

// TOSHAN: 
//   - LL helper functions (maybe)
//   - reading cmd line input
//      - checking flags
//      - opening files
//   - add all tokens into linked list
//
// REAGAN:
//   - sorting
//      - quicksort
//      - insertion sort
//   -  comparators

typedef struct Node {
    void* data;
    struct Node* next;
} Node;

int main(char* argc, char** argv) {
    int fd = open(argv[1], O_RDONLY);
    
}