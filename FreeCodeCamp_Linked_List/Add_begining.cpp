/*
Insert a node at beginning of the list
*/

#include <iostream>
#include <stdio.h>
using namespace std;

struct Node *phead; // global variable, can be access everywhere

struct Node
{
    int data;
    struct Node *next;
};

void Insert(int x)
{
    Node *pTemp = new Node();
    pTemp->data = x;
    if( phead != nullptr){
        pTemp->next= phead;
    }
    phead=pTemp;
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
    int n, x;
    printf("Enter a number of nodes in Linked list: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        printf("Enter the number \n");
        scanf("%d ", &x);
        Insert(x);
        Print();
    }
}