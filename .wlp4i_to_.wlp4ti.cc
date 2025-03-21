// annotating the wlp4 parse tree, pretty much

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

struct Node {
    string rule;
    string type;
    vector<Node*> children;
    ~Node() { for (auto c : children) delete c; }
};

vector<Node*> allNodes;

Node* buildTree(istream& in) {
    string line;
    if (!getline(in, line)) throw runtime_error("ERROR");
    Node* node = new Node{line, ""};
    allNodes.push_back(node);
    istringstream ss(line);
    string lhs; ss >> lhs;
    if (islower(lhs[0])) {
        string token;
        while (ss >> token)
            node->children.push_back(buildTree(in));
    }
    return node;
}

void cleanup() {
    for (auto n : allNodes) delete n;
    allNodes.clear();
}

map<string, string> symTable;

string getType(Node* node) {
    if (node->rule == "type INT") return "int";
    if (node->rule == "type INT STAR") return "int*";
    throw runtime_error("ERROR");
}

string getID(Node* node) {
    istringstream ss(node->rule);
    string kind, name;
    ss >> kind >> name;
    return name;
}

void declare(Node* dcl) {
    string name = getID(dcl->children[1]);
    string type = getType(dcl->children[0]);
    if (symTable.count(name)) throw runtime_error("ERROR");
    symTable[name] = type;
    dcl->children[1]->type = type;
}

string checkType(Node* node) {
    stringstream ss(node->rule);
    string lhs; ss >> lhs;

    if (lhs == "factor") {
        if (node->children[0]->rule.substr(0,3) == "NUM") {
            node->children[0]->type = "int";
            return node->type = "int";
        }
        if (node->children[0]->rule.substr(0,4) == "NULL") {
            node->children[0]->type = "int*";
            return node->type = "int*";
        }
        if (node->children[0]->rule.substr(0,2) == "ID") {
            string name = getID(node->children[0]);
            if (!symTable.count(name)) throw runtime_error("ERROR");
            node->children[0]->type = symTable[name];
            return node->type = symTable[name];
        }
        if (node->children[0]->rule == "LPAREN (")
            return node->type = checkType(node->children[1]);
    }
    else if (lhs == "term" || lhs == "expr" || lhs == "lvalue") {
        return node->type = checkType(node->children[0]);
    }

    throw runtime_error("ERROR");
}

void annotate(Node* node) {
    for (auto child : node->children) annotate(child);
    string lhs;
    istringstream ss(node->rule);
    ss >> lhs;
    if (lhs == "expr" || lhs == "term" || lhs == "factor" || lhs == "lvalue")
        checkType(node);
}

void process(Node* root) {
    if (root->rule != "start BOF procedures EOF")
        throw runtime_error("ERROR");

    Node* main = root->children[1]->children[0];
    if (main->rule.substr(0,4) != "main") throw runtime_error("ERROR");

    Node* dcl1 = main->children[3];
    Node* dcl2 = main->children[5];

    if (dcl1->children.size() != 2 || dcl2->children.size() != 2)
        throw runtime_error("ERROR");

    string name1 = getID(dcl1->children[1]);
    string name2 = getID(dcl2->children[1]);

    if (name1 == name2) throw runtime_error("ERROR");

    declare(dcl1);
    declare(dcl2);

    if (symTable[name2] != "int") throw runtime_error("ERROR");

    Node* retExpr = main->children[11];
    annotate(retExpr);
    if (retExpr->type != "int") throw runtime_error("ERROR");

    // print the annotated tree
    for (Node* node : allNodes) {
        cout << node->rule;
        if (!node->type.empty()) cout << " : " << node->type;
        cout << endl;
    }
}

int main() {
    Node* root = nullptr;
    try {
        root = buildTree(cin);
        process(root);
    } catch (const runtime_error& e) {
        cerr << e.what() << endl;
        cleanup();
        return 1;
    }
    cleanup();
    return 0;
}
