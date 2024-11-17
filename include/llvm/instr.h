#ifndef IR_INSTR_H
#define IR_INSTR_H

#include "value.h"

struct Instruction : public User {
    protected :
        Instruction(std::string name, ValueType* type, int dst_id) : User(name, type, dst_id) {}
        Instruction(std::string name, ValueType* type) : User(name, type) {}
        Instruction(int dst_id, ValueType* type) : User(dst_id, type) {}
        Instruction(ValueType* type) : User(type) {}

    public : 
        void print(std::ostream &os) const override;
};

struct AllocaInstr : public Instruction {
    int elements_num;

    AllocaInstr(int dst_id, ValueType* type) : Instruction("pointer" + dst_id, type, dst_id), elements_num(1) {}
    AllocaInstr(int dst_id, ValueType* type, int elements_num) : Instruction("pointer" + dst_id, type, dst_id), elements_num(elements_num) {}
    void print(std::ostream &os) const override;
};

struct ArithmeticInstr : public Instruction {
    enum AriType {
        ADD, SUB, MUL, SDIV, SREM
    };

    AriType arith_type;
    Value* op1;
    Value* op2;

    ArithmeticInstr(int dst_id, AriType arith_type, Value* op1, Value* op2) : // char类型运算时先转int
        Instruction("arith" + dst_id, &IR_INT, dst_id), arith_type(arith_type), op1(op1), op2(op2) {}

    void print(std::ostream &os) const override;
};

struct BrInstr : public Instruction {
    Value* condition; // conditional : bool -> i1; unconditional : nullptr
    BasicBlock* true_block;
    BasicBlock* false_block;

    BrInstr(Value *condition, BasicBlock* true_block, BasicBlock* false_block) :
        Instruction(&IR_VOID), condition(condition), true_block(true_block), false_block(false_block) {}

    BrInstr(BasicBlock* dst_block) :
        Instruction(&IR_VOID), condition(nullptr), true_block(dst_block), false_block(nullptr) {}

    void print(std::ostream &os) const override;
};

struct RetInstr : public Instruction {
    Value* return_value;

    RetInstr() : Instruction(&IR_VOID), return_value(nullptr) {}
    RetInstr(Value* return_value) : Instruction(&IR_VOID), return_value(return_value) {}

    void print(std::ostream &os) const override;
};

struct CallInstr : public Instruction { // void / int/char
    Function* function;
    std::vector<Value*> args;

    CallInstr(int dst_id, Function* function, std::vector<Value*> args) // int / char
    : Instruction("T" + dst_id, dst_id), function(function), args(args) {} 

    CallInstr(Function* function, std::vector<Value*> args) // void
    : Instruction(&IR_VOID), function(function), args(args) {}

    void print(std::ostream &os) const override;
};

struct GetelementptrInstr : public Instruction {
    Value* array;
    Value* index;

    GetelementptrInstr(int dst_id, ValueType* type, Value* array, Value* index) // value type是一个指针类型
    : Instruction(dst_id, type), array(array), index(index) {}

    void print(std::ostream &os) const override;
};

struct IcmpInstr : public Instruction {
    enum CmpType {
        EQ, NE, SGT, SGE, SLT, SLE
    };

    CmpType compare_type;
    Value* op1;
    Value* op2;

    IcmpInstr(int dst_id, CmpType compare_type, Value* op1, Value* op2) 
    : Instruction("T" + dst_id, &IR_BOOL, dst_id), compare_type(compare_type), op1(op1), op2(op2) {}

    void print(std::ostream &os) const override;
};

struct StoreInstr : public Instruction {
    Value* store_value;
    Value* dst_ptr;

    StoreInstr(Value* store_value, Value* dst_ptr) : 
    Instruction(&IR_VOID), store_value(store_value), dst_ptr(dst_ptr) {}

    void print(std::ostream &os) const override;
};

struct LoadInstr : public Instruction {
    Value* src_ptr;

    LoadInstr(int dst_id, Value* src_ptr) : 
    Instruction("T" + dst_id, ((PointerType*)src_ptr)->referenced_type, dst_id), src_ptr(src_ptr) {}
    
    void print(std::ostream &os) const override;
};

struct ZextInstr : public Instruction {
    Value* operand;

    ZextInstr(int dst_id, Value* operand, ValueType* to_type) :
    Instruction("T" + dst_id, to_type, dst_id) , operand(operand) {}

    void print(std::ostream &os) const override;
};

struct TruncInstr : public Instruction {
    Value* operand;

    TruncInstr(int dst_id, Value* operand, ValueType* to_type) :
    Instruction("T" + dst_id, to_type, dst_id) , operand(operand) {}

    void print(std::ostream &os) const override;
};


#endif // IR_INSTR_H