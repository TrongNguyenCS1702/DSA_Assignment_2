#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class SymbolTable
{
public:
    SymbolTable() {}
    void run(string filename);
};

class Node
{
public:
    Node *left;
    Node *right;
    Node *parent;
    string idf_name;
    string type;
    string para;
    int stat;
    int level;

public:
    Node()
    {
        this->left = NULL;
        this->right = NULL;
        this->parent = NULL;
        this->idf_name = "";
        this->type = "";
        this->para = "";
        this->stat = -1;
        this->level = -1;
    }
    Node(string idf_name, string type, int level, int stat)
    {
        this->left = NULL;
        this->right = NULL;
        this->parent = NULL;
        this->idf_name = idf_name;
        this->type = type;
        this->para = "";
        this->stat = -1;
        this->level = level;
    }
    Node(string idf_name, string para, int stat)
    {
        this->left = NULL;
        this->right = NULL;
        this->parent = NULL;
        this->idf_name = idf_name;
        this->type = "function";
        this->para = para;
        this->stat = stat;
        this->level = 0;
    }
};

class SplayTree
{
public:
    Node *root;
    int level;

public:
    SplayTree()
    {
        this->root = NULL;
        this->level = 0;
    }
};

void Destructor(Node *&root)
{
    if (root)
    {
        Destructor(root->left);
        Destructor(root->right);
        delete root;
    }
}

int compare(Node *comp, Node *x)
{
    int a = comp->idf_name.compare(x->idf_name);
    int b = comp->level - x->level;
    if (b > 0 || (b == 0 && a > 0))
        return 1;
    if (b < 0 || (b == 0 && a < 0))
        return -1;
    return 0;
}
Node *searchLevel(Node *root, string idf_name, int inplevel, int &count_compare)
{
    Node *T = new Node(idf_name, "number", inplevel, 0);
    Node *travel = root;
    while (travel != nullptr)
    {
        count_compare++;
        int count = compare(T, travel);
        if (count == 0)
        {
            return travel;
        }
        else if (count < 0)
        {
            if (travel->left == nullptr)
            {
                return travel;
            }
            travel = travel->left;
        }
        else
        {
            if (travel->right == nullptr)
            {
                return travel;
            }
            travel = travel->right;
        }
    }
    return nullptr;
}

Node *searchTree(Node *root, string idf_name, int inplevel)
{
    Node *T = new Node(idf_name, "number", inplevel, 0);
    Node *travel = root;
    while (travel != nullptr)
    {
        int order = compare(T, travel);
        if (order == 0)
        {
            return travel;
        }
        else if (order < 0)
        {
            if (travel->left == nullptr)
            {
                return travel;
            }
            travel = travel->left;
        }
        else
        {
            if (travel->right == nullptr)
            {
                return travel;
            }
            travel = travel->right;
        }
    }
    return NULL;
}
string searchKey(string str)
{
    if (str.substr(0, 6) == "INSERT")
        return "ins";
    else if (str.substr(0, 6) == "ASSIGN")
        return "ass";
    else if (str.substr(0, 5) == "BEGIN")
        return "be";
    else if (str.substr(0, 3) == "END")
        return "e";
    else if (str.substr(0, 6) == "LOOKUP")
        return "look";
    else if (str.substr(0, 5) == "PRINT")
        return "prt";
    else
        return "";
}

void rightRotate(Node *&root, Node *K)
{
    if (K == nullptr || K->left == nullptr)
        return;
    Node *L = K->left;
    if (K->parent == nullptr)
        root = L;
    else if (K->parent->left == K)
        K->parent->left = L;
    else
        K->parent->right = L;
    L->parent = K->parent;
    K->left = L->right;
    if (L->right)
        L->right->parent = K;
    L->right = K;
    K->parent = L;
}
void leftRotate(Node *&root, Node *K)
{
    if (K == nullptr || K->right == nullptr)
        return;
    Node *L = K->right;
    K->right = L->left;
    if (L->left)
        L->left->parent = K;
    if (K->parent == nullptr)
        root = L;
    else if (K->parent->left == K)
        K->parent->left = L;
    else
        K->parent->right = L;
    L->parent = K->parent;
    L->left = K;
    K->parent = L;
}

int splay(Node *&root, Node *K)
{
    if (K == nullptr || K->parent == nullptr)
        return 0;

    while (K->parent != nullptr)
    {
        Node *E = K->parent;
        if (E->parent == nullptr)
        {
            if (E->left == K)
                rightRotate(root, E);
            else if (E->right == K)
                leftRotate(root, E);

            return 1;
        }
        Node *X = E->parent;
        if (X->left == E && E->left == K)
        {
            rightRotate(root, X);
            rightRotate(root, E);
        }
        else if (X->right == E && E->right == K)
        {
            leftRotate(root, X);
            leftRotate(root, E);
        }
        else if (X->left == E && E->right == K)
        {
            leftRotate(root, E);
            rightRotate(root, X);
        }
        else
        {
            rightRotate(root, E);
            leftRotate(root, X);
        }
    }

    return 1;
}

bool SearchNode(Node *root, string idf_name)
{
    if (root == NULL)
        return false;
    if (root->idf_name == idf_name)
        return true;
    bool res1 = SearchNode(root->left, idf_name);
    if (res1)
        return true;
    bool res2 = SearchNode(root->right, idf_name);
    return res2;
}

Node *GetNode(Node *root, string idf_name)
{
    if (root == NULL || root->idf_name == idf_name)
        return root;
    if (idf_name.compare(root->idf_name) > 0)
        return GetNode(root->right, idf_name);
    return GetNode(root->right, idf_name);
}
Node *search(string idf_name, Node *&root, int inplevel, int &count_compare, int &count_splay)
{
    if (root == NULL)
        return NULL;
    for (int level = inplevel; level >= 0; level--)
    {
        Node *temp = searchTree(root, idf_name, level);
        if (temp->idf_name.compare(idf_name) == 0)
        {
            temp = searchLevel(root, idf_name, inplevel, count_compare);
            count_splay += splay(root, temp);
            return temp;
        }
    }
    return NULL;
}

string sub_arg(string s)
{
    int starts, end;
    if (s.find("(") != string::npos)
    {
        starts = s.find("(");
        end = s.find(")");
    }
    else
    {
        starts = 0;
        end = s.length();
    }
    string result = s.substr(starts, end - starts + 1);
    return result;
}

int countSign(string s)
{
    if (s[0] == '(' && s[1] == ')')
        return 0;
    int count = 1;
    for (int i = 0; i < (int)s.length(); ++i)
        if (s[i] == ',')
            count++;
    return count;
}

string getType(string arg, int &count_compare, int &count_splay, Node *root, int level)
{
    regex isString("\'[A-Za-z0-9 ]*\'");
    regex var("[a-z][\\w]*");
    regex isNumber("\\d+");
    string temp = "";
    string title = "";
    for (unsigned int i = 0; i < arg.length(); i++)
    {
        if (arg[i] != ',')
        {
            title += arg[i];
        }
        if (arg[i] == ',' || i == arg.length() - 1)
        {
            if (regex_match(title, isNumber))
                temp += "number,";
            else if (regex_match(title, isString))
                temp += "string,";
            else if (regex_match(title, var))
            {
                Node *x = search(title, root, level, count_compare, count_splay);
                if (!x)
                    return temp + "undeclared";
                if (x->type == "number")
                    temp += "number,";
                else if (x->type == "string")
                    temp += "string,";
                else
                    temp += "function,";
            }
            else
                return "error";

            title = "";
        }
    }
    if (temp != " ")
        return temp.substr(0, temp.size() - 1);
    return "";
}

Node *getMax(Node *&root)
{
    if (!root)
        return nullptr;
    Node *T = root;
    while (T->right)
    {
        T = T->right;
    }
    return T;
}

void Deletenode(Node *&root, Node *X)
{
    if (root == nullptr)
        return;
    splay(root, X);
    Node *l = root->left;
    Node *r = root->right;
    if (!l && !r)
    {
        delete root;
        root = nullptr;
        return;
    }
    else if (!l)
    {
        r->parent = nullptr;
        delete root;
        root = r;
        return;
    }
    else if (!r)
    {
        l->parent = nullptr;
        delete root;
        root = l;
    }
    else
    {
        l->parent = nullptr;
        delete root;
        Node *x = getMax(l);
        splay(root, x);
        root = x;
        x->right = r;
        r->parent = x;
    }
}

void deleteNode(Node *&root, int level)
{
    while (root)
    {
        Node *X = getMax(root);
        if (X->level != level)
        {
            break;
        }
        splay(root, X);
        Deletenode(root, X);
    }
}

void InsertFunc(string str, Node *&root, int level)
{
    int size = str.size();
    string subs = str.substr(7, size);
    int space_point1 = subs.find(" ") + 7;
    string idf_name = str.substr(7, space_point1 - 7);
    string subss = str.substr(space_point1 + 1, (size - space_point1));
    int space_point2 = subss.find(" ");
    string type = subss.substr(0, space_point2);
    string statS = subss.substr(space_point2 + 1, size - space_point2);
    int statI;
    //check status
    if (statS == "false")
        statI = 0;
    else if (statS == "true")
        statI = 1;
    else
    {
        Destructor(root);
        throw InvalidInstruction(str);
    }

    if (statI == 1)
        level = 0;
    //check whether idf_name is correct
    if ('a' <= idf_name[0] && idf_name[0] <= 'z') //available name
    {
    }
    else
    { //delete all
        Destructor(root);
        throw InvalidInstruction(str);
    }

    for (unsigned int i = 1; i < idf_name.size(); i++)
    {
        if (
            ('0' <= idf_name[i] && idf_name[i] <= '9') ||
            ('a' <= idf_name[i] && idf_name[i] <= 'z') ||
            ('A' <= idf_name[i] && idf_name[i] <= 'Z') ||
            (idf_name[i] == '_'))
        {
        }
        else
        {
            Destructor(root);
            throw InvalidInstruction(str);
        }
    }
    //check type and create node
    Node *a = new Node();
    if (type == "number" || type == "string")
    {
        a = new Node(idf_name, type, level, statI);
    }
    else
    {
        if (level == 0)
        {
            a = new Node(idf_name, type, statI);
        }
        else
        {
            Destructor(root);
            throw InvalidDeclaration(str);
        }
    }
    int count_splay = 0;
    int count_compare = 0;

    Node *walker = root;
    Node *p = nullptr;

    while (walker != nullptr)
    {
        p = walker;
        int order = compare(a, walker);
        if (order < 0)
        {
            walker = walker->left;
            count_compare++;
        }
        else if (order > 0)
        {
            walker = walker->right;
            count_compare++;
        }
        else
        {
            Destructor(root);
            throw Redeclared(str);
        }
    }

    if (p == nullptr)
    {
        root = a;
        cout << count_compare << " " << count_splay << endl;
        return;
    }

    walker = a;
    a->parent = p;

    if (compare(a, p) < 0)
        p->left = walker;
    else
        p->right = walker;

    if (walker != nullptr || walker->parent != nullptr)
    {
        splay(root, walker);
        count_splay++;
    }

    cout << count_compare << " " << count_splay << endl;
}

void AssignFunc(smatch m, Node *&root, int level)
{

    smatch m1;
    int count_splay = 0, count_compare = 0;
    string inputstr = m.str(0);
    string idf_name = m.str(1);
    string value = m.str(2);
    regex number("\\d+");
    regex str("\'[A-Za-z0-9 ]*\'");
    regex var("[a-z][\\w]*");
    regex function_call("([^ ]*)\\((.*)\\)");
    regex ass_func(
        "ASSIGN [a-z][a-zA-Z0-9_]* [a-z][a-zA-Z0-9_]*\\((|(([0-9]+|\\'[a-zA-Z0-9 "
        "]*\\'|[a-z][a-zA-Z0-9_]*)((,[0-9]+)|(,\\'[a-zA-Z0-9 "
        "]*\\')|(,[a-z][a-zA-Z0-9_]*))*))\\)");
    regex ass("ASSIGN ([a-z]|[a-z][a-zA-Z0-9_]*) "
              "([a-z]|[a-z][a-zA-Z0-9_]*|([0-9]*|[\'][a-zA-Z0-9_]*[\']))");

    if (!regex_match(inputstr, ass_func) && !regex_match(inputstr, ass))
    {
        Destructor(root);
        throw InvalidInstruction(inputstr);
    }

    if (regex_match(value, m1, function_call))
    {
        smatch m2;
        string f_name = m1.str(1);
        string pr = m1.str(2);
        string pp;
        string return_type;
        Node *s = search(f_name, root, level, count_compare, count_splay);
        if (s == nullptr || s->idf_name != f_name)
        {
            Destructor(root);
            throw Undeclared(inputstr);
        }
        if (s->type != "function")
        {
            Destructor(root);
            throw TypeMismatch(inputstr);
        }

        string ins_arg = sub_arg(s->para);
        string func_arg = sub_arg(value);
        if (countSign(ins_arg) != countSign(func_arg))
        {
            Destructor(root);
            throw TypeMismatch(inputstr);
        }

        regex funcP("\\(((number|string)(,number|,string)*)?\\)->(number|string)");
        if (regex_match(s->para, m2, funcP))
        {
            pp = m2.str(1);
            return_type = m2.str(m2.size() - 1);
        }

        pr = getType(pr, count_compare, count_splay, root, level);
        int length = (pr.length() > pp.length()) ? pr.length() : pp.length();
        for (int i = 0; i < length; i++)
        {
            if (pr[i] != pp[i] && (pr[i] == 'u' || pp[i] == 'u'))
            {
                Destructor(root);
                throw Undeclared(inputstr);
            }
            if (pr[i] != pp[i])
            {
                Destructor(root);
                throw TypeMismatch(inputstr);
            }
        }
        if (pr == "error")
        {
            Destructor(root);
            throw TypeMismatch(inputstr);
        }

        if (pr == "undeclared")
        {
            Destructor(root);
            throw Undeclared(inputstr);
        }

        if (pr.compare(pp) != 0)
        {
            Destructor(root);
            throw TypeMismatch(inputstr);
        }
        Node *clone = search(idf_name, root, level, count_compare, count_splay);

        if (clone == nullptr || clone->idf_name != idf_name)
        {
            Destructor(root);
            throw Undeclared(inputstr);
        }
        if (clone->type != return_type ||
            clone->para.find("(") != string::npos)
        {
            Destructor(root);
            throw TypeMismatch(inputstr);
        }
        cout << count_compare << " " << count_splay << endl;
        return;
    }
    if (regex_match(value, number) || regex_match(value, str))
    {
        Node *clone = search(idf_name, root, level, count_compare, count_splay);
        if (clone == nullptr)
        {
            Destructor(root);
            throw Undeclared(inputstr);
        }
        if (clone->idf_name != idf_name)
        {
            Destructor(root);
            throw Undeclared(inputstr);
        }
        if (regex_match(value, number) && clone->type != "number")
        {
            Destructor(root);
            throw TypeMismatch(inputstr);
        }
        if (regex_match(value, str) && clone->type != "string")
        {
            Destructor(root);
            throw TypeMismatch(inputstr);
        }
        cout << count_compare << " " << count_splay << endl;
        return;
    }
    if (regex_match(value, var))
    {
        Node *s = search(value, root, level, count_compare, count_splay);
        if (s == nullptr || s->idf_name != value)
        {
            Destructor(root);
            throw Undeclared(inputstr);
        }
        if (s->para.find("(") != string::npos)
        {
            Destructor(root);
            throw TypeMismatch(inputstr);
        }
        Node *clone = search(idf_name, root, level, count_compare, count_splay);
        if (clone == nullptr || clone->idf_name != idf_name)
        {
            Destructor(root);
            throw Undeclared(inputstr);
        }
        if (clone->type != s->type)
        {
            Destructor(root);
            throw TypeMismatch(inputstr);
        }

        cout << count_compare << " " << count_compare << endl;
        return;
    }
    Destructor(root);
    throw InvalidInstruction(inputstr);
}
bool lookup_rec(string idf_name, Node *&root, int level)
{
    Node *T = new Node(idf_name, "number", level, 0);
    Node *travel = root;
    while (travel != nullptr)
    {
        int order = compare(T, travel);
        if (order == 0)
        {
            splay(root, travel);
            return true;
        }
        else if (order < 0)
        {
            if (travel->left == nullptr)
            {
                return false;
            }
            travel = travel->left;
        }
        else
        {
            if (travel->right == nullptr)
            {
                return false;
            }
            travel = travel->right;
        }
    }
    return false;
}

void LookUpFunc(smatch Kol, Node *&root, int inplevel)
{
    if (root == nullptr)
    {
        Destructor(root);
        throw Undeclared(Kol.str(0));
    }
    string idf_name = Kol.str(1);
    for (int level = inplevel; level >= 0; level--)
    {
        if (lookup_rec(idf_name, root, level))
            break;
    }
    if (root->idf_name != idf_name)
    {
        Destructor(root);
        throw Undeclared(Kol.str(0));
    }
    cout << root->level << endl;
}

string Print(Node *root)
{
    string a = "";
    if (root != NULL)
    {
        a = root->idf_name + "//" + to_string(root->level) + " ";
        return a + Print(root->left) + Print(root->right);
    }
    return a;
}

string print_rec(Node *root)
{
    if (root == nullptr)
        return "";

    return root->idf_name + "//" + to_string(root->level) + " " +
           print_rec(root->left) + print_rec(root->right);
}

void PrintFunc(Node *root)
{
    string res = print_rec(root);
    if (res != "")
    {
        cout << res.substr(0, res.size() - 1) << endl;
    }
}

#endif