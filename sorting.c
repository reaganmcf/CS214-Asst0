#include<stdio.h>
#include<stdlib.h>

typedef struct Node {
    void* data;
    struct Node* next;
} Node;

void printLinkedList(void* head);

int insertionSort(void* toSort, int (*comparator)(void*, void*));
int quickSort(void* toSort, int (*comparator)(void*, void*));

int insertionSortComparator(const void* a, const void* b);

int main(char* argc, char** argv) {

    printf("sorting started\n");

    Node* head = malloc(sizeof(Node));
    Node* n2 = malloc(sizeof(Node));
    Node* n3 = malloc(sizeof(Node));
    Node* n4 = malloc(sizeof(Node));
  
    char* str1 = "hello";
    char* str2 = "my";
    char* str3 = "name";
    char* str4 = "reagan";

    int* value = (int*) 5;
    int* value2 = (int*) -10;
    int* value3 = (int*) 12;
    int* value4 = (int*) 3;


    //FOR STRINGS
    head -> data = str1;
    n2 -> data = str4;
    n3 -> data = str3;
    n4 -> data = str2;

    //FOR INTEGERS
    head -> data = &value;
    n2 -> data = &value2;
    n3 -> data = &value3;
    n4 -> data = &value4;

    head -> next = n2;
    n2 -> next = n3;
    n3 -> next = n4;

    void* voidhead = head;
  
    printLinkedList(voidhead);
    insertionSort(voidhead, insertionSortComparator);

    return 0;
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
        int t1 = *(int*)a;
        int t2 = *(int*)b;

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
    printLinkedList(t);

    free(dummy);
    free(i);
    free(tmp);
}


void printLinkedList(void* head) {
    Node* curr = (Node*)head;
    while(curr != NULL) {

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

        if(isalpha(*(char*)(curr -> data))) { 
            printf("is alphabetical! %s\n", (char*)(curr -> data));
        } else {
            printf("is digit! %d\n", *(int*)(curr -> data));
        }

        curr = curr -> next;
    }

    printf("\n");
}