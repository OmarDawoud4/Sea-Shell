
#ifndef INPUT_STREAM_H
#define INPUT_STREAM_H

#define END_OF_FILE     (-1)
#define ERROR_CHAR      ( 0)
#define INIT_STREAM_POS    (-2)

typedef struct  {
  
  char *buffer;
  int   buffer_size;
  int   curr_pos;

}input_stream_s;

char get_next_char(input_stream_s *input_stream);
char peek_next_char(input_stream_s *input_stream);
void unget_char(input_stream_s *input_stream);
void skip_whitespace(input_stream_s *input_stream);

#endif
