#include "SymbolTable.h"

regex insert_normal("(INSERT) ([a-z][a-zA-Z0-9_]*) (number|string) (true|false)");
regex insert_function("(INSERT) ([a-z][a-zA-Z0-9_]*) \\(((number|string)(,number|,string)*)?\\)->(string|number) (true|false)");
regex assign_value("(ASSIGN) ([a-z][a-zA-Z0-9_]*) (.*)");
regex lookup_exp("(LOOKUP) ([a-z][a-zA-Z0-9_]*)");
regex begin_block("BEGIN");
regex end_block("END");
regex print_exp("PRINT");

regex value_number(("[0-9]+"));
regex value_string("\'[a-zA-Z0-9 ]*\'");
regex value_function("([a-z][a-zA-Z0-9_]*)\\((([0-9]*|\\'[a-zA-Z0-9 ]*\\'|[a-z][a-zA-Z0-9_]*)(,[0-9]*|,\\'[a-zA-Z0-9 ]*\\'|,[a-z][a-zA-Z0-9_]*)*)?\\)");
regex value_variable("[a-z][a-zA-Z0-9_]*");

regex type_function("((number|string)(,number|,string)*)?->(string|number)");

void SymbolTable::run(string filename)
{
    string s;
    smatch m;
    int level = 0;
    ifstream myfile(filename);
    if (myfile.is_open())
    {
        while (getline(myfile, s))
        {
            if (regex_match(s, m, insert_normal))
            {
                string name = m.str(2), type = m.str(3), global = m.str(4);
                if (global == "true")
                {
                    this->insert(this->root, name, type, s, 0);
                }
                else
                {
                    this->insert(this->root, name, type, s, level);
                }
            }
            else if (regex_match(s, m, insert_function))
            {
                string name = m.str(2), type = m.str(3) + "->" + m.str(m.size() - 2), global = m.str(m.size() - 1);
                if (global == "true")
                {
                    this->insert(this->root, name, type, s, 0);
                }
                else if (global == "false" && level == 0)
                {
                    this->insert(this->root, name, type, s, level);
                }
                else
                {
                    InvalidDeclaration e(s);
                    throw e;
                }
            }
            else if (regex_match(s, m, lookup_exp))
            {
                string name = m.str(2);
                this->lookup(name, s, level);
            }
            else if (regex_match(s, m, begin_block))
            {
                level++;
            }
            else if (regex_match(s, m, end_block))
            {
                this->delete_end(level);
                level--;
            }
            else if (regex_match(s, m, assign_value))
            {
                string name = m.str(2), value = m.str(3);
                this->assign(name, value, s, level);
            }
            else if (regex_match(s, m, print_exp))
            {
                this->print_tree(this->root);
            }
            else
            {
                InvalidInstruction e(s);
                throw e;
            }

            // check valid end
            if (level < 0)
            {
                UnknownBlock e;
                throw e;
            }

            // this->print(root);
            // cout<<"  root is: "<<root->identifier<<endl;
            // cout<<endl;
        }
        myfile.close();
    }

    if (level > 0)
    {
        UnclosedBlock e(level);
        throw e;
    }
}

///////////////////////////////////////////////////////////////////////

void SymbolTable::print(Node *root)
{
    if (root == nullptr)
    {
        return;
    }
    cout << root->identifier << "//" << root->level << "//" << root->type << " ";
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

void SymbolTable::insert(Node *root, string identifier, string type, string s, int level)
{
    Node *pNew = new Node();
    pNew->identifier = identifier;
    pNew->type = type;
    pNew->pLeft = nullptr;
    pNew->pRight = nullptr;
    pNew->level = level;

    Node *ptemp = root;
    Node *prev = nullptr;

    while (ptemp != nullptr)
    {
        prev = ptemp;
        if (compare(ptemp, pNew) == 1)
        {
            ptemp = ptemp->pLeft;
            count_compare++;
        }
        else if (compare(ptemp, pNew) == -1)
        {
            ptemp = ptemp->pRight;
            count_compare++;
        }
        else if (compare(ptemp, pNew) == 0)
        {
            Redeclared e(s);
            throw e;
        }
    }

    if (prev == nullptr)
    {
        prev = pNew;
        this->root = prev;
    }
    else if (compare(prev, pNew) == 1)
    {
        prev->pLeft = pNew;
    }
    else if (compare(prev, pNew) == -1)
    {
        prev->pRight = pNew;
    }
    else if (compare(prev, pNew) == 0)
    {
        Redeclared e(s);
        throw s;
    }

    splay(pNew);
    cout <<count_compare << " " << count_splay << endl;
    count_compare = 0;
    count_splay = 0;
    return;
}

Node *SymbolTable::getParent(Node *proot, Node *node)
{
    Node *ptemp = proot;

    if (!proot || compare(ptemp, node) == 0)
    {
        return nullptr;
    }

    while (ptemp != nullptr)
    {
        int cmp = compare(ptemp, node);
        if (cmp == 1 && ptemp->pLeft && compare(ptemp->pLeft, node) == 0)
        {
            return ptemp;
        }
        else if (cmp == 1 && ptemp->pLeft)
        {
            ptemp = ptemp->pLeft;
        }
        else if (cmp == -1 && ptemp->pRight && compare(ptemp->pRight, node) == 0)
        {
            return ptemp;
        }
        else if (cmp == -1 && ptemp->pRight)
        {
            ptemp = ptemp->pRight;
        }
    }

    return nullptr;
}

Node *SymbolTable::turnLeft(Node *node)
{
    if (node->pRight == nullptr)
    {
        return node;
    }
    Node *temp = node->pRight;
    node->pRight = temp->pLeft;
    temp->pLeft = node;
    return temp;
}

Node *SymbolTable::turnRight(Node *node)
{
    if (node->pLeft == nullptr)
    {
        return node;
    }
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
    if (compare(node, root) == 0)
    {
        return;
    }
    else
    {
        count_splay++;
        while (parent != nullptr)
        {
            if (parent == root)
            {
                if (node == parent->pLeft)
                {
                    root = turnRight(parent);
                }
                else
                {
                    root = turnLeft(parent);
                }
            }
            else
            {
                parent = getParent(root, node);
                grandpa = getParent(root, parent);
                grandgrand = getParent(root, grandpa);

                int cmp1 = compare(node, parent);
                int cmp2 = compare(parent, grandpa);

                if (cmp1 == -1 && cmp2 == -1) // L-L
                {
                    if (grandgrand == nullptr)
                    {
                        root = turnRight(grandpa);
                        root = turnRight(parent);
                    }
                    else if (compare(grandpa, grandgrand) == 1)
                    {
                        grandgrand->pRight = turnRight(grandpa);
                        grandgrand->pRight = turnRight(parent);
                    }
                    else if (compare(grandpa, grandgrand) == -1)
                    {
                        grandgrand->pLeft = turnRight(grandpa);
                        grandgrand->pLeft = turnRight(parent);
                    }
                }
                else if (cmp1 == 1 && cmp2 == 1) // R-R
                {
                    if (grandgrand == nullptr)
                    {
                        root = turnLeft(grandpa);
                        root = turnLeft(parent);
                    }
                    else if (compare(grandpa, grandgrand) == 1)
                    {
                        grandgrand->pRight = turnLeft(grandpa);
                        grandgrand->pRight = turnLeft(parent);
                    }
                    else if (compare(grandpa, grandgrand) == -1)
                    {
                        grandgrand->pLeft = turnLeft(grandpa);
                        grandgrand->pLeft = turnLeft(parent);
                    }
                }
                else if (cmp1 == 1 && cmp2 == -1) // L-R
                {
                    if (grandgrand == nullptr)
                    {
                        root->pLeft = turnLeft(parent);
                        root = turnRight(grandpa);
                    }
                    else if (compare(grandpa, grandgrand) == 1)
                    {
                        grandgrand->pLeft->pRight = turnLeft(parent);
                        grandgrand->pLeft = turnRight(grandpa);
                    }
                    else if (compare(grandpa, grandgrand) == -1)
                    {
                        grandgrand->pLeft->pLeft = turnLeft(parent);
                        grandgrand->pRight = turnRight(grandpa);
                    }
                }
                else if (cmp1 == -1 && cmp2 == 1) // R-L
                {
                    if (grandgrand == nullptr)
                    {
                        root->pRight = turnRight(parent);
                        root = turnLeft(grandpa);
                    }
                    else if (compare(grandpa, grandgrand) == 1)
                    {
                        grandgrand->pRight->pRight = turnRight(parent);
                        grandgrand->pRight = turnLeft(grandpa);
                    }
                    else if (compare(grandpa, grandgrand) == -1)
                    {
                        grandgrand->pLeft->pRight = turnRight(parent);
                        grandgrand->pRight = turnLeft(grandpa);
                    }
                }
            }
            parent = getParent(root, node);
        }
    }
    return;
}

void SymbolTable::lookup(string identifier, string s, int level)
{
    while (level >= 0)
    {
        Node New;
        New.identifier = identifier;
        New.level = level;

        Node *pFound = lookup_level(&New);
        if (pFound == nullptr)
        {
            level--;
        }
        else
        {
            splay(pFound);
            cout << pFound->level << endl;
            count_compare = 0;
            count_splay = 0;
            return;
        }
    }
    Undeclared e(s);
    throw e;
}

void SymbolTable::get_print_PO(Node *root, string &result){
    if(root == nullptr){
        return;
    }
    result = result + root->identifier+"//"+to_string(root->level)+" ";
    get_print_PO(root->pLeft, result);
    get_print_PO(root->pRight,result);
}

void SymbolTable::print_tree(Node *root)
{
    string result = "";
    get_print_PO(root, result);
    cout << result.substr(0, result.size() - 1) << endl;
    count_compare = 0;
    count_splay = 0;
}

Node *SymbolTable::getMaxLeftSB(Node *root)
{
    Node *cur = root;
    while (cur->pRight != nullptr)
    {
        cur = cur->pRight;
    }
    return cur;
}

void SymbolTable::delete_node(Node *&ptemp)
{
    if (!root)
        return;
    Node *cur = root;
    while (cur != nullptr && compare(cur, ptemp) != 0)
    {
        if (compare(cur, ptemp) == 1)
        {
            cur = cur->pLeft;
        }
        else if (compare(cur, ptemp) == -1)
        {
            cur = cur->pRight;
        }
    }

    Node *newCur;
    Node *pNewCur;
    if (cur->pLeft == nullptr || cur->pRight == nullptr)
    {
        if (cur->pRight == nullptr)
        {
            newCur = cur->pLeft;
        }
        else if (cur->pLeft == nullptr)
        {
            newCur = cur->pRight;
        }

        delete cur;
        this->root = newCur;
    }
    // if the node has 2 child
    else
    {
        newCur = getMaxLeftSB(cur->pLeft);
        pNewCur = getParent(root, newCur);
        if (compare(pNewCur, newCur) == -1)
        {
            Node nodeTemp;
            nodeTemp.identifier = cur->identifier;
            nodeTemp.level = cur->level;
            nodeTemp.type = cur->type;
            nodeTemp.value = cur->value;

            cur->identifier = newCur->identifier;
            cur->level = newCur->level;
            cur->type = newCur->type;
            cur->value = newCur->value;

            newCur->identifier = nodeTemp.identifier;
            newCur->level = nodeTemp.level;
            newCur->type = nodeTemp.type;
            newCur->value = nodeTemp.value;

            pNewCur->pRight = newCur->pLeft;
        }
        else if (compare(pNewCur, newCur) == 1)
        {
            Node nodeTemp;
            nodeTemp.identifier = cur->identifier;
            nodeTemp.level = cur->level;
            nodeTemp.type = cur->type;
            nodeTemp.value = cur->value;

            cur->identifier = newCur->identifier;
            cur->level = newCur->level;
            cur->type = newCur->type;
            cur->value = newCur->value;

            newCur->identifier = nodeTemp.identifier;
            newCur->level = nodeTemp.level;
            newCur->type = nodeTemp.type;
            newCur->value = nodeTemp.value;

            pNewCur->pLeft = newCur->pLeft;
        }

        delete newCur;
    }
}

void SymbolTable::delete_end(int level)
{
    while (root != nullptr)
    {
        Node *ptemp = getMaxLeftSB(root);
        if (ptemp->level != level)
        {
            break;
        }
        splay(ptemp);
        delete_node(ptemp);
    }
    count_compare = 0;
    count_splay = 0;
    return;
}

Node *SymbolTable::lookup_level(Node *pNew)
{
    Node *ptemp = root;
    int count_temp = 0;

    while (ptemp != nullptr)
    {
        int cmp = compare(ptemp, pNew);
        count_compare++;
        count_temp++;
        if (cmp == -1)
        {
            ptemp = ptemp->pRight;
        }
        else if (cmp == 1)
        {
            ptemp = ptemp->pLeft;
        }
        else if (cmp == 0)
        {
            return ptemp;
        }
    }

    count_compare -= count_temp;
    return nullptr;
}

Node *SymbolTable::lookup_node(string identifier, int level)
{
    while (level >= 0)
    {   
        Node New;
        New.identifier = identifier;
        New.level = level;
        // cout <<"level is: "<<level<<endl;
        Node *pFound = lookup_level(&New);
        if (pFound == nullptr)
        {
            level--;
        }
        else
        {
            splay(pFound);
            // cout <<"found and splayed"<<endl;
            return pFound;
        }
    }
    // cout<<"exit loop"<<endl;
    return nullptr;
}

void SymbolTable::assign(string identifier, string value, string s, int level)
{
    smatch m;
    // check which type of value
    //      find identifier
    Node *pFind;
    if (regex_match(value, m, value_number))
    {
        pFind = lookup_node(identifier, level);
        if (pFind == nullptr)
        {
            Undeclared e(s);
            throw e;
        }
        else
        {
            if (pFind->type != "number")
            {
                TypeMismatch e(s);
                throw e;
            }
        }
    }
    else if (regex_match(value, m, value_string))
    {
        pFind = lookup_node(identifier, level);
        if (pFind == nullptr)
        {
            Undeclared e(s);
            throw e;
        }
        else
        {
            if (pFind->type != "string")
            {
                TypeMismatch e(s);
                throw e;
            }
        }
    }
    else if (regex_match(value, m, value_variable))
    {

        Node *pValue = lookup_node(value, level);
        // if value_variable has in the tree
        if (pValue == nullptr)
        {
            Undeclared e(s);
            throw e;
        }
        // cout <<"Node found"<<endl;
        if (pValue->type == "number" || pValue->type == "string")
        {
            pFind = lookup_node(identifier, level);
            if (pFind == nullptr)
            {
                Undeclared e(s);
                throw e;
            }
            // the node_value is found and has the same type with Node_assign
            if (pFind->type != pValue->type)
            {
                TypeMismatch e(s);
                throw e;
            }
        }else{
            TypeMismatch e(s);
            throw e;
        }
    }
    else if (regex_match(value, m, value_function))
    {
        string func_name = m.str(1), func_value = m.str(2);
        string func_value_type = "";
        string func_return_type = "";

        // find node has function
        Node *pFunc = lookup_node(func_name, level);
        if (pFunc == nullptr)
        {
            Undeclared e(s);
            throw e;
        }
        else
        {
            smatch a;
            if (regex_match(pFunc->type, a, type_function))
            {
                func_return_type = a.str(a.size() - 1);
            }

            // ()->num/str
            int i = 0;
            string func_para = "";
            while (pFunc->type[i] != '-')
            {
                func_para += pFunc->type[i];
                i++;
            }

            func_value_type = getTypeFromValue(func_value, level, s);

            if (func_para != func_value_type)
            {
                TypeMismatch e(s);
                throw e;
            }
        }

        // find node to assign
        pFind = lookup_node(identifier, level);
        if (pFind == nullptr)
        {
            Undeclared e(s);
            throw e;
        }
        else
        {
            if (pFind->type != func_return_type)
            {
                TypeMismatch e(s);
                throw e;
            }
        }
    }
    else
    {
        TypeMismatch e(s);
        throw;
    }

    cout <<count_compare << " " << count_splay << endl;
    count_compare = 0;
    count_splay = 0;
    return;
}

string SymbolTable::getTypeFromValue(string value, int level, string s)
{
    value += ",";
    string num = "number,";
    string str = "string,";

    string result = "";
    string temp = "";

    for (unsigned int i = 0; i < value.size(); i++)
    {
        if (value[i] != ',')
        {
            temp += value[i];
        }
        else
        {
            if (regex_match(temp, value_number))
            {
                result += num;
            }
            else if (regex_match(temp, value_string))
            {
                result += str;
            }
            else if (regex_match(temp, value_variable))
            {
                Node *ptemp = lookup_node(temp, level);
                if (ptemp == nullptr)
                {
                    Undeclared e(s);
                    throw e;
                }
                else
                {
                    result = result + ptemp->type + ",";
                }
            }
            temp = "";
        }
    }
    result = result.substr(0, result.size() - 1);
    return result;
}

void SymbolTable::deleteRecursively(Node *node)
{
    if (node != nullptr)
    {
        deleteRecursively(node->pLeft);
        deleteRecursively(node->pRight);
        delete node;
    }
}