
#ifndef PARSER_H
#define PARSER_H
#include "../input/input_stream.h"
#include "../lexer/lexer.h"
#include "node.h"

node_s *parse_simple_command(token_s *tok);

#endif