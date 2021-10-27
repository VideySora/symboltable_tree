#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"
class Node
{
public:
    string identifier="";
    string type="";
    string value="";
    int level;
    Node *pLeft;
    Node *pRight;
    friend class SymbolTable;

public:
    Node()
    {
        pLeft = nullptr;
        pRight = nullptr;
    };
    ~Node(){};
};

class SymbolTable // splaytree
{
public:
    Node *root;

public:
    SymbolTable() : root(nullptr){};

public:
    void run(string filename);

    void insert(Node *&, string, string, string, int);
    void assign();
    void lookup();
    void print();
    void rprint();

    void splay(Node *&);
    int compare(Node *, Node *);
    Node *getParent(Node *, Node *);
    Node *turnLeft(Node *&);
    Node *turnRight(Node *&);

    void print(Node *);
};

void SymbolTable::print(Node* root){
    if(root == nullptr){
        return;
    }
    cout<< root->identifier << "//"<< root->level<<"//" << root->type<< " ";
    print(root->pLeft);
    print(root->pRight);
}

int SymbolTable::compare(Node *node1, Node *node2)
{
    if (node1->level > node2->level)
    {
        return 1;
    }
    else if (node1->level < node2->level)
    {
        return -1;
    }
    else
    {
        if (!node1->identifier.compare(node2->identifier))
        {
            return 0;
        }
        else if (node1->identifier.compare(node2->identifier) < 0)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
}

void SymbolTable::insert(Node *&node, string identifier, string type, string s, int level)
{
    Node *pNew = new Node();
    pNew->identifier = identifier;
    pNew->type = type;
    pNew->pLeft = nullptr;
    pNew->pRight = nullptr;

    //base case
    if (!node || !root)
    {
        node = pNew;
        splay(node);
        return;
    }

    int cmp = compare(pNew, node);
    if (cmp == -1)
    {
        this->insert(node->pLeft, identifier, type, s, level);
    }
    else if (cmp == 1)
    {
        this->insert(node->pRight, identifier, type, s, level);
    }
    else
    {
        Redeclared e(s);
        throw e;
    }

}

Node *SymbolTable::getParent(Node *proot, Node *node)
{
    // if node is root
    if (proot == nullptr || proot == node)
    {
        return nullptr;
    }
    // if parent of that node is root
    if ((proot->pLeft != nullptr && proot->pLeft == node) || (proot->pRight != nullptr && proot->pRight == node))
    {
        return proot;
    }
    // traverse to the left
    Node *temp = getParent(proot->pLeft, node);
    if (temp == nullptr)
        return temp;
    // traverse to the right
    temp = getParent(proot->pRight, node);
    if (temp == nullptr){
        return temp;
    }
    return nullptr;
}

Node *SymbolTable::turnLeft(Node *&node)
{
    Node *temp = node->pRight;
    node->pRight = temp->pLeft;
    temp->pLeft = node;
    return temp;
}

Node *SymbolTable::turnRight(Node *&node)
{
    Node *temp = node->pLeft;
    node->pLeft = temp->pRight;
    temp->pRight = node;
    return temp;
}

void SymbolTable::splay(Node *&node)
{
    // get parent and grand parent
    Node *parent = getParent(root, node);
    Node *grandpa;
    Node *grandgrand;
    while (parent != nullptr)
    {
        cout << "hellow" <<endl;
        if (parent == root)
        {
            if (node == parent->pLeft)
            {
                root = turnRight(parent);
                return;
            }
            else
            {
                root = turnLeft(parent);
                return;
            }
        }
        else
        {
            parent = getParent(root, node);
            grandpa = getParent(root, parent);
            grandgrand = getParent(root, grandpa);

            int cmp1 = compare(node, parent);
            int cmp2 = compare(parent, grandpa);
            cout << cmp1 <<" "<<cmp2<<endl;

            if (cmp1 == -1 && cmp2 == -1) // L-L
            {
                cout<< "L-L"<<endl;
                if (grandgrand == nullptr)
                {
                    root = turnRight(grandpa);
                    root = turnRight(parent);
                }
                else if(compare(grandpa,grandgrand) == 1){
                    grandgrand->pLeft = turnRight(grandpa);
                    grandgrand->pLeft = turnRight(parent);
                }else if(compare(grandpa,grandgrand) == -1){
                    grandgrand->pRight = turnRight(grandpa);
                    grandgrand->pRight = turnRight(parent);
                }
            }
            else if (cmp1 == 1 && cmp2 == 1) // R-R
            {
                cout<< "R-R"<<endl;
                if (grandgrand == nullptr)
                {
                    root = turnLeft(grandpa);
                    root = turnLeft(parent);
                }
                else if(compare(grandpa,grandgrand) == 1){
                    grandgrand->pLeft = turnLeft(grandpa);
                    grandgrand->pLeft = turnLeft(parent);
                }else if(compare(grandpa,grandgrand) == -1){
                    grandgrand->pRight = turnLeft(grandpa);
                    grandgrand->pRight = turnLeft(parent);
                }
            }
            else if (cmp1 == 1 && cmp2 == -1) // L-R
            {
                cout<< "L-R"<<endl;
                if (grandgrand == nullptr)
                {
                    root = turnRight(grandpa);
                    root = turnLeft(parent);
                }
                else if(compare(grandpa,grandgrand) == 1){
                    grandgrand->pLeft = turnRight(grandpa);
                    grandgrand->pLeft = turnLeft(parent);
                }else if(compare(grandpa,grandgrand) == -1){
                    grandgrand->pRight = turnRight(grandpa);
                    grandgrand->pRight = turnLeft(parent);
                }
            }
            else if (cmp1 == -1 && cmp2 == 1) // R-L
            {
                cout<< "R-L"<<endl;
                if (grandgrand == nullptr)
                {
                    root = turnRight(grandpa);
                    root = turnRight(parent);
                }
                else if(compare(grandpa,grandgrand) == 1){
                    grandgrand->pLeft = turnLeft(grandpa);
                    grandgrand->pLeft = turnRight(parent);
                }else if(compare(grandpa,grandgrand) == -1){
                    grandgrand->pRight = turnLeft(grandpa);
                    grandgrand->pRight = turnRight(parent);
                }
            }
        }
    }
}
#endif