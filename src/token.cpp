#include "token.h"

Token::Token(std::string token, TokenType type, int line_number) {
    this->token = token;
    this->type = type;
    this->line_number = line_number;
}

Token::~Token() {

}

std::string Token::get_token() {
    return this->token;
}

Token::TokenType Token::get_type() {
    return this->type;
}

int Token::get_line_number() {
    return this->line_number;
}

std::string Token::to_string() {
    // 要原样输出type
    std::string type_str = "";
    switch (this->type) {
        case IDENFR:
            type_str = "IDENFR";
            break;
        case INTCON:
            type_str = "INTCON";
            break;
        case STRCON:
            type_str = "STRCON";
            break;
        case CHRCON:
            type_str = "CHRCON";
            break;
        case MAINTK:
            type_str = "MAINTK";
            break;
        case CONSTTK:
            type_str = "CONSTTK";
            break;
        case INTTK:
            type_str = "INTTK";
            break;
        case CHARTK:
            type_str = "CHARTK";
            break;
        case BREAKTK:
            type_str = "BREAKTK";
            break;
        case CONTINUETK:
            type_str = "CONTINUETK";
            break;
        case IFTK:
            type_str = "IFTK";
            break;
        case ELSETK:
            type_str = "ELSETK";
            break;
        case NOT:
            type_str = "NOT";
            break;
        case AND:
            type_str = "AND";
            break;
        case OR:
            type_str = "OR";
            break;
        case FORTK:
            type_str = "FORTK";
            break;
        case GETINTTK:
            type_str = "GETINTTK";
            break;
        case GETCHARTK:
            type_str = "GETCHARTK";
            break;
        case PRINTFTK:
            type_str = "PRINTFTK";
            break;
        case RETURNTK:
            type_str = "RETURNTK";
            break;
        case PLUS:
            type_str = "PLUS";
            break;
        case MINU:
            type_str = "MINU";
            break;
        case VOIDTK:
            type_str = "VOIDTK";
            break;
        case MULT:
            type_str = "MULT";
            break;
        case DIV:
            type_str = "DIV";
            break;
        case MOD:
            type_str = "MOD";
            break;
        case LSS:
            type_str = "LSS";
            break;
        case LEQ:
            type_str = "LEQ";
            break;
        case GRE:
            type_str = "GRE";
            break;
        case GEQ:
            type_str = "GEQ";
            break;
        case EQL:
            type_str = "EQL";
            break;
        case NEQ:
            type_str = "NEQ";
            break;
        case ASSIGN:
            type_str = "ASSIGN";
            break;
        case SEMICN:
            type_str = "SEMICN";
            break;
        case COMMA:
            type_str = "COMMA";
            break;
        case LPARENT:
            type_str = "LPARENT";
            break;
        case RPARENT:
            type_str = "RPARENT";
            break;
        case LBRACK:
            type_str = "LBRACK";
            break;
        case RBRACK:
            type_str = "RBRACK";
            break;
        case LBRACE:
            type_str = "LBRACE";
            break;
        case RBRACE:
            type_str = "RBRACE";
            break;
        case Null:
            break;
    }
    return type_str + " " + this->token;    
}