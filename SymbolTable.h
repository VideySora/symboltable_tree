#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"
class Node
{
public:
    string identifier = "";
    string type = "";
    string value = "";
    int level = 0;
    Node *pLeft;
    Node *pRight;
    friend class SymbolTable;

public:
    ~Node(){};
};

class SymbolTable // splaytree
{
public:
    Node *root;
    int count_splay = 0;
    int count_compare = 0;

public:
    SymbolTable() : root(nullptr){};
    ~SymbolTable()
    {
        this->deleteRecursively(this->root);
    }

public:
    void run(string filename);

    void insert(Node *, string, string, string, int);
    void assign(string, string, string, int);
    void lookup(string, string, int);
    void print_tree(Node *);
    void delete_end(int level);
    void delete_node(Node *&);

    void splay(Node *&);
    int compare(Node *, Node *);
    Node *getParent(Node *, Node *);
    Node *turnLeft(Node *);
    Node *turnRight(Node *);

    void print(Node *);
    Node *lookup_level(Node *);
    Node *getMaxLeftSB(Node *);
    Node *lookup_node(string, int);
    string getTypeFromValue(string, int, string);
    void deleteRecursively(Node *);
    void get_print_PO(Node *, string &);
};

#endif