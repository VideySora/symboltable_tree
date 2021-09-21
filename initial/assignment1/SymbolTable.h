#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

struct symbol
{
    string value = "";
    string identifier;
    string type;
    int level;
    symbol *next;
    symbol *prev;
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
    ~SymbolTable(){
        symbol *ptemp = new symbol();
        
        while (phead != nullptr)
        {
            phead = phead->next;
            ptemp = phead->prev;
            delete ptemp;
            ptemp=phead;
        }

        delete ptail;
    }
public:
    void run(string filename);
    void INSERT(string, string, string, int);
    void ASSIGN(string, string, string, int);
    void INSERT_BEGIN(string, int);
    void LOOKUP(string, string, int);
    void PRINT(int);
    void RPRINT(int);

public:
    int lookup(string, int);
};

void SymbolTable::INSERT(string identifier, string type, string s, int level)
{
    symbol *pNew = new symbol;
    pNew->identifier = identifier;
    pNew->type = type;
    pNew->next = nullptr;
    pNew->prev = nullptr;
    pNew->level = level;

    if (phead == nullptr)
    {
        phead = pNew;
        ptail = pNew;
        return;
    }

    symbol *ptemp = new symbol();

    //check error
    ptemp = ptail;
    while (ptemp != nullptr)
    {
        if (ptemp->identifier == pNew->identifier && ptemp->level == pNew->level)
        {
            Redeclared e(s);
            throw e;
        }

        if (ptemp->identifier == "BEGIN" && ptemp->level == pNew->level)
        {
            break;
        }
        ptemp = ptemp->prev;
    }

    //add node
    ptail->next = pNew;
    pNew->prev = ptail;
    ptail = pNew;
};

void SymbolTable::ASSIGN(string identifier, string val, string s, int level)
{
    symbol *ptemp = ptail;

    int leveltemp;
    leveltemp = level;

    while (ptemp != nullptr)
    {
        if (ptemp->identifier == identifier && ptemp->level == leveltemp)
        {
            ptemp->value = "";
            // value is number or string

            if (ptemp->type == "number")
            {
                //run list check declared variable
                symbol *ptemp2 = new symbol();
                ptemp2 = phead;
                while (ptemp2 != nullptr)
                {
                    if (ptemp2->identifier == val)
                    {
                        val = ptemp2->value;
                        break;
                    }
                    ptemp2 = ptemp2->next;
                }

                //check for error
                for (int i = 0; i < val.size() - 1; i++)
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
                //run list for declared variable
                symbol *ptemp2 = new symbol();
                ptemp2 = phead;
                while (ptemp2 != nullptr)
                {
                    if (ptemp2->identifier == val)
                    {
                        val = ptemp2->value;
                        break;
                    }
                    ptemp2 = ptemp2->next;
                }

                //check for error (char in string can only be 1,..9 , " ", A to z)
                if (val[0] == '\'' && val[val.size() - 1] == '\'')
                {
                    for (int i = 1; i < val.size() - 2; i++)
                    {
                        if (val[i] < '0' && val[i] != ' ')
                        {
                            TypeMismatch e(s);
                            throw e;
                        }
                        else if (val[i] > '9')
                        {
                            if (val[i] < 'A' || val[i] > 'z' || (val[i] > 'Z' && val[i] < 'a'))
                            {
                                TypeMismatch e(s);
                                throw e;
                            }
                        }
                    }
                    ptemp->value += val;
                    return;
                }
                else
                {
                    TypeMismatch e(s);
                    throw e;
                }
            }
        }

        if (ptemp->identifier == "BEGIN" && ptemp->level == leveltemp)
        {
            leveltemp--;
        }

        ptemp = ptemp->prev;
    }
    Undeclared e(s);
    throw e;
}

void SymbolTable::INSERT_BEGIN(string begin, int level)
{
    symbol *pNew = new symbol();
    pNew->identifier = begin;
    pNew->level = level;
    pNew->next = nullptr;
    pNew->prev = nullptr;

    if (phead == nullptr)
    {
        phead = pNew;
        ptail = pNew;
        return;
    }
    ptail->next = pNew;
    pNew->prev = ptail;
    ptail = pNew;
}

void SymbolTable::LOOKUP(string identifier, string s, int level)
{
    symbol *ptemp = new symbol();
    ptemp = ptail;
    int leveltemp = level;
    while (ptemp != nullptr)
    {
        if (ptemp->identifier == identifier && ptemp->level == leveltemp)
        {
            cout << leveltemp << endl;
            return;
        }
        if (ptemp->identifier == "BEGIN" && ptemp->level == leveltemp)
        {
            leveltemp--;
        }
        ptemp = ptemp->prev;
    }
    Undeclared e(s);
    throw e;
}
int SymbolTable::lookup(string identifier, int level)
{
    symbol *ptemp = ptail;
    int leveltemp = level;
    while (ptemp != nullptr)
    {
        if (ptemp->identifier == identifier && ptemp->level == leveltemp)
        {
            return leveltemp;
        }
        if (ptemp->identifier == "BEGIN" && ptemp->level == leveltemp)
        {
            leveltemp--;
        }
        ptemp = ptemp->prev;
    }
    return -1;
}

void SymbolTable::PRINT(int level)
{
    symbol *ptemp = ptail;
    int leveltemp = level;
    string result = "";

    while (ptemp != nullptr)
    {
        if (ptemp->identifier != "BEGIN" && ptemp->level == lookup(ptemp->identifier, level))
        {
            result = ptemp->identifier + "//" + to_string(leveltemp) + " " + result;
        }

        if (ptemp->identifier == "BEGIN" && ptemp->level == leveltemp)
        {
            leveltemp--;
        }
        ptemp = ptemp->prev;
    }

    result = result.substr(0, result.size() - 1);
    cout << result << endl;
}

void SymbolTable::RPRINT(int level)
{
    symbol *ptemp = ptail;
    int leveltemp = level;
    string result = "";

    while (ptemp != nullptr)
    {
        if (ptemp->identifier != "BEGIN" && ptemp->level == lookup(ptemp->identifier, level))
        {
            result = result + ptemp->identifier + "//" + to_string(leveltemp) + " ";
        }

        if (ptemp->identifier == "BEGIN" && ptemp->level == leveltemp)
        {
            leveltemp--;
        }
        ptemp = ptemp->prev;
    }

    result = result.substr(0, result.size() - 1);
    cout << result << endl;
}
#endif
