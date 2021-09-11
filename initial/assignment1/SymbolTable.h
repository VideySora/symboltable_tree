#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

struct symbol
{
    string value;
    string identifier;
    string type;
    symbol *next;
};

class SymbolTable
{
private:
    symbol *phead;
    symbol *ptail;

public:
    SymbolTable()
    {
        this->phead = nullptr;
        this->ptail = nullptr;
    };

    void run(string filename);
    void INSERT(string, string, string);
    void ASSIGN(string, string, string);
};

void SymbolTable::INSERT(string identifier, string type, string s)
{
    symbol *pNew = new symbol;
    pNew->identifier = identifier;
    pNew->type = type;
    pNew->next = nullptr;

    if (phead == nullptr)
    {
        phead = pNew;
        ptail = pNew;
        return;
    }

    symbol *ptemp = new symbol();
    ptemp = phead;
    while (ptemp != nullptr)
    {
        if (ptemp->identifier == pNew->identifier)
        {
            Redeclared e(s);
            throw e;
        }
        ptemp = ptemp->next;
    }

    ptail->next = pNew;
    ptail = pNew;
};

void SymbolTable::ASSIGN(string identifier, string val, string s)
{
    symbol *ptemp = new symbol();
    ptemp = phead;
    while (ptemp != nullptr)
    {
        if (ptemp->identifier == identifier)
        {
            ptemp->value = "";
            if (ptemp->type == "number")
            {
                for (int i = 0; i < val.size(); i++)
                {
                    if (val[i] > '9' || val[i] < '0')
                    {
                        TypeMismatch e(s);
                        throw e;
                    }
                }
                ptemp->value = val;
                return;
            }
            else if (ptemp->type == "string")
            {
                for (int i = 0; i < val.size(); i++)
                {
                    if (val[i] < 'A' || val[i] > 'z' || (val[i] > 'Z' && val[i] < 'a'))
                    {
                        TypeMismatch e(s);
                    }
                }
                ptemp->value += "'";
                ptemp->value += val;
                ptemp->value += "'";
                return;
            }
        }
        ptemp = ptemp->next;
    }
    Undeclared e(s);
    throw e;
}
#endif