int params(t_token *token, ul hash){
    GET_TOKEN(token);
    bool ret_param = false;
    //token == id
    node *function_node =  tree_search(global_table, hash);
    if(token->token_name == TOKEN_IDENTIFIER){
        if(function_node->data->declaration && !(function_node->data->define)){
            function_node->data->params = malloc(sizeof (t_str_param));
            if(!function_node->data->params){
                return ERROR_INTERNAL;
            }
            function_node->data->type_params = malloc(sizeof (t_str_param));
            if(!function_node->data->type_params){
                return ERROR_INTERNAL;
            }
            string_param_init(function_node->data->params);
            string_param_init(function_node->data->type_params);
            string_param_copy_string(function_node->data->params, token->lexeme->inter);

            GET_TOKEN(token);

            if(token->token_name != TOKEN_ASSIGNMENT_TYPE){
                //TODO добавить обработку ошибок
                return ERROR_SYN_ANALYSIS;
            }

            if(data_type(token, function_node,ret_param)){
                return ERROR_SYN_ANALYSIS;
                //TODO добавить обрабутку ошибок
            }

            if(next_param(token, function_node, ret_param)){
                return ERROR_SYN_ANALYSIS;
                //TODO добавить обработку ошибок
            }

        }else if(function_node->data->declaration && function_node->data->define) {
            function_node->data->help_count++;
            if(!string_param_cmp_string(function_node->data->params, (function_node->data->help_count - 1), token->lexeme->inter)){
                return ERROR_SYN_ANALYSIS;
                //TODO не сответствует название переменнорй в декларации и дефиниции
            }
            GET_TOKEN(token);

            if (token->token_name != TOKEN_ASSIGNMENT_TYPE) {
                //TODO добавить обработку ошибок
                return ERROR_SYN_ANALYSIS;
            }

            if (data_type(token, function_node,ret_param)) {
                return ERROR_SYN_ANALYSIS;
                //TODO добавить обрабутку ошибок
            }

            if (next_param(token, function_node, ret_param)) {
                return ERROR_SYN_ANALYSIS;
                //TODO добавить обработку ошибок
            }
            function_node->data->help_count = 0;
        }

    }else if(function_node->data->count_params != 0){
        return ERROR_SYN_ANALYSIS;
        //TODO количесво параметров в декларации и дефиниции не совпадает
    }
    // -> ε
    return IT_IS_OK;
}


int next_param(t_token *token, node* function_node, bool ret_param){
    GET_TOKEN(token);
    if(token->token_name == TOKEN_COMMA){
        GET_TOKEN(token);
        if(function_node->data->declaration && !(function_node->data->define)) { //token == id
            if (token->token_name == TOKEN_IDENTIFIER) {
                string_param_copy_string(function_node->data->params, token->lexeme->inter);
                GET_TOKEN(token);

                if (token->token_name != TOKEN_ASSIGNMENT_TYPE) {
                    //TODO добавить обработку ошибок
                    return ERROR_SYN_ANALYSIS;
                }

                if (data_type(token, function_node, ret_param)) {
                    return ERROR_SYN_ANALYSIS;
                    //TODO добавить обрабутку ошибок
                }

                if (next_param(token,function_node, ret_param)) {
                    return ERROR_SYN_ANALYSIS;
                    //TODO добавить обработку ошибок
                }

            }
            // если функция уже была декларована или дифинована
        }else if(function_node->data->declaration && function_node->data->define){
            function_node->data->help_count++;
            if(!string_param_cmp_string(function_node->data->params, (function_node->data->help_count - 1), token->lexeme->inter)){
                return ERROR_SYN_ANALYSIS;
                //TODO не сответствует название переменнорй в декларации и дефиниции
            }

            if (token->token_name != TOKEN_ASSIGNMENT_TYPE) {
                //TODO добавить обработку ошибок
                return ERROR_SYN_ANALYSIS;
            }

            if (data_type(token, function_node, ret_param)) {
                return ERROR_SYN_ANALYSIS;
                //TODO добавить обрабутку ошибок
            }

            if (next_param(token,function_node, ret_param)) {
                return ERROR_SYN_ANALYSIS;
                //TODO добавить обработку ошибок
            }
        }
    }else{// если идет не запятая
        if(function_node->data->declaration && function_node->data->define) {
            if (function_node->data->count_params != function_node->data->help_count) {
                return ERROR_SYN_ANALYSIS;
                //TODO корличесво парметров не соответствует в деклорации и дефеницуии
            }
        }
        hold_token();
    }
    // -> ε
    return IT_IS_OK;
}



int function_call(t_token *token){
    if(token->token_name == TOKEN_IDENTIFIER){

        t_str id_fun; //TODO заглушшка перерписать

        //TODO у каждего ифа долдна происзодить запиись до узла
        if(string_arr_cmp(&id_fun,"write")){
            GET_TOKEN(token);
            if(token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            if(args(token)){
                return ERROR_SYN_ANALYSIS;
                //TODO ожидались параметры функции
            }

            GET_TOKEN(token);
            if(token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
        }
        else if(string_arr_cmp(&id_fun,"reads")){
            GET_TOKEN(token);
            if(token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            GET_TOKEN(token);
            if(token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
        }
        else if(string_arr_cmp(&id_fun,"readi")){
            GET_TOKEN(token);
            if(token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            GET_TOKEN(token);
            if(token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
        }
        else if(string_arr_cmp(&id_fun,"readn")){

            GET_TOKEN(token);
            if(token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            GET_TOKEN(token);
            if(token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
        }
        else if(string_arr_cmp(&id_fun,"tointager")){
            GET_TOKEN(token);
            if(token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            if(args(token)){
                return ERROR_SYN_ANALYSIS;
                //TODO ожидались параметры функции
            }

            GET_TOKEN(token);
            if(token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
        }
        else if(string_arr_cmp(&id_fun,"substr")){
            GET_TOKEN(token);
            if(token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            if(args(token)){
                return ERROR_SYN_ANALYSIS;
                //TODO ожидались параметры функции
            }

            GET_TOKEN(token);
            if(token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
        }
        else if(string_arr_cmp(&id_fun,"ord")){
            GET_TOKEN(token);
            if(token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            if(args(token)){
                return ERROR_SYN_ANALYSIS;
                //TODO ожидались параметры функции
            }

            GET_TOKEN(token);
            if(token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
        }
        else if(string_arr_cmp(&id_fun,"chr")){
            GET_TOKEN(token);
            if(token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            if(args(token)){
                return ERROR_SYN_ANALYSIS;
                //TODO ожидались параметры функции
            }

            GET_TOKEN(token);
            if(token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
        }
        else {
            //TODO спросить у табулке символов а был ли он определен если нет ошибка
            GET_TOKEN(token);
            if (token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            if (args(token)) {
                return ERROR_SYN_ANALYSIS;
                //TODO ожидались параметры функции
            }

            GET_TOKEN(token);
            if (token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
            return IT_IS_OK;
        }
    }
    return IT_IS_OK;
}
