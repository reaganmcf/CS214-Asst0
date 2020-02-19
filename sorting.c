#include<stdio.h>
#include<stdlib.h>

typedef struct Node {
  void* data;
  struct Node* next;
} Node;

void printLinkedList(Node* head);

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

  head->data = &value;
  n2->data = str2;
  n3->data = str3;
  n4->data = str4;

  head->next = n2;
  n2->next = n3;
  n3->next = n4;
  
  printLinkedList(head);

  return 0;
}

void printLinkedList(Node* head) {
  Node* curr = head;
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

    if(isalpha(*(char*)(curr->data))) { 
      printf("is alphabetical! %s\n", (char*)(curr->data));
    } else {
      printf("is digit! %d\n", *(int*)(curr->data));
    }
    
    curr = curr->next;
  }
  printf("\n");
}
