#include <iostream>
#include "exp.h"
using namespace std;
BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op):left(l),right(r),op(op) {}
NumberExp::NumberExp(int v):value(v) {}
IdentifierExp::IdentifierExp(const string& n):name(n) {}
Exp::~Exp() {}
BinaryExp::~BinaryExp() { delete left; delete right; }
NumberExp::~NumberExp() { }
IdentifierExp::~IdentifierExp() { }
AssignStatement::AssignStatement(string id, Exp* e): id(id), rhs(e) {}
AssignStatement::~AssignStatement() {
    delete rhs;
}
PrintStatement::PrintStatement(Exp* e): e(e) {}
PrintStatement::~PrintStatement() {
    delete e;
}

WhileStatement::WhileStatement(Exp* c, list<Stm*> b): condition(c), body(b) {}
WhileStatement::~WhileStatement() {
    delete condition;
    for (Stm* s : body) {
        delete s;
    }
}

IFExp::IFExp(Exp* a1, Exp* a2, Exp* a3): e1(a1), e2(a2), e3(a3) {}

IFExp::~IFExp() {
    delete e1;delete e2;delete e3;
}

IfStatement::IfStatement(Exp* c, list<Stm*> t, list<Stm*> e): condition(c), then(t), els(e) {}
IfStatement::~IfStatement() {
    delete condition;
    for (Stm* s : then) {
        delete s;
    }
    for (Stm* s : els) {
        delete s;
    }
}

Program::Program() {}
void Program::add(Stm* s) {
    slist.push_back(s);
}
Program::~Program() {
    for (Stm* s : slist) {
        delete s;
    }
}
Stm::~Stm() {}
string Exp::binopToChar(BinaryOp op) {
    string  c;
    switch(op) {
        case PLUS_OP: c = "+"; break;
        case MINUS_OP: c = "-"; break;
        case MUL_OP: c = "*"; break;
        case DIV_OP: c = "/"; break;
        case LT_OP: c = "<"; break;
        case LE_OP: c = "<="; break;
        case EQ_OP: c = "=="; break;
        default: c = "$";
    }
    return c;
}

ForStatement::ForStatement(string start, Exp *e1, Exp *e2, Exp *e3, list<Stm *> b) {
    this->start=start;
    this->e1 = e1;
    this->e2 = e2;
    this->e3 = e3;
    this->body = b;
}

int ForStatement::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

ForStatement::~ForStatement() {
    delete e1;
    delete e2;
    delete e3;
    for (Stm* s : body) {
        delete s;
    }
}

IndexExpr::IndexExpr(const string &id_, Exp *index_) {
    id = id_;
    index = index_;
}

int IndexExpr::accept(Visitor *visitor) {
    visitor->visit(reinterpret_cast<AssignStatement *>(this));
    return 0;
}

string IndexExpr::acceptString(Visitor *visitor) {
    return visitor->visit(this);
}

IndexExpr::~IndexExpr() {
    delete index;
}


StringLiteral::StringLiteral(const string& v) {
    this->value = v;
}

int StringLiteral::accept(Visitor *visitor) {
    visitor->visit(this);
    return 0;
}

string StringLiteral::acceptString(Visitor *visitor) {
    return std::string();
}

StringLiteral::~StringLiteral() {

}
