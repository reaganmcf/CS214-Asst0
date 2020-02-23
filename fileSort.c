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
void printLL(void* head);

Node* getTailNode(Node* cur);
Node* qs_partition(Node* head, Node* end, Node** newHead, Node** newEnd, int (*comparator)(void*, void*));
Node* qs_recur(Node* head, Node* end, int (*comparator) (void*, void*));

int insertionSort(void* toSort, int (*comparator)(void*, void*));
int quickSort(void* toSort, int (*comparator)(void*, void*));

int insertionSortComparator(const void* a, const void* b);
int quickSortComparator(const void* a, const void* b);

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

    //TODO: check input flags to see if the user wants insertionSort or quickSort
    insertionSort((void*) head, insertionSortComparator);
    // quickSort((void*) head, quickSortComparator);


    //TODO: free mem
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

void printLL(void* head) {
    Node* currPtr = (Node*)head;
    while (currPtr != NULL) {
        printf("Data: %s\n", currPtr -> data);
        currPtr = currPtr -> next;
    }
}


int insertionSortComparator(const void* a, const void* b) { 
    if(isalpha(*(char*)a)) {
        char* t1 = (char*)a;
        char* t2 = (char*)b;

        while(*t1 != '\0') {
            if(*t2 == '\0') return 1;
            if(*t2 > *t1) return -1;
            if(*t1 > *t2) return 1;

            t1++;
            t2++;
        }

        if(*t2 != '\0') return -1;

        return 0;
    } else {
        int t1 = *(int*) a;
        int t2 = *(int*) b;

        return t1 - t2;
    }
}

int quickSortComparator(const void* a, const void* b) {
    if(isalpha(*(char*)a)) {
        char* t1 = (char*) a;
        char* t2 = (char*) b;

        while(*t1 != '\0') {
            if(*t2 == '\0') return 1;
            if(*t2 > *t1) return -1;
            if(*t1 > *t2) return 1;

            t1++;
            t2++;
        }

        if(*t2 != '\0') return -1;

        return 0;
    } else {
        int t1 = *(int*) a;
        int t2 = *(int*) b;

        return t1 - t2;
    }
}

int insertionSort(void* toSort, int (*comparator)(void*, void*)) {
    Node* head = (Node*)toSort;
    Node* dummy = malloc(sizeof(Node));
    int* tval = -1111111;
    dummy -> data = &tval;
    Node* cur = head;
    Node* i = malloc(sizeof(Node));
    Node* tmp = malloc(sizeof(Node));
    while(cur != NULL) {
        i = dummy;

        while(i -> next != NULL && comparator(i -> next -> data, cur -> data) < 0) {
            i = i -> next;
        }

        tmp = cur -> next;
        cur -> next = i -> next;
        i -> next = cur;
        cur = tmp;
    }

    void* t = dummy -> next;
    printLL(t);

    free(dummy);
    free(i);
    free(tmp);
}

int quickSort(void* toSort, int (*comparator)(void*, void*)) {
    Node* head = (Node*) toSort;
    Node* newHead = malloc(sizeof(Node));
    
    newHead = qs_recur(head, getTailNode(head), comparator);
    printLL(newHead);
}

Node* getTailNode(Node* cur) {
    while(cur != NULL && cur->next != NULL) {
        cur = cur->next;
    }
    return cur;
}

Node* qs_partition(Node* head, Node* end, Node** newHead, Node** newEnd, int (*comparator)(void*, void*)) {
    Node* pivot = end;
    Node* prev = NULL, *cur = head, *tail = pivot;

    while(cur != pivot) {
        if(comparator(cur->data, pivot->data) < 0) {
            if((*newHead) == NULL) {
                (*newHead) = cur;
            }
            prev = cur;
            cur = cur->next;
        } else {
            if(prev) {
                prev->next = cur->next;
            }
            Node* tmp = cur->next;
            cur->next = NULL;
            tail->next = cur;
            tail = cur;
            cur = tmp;
        }
    }

    if((*newHead) == NULL) {
        (*newHead) = pivot;
    }

    (*newEnd) = tail;

    return pivot;
}

Node* qs_recur(Node* head, Node* end, int (*comparator) (void*, void*)) {
    if(!head || head == end) return head;

    Node* newHead = NULL, *newEnd = NULL;

    Node* pivot = qs_partition(head, end, &newHead, &newEnd, comparator);

    if(newHead != pivot) {
        Node* tmp = newHead;
        while(tmp->next != pivot) {
            tmp = tmp->next;
        }
        tmp->next = NULL;

        newHead = qs_recur(newHead, tmp, comparator);

        tmp = getTailNode(newHead);
        tmp->next = pivot;
    }

    pivot->next = qs_recur(pivot->next, newEnd, comparator);

    return newHead;
}
