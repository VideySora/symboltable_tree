#include <iostream>
#include <stdio.h>
using namespace std;

struct Node *phead;

struct Node
{
    int data;
    struct Node *next;
};

void Insert(int data, int nth)
{
    Node *ptemp1 = new Node();
    ptemp1->data = data;
    ptemp1->next = nullptr;
    if (nth == 1)
    {
        ptemp1->next = phead;
        phead = ptemp1;
        return;
    }
    
    Node *ptemp2 = new Node();
    ptemp2 = phead;
    for (int i = 0; i < nth - 2; i++)
    {
        ptemp2 = ptemp2->next;
    }
    ptemp1->next=ptemp2->next;
    ptemp2->next=ptemp1;
}

void Print()
{
    Node *pTemp = new Node();
    pTemp = phead;
    printf("List is: ");
    while (pTemp != nullptr)
    {
        printf("%d ", pTemp->data);
        pTemp = pTemp->next;
    }
}

int main()
{
    phead = nullptr;
    Insert(2, 1);
    Insert(3, 2);
    Insert(4, 1);
    Insert(5, 2);
    Print();
}