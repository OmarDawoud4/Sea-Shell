#include "lexer.h"
#include "../core/shell.h"
#include "../input/input_stream.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *tok_buf = NULL;
int tok_bufsize = 0;
int tok_bufindex = -1;

token_s eof_token = {
    .token_len = 0,
};

void add_to_buf(char c) {
    tok_buf[tok_bufindex++] = c;

    if (tok_bufindex >= tok_bufsize) {
        char *tmp = realloc(tok_buf, tok_bufsize * 2);
        if (!tmp) {
            errno = ENOMEM;
            return;
        }
        tok_buf = tmp;
        tok_bufsize *= 2;
    }
}

token_s *create_token(char *str) {
    token_s *tok = malloc(sizeof(token_s));
    if (!tok) {
        return NULL;
    }
    memset(tok, 0, sizeof(token_s));
    tok->token_len = strlen(str);

    char *nstr = malloc(tok->token_len + 1);
    if (!nstr) {
        free(tok);
        return NULL;
    }
    strcpy(nstr, str);
    tok->token_text = nstr;

    return tok;
}

void free_token(token_s *tok) {
    if (tok->token_text) {
        free(tok->token_text);
    }
    free(tok);
}

int is_redirection_char(char c) {
    return c == '>' || c == '<';
}

token_s *tokenize(input_stream_s *input_stream) {
    int endloop = 0;

    if (!input_stream || !input_stream->buffer || !input_stream->buffer_size) {
        errno = ENODATA;
        return &eof_token;
    }

    if (!tok_buf) {
        tok_bufsize = 1024;
        tok_buf = malloc(tok_bufsize);
        if (!tok_buf) {
            errno = ENOMEM;
            return &eof_token;
        }
    }

    tok_bufindex = 0;
    tok_buf[0] = '\0';

    char nc = get_next_char(input_stream);
    if (nc == ERROR_CHAR || nc == EOF) {
        return &eof_token;
    }

    do {
        switch (nc) {
            case ' ':
            case '\t':
                if (tok_bufindex > 0) {
                    endloop = 1;
                }
                break;

            case '\n':
                if (tok_bufindex > 0) {
                    unget_char(input_stream);
                } else {
                    add_to_buf(nc);
                }
                endloop = 1;
                break;

            case '>':
            case '<':
                if (tok_bufindex > 0) {
                    unget_char(input_stream);
                    endloop = 1;
                } else {
                    add_to_buf(nc);
                    // Check for '>>'
                    char next_char = peek_next_char(input_stream);
                    if (next_char == nc) {
                        add_to_buf(nc);
                        get_next_char(input_stream);
                    }
                    endloop = 1;
                }
                break;

            default:
                add_to_buf(nc);
                break;
        }

        if (endloop) {
            break;
        }
    } while ((nc = get_next_char(input_stream)) != EOF);

    if (tok_bufindex == 0) {
        return &eof_token;
    }

    if (tok_bufindex >= tok_bufsize) {
        tok_bufindex--;
    }
    tok_buf[tok_bufindex] = '\0';

    token_s *tok = create_token(tok_buf);
    if (!tok) {
        fprintf(stderr, "error: failed to alloc buffer: %s\n", strerror(errno));
        return &eof_token;
    }

    tok->input_stream = input_stream;
    return tok;
}