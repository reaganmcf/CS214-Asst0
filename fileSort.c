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
    int len;
    struct Node* next;
} Node;

int isDelim(char curr);
void printLL(Node* head);

int main(char* argc, char** argv) {
    char flag = argv[1][1];

    int fd = open(argv[2], O_RDONLY);

    // initialize LL here
    Node* head = malloc(sizeof(Node));
    Node* currPtr = head;

    char currChar;
    int index = 0;
    int num_bytes = 0;

    char* currToken = malloc(1000 * sizeof(char));
    int currTokenSize = 0;

    while(1) {
        num_bytes = read(fd, &currChar, 1);
        if (isDelim(currChar) || num_bytes == 0) {
            char* tempToken = malloc(currTokenSize * sizeof(char));

            int i = 0;
            for (i; i < currTokenSize; i++) {
                tempToken[i] = currToken[i];
            }

            currPtr -> data = tempToken;
            currPtr -> len = currTokenSize;

            if (num_bytes == 0) {
                free(currToken);
                break;
            } else {
                free(currToken);
                char* currToken = malloc(1000 * sizeof(char));
                currTokenSize = 0;

                Node* temp = malloc(sizeof(Node));
                currPtr -> next = temp;
                currPtr = temp;
            }
        } else {
            if (!isspace(currChar)) {
                currToken[currTokenSize] = currChar;
                currTokenSize++;
            }
        }
    }

    printLL(head);

    currPtr = head;
    while (currPtr != NULL) {
        Node* temp = currPtr;
        currPtr = currPtr -> next;
        free(temp -> data);
        free(temp);
    }

    return 0;
}

// checks if the current character is a delimiter
int isDelim(char curr) {
    if (curr == ',') {
        return 1;
    } else {
        return 0;
    }
}

void printLL(Node* head) {
    Node* currPtr = head;
    while (currPtr != NULL) {
        printf("Data: %s\n", currPtr -> data);
        currPtr = currPtr -> next;
    }
}