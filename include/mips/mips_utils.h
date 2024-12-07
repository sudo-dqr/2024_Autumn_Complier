#ifndef MIPS_UTILS_H
#define MIPS_UTILS_H

int ir_type_size(ValueType* ir_type, bool is_global) {
    if (auto int_type = dynamic_cast<IntType*>(ir_type)) {
        return 4;
    } else if (auto char_type = dynamic_cast<CharType*>(ir_type)) {
        if (is_global) return 1;
        else return 4;
    } else if (auto array_type = dynamic_cast<ArrayType*>(ir_type)) {
        return array_type->size * ir_type_size(array_type->element_type, is_global);
    } else if (auto pointer_type = dynamic_cast<PointerType*>(ir_type)) {
        return 4;
    } else {
        std::cout << "MIPS Alloca : Invalid Type!" << std::endl;
        return 0;
    }
}

bool is_const_value(Value* value) {
    if (auto intconst_ptr = dynamic_cast<IntConst*>(value)) {
        return true;
    } else if (auto charconst_ptr = dynamic_cast<CharConst*>(value)) {
        return true;
    } else {
        return false;
    }
}

int get_const_value(Value* value) {
    if (auto intconst_ptr = dynamic_cast<IntConst*>(value)) {
        return intconst_ptr->value;
    } else if (auto charconst_ptr = dynamic_cast<CharConst*>(value)) {
        return charconst_ptr->value;
    } else {
        std::cout << "Get Const Value : Invalid Value!" << std::endl;
        return -1;
    }
}

#endif // MIPS_UTILS_H