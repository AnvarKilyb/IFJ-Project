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