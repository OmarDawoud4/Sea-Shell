#include "../executor/executor.h"
#include "../input/input_stream.h"
#include "../parser/parser.h"
#include "shell.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMD_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
  char *cmd;

  initsh(); // initialize global symbol table
  do {

    print_prompt_dollar();
    cmd = read_cmd();
    if (!cmd) {
      printf("exiting\n");
      exit(EXIT_SUCCESS);
    }

    if (cmd[0] == '\0' || strcmp(cmd, "\n") == 0) {
      free(cmd);
      continue;
    }

    if (strcmp(cmd, "exit\n") == 0) {
      free(cmd);
      break;
    }
    input_stream_s input_stream;
    input_stream.buffer = cmd;
    input_stream.buffer_size = strlen(cmd); 
    input_stream.curr_pos = INIT_STREAM_POS;

    parse_and_execute(&input_stream);
    free(cmd);

  } while (1);
  exit(EXIT_SUCCESS);
}

char *resize_buffer(char *ptr, int new_size) {
  char *new_ptr = realloc(ptr, new_size);
  if (!new_ptr) {
    free(ptr);
    fprintf(stderr, "error: failed to allocate buffer: %s\n", strerror(errno));
    return NULL;
  }
  return new_ptr;
}

char *read_cmd(void) {
  char buf[CMD_BUFFER_SIZE];
  char *cmd_buffer = NULL;
  int cmd_buffer_len = 0;

  while (fgets(buf, CMD_BUFFER_SIZE, stdin)) {
    int buflen = strlen(buf);

    // Resize buffer to hold the new input
    cmd_buffer = resize_buffer(cmd_buffer, cmd_buffer_len + buflen + 1);
    if (!cmd_buffer) {
      return NULL; 
    }

   
    strcpy(cmd_buffer + cmd_buffer_len, buf);
    cmd_buffer_len += buflen;

    // Check for newline character and handle line continuation (repeat the loop)
    if (buf[buflen - 1] == '\n') {
      if (buflen == 1 || buf[buflen - 2] != '\\') {
        return cmd_buffer;
      }
      // Handle line continuation by replacing "\n" with null terminator
      cmd_buffer_len -= 2;
      cmd_buffer[cmd_buffer_len] = '\0';
      print_prompt_gt();
    }
  }
  return cmd_buffer;
}


int parse_and_execute(input_stream_s *input_stream) {
  skip_whitespace(input_stream);

  token_s *tok = tokenize(input_stream);

  if (tok == &eof_token) {
    return 0;
  }

  while (tok && tok != &eof_token) {
    node_s *cmd = parse_simple_command(tok);

    if (!cmd) {
      break;
    }

    execute_command(cmd);
    free_node_tree(cmd);
    tok = tokenize(input_stream);
  }
  return 1;
}
