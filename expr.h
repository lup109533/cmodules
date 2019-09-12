#ifndef __CMODULES_EXPR__
#define __CMODULES_EXPR__

#include "types.h"
#include "cmio.h"

/** Simple symbolic expression solver that can solve expressions of arbitrary type.
 ** User-defined types should provide their own library of functions.
 **/

typedef enum {
    INTEGER_CONSTANT,
    REAL_CONSTANT,
    VARIABLE,
    PLUS,
    MINUS,
    TIMES,
    DIVIDE,
    POWER,
    AND,
    OR,
    NOT,
    XOR,
    LPAREN,
    RPAREN,
    OTHER
} expr_elem_type;

typedef struct {
    raw_string literal;
    size_t priority;
    expr_elem_type type;
} expr_elem_struct;
typedef expr_elem_struct* expr_elem;

typedef struct {
    expr_elem* elems;
    size_t len;
} expr_t_struct;
typedef expr_t_struct* expr_t;

expr_t parse_expr(const raw_string);
expr_t validate_expr(expr_t);
expr_t to_postfix_notation(expr_t);

#define __cmodules_expr_t_new_macro(...) __cmodules_expr_t_new(__VA_ARGS__)
expr_t __cmodules_expr_t_new(const raw_string);
void del_expr_elem(expr_elem);
void del_expr(expr_t);

#define define_expr_solver_function(T, func, T_del,\
                                    int_const_parser, real_const_parser,\
                                    add_func, sub_func, inv_func,\
                                    mul_func, div_func, power_func,\
                                    and_func, or_func, not_func, xor_func)\
                                    \
                                    T func(expr_t expr, ...) {\
                                        stack op = new(stack);\
                                        dict var = new(dict, raw_string, T);\
                                        list garbage_pile = new(list);\
                                        T cur_var;\
                                        T lhs, rhs;\
                                        T result;\
                                        va_list vars;\
                                        va_start(vars, expr);\
                                        for (size_t i = 0; i < len(expr); i++) {\
                                            expr_elem cur_elem = expr->elems[i];\
                                            switch (cur_elem->type) {\
                                                case VARIABLE:\
                                                case OTHER:\
                                                    if (!has_key(var, cur_elem->literal)) {\
                                                        cur_var = icast(T, (va_arg(vars,int)));\
                                                        add(var, _kv(cur_elem->literal, cur_var));\
                                                    }\
                                                    else {\
                                                        cur_var = icast(T, getval(var, cur_elem->literal));\
                                                    }\
                                                    add(op, _node(T, cur_var));\
                                                    break;\
                                                \
                                                case INTEGER_CONSTANT:\
                                                    cur_var = int_const_parser(cur_elem->literal);\
                                                    add(op, _node(T, cur_var));\
                                                    break;\
                                                \
                                                case REAL_CONSTANT:\
                                                    cur_var = real_const_parser(cur_elem->literal);\
                                                    add(op, _node(T, cur_var));\
                                                    break;\
                                                \
                                                case PLUS:\
                                                    lhs = icast(T, pop(op));\
                                                    rhs = icast(T, pop(op));\
                                                    add(garbage_pile, _node(T, rhs));\
                                                    lhs = add_func(lhs,rhs);\
                                                    add(op, _node(T, lhs));\
                                                    break;\
                                                \
                                                case MINUS:\
                                                    lhs = icast(T, pop(op));\
                                                    if (cur_elem->priority > 1) {\
                                                        lhs = inv_func(lhs);\
                                                    }\
                                                    else {\
                                                        rhs = icast(T, pop(op));\
                                                        add(garbage_pile, _node(T, rhs));\
                                                        lhs = add_func(lhs,rhs);\
                                                    }\
                                                    add(op, _node(T, lhs));\
                                                    break;\
                                                \
                                                case TIMES:\
                                                    lhs = icast(T, pop(op));\
                                                    rhs = icast(T, pop(op));\
                                                    add(garbage_pile, _node(T, rhs));\
                                                    lhs = mul_func(lhs,rhs);\
                                                    add(op, _node(T, lhs));\
                                                    break;\
                                                \
                                                case DIVIDE:\
                                                    lhs = icast(T, pop(op));\
                                                    rhs = icast(T, pop(op));\
                                                    add(garbage_pile, _node(T, rhs));\
                                                    lhs = div_func(lhs,rhs);\
                                                    add(op, _node(T, lhs));\
                                                    break;\
                                                \
                                                case POWER:\
                                                    lhs = icast(T, pop(op));\
                                                    rhs = icast(T, pop(op));\
                                                    add(garbage_pile, _node(T, rhs));\
                                                    lhs = power_func(rhs,lhs);\
                                                    add(op, _node(T, lhs));\
                                                    break;\
                                                \
                                                case AND:\
                                                    lhs = icast(T, pop(op));\
                                                    rhs = icast(T, pop(op));\
                                                    add(garbage_pile, _node(T, rhs));\
                                                    lhs = and_func(lhs,rhs);\
                                                    add(op, _node(T, lhs));\
                                                    break;\
                                                \
                                                case OR:\
                                                    lhs = icast(T, pop(op));\
                                                    rhs = icast(T, pop(op));\
                                                    add(garbage_pile, _node(T, rhs));\
                                                    lhs = or_func(lhs,rhs);\
                                                    add(op, _node(T, lhs));\
                                                    break;\
                                                \
                                                case NOT:\
                                                    lhs = icast(T, pop(op));\
                                                    lhs = not_func(lhs);\
                                                    add(op, _node(T, lhs));\
                                                    break;\
                                                \
                                                case XOR:\
                                                    lhs = icast(T, pop(op));\
                                                    rhs = icast(T, pop(op));\
                                                    add(garbage_pile, _node(T, rhs));\
                                                    lhs = xor_func(lhs,rhs);\
                                                    add(op, _node(T, lhs));\
                                                    break;\
                                                \
                                                default:\
                                                    break;\
                                                \
                                            }\
                                        }\
                                        result = icast(T, pop(op));\
                                        va_end(vars);\
                                        if (T_del != NULL) {\
                                            foreach(node, garbage_pile) {\
                                                node->type_del = T_del;\
                                            }\
                                        }\
                                        del(garbage_pile);\
                                        del(op);\
                                        del(var);\
                                        return result;\
                                    }


#include "src/expr.c"

#endif // __CMODULES_EXPR__
