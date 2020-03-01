#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct Node
{
    void *data;
    int len;
    struct Node *next;
} Node;

int isDelim(char curr);
void printLL(void *head);

Node *getTailNode(Node *cur);
Node *quickSortPartition(Node *head, Node *end, Node **newHead, Node **newEnd, int (*comparator)(void *, void *));
Node *qucikSortRecurisve(Node *head, Node *end, int (*comparator)(void *, void *));

void insertionSort(void *toSort, int (*comparator)(void *, void *));
void quickSort(void *toSort, int (*comparator)(void *, void *));

int insertionSortComparator(const void *a, const void *b);
int quickSortComparator(const void *a, const void *b);

int main(char *argc, char **argv)
{

    //check if there are 2 arguments
    if (argc < 2)
    {
        printf("FATAL ERROR: not enough arguments were passed in, please specify sorting flag and file name\n");
        exit(0);
    }

    //check flag
    if (argv[1][0] != '-')
    {
        printf("FATAL ERROR: please pass in a flag for type of sort as the first argument\n");
        exit(0);
    }

    char flag = argv[1][1];
    if (flag != 'q' && flag != 'i')
    {
        printf("FATAL ERROR: please pass in either -i for insertion sort or -q for quick sort\n");
        exit(0);
    }

    // open file
    int fd = open(argv[2], O_RDONLY);

    if (fd == -1)
    {
        printf("FATAL ERROR: the file that was passed in does not exist\n");
        exit(0);
    }

    // initialize LL here
    Node *head = malloc(sizeof(Node));
    Node *currPtr = head;

    char currChar;
    int index = 0;
    int num_bytes = 0;

    char *currToken = malloc(1000 * sizeof(char));
    int currTokenSize = 0;

    while (1)
    {
        num_bytes = read(fd, &currChar, 1);
        if (isDelim(currChar) || num_bytes == 0)
        {
            if (num_bytes == 0 && currPtr == head)
            {
                printf("WARNING: The file that was given is empty\n");
                free(currToken);
                free(head);
                close(fd);
                return 0;
            }

            if (currTokenSize == 0)
            {
                currTokenSize = 1;
                currToken[0] = ' ';
            }

            char *tempToken = malloc(currTokenSize * sizeof(char));

            int i = 0;
            for (i; i < currTokenSize; i++)
            {
                tempToken[i] = currToken[i];
            }

            //We will handle conversions to ints in comparators.
            //But, dynamically creating unique int pointer address' was creating unrealiable behavior
            char *tempStr = tempToken;
            currPtr->data = tempStr;

            currPtr->len = currTokenSize;

            if (num_bytes == 0)
            {
                free(currToken);
                break;
            }
            else
            {
                free(currToken);
                char *currToken = malloc(1000 * sizeof(char));
                currTokenSize = 0;

                Node *temp = malloc(sizeof(Node));
                currPtr->next = temp;
                currPtr = temp;
            }
        }
        else
        {
            if (!isspace(currChar))
            {
                currToken[currTokenSize] = currChar;
                currTokenSize++;
            }
        }
    }

    if (flag == 'i')
    {
        //insertion sort
        insertionSort((void *)head, insertionSortComparator);
    }
    else if (flag == 'q')
    {
        //quick sort
        quickSort((void *)head, quickSortComparator);
    }

    //TODO: free mem
    Node *curr = (Node *)head;
    while (curr != NULL)
    {
        Node *temp = curr;
        curr = curr->next;

        free(temp->data);
        free(temp);
    }

    //free(curr);
    free(head);
    close(fd);

    return 0;
}

// checks if the current character is a delimiter
int isDelim(char curr)
{
    if (curr == ',')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void printLL(void *head)
{
    Node *curr = (Node *)head;
    while (curr != NULL)
    {

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
        for (i = 0; i < curr->len; i++)
        {
            if (!isalpha(checkingChar[i]))
            {
                isCompletelyAlpha = 0;
                break;
            }
        }

        if (checkingChar[0] == ' ')
        {
            printf(" \n");
        }
        else
        {

            if (isCompletelyAlpha)
            {
                printf("%s\n", (char *)(curr->data));
            }
            else
            {
                char *str = (char *)(curr->data);
                printf("%d\n", atoi(str));
            }
        }

        curr = curr->next;
    }
}

int insertionSortComparator(const void *a, const void *b)
{
    if (*(char *)a == ' ' && *(char *)b == ' ')
    {
        return 0;
    }
    else if (*(char *)a == ' ')
    {
        return -1;
    }
    else if (*(char *)b == ' ')
    {
        return 1;
    }

    char *checkingChar = (char *)a;
    int i;
    int isCompletelyAlpha = 1;
    for (i = 0; i < strlen(a); i++)
    {
        if (!isalpha(checkingChar[i]))
        {
            isCompletelyAlpha = 0;
            break;
        }
    }

    if (isCompletelyAlpha)
    {
        //printf("list is made of chars %d\n", *(int *)a);
        char *t1 = (char *)a;
        char *t2 = (char *)b;

        while (*t1 != '\0')
        {
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
    }
    else
    {
        //printf("list is made of ints\n");
        int t1 = atoi((char *)a);
        int t2 = atoi((char *)b);

        return t1 - t2;
    }
}

int quickSortComparator(const void *a, const void *b)
{
    if (*(char *)a == ' ' && *(char *)b == ' ')
    {
        return 0;
    }
    else if (*(char *)a == ' ')
    {
        return -1;
    }
    else if (*(char *)b == ' ')
    {
        return 1;
    }

    char *checkingChar = (char *)a;
    int i;
    int isCompletelyAlpha = 1;
    for (i = 0; i < strlen(a); i++)
    {
        if (!isalpha(checkingChar[i]))
        {
            isCompletelyAlpha = 0;
            break;
        }
    }

    if (isCompletelyAlpha)
    {
        char *t1 = (char *)a;
        char *t2 = (char *)b;

        while (*t1 != '\0')
        {
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
    }
    else
    {
        int t1 = atoi((char *)a);
        int t2 = atoi((char *)b);

        return t1 - t2;
    }
}

void insertionSort(void *toSort, int (*comparator)(void *, void *))
{
    Node *head = (Node *)toSort;
    Node *dummy = malloc(sizeof(Node));
    int *tval = -1111111;
    dummy->data = &tval;
    Node *curr = head;
    Node *i = dummy;
    Node *tempNode = malloc(sizeof(Node));
    while (curr != NULL)
    {
        i = dummy;

        while (i->next != NULL && comparator(i->next->data, curr->data) < 0)
        {
            i = i->next;
        }

        tempNode = curr->next;
        curr->next = i->next;
        i->next = curr;
        curr = tempNode;
    }

    void *t = dummy->next;
    printLL(t);

    free(dummy);
    free(tempNode);
}

void quickSort(void *toSort, int (*comparator)(void *, void *))
{
    Node *head = (Node *)toSort;
    Node *newHead = qucikSortRecurisve(head, getTailNode(head), comparator);
    printLL(newHead);
}

Node *getTailNode(Node *cur)
{
    while (cur != NULL && cur->next != NULL)
    {
        cur = cur->next;
    }

    return cur;
}

Node *quickSortPartition(Node *head, Node *end, Node **newHeadNode, Node **newEndNode, int (*comparator)(void *, void *))
{
    Node *pivot = end;
    Node *curr = head;
    Node *tail = pivot;
    Node *prev = NULL;

    while (curr != end)
    {
        if (comparator(curr->data, pivot->data) < 0)
        {
            if (*newHeadNode == NULL)
                *newHeadNode = curr;

            prev = curr;
            curr = curr->next;
        }
        else
        {
            if (prev)
                prev->next = curr->next;

            Node *tempNode = curr->next;
            curr->next = NULL;
            tail->next = curr;
            tail = curr;
            curr = tempNode;
        }
    }

    if (*newHeadNode == NULL)
        *newHeadNode = pivot;

    *newEndNode = tail;

    return pivot;
}

Node *qucikSortRecurisve(Node *head, Node *end, int (*comparator)(void *, void *))
{
    if (head == NULL || head == end)
        return head;

    Node *newHeadNode = NULL;
    Node *newEndNode = NULL;

    Node *pivot = quickSortPartition(head, end, &newHeadNode, &newEndNode, comparator);

    if (newHeadNode != pivot)
    {
        Node *tempNode = newHeadNode;
        while (tempNode->next != pivot)
            tempNode = tempNode->next;

        tempNode->next = NULL;

        newHeadNode = qucikSortRecurisve(newHeadNode, tempNode, comparator);

        tempNode = getTailNode(newHeadNode);
        tempNode->next = pivot;
    }

    pivot->next = qucikSortRecurisve(pivot->next, newEndNode, comparator);

    return newHeadNode;
}