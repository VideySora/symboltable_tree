#include<iostream>
#include<stdlib.h>
using namespace std;

struct Node *phead;
 
struct Node{
    int data;
    Node *next;
};

void Insert(int data){
    Node *ptemp= new Node();
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
        ptemp= ptemp->next;
    }
    ptemp->next=pNew;
    
}

void Print(Node *head){
    if(head == nullptr){
        printf("\n");
        return;
    }
    printf("%d ",head->data);
    Print(head->next);
}

void ReversePrint(Node *head){
    if (head == nullptr)
    {
        return;
    }
    ReversePrint(head->next);
    printf("%d ",head->data);    // return to this
    return;
}

int main(){
    phead = nullptr;
    Insert(2);
    Insert(4);
    Insert(6);
    Insert(8);
    Insert(5);
    printf("Normal list:\n");
    Print(phead);
    printf("Reverse list:\n");
    ReversePrint(phead);
    return 0;
}