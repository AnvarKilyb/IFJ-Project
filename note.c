//int params(t_token *token, ul hash){
//    GET_TOKEN(token);
//    bool ret_param = false;
//    //token == id
//    node *function_node =  tree_search(global_table, hash);
//    if(token->token_name == TOKEN_IDENTIFIER){
//        if(function_node->data->declaration && !(function_node->data->define)){
//            function_node->data->params = malloc(sizeof (t_str_param));
//            if(!function_node->data->params){
//                return ERROR_INTERNAL;
//            }
//            function_node->data->type_params = malloc(sizeof (t_str_param));
//            if(!function_node->data->type_params){
//                return ERROR_INTERNAL;
//            }
//            string_param_init(function_node->data->params);
//            string_param_init(function_node->data->type_params);
//            string_param_copy_string(function_node->data->params, token->lexeme->inter);
//
//            GET_TOKEN(token);
//
//            if(token->token_name != TOKEN_ASSIGNMENT_TYPE){
//                //TODO добавить обработку ошибок
//                return ERROR_SYN_ANALYSIS;
//            }
//
//            if(data_type(token, function_node,ret_param)){
//                return ERROR_SYN_ANALYSIS;
//                //TODO добавить обрабутку ошибок
//            }
//
//            if(next_param(token, function_node, ret_param)){
//                return ERROR_SYN_ANALYSIS;
//                //TODO добавить обработку ошибок
//            }
//
//        }else if(function_node->data->declaration && function_node->data->define) {
//            function_node->data->help_count++;
//            if(!string_param_cmp_string(function_node->data->params, (function_node->data->help_count - 1), token->lexeme->inter)){
//                return ERROR_SYN_ANALYSIS;
//                //TODO не сответствует название переменнорй в декларации и дефиниции
//            }
//            GET_TOKEN(token);
//
//            if (token->token_name != TOKEN_ASSIGNMENT_TYPE) {
//                //TODO добавить обработку ошибок
//                return ERROR_SYN_ANALYSIS;
//            }
//
//            if (data_type(token, function_node,ret_param)) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO добавить обрабутку ошибок
//            }
//
//            if (next_param(token, function_node, ret_param)) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO добавить обработку ошибок
//            }
//            function_node->data->help_count = 0;
//        }
//
//    }else if(function_node->data->count_params != 0){
//        return ERROR_SYN_ANALYSIS;
//        //TODO количесво параметров в декларации и дефиниции не совпадает
//    }
//    // -> ε
//    return IT_IS_OK;
//}
//
//int next_param(t_token *token, node* function_node, bool ret_param){
//    GET_TOKEN(token);
//    if(token->token_name == TOKEN_COMMA){
//        GET_TOKEN(token);
//        if(function_node->data->declaration && !(function_node->data->define)) { //token == id
//            if (token->token_name == TOKEN_IDENTIFIER) {
//                string_param_copy_string(function_node->data->params, token->lexeme->inter);
//                GET_TOKEN(token);
//
//                if (token->token_name != TOKEN_ASSIGNMENT_TYPE) {
//                    //TODO добавить обработку ошибок
//                    return ERROR_SYN_ANALYSIS;
//                }
//
//                if (data_type(token, function_node, ret_param)) {
//                    return ERROR_SYN_ANALYSIS;
//                    //TODO добавить обрабутку ошибок
//                }
//
//                if (next_param(token,function_node, ret_param)) {
//                    return ERROR_SYN_ANALYSIS;
//                    //TODO добавить обработку ошибок
//                }
//
//            }
//            // если функция уже была декларована или дифинована
//        }else if(function_node->data->declaration && function_node->data->define){
//            function_node->data->help_count++;
//            if(!string_param_cmp_string(function_node->data->params, (function_node->data->help_count - 1), token->lexeme->inter)){
//                return ERROR_SYN_ANALYSIS;
//                //TODO не сответствует название переменнорй в декларации и дефиниции
//            }
//
//            if (token->token_name != TOKEN_ASSIGNMENT_TYPE) {
//                //TODO добавить обработку ошибок
//                return ERROR_SYN_ANALYSIS;
//            }
//
//            if (data_type(token, function_node, ret_param)) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO добавить обрабутку ошибок
//            }
//
//            if (next_param(token,function_node, ret_param)) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO добавить обработку ошибок
//            }
//        }
//    }else{// если идет не запятая
//        if(function_node->data->declaration && function_node->data->define) {
//            if (function_node->data->count_params != function_node->data->help_count) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO корличесво парметров не соответствует в деклорации и дефеницуии
//            }
//        }
//        hold_token();
//    }
//    // -> ε
//    return IT_IS_OK;
//}
//
//int function_call(t_token *token){
//    if(token->token_name == TOKEN_IDENTIFIER){
//
//        t_str id_fun; //TODO заглушшка перерписать
//
//        //TODO у каждего ифа долдна происзодить запиись до узла
//        if(string_arr_cmp(&id_fun,"write")){
//            GET_TOKEN(token);
//            if(token->token_name != TOKEN_LEFT_BRACKET) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO обработка ошибок ожидалась скобка
//            }
//
//            if(args(token)){
//                return ERROR_SYN_ANALYSIS;
//                //TODO ожидались параметры функции
//            }
//
//            GET_TOKEN(token);
//            if(token->token_name != TOKEN_RIGHT_BRACKET) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO обработка ошибок ожидалась скобка
//            }
//        }
//        else if(string_arr_cmp(&id_fun,"reads")){
//            GET_TOKEN(token);
//            if(token->token_name != TOKEN_LEFT_BRACKET) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO обработка ошибок ожидалась скобка
//            }
//
//            GET_TOKEN(token);
//            if(token->token_name != TOKEN_RIGHT_BRACKET) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO обработка ошибок ожидалась скобка
//            }
//        }
//        else if(string_arr_cmp(&id_fun,"readi")){
//            GET_TOKEN(token);
//            if(token->token_name != TOKEN_LEFT_BRACKET) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO обработка ошибок ожидалась скобка
//            }
//
//            GET_TOKEN(token);
//            if(token->token_name != TOKEN_RIGHT_BRACKET) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO обработка ошибок ожидалась скобка
//            }
//        }
//        else if(string_arr_cmp(&id_fun,"readn")){
//
//            GET_TOKEN(token);
//            if(token->token_name != TOKEN_LEFT_BRACKET) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO обработка ошибок ожидалась скобка
//            }
//
//            GET_TOKEN(token);
//            if(token->token_name != TOKEN_RIGHT_BRACKET) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO обработка ошибок ожидалась скобка
//            }
//        }
//        else if(string_arr_cmp(&id_fun,"tointager")){
//            GET_TOKEN(token);
//            if(token->token_name != TOKEN_LEFT_BRACKET) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO обработка ошибок ожидалась скобка
//            }
//
//            if(args(token)){
//                return ERROR_SYN_ANALYSIS;
//                //TODO ожидались параметры функции
//            }
//
//            GET_TOKEN(token);
//            if(token->token_name != TOKEN_RIGHT_BRACKET) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO обработка ошибок ожидалась скобка
//            }
//        }
//        else if(string_arr_cmp(&id_fun,"substr")){
//            GET_TOKEN(token);
//            if(token->token_name != TOKEN_LEFT_BRACKET) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO обработка ошибок ожидалась скобка
//            }
//
//            if(args(token)){
//                return ERROR_SYN_ANALYSIS;
//                //TODO ожидались параметры функции
//            }
//
//            GET_TOKEN(token);
//            if(token->token_name != TOKEN_RIGHT_BRACKET) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO обработка ошибок ожидалась скобка
//            }
//        }
//        else if(string_arr_cmp(&id_fun,"ord")){
//            GET_TOKEN(token);
//            if(token->token_name != TOKEN_LEFT_BRACKET) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO обработка ошибок ожидалась скобка
//            }
//
//            if(args(token)){
//                return ERROR_SYN_ANALYSIS;
//                //TODO ожидались параметры функции
//            }
//
//            GET_TOKEN(token);
//            if(token->token_name != TOKEN_RIGHT_BRACKET) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO обработка ошибок ожидалась скобка
//            }
//        }
//        else if(string_arr_cmp(&id_fun,"chr")){
//            GET_TOKEN(token);
//            if(token->token_name != TOKEN_LEFT_BRACKET) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO обработка ошибок ожидалась скобка
//            }
//
//            if(args(token)){
//                return ERROR_SYN_ANALYSIS;
//                //TODO ожидались параметры функции
//            }
//
//            GET_TOKEN(token);
//            if(token->token_name != TOKEN_RIGHT_BRACKET) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO обработка ошибок ожидалась скобка
//            }
//        }
//        else {
//            //TODO спросить у табулке символов а был ли он определен если нет ошибка
//            GET_TOKEN(token);
//            if (token->token_name != TOKEN_LEFT_BRACKET) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO обработка ошибок ожидалась скобка
//            }
//
//            if (args(token)) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO ожидались параметры функции
//            }
//
//            GET_TOKEN(token);
//            if (token->token_name != TOKEN_RIGHT_BRACKET) {
//                return ERROR_SYN_ANALYSIS;
//                //TODO обработка ошибок ожидалась скобка
//            }
//            return IT_IS_OK;
//        }
//    }
//    return IT_IS_OK;
//}
//
//
//int value(t_token *token){ //todo равенства чего с чемто
//    //TODO first write the prec analysis table in code
//    ast_node->count_expression++;
//    if(ast_node->count_variable < ast_node->count_expression){
//        return ERROR_SEMANTIC_ANALYSIS_EQ;
//    }
//
//    if(!ast_node->expression){
//        ast_node->expression = malloc(sizeof (t_exp_list));
//        exp_init(ast_node->expression);
//    }
//    GET_TOKEN(token);
//    if(token->token_name == TOKEN_IDENTIFIER){
//        GET_TOKEN(token);
//        if(token->token_name == TOKEN_LEFT_BRACKET){
//            //Есили в присвоение несколько раз вызов функции
//            if(ast_node->count_expression > 1){
//                return ERROR_SEMANTIC_ANALYSIS;
//            }
//            hold_token();
//            get_old_token(token);
//            if(function_call(token)){
//                return ERROR_SYN_ANALYSIS;
//                //TODO error
//            }
//        }else if(token->token_name == TOKEN_COMMA){
//            hold_token();
//            get_old_token(token);
//            node* function_var = NULL;
//            if((ast_node->it_is_loop || ast_node->it_is_if) && ast_node->local){
//                function_var = tree_search(ast_node->local, hashcode(token->lexeme->inter->data));
//                if(!function_var){
//                    function_var = tree_search(ast_node->in_function, hashcode(token->lexeme->inter->data));
//                    if(!function_var){
//                        return ERROR_SEMANTIC_ANALYSIS;
//                    }
//                }
//            }else{
//                function_var = tree_search(ast_node->in_function, hashcode(token->lexeme->inter->data));
//                if(!function_var){
//                    return ERROR_SEMANTIC_ANALYSIS;
//                }
//            }
//            //если переменная идентифицирована то аллакуем под нее все
//            ast_node->expression->var = true;
//            if(string_param_cmp_string(ast_node->type_variable, ast_node->count_expression, function_var->data->type)){
//                ast_node->expression->variable = malloc(sizeof(t_str));
//                string_init(ast_node->expression->variable);
//                string_copy(token->lexeme->inter,ast_node->expression->variable);
//
//                exp_next();
//
//            }else{
//                return ERROR_SEMANTIC_ANALYSIS_EQ;
//                //TODO error
//            }
//        }else{
//            hold_token();
//            get_old_token(token);
//            //TODO прецедечни
//        }
//    }else if(token->token_name == TOKEN_STRING){
//        if(string_param_cmp_arr(ast_node->type_variable, ast_node->count_expression, strin)){
//            ast_node->expression->str = true;
//            ast_node->expression->data_string = malloc(sizeof (t_str));
//            string_init(ast_node->expression->data_string);
//            string_copy(token->lexeme->inter, ast_node->expression->data_string);
//            exp_next();
//        }else{
//            return ERROR_SEMANTIC_ANALYSIS_EQ;
//        }
//    }else if(token->token_name == TOKEN_INTEGER){
//        GET_TOKEN(token);
//        if(token->token_name == TOKEN_COMMA){
//            hold_token();
//            get_old_token(token);
//            if(!string_param_cmp_arr(ast_node->type_variable, ast_node->count_expression, integ)) {
//                if (!string_param_cmp_arr(ast_node->type_variable, ast_node->count_expression, numb)) {
//                    return ERROR_SEMANTIC_ANALYSIS_EQ;
//                }else{
//                    ast_node->expression->numb = true;
//                    ast_node->expression->data_double = token->lexeme->number;
//                }
//            }else{
//                ast_node->expression->integer = true;
//                ast_node->expression->data_int = token->lexeme->integer;
//            }
//            exp_next();
//        }else
//        {
//            hold_token();
//            get_old_token(token);
//            //TODO анализа прецеденчни
//        }
//
//    }else if(token->token_name == TOKEN_NUMBER || token->token_name == TOKEN_NUMBER_EXPONENT){
//        GET_TOKEN(token);
//        if(token->token_name == TOKEN_COMMA){
//            hold_token();
//            get_old_token(token);
//            if(!string_param_cmp_arr(ast_node->type_variable, ast_node->count_expression, numb)) {
//                return ERROR_SEMANTIC_ANALYSIS_EQ;
//            }else{
//                ast_node->expression->numb = true;
//                ast_node->expression->data_double = token->lexeme->number;
//            }
//            exp_next();
//        }else{
//            hold_token();
//            get_old_token(token);
//            //TODO анализа прецеденчни
//        }
//    }
//
//    GET_TOKEN(token);
//    if(token->token_name == TOKEN_COMMA){
//        if(value(token)){
//            return ERROR_SEMANTIC_ANALYSIS;
//            //TODO error
//        }
//    }else
//    {
//        if(ast_node->count_variable > ast_node->count_expression)
//        {
//            if(ast_node->count_expression != 1)
//            {
//                return ERROR_SEMANTIC_ANALYSIS_EQ;
//            }
//        }
//        hold_token();
//    }
//    return IT_IS_OK;
//}




//int args(t_token *token){ //TODO предпологаю что функция будет определять параметры есть или нет
//    GET_TOKEN(token);
//    if(ast_node->count_func_param >= ast_node->func->count_params){
//        return ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC;
//    }else{
//        ast_node->count_func_param++;
//    }
//
//    if(!ast_node->func_param){
//        ast_node->func_param = malloc(sizeof (t_str_param));
//        string_param_init(ast_node->func_param);
//    }
//
//    if(token->token_name == TOKEN_INTEGER){
//        if(string_param_cmp_arr(ast_node->func->type_params,ast_node->count_func_param,integ) ||
//           string_param_cmp_arr(ast_node->func->type_params,ast_node->count_func_param,numb)){
//            //записываем число в параметры;
//            string_param_copy_string(ast_node->func_param, token->lexeme->inter);
//        }else{
//            return ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC;
//        }
//    }else if(token->token_name == TOKEN_STRING){
//        if(string_param_cmp_arr(ast_node->func->type_params,ast_node->count_func_param,strin)){
//            //записываем строку в параметры;
//            string_param_copy_string(ast_node->func_param, token->lexeme->inter);
//        }else{
//            return ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC;
//        }
//    }else if(token->token_name == TOKEN_NUMBER || token->token_name == TOKEN_NUMBER_EXPONENT ){
//        if(string_param_cmp_arr(ast_node->func->type_params,ast_node->count_func_param,numb)){
//            //записываем число в параметры;
//            string_param_copy_string(ast_node->func_param, token->lexeme->inter);
//        }else{
//            return ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC;
//        }
//    }else if(token->token_name == TOKEN_IDENTIFIER){
//        if(ast_node->it_is_in_function){
//            if((ast_node->it_is_loop || ast_node->it_is_if) && ast_node->local){
//                node* function_var = tree_search(ast_node->local, hashcode(token->lexeme->inter->data));
//                if(function_var){
//                    string_param_copy_string(ast_node->func_param, token->lexeme->inter);
//                }else{
//                    function_var = tree_search(ast_node->in_function, hashcode(token->lexeme->inter->data));
//                    if(function_var){
//                        string_param_copy_string(ast_node->func_param, token->lexeme->inter);
//                    }else{
//                        return ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC;
//                    }
//                }
//            }else{
//                node* function_var = tree_search(ast_node->in_function, hashcode(token->lexeme->inter->data));
//                if(function_var){
//                    string_param_copy_string(ast_node->func_param, token->lexeme->inter);
//                }else{
//                    return ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC;
//                }
//            }
//        }else{
//            return ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC;
//        }
//    }else{
//        return ERROR_SYN_ANALYSIS;
//        // ожидался параметр
//    }
//    if(next_args(token)){
//        return ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC;
//    }
//    return IT_IS_OK;
//}





//
//node *function_var = NULL;
//ul hash = hashcode(token->lexeme->inter->data);
//if (ast_node->it_is_if || ast_node->it_is_loop) {
//function_var = tree_search(ast_node->local, hash);
//if (!function_var) {
//function_var = tree_search(ast_node->in_function, hash);
//if (!function_var) {
//*e_check = ERROR_SEMANTIC_ANALYSIS;
//}
//}
//} else {
//function_var = tree_search(ast_node->in_function, hash);
//if (!function_var) {
//*e_check = ERROR_SEMANTIC_ANALYSIS;
//return  NULL;
//
//}
//}
//var_type = ast_node->type_variable->data[0]->data;
//
//token_type = function_var->data->type->data;
//if(strcmp(var_type,"integer") == 0){
//    if(strcmp(var_type,token_type) != 0){
//    *e_check = ERROR_SEMANTIC_ANALYSIS_EXPR;
//    }
//}
//else if(strcmp(var_type,"string") == 0){
//if(strcmp(var_type,token_type) != 0){
//*e_check = ERROR_SEMANTIC_ANALYSIS_EXPR;
//}
//}
//else if(strcmp(var_type,"number") == 0){
//if(strcmp("string",token_type) == 0){
//*e_check = ERROR_SEMANTIC_ANALYSIS_EXPR;
//}
//}











//void ast_free(t_ast_node* ast){
//    if(ast->variable) {
//        string_param_free(ast->variable);
//    }
//
//    if(ast->type_variable)
//        string_param_free(ast->type_variable);
////    if(ast->func_param)
////        string_param_free(ast->func_param);
//
//    t_ast_node *ptr = ast->next_node;
//    while(ast->next_node){
//        ast->next_node = ptr->next_node;
//
//        if(ptr->expression){
//            if(ptr->expression->first_exp){
//                ptr->expression = ptr->expression->first_exp;
//            }
//
//            t_exp_list *p = ptr->expression->next_exp;
//            while (ptr->expression->next_exp) {
//                ptr->expression->next_exp = p->next_exp;
//                if(p->data){
//                    string_free(p->data);
//                    p->data = NULL;
//                }
//                if(p->preced_expression_tree){
//                    delete_ast(p->preced_expression_tree);
//                    p->preced_expression_tree = NULL;
//                }
//                free(p);
//                p = ptr->expression->next_exp;
//            }
//            if(ptr->expression->data){
//                string_free(ptr->expression->data);
//                ptr->variable = NULL;
//            }
//            if(ptr->expression->preced_expression_tree){
//                delete_ast(ptr->expression->preced_expression_tree);
//                ptr->expression->preced_expression_tree = NULL;
//            }
//            free(ptr->expression);
//        }
//
//
//        free(ptr);
//        ptr = ast->next_node;
//    }
//
//    if(ast->expression){
//        if(ast->expression->first_exp){
//            ast->expression = ast->expression->first_exp;
//        }
//
//        t_exp_list *p = ast->expression->next_exp;
//        while (ast->expression->next_exp) {
//            ast->expression->next_exp = p->next_exp;
//            if(p->data){
//                string_free(p->data);
//                p->data = NULL;
//            }
//            if(p->preced_expression_tree){
//                delete_ast(p->preced_expression_tree);
//                p->preced_expression_tree = NULL;
//            }
//            free(p);
//            p = ast->expression->next_exp;
//        }
//        if(ast->expression->data){
//            string_free(ast->expression->data);
//            ast->variable = NULL;
//        }
//        if(ast->expression->preced_expression_tree){
//            delete_ast(ast->expression->preced_expression_tree);
//            ast->expression->preced_expression_tree = NULL;
//        }
//        free(ast->expression);
//    }
//
//    ast_init(ast);
//}

