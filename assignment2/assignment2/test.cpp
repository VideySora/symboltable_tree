#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include "error.h"
using namespace std;

regex insert_normal ("(INSERT) ([a-z][a-zA-Z0-9_]*) (number|string) (true|false)");
regex insert_function ("(INSERT) ([a-z][a-zA-Z0-9_]*) \\(((number|string)(,number|,string)*)?\\)->(string|number) (true|false)");
regex assign ("(ASSIGN) ([a-z][a-zA-Z0-9_]*) ([0-9]*|\\'[a-zA-Z ]*\\')");
regex lookup ("(LOOKUP) ([a-z][a-zA-Z0-9_]*)");
regex begin_block ("BEGIN");
regex end_block ("END");
regex print ("PRINT");
regex rprint ("RPRINT");

int main(){
    smatch m;
    string s1 = "INSERT b2 string false";
    string s2 = "INSERT b1 (number,string,number)->string false";
    cout << regex_match(s1, m, insert_normal);
    for (int i = 0; i < 10; i++)
    {
        cout << m.str(i)<<endl;
    }
    
}