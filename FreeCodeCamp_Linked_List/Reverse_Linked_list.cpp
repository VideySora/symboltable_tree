#include <iostream>
#include <stdlib.h>
using namespace std;

struct Node *phead;
struct Node
{
    int data;
    Node *next;
};

void Insert(int data)
{
    Node *ptemp = new Node();
    Node *pNew = new Node();
    pNew->data = data;
    pNew->next = nullptr;

    ptemp = phead;
    if (phead == nullptr)
    {
        phead = pNew;
        return;
    }

    while (ptemp->next != nullptr)
    {
        ptemp = ptemp->next;
    }
    ptemp->next = pNew;
}

void Reverse()
{
    Node *pCurrent = phead;
    Node *pPre = nullptr;
    Node *pAft = nullptr;
    
    while (pCurrent != nullptr)
    {
        pAft = pCurrent->next;
        pCurrent->next = pPre;
        pPre = pCurrent;
        pCurrent = pAft;
    }
    phead = pPre;
}

void Print()
{
    Node *ptemp = new Node();
    ptemp = phead;
    printf("List is: ");
    while (ptemp != nullptr)
    {
        printf("%d ", ptemp->data);
        ptemp = ptemp->next;
    }
    printf("\n");
}

int main()
{
    phead = nullptr;
    Insert(2);
    // Insert(4);
    // Insert(6);
    // Insert(5);
    Print(); // list is 2 4 6 5

    Reverse();
    Print(); // list is 5 6 4 2
    return 0;
}