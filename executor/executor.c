
#include "executor.h"
#include "../core/shell.h"
#include "../parser/node.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define PATH_DELIMITER ":"
#define PATH_SEPARATOR "/"

char *get_path_env() { return getenv("PATH"); }

char *build_path(const char *dir, const char *file) {
  size_t dir_len = strlen(dir);
  size_t file_len = strlen(file);
  int needs_separator = dir[dir_len - 1] != '/';
  char *full_path = malloc(dir_len + needs_separator + file_len + 1);

  if (!full_path)
    return NULL;

  strcpy(full_path, dir);
  if (needs_separator) {
    strcat(full_path, PATH_SEPARATOR);
  }
  strcat(full_path, file);

  return full_path;
}


int is_regular_file(const char *path) {
  struct stat st;
  return (stat(path, &st) == 0 && S_ISREG(st.st_mode));
}


char *find_executable_path(char *file) {
  char *PATH = get_path_env();
  if (!PATH) {
    errno = ENOENT;
    return NULL;
  }

  char *p = PATH;
  while (*p) {
  
    char *p2 = strchr(p, ':');
    if (!p2)
      p2 = p + strlen(p); // Handle the last path without delimiter

    
    size_t dir_len = p2 - p;
    char dir[dir_len + 1];
    strncpy(dir, p, dir_len);
    dir[dir_len] = '\0';

    
    char *full_path = build_path(dir, file);
    if (!full_path) {
      errno = ENOMEM;
      return NULL;
    }

    if (is_regular_file(full_path)) {
      return full_path;
    }

    free(full_path);

   
    p = (*p2 == ':') ? p2 + 1 : p2;
  }

  errno = ENOENT;
  return NULL;
}


int do_exec(int argc, char **argv) {
  if (strchr(argv[0], '/')) {
    execv(argv[0], argv);
  } else {
    char *path = find_executable_path(argv[0]);
    if (!path) {
      return 0;
    }
    execv(path, argv);
    free(path);
  }
  return 0;
}

static inline void free_argv(int argc, char **argv) {
  if (!argc) {
    return;
  }

  while (argc--) {
    free(argv[argc]);
  }
}

int execute_command(node_s *node) {
  if (!node) {
    return 0;
  }

  node_s *child = node->first_child;
  if (!child) {
    return 0;
  }

  int argc = 0;
  long max_args = 255;
  char *argv[max_args + 1];
  char *str;

  while (child) {
    str = child->val.str;
    argv[argc] = malloc(strlen(str) + 1);

    if (!argv[argc]) {
      free_argv(argc, argv);
      return 0;
    }

    strcpy(argv[argc], str);
    if (++argc >= max_args) {
      break;
    }
    child = child->next_sibling;
  }
  argv[argc] = NULL;

  int i = 0;
  for (; i < builtins_count; i++) {
    if (strcmp(argv[0], builtins[i].name) == 0) {
      builtins[i].func(argc, argv);
      free_argv(argc, argv);
      return 1;
    }
  }

  pid_t child_pid = 0;
  if ((child_pid = fork()) == 0) {
    do_exec(argc, argv);
    fprintf(stderr, "error: failed to execute command: %s\n", strerror(errno));
    if (errno == ENOEXEC) {
      exit(126);
    } else if (errno == ENOENT) {
      exit(127);
    } else {
      exit(EXIT_FAILURE);
    }
  } else if (child_pid < 0) {
    fprintf(stderr, "error: failed to forkcommand: %s\n", strerror(errno));
    return 0;
  }

  int status = 0;
  waitpid(child_pid, &status, 0);
  free_argv(argc, argv);

  return 1;
}
