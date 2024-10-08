#include "parser.h"

void Parser::next_token() {
    if (!backbuf.empty()) { // 将预读的token从backbuf移回buffer
        buffer.push_back(backbuf.back());
        backbuf.pop_back();
    } else if (lexer.has_next()) {
        buffer.push_back(lexer.next());
    } else {
        std::cout << "Error! No Lexer Read!" << std::endl;
    }

    // backbuf是不断消耗的 不需要关心内存
    // buffer需要不断从头部移除旧元素来控制内存
    if (buffer.size() > 5) {
        buffer.pop_front();
    }
}

Token Parser::get_curtoken() {
    return buffer.back();
}

void Parser::unget_token() { // 将预读的token从buffer转移到backbuf
    backbuf.push_back(buffer.back());
    buffer.pop_back();
}

std::unique_ptr<CompUnit> Parser::parse() {
    auto comp_unit = parse_comp_unit();
    return comp_unit;
}

std::unique_ptr<CompUnit> Parser::parse_comp_unit() {
    next_token();
    auto comp_unit = std::make_unique<CompUnit>();
    while (comp_unit_judge_next_type() == CompUnitType::DECL) {
        comp_unit->decls.push_back(parse_decl());
    }
    while (comp_unit_judge_next_type() == CompUnitType::FUNCDEF) {
        comp_unit->func_defs.push_back(parse_funcdef());
    }
    comp_unit->main_func = parse_mainfunc();
    return comp_unit;
}

Parser::CompUnitType Parser::comp_unit_judge_next_type() {
    Token t = get_curtoken();
    if (t.get_type() == Token::CONSTTK) {
        return CompUnitType::DECL;
    } else if (t.get_type() == Token::VOIDTK) {
        return CompUnitType::FUNCDEF;
    }
    next_token();
    t = get_curtoken();
    if (t.get_type() == Token::MAINTK) {
        return CompUnitType::MAINFUNC;
    }
    next_token();
    t = get_curtoken();
    unget_token(); // 回退预读的token
    unget_token();
    if (t.get_type() == Token::LPARENT) {
        return CompUnitType::FUNCDEF;
    } else {
        return CompUnitType::DECL;
    }
}

std::unique_ptr<Decl> Parser::parse_decl() {
    Token t = get_curtoken();
    if (t.get_type() == Token::CONSTTK) {
        return std::make_unique<Decl>(std::move(parse_constdecl()));
    } else {
        return std::make_unique<Decl>(std::move(parse_vardecl()));
    }
}

std::unique_ptr<ConstDecl> Parser::parse_constdecl() {
    next_token(); // 跳过const
    auto res = std::make_unique<ConstDecl>();
    res->btype = parse_btype();
    res->const_defs.push_back(parse_constdef());
    while (get_curtoken().get_type() == Token::COMMA) { // ConstDef {, ConstDef}
        next_token();
        res->const_defs.push_back(parse_constdef());
    }
    if (get_curtoken().get_type() == Token::SEMICN) { // ;结尾
        next_token();
    } else { // i error
        unget_token();
        report_error(get_curtoken().get_line_number(), 'i');
        next_token();
    }
}

std::unique_ptr<VarDecl> Parser::parse_vardecl() {
    auto res = std::make_unique<VarDecl>();
    res->btype = parse_btype();
    res->var_defs.push_back(parse_vardef());
    while (get_curtoken().get_type() == Token::COMMA) { // VarDef {, VarDef}
        next_token();
        res->var_defs.push_back(parse_vardef());
    }
    if (get_curtoken().get_type() == Token::SEMICN) {
        next_token();
    } else { // i error
        unget_token();
        report_error(get_curtoken().get_line_number(), 'i');
        next_token();
    }
}

std::unique_ptr<BType> Parser::parse_btype() {
    Token t = get_curtoken();
    std::unique_ptr<BType> res = std::make_unique<BType>();
    res->btype = t.get_token();
    next_token();
    return res;
}

std::unique_ptr<ConstDef> Parser::parse_constdef() {
    auto res = std::make_unique<ConstDef>();
    res->ident = std::make_unique<Ident>(get_curtoken().get_token());
    next_token(); // 跳过ident
    if (get_curtoken().get_type() == Token::LBRACK) {
        next_token();
        res->const_exp = parse_constexp();
        if (get_curtoken().get_type() == Token::RBRACK) {
            next_token();
        } else { // k  error
            unget_token();
            report_error(get_curtoken().get_line_number(), 'k');
            next_token();
        }
    }
    next_token();
    res->const_init_val = parse_const_initval();
    return res;
}

std::unique_ptr<VarDef> Parser::parse_vardef() {
    auto res = std::make_unique<VarDef>();
    res->ident = std::make_unique<Ident>(get_curtoken().get_token());
    next_token();
    if (get_curtoken().get_type() == Token::LBRACK) {
        next_token();
        res->const_exp = parse_constexp();
        if (get_curtoken().get_type() == Token::RBRACK) {
            next_token();
        } else { // k error
            unget_token();
            report_error(get_curtoken().get_line_number(), 'k');
            next_token();
        }
    }
    if (get_curtoken().get_type() == Token::ASSIGN) {
        next_token();
        res->init_val = parse_initval();
    } else {
        res->init_val = nullptr;
    }
    return res;
}

std::unique_ptr<ConstExp> Parser::parse_constexp() {
    auto res = std::make_unique<ConstExp>();
    res->add_exp = parse_addexp();
    return res;
}

std::unique_ptr<ConstInitVal> Parser::parse_const_initval() {
    if (get_curtoken().get_type() == Token::STRCON) {
        return std::make_unique<ConstInitVal>(std::make_unique<StringConst>(get_curtoken().get_token()));
    } else if (get_curtoken().get_type() == Token::LBRACE) {
        auto vec = std::vector<std::unique_ptr<ConstExp>>();
        next_token();
        if (get_curtoken().get_type() != Token::RBRACE) {
            vec.push_back(parse_constexp());
            while (get_curtoken().get_type() == Token::COMMA) {
                next_token();
                vec.push_back(parse_constexp());
            }
        }
        next_token(); // 跳过 }
        return std::make_unique<ConstInitVal>(vec);
    } else {
        return std::make_unique<ConstInitVal>(parse_constexp());
    }
}

std::unique_ptr<InitVal> Parser::parse_initval() {
    if (get_curtoken().get_type() == Token::STRCON) {
        return std::make_unique<InitVal>(std::make_unique<StringConst>(get_curtoken().get_token()));
    } else if (get_curtoken().get_type() == Token::LBRACE) {
        auto vec = std::vector<std::unique_ptr<Exp>>();
        next_token();
        if (get_curtoken().get_type() != Token::RBRACE) {
            vec.push_back(parse_exp());
            while (get_curtoken().get_type() == Token::COMMA) {
                next_token();
                vec.push_back(parse_exp());
            }
        }
        next_token();
        return std::make_unique<InitVal>(vec);
    } else {
        return std::make_unique<InitVal>(parse_exp());
    }
}