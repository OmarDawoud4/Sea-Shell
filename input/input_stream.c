#include "input_stream.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include"../core/shell.h"

char get_next_char(input_stream_s *input_stream) {

  if (!input_stream->buffer || !input_stream) {
    return END_OF_FILE;
  }

  if (input_stream->curr_pos == INIT_STREAM_POS) {
    input_stream->curr_pos = -1;
  }

  if (++input_stream->curr_pos >= input_stream->buffer_size) {
    input_stream->curr_pos = input_stream->buffer_size;
    return EOF;
  }

  return (input_stream->buffer[input_stream->curr_pos]);
}

char peek_next_char(input_stream_s *input_stream) {

  if (!input_stream->buffer || !input_stream) {
    errno = ENODATA;
    return ERROR_CHAR;
  }

  if (input_stream->curr_pos == INIT_STREAM_POS) {
    input_stream->curr_pos = -1;
  }

  int peek_pos = input_stream->curr_pos + 1;
  if (peek_pos >= input_stream->buffer_size) {
    return EOF;
  }

  return (input_stream->buffer[peek_pos]);
}
void unget_char(input_stream_s *input_stream) {
  if (input_stream->curr_pos < 0) {

    return;
  }
  input_stream->curr_pos--;
}

void skip_whitespace(input_stream_s *input_stream) {

  if (!input_stream->buffer || !input_stream) {
    errno = ENODATA;
    return;
  }

  char nc;
  
  while (((nc = peek_next_char(input_stream)) != EOF) && (nc == ' ' || nc == '\t')) {
    get_next_char(input_stream);
  }
}
