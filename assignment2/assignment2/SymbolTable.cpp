#include "SymbolTable.h"

regex insert_normal("(INSERT) ([a-z][a-zA-Z0-9_]*) (number|string) (true|false)");
regex insert_function("(INSERT) ([a-z][a-zA-Z0-9_]*) \\(((number|string)(,number|,string)*)?\\)->(string|number) (true|false)");
regex assign("(ASSIGN) ([a-z][a-zA-Z0-9_]*) ([0-9]*|\\'[a-zA-Z ]*\\')");
regex lookup("(LOOKUP) ([a-z][a-zA-Z0-9_]*)");
regex begin_block("BEGIN");
regex end_block("END");
regex print("PRINT");
regex rprint("RPRINT");

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
                    cout << global << endl;
                    this->insert(this->root, m.str(2), m.str(3), s, 0);
                }
                else{
                    this->insert(root, m.str(2), m.str(3), s, level);
                }
            }
            else if (regex_match(s, m, insert_function))
            {
                string name = m.str(2), type = m.str(3), global = m.str(4);
                if (global == "true")
                {
                    this->insert(this->root, m.str(2), m.str(3), s, level);
                }else{
                    this->insert(this->root, m.str(2), m.str(3), s, level);
                }
            }
            if (regex_match(s, m, begin_block))
            {
                level++;
            }

            this->print(this->root);
            cout<<endl;
        }
        myfile.close();
    }

    cout << "success";
}