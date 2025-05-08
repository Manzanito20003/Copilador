#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token {
public:
    enum Type {
        PLUS,STR,CORCHETE_I,CORCHETE_D ,IN,MINUS,FOR,RANGE,END_FOR,DO, MUL, DIV, NUM, ERR, PD, PI, END, ID,WHILE,ENDWHILE,IFEXP,COMA, PRINT, ASSIGN, PC,LT, LE, EQ, IF, THEN, ELSE, ENDIF
    };

    Type type;
    std::string text;

    Token(Type type);
    Token(Type type, char c);
    Token(Type type, const std::string& source, int first, int last);

    friend std::ostream& operator<<(std::ostream& outs, const Token& tok);
    friend std::ostream& operator<<(std::ostream& outs, const Token* tok);
};

#endif // TOKEN_H