expr_t __cmodules_expr_t_new(const raw_string expr_string) {
    expr_t expr = parse_expr(expr_string);
    expr = validate_expr(expr);
    if (expr->len > 0)
        expr = to_postfix_notation(expr);
    return expr;
}

void del_expr_elem(expr_elem elem) {
    free(elem->literal);
    free(elem);
}

void del_expr(expr_t expr) {
    for (size_t i = 0; i < expr->len; i++)
        del_expr_elem(expr->elems[i]);
    free(expr->elems);
    free(expr);
}

expr_t parse_expr(const raw_string expr) {
    bool is_sep_or_op(char c) {
        switch (c) {
            case '+':
            case '-':
            case '*':
            case '/':
            case '&':
            case '^':
            case '|':
            case '!':
            case '~':
            case '(':
            case ')':
            case ' ':
                return true;

            default:
                return false;
        }
    }
    void get_token(raw_string token, const raw_string expr, size_t* i) {
        while (expr[*i] == ' ')
            *i += 1;

        size_t cur = 0;
        switch (expr[*i]) {
            case '+':
            case '-':
            case '/':
            case '&':
            case '|':
            case '!':
            case '~':
            case '^':
            case '(':
            case ')':
                token[cur] = expr[*i];
                cur += 1;
                *i += 1;
                break;

            case '*':
                if (expr[*i + 1] == '*') {
                    token[cur] = expr[*i];
                    token[cur +1] = expr[*i + 1];
                    cur += 2;
                    *i += 2;
                }
                else {
                    token[cur] = expr[*i];
                    cur += 1;
                    *i += 1;
                }
                break;

            default:
                while (!is_sep_or_op(expr[*i])) {
                    token[cur] = expr[*i];
                    cur += 1;
                    *i += 1;
                }
                break;
        }
        token[cur] = 0;
    }

    expr_elem_type parse_token_type(const raw_string token) {
        bool only_numbers = true;
        size_t num_of_dots = 0;
        size_t token_len = strlen(token);

        switch (token[0]) {
            case '+':
                if (token_len > 1)
                    return OTHER;
                else
                    return PLUS;

            case '-':
                if (token_len > 1)
                    return OTHER;
                else
                    return MINUS;

            case '*':
                if (token_len > 2)
                    return OTHER;
                elif (token_len == 2 && token[1] == '*')
                    return POWER;
                else
                    return TIMES;

            case '/':
                if (token_len > 1)
                    return OTHER;
                else
                    return DIVIDE;

            case '&':
                if (token_len > 1)
                    return OTHER;
                else
                    return AND;

            case '|':
                if (token_len > 1)
                    return OTHER;
                else
                    return OR;

            case '!':
            case '~':
                if (token_len > 1)
                    return OTHER;
                else
                    return NOT;

            case '^':
                if (token_len > 1)
                    return OTHER;
                else
                    return XOR;

            case '(':
                if (token_len > 1)
                    return OTHER;
                else
                    return LPAREN;

            case ')':
                if (token_len > 1)
                    return OTHER;
                else
                    return RPAREN;

            default:
                break;
        }

        for (size_t i = 0; i < token_len; i++) {
            if (only_numbers && i == 1) {

            }
            if (token[i] == '.')
                num_of_dots += 1;
            elif (token[i] < '0' || token[i] > '9') {
                if (only_numbers && i == 1) {
                    switch(token[i]) {
                    case 'b':
                    case 'B':
                    case 'o':
                    case 'O':
                    case 'd':
                    case 'D':
                    case 'x':
                    case 'X':
                        continue;
                    default:
                        ;
                    }
                }
                only_numbers = false;
            }
        }

        if (only_numbers) {
            if (num_of_dots == 0)
                return INTEGER_CONSTANT;
            elif (num_of_dots == 1)
                return REAL_CONSTANT;
            else
                return OTHER;
        }
        else {
            if (token[0] >= '0' && token[0] <= '9')
                return OTHER;
            else
                return VARIABLE;
        }
    }

    void assign_priority(expr_t expr) {
        for (size_t i = 0; i < expr->len; i++) {
            switch (expr->elems[i]->type) {
                case VARIABLE:
                case INTEGER_CONSTANT:
                case REAL_CONSTANT:
                case PLUS:
                case OTHER:
                    expr->elems[i]->priority = 0;
                    break;

                case MINUS:
                    if (i == 0)
                        expr->elems[i]->priority = 3;
                    elif ((expr->elems[i-1]->type == TIMES ||
                           expr->elems[i-1]->type == DIVIDE ||
                           expr->elems[i-1]->type == POWER ||
                           expr->elems[i-1]->type == LPAREN))
                        expr->elems[i]->priority = 3;
                    else
                        expr->elems[i]->priority = 0;
                    break;

                case TIMES:
                case DIVIDE:
                    expr->elems[i]->priority = 1;
                    break;

                case POWER:
                case AND:
                case OR:
                case NOT:
                case XOR:
                    expr->elems[i]->priority = 2;
                    break;

                case LPAREN:
                    expr->elems[i]->priority = 4;
                    break;

                case RPAREN:
                    expr->elems[i]->priority = 5;
                    break;
            }
        }
    }

    raw_string copy_token(const raw_string token) {
        size_t token_len = strlen(token);
        raw_string literal = alloc_string(token_len);
        strcpy(literal, token);
        return literal;
    }

    size_t expr_len = strlen(expr);

    expr_t result = alloc(expr_t_struct);
    if (expr_len == 0) {
        result->elems = NULL;
        result->len = 0;
        return result;
    }

    size_t elem_num = 0;
    for (size_t i = 0; i < expr_len; i++) {
        if (is_sep_or_op(expr[i]) && expr[i] != ' ' && !(expr[i] == '*' && expr[i+1] == '*'))
                elem_num += 2;
    }
    if (elem_num == 0)
        elem_num = 1;
    expr_elem* elems = alloc_array(expr_elem, elem_num);

    raw_pointer cur_token = alloc_string(expr_len);
    size_t cur_elem = 0;
    for (size_t i = 0; i < expr_len;) {
        get_token(cur_token, expr, &i);
        elems[cur_elem] = alloc(expr_elem_struct);
        elems[cur_elem]->type = parse_token_type(cur_token);
        elems[cur_elem]->literal = copy_token(cur_token);
        cur_elem += 1;
    }

    result->elems = elems;
    result->len   = cur_elem;
    assign_priority(result);
    return result;
}

expr_t validate_expr(expr_t expr) {
    bool is_operator(expr_elem elem) {
        return !(elem->type == INTEGER_CONSTANT ||
                 elem->type == REAL_CONSTANT ||
                 elem->type == VARIABLE ||
                 elem->type == OTHER ||
                 elem->type == LPAREN ||
                 elem->type == RPAREN);
    }

    int paren_count = 0;
    bool wrong_operator_sequence = false;
    for (size_t i = 0; i < expr->len; i++) {
        expr_elem cur_elem = expr->elems[i];
        if (cur_elem->type == LPAREN)
            paren_count += 1;
        elif (cur_elem->type == RPAREN)
            paren_count -= 1;

        if (i > 0) {
            expr_elem prev_elem = expr->elems[i-1];
            if (is_operator(prev_elem) && is_operator(cur_elem)) {
                if (is_operator(prev_elem) && (cur_elem->type == MINUS && cur_elem->priority > 1))
                    continue;
                else {
                    wrong_operator_sequence = true;
                    break;
                }
            }
        }
    }
    if (paren_count != 0 || wrong_operator_sequence) {
        for (size_t i = 0; i < expr->len; i++) {
            del_expr_elem(expr->elems[i]);
        }
        expr->len = 0;
    }

    return expr;
}

expr_t to_postfix_notation(expr_t expr) {
    stack ops = new(stack);
    fifo out = new(fifo);

    for (size_t i = 0; i < expr->len; i++) {
        expr_elem cur_elem = expr->elems[i];
        expr_elem tmp_elem;

        switch (cur_elem->type) {
            case VARIABLE:
            case INTEGER_CONSTANT:
            case REAL_CONSTANT:
            case OTHER:
                add(out, _node(expr_elem, cur_elem));
                break;

            case PLUS:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case POWER:
            case AND:
            case OR:
            case NOT:
            case XOR:
            case LPAREN:
                tmp_elem = NULL;
                while (!is_empty(ops)) {
                    tmp_elem = (expr_elem) pop(ops);
                    if (tmp_elem->priority < cur_elem->priority || tmp_elem->type == LPAREN) {
                        add(ops, _node(expr_elem, tmp_elem));
                        break;
                    }
                    else {
                        add(out, _node(expr_elem, tmp_elem));
                    }
                }
                add(ops, _node(expr_elem, cur_elem));
                break;

            case RPAREN:
                tmp_elem = (expr_elem) pop(ops);
                while (tmp_elem->type != LPAREN) {
                    add(out, _node(expr_elem, tmp_elem));
                    tmp_elem = (expr_elem) pop(ops);
                }
                del_expr_elem(tmp_elem);
                del_expr_elem(cur_elem);
                break;
        }
    }

    while (!is_empty(ops)) {
        add(out, _node(expr_elem, ((expr_elem) pop(ops))));
    }

    size_t cur = 0;
    while (!is_empty(out)) {
        expr->elems[cur] = (expr_elem) pop(out);
        cur += 1;
    }

    for (size_t i = 0; i < cur; i++) {
        expr->elems[cur] = NULL;
    }
    expr->len = cur;

    del(ops);
    del(out);
    return expr;
}
