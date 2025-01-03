#ifndef IR_TYPE_H
#define IR_TYPE_H

#include <vector>
#include <memory>
#include <iostream>

struct IRPrintable {
    virtual void print(std::ostream &os) const = 0;
    virtual ~IRPrintable() = default;
};

struct ValueType : public IRPrintable { // abstract class as a implement
    virtual void print(std::ostream &os) const = 0;
};

struct ArrayType : public ValueType {
    ValueType* element_type;
    int size;

    ArrayType(ValueType* element_type, int size) : element_type(element_type), size(size) {}
    void print(std::ostream &os) const override;   
};

struct FunctionType : public ValueType {
    ValueType* return_type;
    std::vector<ValueType*> arg_types;

    FunctionType(ValueType* return_type, std::vector<ValueType*> arg_types) : 
                    return_type(return_type), arg_types(arg_types) {}
    void print(std::ostream &os) const override;
};

struct IntType : public ValueType {
    bool is_logical; // int1 -> bool

    IntType(bool is_logical) : is_logical(is_logical) {}
    void print(std::ostream &os) const override;  
};
static IntType IR_INT = IntType(false);
static IntType IR_BOOL = IntType(true);

struct CharType : public ValueType {
    void print(std::ostream &os) const override;
};
static CharType IR_CHAR = CharType();

struct LabelType : public ValueType {
    void print(std::ostream &os) const override;
};
static LabelType IR_LABEL = LabelType();

struct PointerType : public ValueType {
    ValueType* referenced_type;

    PointerType() : referenced_type(nullptr) {}
    PointerType(ValueType* referenced_type) : referenced_type(referenced_type) {}
    void print(std::ostream &os) const override;    
};

struct VoidType : public ValueType {
    void print(std::ostream &os) const override;
};
static VoidType IR_VOID = VoidType();

#endif // IR_TYPE_H