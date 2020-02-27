#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

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
    void *data;
    int len;
    struct Node *next;
} Node;

int isDelim(char curr);
void printLL(void *head);

Node *getTailNode(Node *cur);
Node *qs_partition(Node *head, Node *end, Node **newHead, Node **newEnd, int (*comparator)(void *, void *));
Node *qs_recur(Node *head, Node *end, int (*comparator)(void *, void *));

int insertionSort(void *toSort, int (*comparator)(void *, void *));
int quickSort(void *toSort, int (*comparator)(void *, void *));

int insertionSortComparator(const void *a, const void *b);
int quickSortComparator(const void *a, const void *b);

int main(char *argc, char **argv) {

    //check if there are 2 arguments
    if (argc < 2) {
        printf("FATAL ERROR: not enough arguments were passed in, please specify sorting flag and file name\n");
        exit(0);
    }

    //check flag
    if (argv[1][0] != '-') {
        printf("FATAL ERROR: please pass in a flag for type of sort as the first argument\n");
        exit(0);
    }

    char flag = argv[1][1];
    if (flag != 'q' && flag != 'i') {
        printf("FATAL ERROR: please pass in either -i for insertion sort or -q for quick sort\n");
        exit(0);
    }

    // open file
    int fd = open(argv[2], O_RDONLY);

    if (fd == -1) {
        printf("FATAL ERROR: the file that was passed in does not exist\n");
        exit(0);
    }

    // TODO: check if tokens that are only whitespace should be dropped or kept at beginning

    // initialize LL here
    Node *head = malloc(sizeof(Node));
    Node *currPtr = head;

    char currChar;
    int index = 0;
    int num_bytes = 0;

    char *currToken = malloc(1000 * sizeof(char));
    int currTokenSize = 0;

    // type = 1 -> inputing ints
    // type = 2 -> inputing chars
    int type = 0;
    int *z;

    while (1) {
        num_bytes = read(fd, &currChar, 1);
        if (isDelim(currChar) || num_bytes == 0) {
            char *tempToken = malloc(currTokenSize * sizeof(char));

            int i = 0;
            for (i; i < currTokenSize; i++) {
                tempToken[i] = currToken[i];
            }

            if (isalpha(tempToken[0])) {
                char *tempStr = tempToken;
                currPtr->data = tempStr;
            } else {
                //We will handle conversions to ints in comparators. But, dynamically creating unique int pointer address' was creating unrealiable behavior
                char* tempStr = tempToken;
                currPtr->data = tempStr;
            }

            currPtr->len = currTokenSize;

            if (num_bytes == 0) {
                free(currToken);
                break;
            } else {
                free(currToken);
                char *currToken = malloc(1000 * sizeof(char));
                currTokenSize = 0;

                Node *temp = malloc(sizeof(Node));
                currPtr->next = temp;
                currPtr = temp;
            }
        } else {
            if (!isspace(currChar)) {
                currToken[currTokenSize] = currChar;
                currTokenSize++;
            }
        }
    }

    if (flag == 'i') {
        //insertion sort
        insertionSort((void *)head, insertionSortComparator);
    } else if (flag == 'q') {
        quickSort((void *)head, quickSortComparator);
    }

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

void printLL(void *head) {
    Node *curr = (Node *)head;
    while (curr != NULL) {

        /**
        * To retrieve the String a void* points to:
        *  - Cast to char*
        *  - i.e (char*)(some_void*_pointer)
        *
        *  To retrieve the int a void* points to:
        *  - Cast to int*
        *  - Dereference the int*
        *  - i.e. *(int*)(some_void*_pointer)
        */

        // printf("%d\n", *(int *)(curr->data));

        char *checkingChar = (char *)(curr->data);
        int i;
        int isCompletelyAlpha = 1;
        for (i = 0; i < curr->len; i++) {
            if (!isalpha(checkingChar[i])) {
                isCompletelyAlpha = 0;
                break;
            }
        }
        
        if (isCompletelyAlpha) {
            printf("%s\n", (char *)(curr->data));
        } else {
            char* str = (char*)(curr->data);
            printf("%d\n", atoi(str));
        }

        curr = curr->next;
    }

    printf("\n");
}

int insertionSortComparator(const void *a, const void *b) {
    if (isalpha(*(char *)a)) {
        //printf("list is made of chars %d\n", *(int *)a);
        char *t1 = (char *)a;
        char *t2 = (char *)b;

        while (*t1 != '\0') {
            if (*t2 == '\0')
                return 1;
            if (*t2 > *t1)
                return -1;
            if (*t1 > *t2)
                return 1;

            t1++;
            t2++;
        }

        if (*t2 != '\0')
            return -1;

        return 0;
    } else {
        printf("list is made of ints\n");
        int t1 = atoi((char*)a);
        int t2 = atoi((char*)b);

        return t1 - t2;
    }
}

int quickSortComparator(const void *a, const void *b) {
    if (isalpha(*(char *)a)) {
        char *t1 = (char *)a;
        char *t2 = (char *)b;

        while (*t1 != '\0') {
            if (*t2 == '\0')
                return 1;
            if (*t2 > *t1)
                return -1;
            if (*t1 > *t2)
                return 1;

            t1++;
            t2++;
        }

        if (*t2 != '\0')
            return -1;

        return 0;
    } else {
        int t1 = atoi((char*)a);
        int t2 = atoi((char*)b);

        return t1 - t2;
    }
}

int insertionSort(void *toSort, int (*comparator)(void *, void *)) {
    Node *head = (Node *)toSort;
    Node *dummy = malloc(sizeof(Node));
    int *tval = -1111111;
    dummy->data = &tval;
    Node *cur = head;
    Node *i = malloc(sizeof(Node));
    Node *tmp = malloc(sizeof(Node));
    while (cur != NULL) {
        i = dummy;

        while (i->next != NULL && comparator(i->next->data, cur->data) < 0) {
            i = i->next;
        }

        tmp = cur->next;
        cur->next = i->next;
        i->next = cur;
        cur = tmp;
    }

    void *t = dummy->next;
    printLL(t);

    free(dummy);
    free(tmp);
}

int quickSort(void *toSort, int (*comparator)(void *, void *)) {
    Node *head = (Node *)toSort;
    Node *newHead = malloc(sizeof(Node));

    newHead = qs_recur(head, getTailNode(head), comparator);
    printLL(newHead);
}

Node *getTailNode(Node *cur) {
    while (cur != NULL && cur->next != NULL) {
        cur = cur->next;
    }
    return cur;
}

Node *qs_partition(Node *head, Node *end, Node **newHead, Node **newEnd, int (*comparator)(void *, void *)) {
    Node *pivot = end;
    Node *prev = NULL, *cur = head, *tail = pivot;

    while (cur != pivot) {
        if (comparator(cur->data, pivot->data) < 0) {
            if ((*newHead) == NULL) {
                (*newHead) = cur;
            }

            prev = cur;
            cur = cur->next;
        } else {
            if (prev) {
                prev->next = cur->next;
            }

            Node *tmp = cur->next;
            cur->next = NULL;
            tail->next = cur;
            tail = cur;
            cur = tmp;
        }
    }

    if ((*newHead) == NULL) {
        (*newHead) = pivot;
    }

    (*newEnd) = tail;

    return pivot;
}

Node *qs_recur(Node *head, Node *end, int (*comparator)(void *, void *)) {
    if (!head || head == end)
        return head;

    Node *newHead = NULL, *newEnd = NULL;

    Node *pivot = qs_partition(head, end, &newHead, &newEnd, comparator);

    if (newHead != pivot) {
        Node *tmp = newHead;

        while (tmp->next != pivot) {
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
