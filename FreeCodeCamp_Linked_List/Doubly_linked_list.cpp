#include <iostream>
#include <stdlib.h>
using namespace std;

struct Node *phead;
struct Node *ptail;

struct Node
{
    int data;
    Node *next;
    Node *previous;
};

void Insert_at_end(int data)
{
    Node *pNew = new Node();
    pNew->data = data;
    pNew->next = nullptr;
    pNew->previous = nullptr;

    if (phead == nullptr)
    {
        phead = pNew;
        ptail->next = pNew;
        ptail = pNew;
        return;
    }

    ptail->next = pNew;
    pNew->previous = ptail;
    ptail = pNew;
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
    cout << endl;
    ptemp = ptail;
    printf("Reverse List is: ");
    while (ptemp != nullptr)
    {
        printf("%d ", ptemp->data);
        ptemp = ptemp->previous;
    }
    cout << endl;
}

void Insert_at_nth(int data, int position)
{
    Node *pNew = new Node();
    pNew->data = data;
    pNew->previous = nullptr;
    pNew->next = nullptr;

    if (position <= 1)
    {
        pNew->next = phead;
        phead = pNew;
        return;
    }

    Node *ptemp1 = new Node();
    ptemp1 = phead;
    for (int i = 1; i < position - 1; i++)
    {
        ptemp1 = ptemp1->next;
    }

    Node *ptemp2 = new Node();
    ptemp2 = ptemp1->next;

    ptemp2->previous = pNew;
    ptemp1->next = pNew;
    pNew->next = ptemp2;
    pNew->previous = ptemp1;
}

int main()
{
    phead = nullptr;
    ptail = nullptr;
    Insert_at_end(2);
    Insert_at_end(4);
    Insert_at_end(6);
    Insert_at_end(8);
    Insert_at_end(5);
    Print();
    Insert_at_nth(3,1);
    Insert_at_nth(7,5);
    Print();
}