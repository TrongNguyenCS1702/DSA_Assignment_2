#include "SymbolTable.h"

void SymbolTable::run(string filename)
{
    smatch m;
    regex assign_regex("ASSIGN ([a-z][\\w]*) (.*)");
    regex lookup_regex("LOOKUP ([a-z][\\w]*)");
    SplayTree *splay = new SplayTree();
    ifstream file(filename);
    string tmp = "";
    while (getline(file, tmp))
    {
        if (searchKey(tmp) == "ins")
        {
            InsertFunc(tmp, splay->root, splay->level);
        }
        else if (regex_match(tmp, m, assign_regex))
        {
            AssignFunc(m, splay->root, splay->level);
        }
        else if (searchKey(tmp) == "be")
        {
            splay->level++;
        }
        else if (searchKey(tmp) == "e")
        {
            splay->level--;
            if (splay->level < 0)
            {
                Destructor(splay->root);
                delete splay;
                throw UnknownBlock();
            }

            deleteNode(splay->root, splay->level + 1);
        }
        else if (regex_match(tmp, m, lookup_regex))
        {
            LookUpFunc(m, splay->root, splay->level);
        }
        else if (searchKey(tmp) == "prt")
        {
            PrintFunc(splay->root);
        }
        else
            throw InvalidInstruction(tmp); //delete all(leak)
    }
    if (splay->level > 0)
    {
        Destructor(splay->root);
        throw UnclosedBlock(splay->level);
        delete splay;
    }
    file.close();
    Destructor(splay->root);
    delete splay;
    //delete all(leak)
}