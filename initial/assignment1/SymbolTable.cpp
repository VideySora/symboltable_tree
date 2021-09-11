#include "SymbolTable.h"

string Get_variable(char a, int &count)
{
    string variable = "";
    if (a != ' ')
    {
        variable += a;
    }
    else
    {
        count++;
    }
    return variable;
}

void SymbolTable::run(string filename)
{
    string s;
    string command, var, val;
    fstream myfile(filename);
    int count;
    // myfile.open(filename, ios::in);
    if (myfile.is_open())
    {
        while (getline(myfile, s))
        {
            command = "";
            var = "";
            val = "";
            count = 1;
            for (int i = 0; i < s.size(); i++)
            {
                if (count == 1)
                {
                    command += Get_variable(s[i], count);
                }
                else if (count == 2)
                {
                    var += Get_variable(s[i], count);
                }
                else if (count == 3)
                {
                    val += Get_variable(s[i], count);
                }
            }
            // cout << command <<" "<< var << " " << value << " " << endl;

            if (command == "INSERT")
            {
                this->INSERT(var, val, s);
            }
            else if (command == "ASSIGN")
            {
                this->ASSIGN(var, val, s);
            }

            cout << "success" << endl;
        }
        myfile.close();
    }
}