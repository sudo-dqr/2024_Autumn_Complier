#include "error.h"

void report_error(int line_number, char type) {
    std::ofstream error_out("error.txt", std::ofstream::app);
    error_out << line_number << " " << type << std::endl;
    error_out.close();
}

/*
 * b 函数名/变量名在当前作用域已经定义
 * g 有返回值函数缺号return语句
 * h 不能改变常量的值
 * m 在非循环块中使用break/conitnue
 * f 无返回值函数存在不匹配的return语句
 * c 使用了未定义标识符
 * d 函数调用语句中，参数个数与函数定义中的参数个数不匹配
 * e 函数参数类型不匹配
 * l printf中格式字符与表达式个数不匹配
 */