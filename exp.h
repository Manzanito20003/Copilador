#ifndef EXP_H
#define EXP_H

#include <string>
#include <unordered_map>
#include <list>
#include "visitor.h"
using namespace std;
enum BinaryOp { PLUS_OP, MINUS_OP, MUL_OP, DIV_OP,LT_OP, LE_OP, EQ_OP };

class Exp {
public:
    virtual int  accept(Visitor* visitor) = 0;
    virtual ~Exp() = 0;
    static string binopToChar(BinaryOp op);
};

class BinaryExp : public Exp {
public:
    Exp *left, *right;
    BinaryOp op;
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    int accept(Visitor* visitor);
    ~BinaryExp();
};

class IFExp : public Exp {
public:
    Exp *e1,*e2, *e3;
    IFExp(Exp* a1, Exp* a2, Exp* a3);
    int accept(Visitor* visitor);
    ~IFExp();
};


class NumberExp : public Exp {
public:
    int value;
    NumberExp(int v);
    int accept(Visitor* visitor);
    ~NumberExp();
};

class IdentifierExp : public Exp {
public:
    std::string name;
    IdentifierExp(const std::string& n);
    int accept(Visitor* visitor);
    ~IdentifierExp();
};

class Stm {
public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Stm() = 0;
};

class AssignStatement : public Stm {
public:
    std::string id;
    Exp* rhs;
    AssignStatement(std::string id, Exp* e);
    int accept(Visitor* visitor);
    ~AssignStatement();
};

class PrintStatement : public Stm {
public:
    Exp* e;
    PrintStatement(Exp* e);
    int accept(Visitor* visitor);
    ~PrintStatement();
};


class IfStatement : public Stm {
public:
    Exp* condition;
    list<Stm*> then;
    list<Stm*> els;
    IfStatement(Exp* condition, list<Stm*> then, list<Stm*> els);
    int accept(Visitor* visitor);
    ~IfStatement();
};


class WhileStatement : public Stm {
public:
    Exp* condition;
    list<Stm*> body;
    WhileStatement(Exp* c, list<Stm*> b);
    int accept(Visitor* visitor);
    ~WhileStatement();
};
class ForStatement : public Stm {
public:
    string  start;
    Exp* e1;
    Exp* e2;
    Exp* e3;

    list<Stm*> body;
    ForStatement(string start,Exp* e1,Exp* e2,Exp* e3,list<Stm*> b);
    int accept(Visitor* visitor);
    ~ForStatement();
};
class StringLiteral : public Exp {
public:
    string value;  // El contenido de la cadena literal, por ejemplo: "hola mundo"

    StringLiteral(const string& v);     // constructor
    int accept(Visitor* visitor);       // para visitar y evaluar
    string acceptString(Visitor* visitor); // para imprimir o devolver texto directamente
    ~StringLiteral();                   // destructor
};

class IndexExpr : public Exp {
public:
    string id;     // nombre de la variable (id)
    Exp* index;    // expresión dentro de los corchetes
    //string literalValue;  // solo se usa si quieres guardar algo evaluado

    IndexExpr(const string& id_, Exp* index_);

    int accept(Visitor* visitor) override;
    string acceptString(Visitor* visitor) ;

    ~IndexExpr();
};


class Program {
public:
    std::list<Stm*> slist;
    Program();
    void add(Stm* s);
    ~Program();
};

#endif // EXP_H