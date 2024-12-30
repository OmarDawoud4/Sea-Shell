#include "parser.h"
#include "../core/shell.h"
#include "../input/input_stream.h"
#include "../lexer/lexer.h"
#include "node.h"
#include <unistd.h>

int is_redirection_token(const char *token) {
    return strcmp(token, ">") == 0 || strcmp(token, "<") == 0 || strcmp(token, ">>") == 0;
}

node_s *parse_simple_command(token_s *tok) {
    if (!tok) {
        return NULL;
    }

    node_s *cmd_root = create_node(NODE_COMMAND);
    if (!cmd_root) {
        free_token(tok);
        return NULL;
    }

    input_stream_s *input_stream = tok->input_stream;

    do {
        if (tok->token_text[0] == '\n') {
            free_token(tok);
            break;
        }

        if (is_redirection_token(tok->token_text)) {
            // Handle redirection
            node_s *redir = create_node(NODE_REDIRECT);
            redir->redirect_type = strdup(tok->token_text);
            tok = tokenize(input_stream);
            if (tok) {
                redir->filename = strdup(tok->token_text);
                add_child(cmd_root, redir);
            }
        } else {
        
            node_s *word = create_node(NODE_VAR);
            set_node_val_str(word, tok->token_text);
            add_child(cmd_root, word);
        }

        free_token(tok);
    } while ((tok = tokenize(input_stream)) != &eof_token);

    return cmd_root;
}