// Delete the node at nth position
#include<iostream>
#include<stdlib.h>
using namespace std;

struct Node* phead;

struct Node{
    int data;
    Node* next;
};

void Insert(int data){
    Node *pTemp1 = new Node();
    Node *pNew = new Node();
    pNew->next = nullptr;
    pNew->data = data;

    if(phead == nullptr){
        phead=pNew;
        return;
    }

    pTemp1=phead;   
    while ( pTemp1->next != nullptr)
    {
        pTemp1=pTemp1->next;
    }
    pTemp1->next=pNew;
    
}

void Print(){
    Node *pTemp = new Node();
    pTemp = phead;
    printf("List is: ");
    while(pTemp != nullptr){
        printf("%d ", pTemp->data);
        pTemp= pTemp->next;
    }
    printf("\n");
}

void Delete(int n){
    Node *pTemp1 =new Node();
    pTemp1 = phead;
    if(n==1){
        phead = pTemp1->next;    // move head pointer to the second
        free(pTemp1);            // delete pTemp1;
        return;
    }

    for (int i = 0; i < n-2; i++)
    {
        pTemp1= pTemp1->next;
    }
    // Node* ptemp2= new Node();
    // ptemp2=pTemp1->next;
    pTemp1->next=pTemp1->next->next;
    // free(ptemp2);
}

int main(){
    phead = nullptr;
    Insert(2);
    Insert(4);
    Insert(6);
    Insert(5); 
    Print();    // list : 2 4 6 5

    int n;
    printf("Enter a position \n");
    scanf("%d", &n);
    Delete(n);
    Print();
}