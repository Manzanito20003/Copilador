#include <iostream>
#include <stdexcept>
#include "token.h"
#include "scanner.h"
#include "exp.h"
#include "parser.h"

using namespace std;

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERR)) {
            cout << "Error de análisis, carácter no reconocido: " << current->text << endl;
            exit(1);
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}

Parser::Parser(Scanner* sc):scanner(sc) {
    previous = NULL;
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        cout << "Error en el primer token: " << current->text << endl;
        exit(1);
    }
}

Program* Parser::parseProgram() {
    Program* p = new Program();
    try {
        p->slist = parseStmList();
    } catch (const exception& e) {
        cout << "Error durante el parsing: " << e.what() << endl;
        delete p;
        exit(1);
    }
    return p;
}

list<Stm*> Parser::parseStmList() {
    list<Stm*> slist;
    slist.push_back(parseStatement());
    while(match(Token::PC)) {
        slist.push_back(parseStatement());
    }
    return slist;
}

Stm* Parser::parseStatement() {
    Stm* s = NULL;
    Exp* e;

    if (current == NULL) {
        cout << "Error: Token actual es NULL" << endl;
        exit(1);
    }

    if (match(Token::ID)) {
        string lex = previous->text;

        if (!match(Token::ASSIGN)) {
            cout << "Error: se esperaba un '=' después del identificador." << endl;
            exit(1);
        }
        e = parseCExp();
        s = new AssignStatement(lex, e);
    } else if (match(Token::PRINT)) {
        if (!match(Token::PI)) {
            cout << "Error: se esperaba un '(' después de 'print'." << endl;
            exit(1);
        }
        e = parseCExp();
        cout << *current << endl;
        if (!match(Token::PD)) {

            cout << "Error: se esperaba un ')' después de la expresión." << endl;
            exit(1);
        }
        s = new PrintStatement(e);
    }

    else if (match(Token::WHILE)) {
        e = parseCExp();
        if (!match(Token::DO)) {
            cout << "Error: se esperaba 'DO' después de la expresión." << endl;
            exit(1);
        }
        list<Stm*> b;
        b = parseStmList();
        if (!match(Token::ENDWHILE)) {
            cout << "Error: se esperaba 'endwhile' al final de la declaración." << endl;
            exit(1);
        }
        s = new WhileStatement(e, b);
    }else if (match(Token::FOR)) {
//        e = parseCExp();
        if (!match(Token::ID)) {
            cout << "Error: se esperaba 'ID' después de la expresión." << endl;
            exit(1);
        }
        string start = previous->text;

        if (!match(Token::IN)) {
            cout << "Error: se esperaba 'IN' después de la expresión." << endl;
            exit(1);
        }
        if (!match(Token::RANGE)) {
            cout << "Error: se esperaba 'RANGE' después de la expresión." << endl;
            cout<<"test:"<<start<<endl;
            cout<<"test:"<<start<<endl;
            cout<<"test:"<<start<<endl;

            exit(1);
        }
        if (!match(Token::PI)) {
            cout << "Error: se esperaba '(' después de 'RANGE'." << endl;
            exit(1);
        }
        Exp* e1 = parseCExp();
        if (!match(Token::COMA)) {
            cout << "Error: se esperaba '(' después de 'RANGE'." << endl;
            exit(1);
        }
        Exp* e2 = parseCExp();
        if (!match(Token::COMA)) {
            cout << "Error: se esperaba ')' después de 'RANGE'." << endl;
            exit(1);
        }
        Exp* e3 = parseCExp();

        if (!match(Token::PD)) {
            cout << "Error: se esperaba ')' después de 'RANGE'." << endl;
            exit(1);
        }
        if (!match(Token::DO)) {
            cout << "Error: se esperaba 'DO' después de la expresión." << endl;
            exit(1);
        }
        list<Stm*> b;
        b = parseStmList();
        if (!match(Token::END_FOR)) {
            cout << "Error: se esperaba 'endwhile' al final de la declaración." << endl;
            exit(1);
        }
        s = new ForStatement(start, e1,e2, e3, b);
    }
    else if (match(Token::IF)) {
        e = parseCExp();
        if (!match(Token::THEN)) {
            cout << "Error: se esperaba 'then' después de la expresión." << endl;
            exit(1);
        }
        list<Stm*> then;
        list<Stm*> els;
        then = parseStmList();
        if (match(Token::ELSE)) {
            els = parseStmList();
        }
        if (!match(Token::ENDIF)) {
            cout << "Error: se esperaba 'end' al final de la declaración." << endl;
            exit(1);
        }
        s = new IfStatement(e, then, els);

    }
    else {
        cout << "Error: Se esperaba un identificador o 'print', pero se encontró: " << *current << endl;
        exit(1);
    }
    return s;
}

Exp* Parser::parseCExp(){
    Exp* left = parseExpression();
    if (match(Token::LT) || match(Token::LE) || match(Token::EQ)){
        BinaryOp op;
        if (previous->type == Token::LT){
            op = LT_OP;
        }
        else if (previous->type == Token::LE){
            op = LE_OP;
        }
        else if (previous->type == Token::EQ){
            op = EQ_OP;
        }
        Exp* right = parseExpression();
        left = new BinaryExp(left, right, op);
    }
    cout<<"tokend::"<<*current<<endl;
    return left;
}

Exp* Parser::parseExpression() {
    Exp* left = parseTerm();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS){
            op = PLUS_OP;
        }
        else if (previous->type == Token::MINUS){
            op = MINUS_OP;
        }
        Exp* right = parseTerm();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseTerm() {
    Exp* left = parseFactor();
    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        if (previous->type == Token::MUL){
            op = MUL_OP;
        }
        else if (previous->type == Token::DIV){
            op = DIV_OP;
        }
        Exp* right = parseFactor();
        left = new BinaryExp(left, right, op);
    }
    return left;
}
Exp* Parser::parseString() {
    if (match(Token::STR)) {
        return new StringLiteral(previous->text);
    }
    else if (match(Token::ID)) {
        string id = previous->text;

        // Verifica si es un acceso tipo id[exp]
        if (check(Token::CORCHETE_I)) {
            match(Token::CORCHETE_I);  // consumir [
            Exp* index = parseCExp();
            if (!match(Token::CORCHETE_D)) {
                cout << "Error: se esperaba ']' después de la expresión." << endl;
                exit(1);
            }
            return new IndexExpr(id, index);  // id[Cexp]
        }

        // Si no hay corchete, es un identificador simple
        return new IdentifierExp(id);
    }

    cout << "Error: se esperaba una cadena literal o un identificador (con o sin índice)." << endl;
    exit(1);
}

Exp* Parser::parseFactor() {
    //test
    cout<<"factor::"<<*current<<endl;
    Exp* e, *e1, *e2;
    if (match(Token::NUM)) {
        return new NumberExp(stoi(previous->text));
    }
    else if (check(Token::STR) || check(Token::ID)) {
        return parseString();
    }
    else if (match(Token::PI)){
        e = parseCExp();
        if (!match(Token::PD)){
            cout << "Falta paréntesis derecho" << endl;
            exit(0);
        }
        return e;
    }
    else if (match(Token::IFEXP)){
        match(Token::PI);
        e = parseCExp();
        match(Token::COMA);
        e1 = parseCExp();
        match(Token::COMA);
        e2 = parseCExp();
        match(Token::PD);
        return new IFExp(e, e1, e2);
    }
    else if (check(Token::STR) || check(Token::ID)) {
        return parseString();  // ← Aquí se cubre String ::= "..." | id[...]
    }


    cout << "Error: se esperaba un número o identificador." << endl;
    exit(0);
}

