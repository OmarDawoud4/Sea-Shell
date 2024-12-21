
#ifndef LEXER
#define LEXER

#include "../input/input_stream.h"

typedef struct {
  input_stream_s *input_stream;
  char *token_text;
  int token_len;
} token_s;

extern token_s eof_token;

token_s *tokenize(input_stream_s *input_stream);
void free_token(token_s *token);

#endif
