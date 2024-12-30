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
#include <fcntl.h>

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
            p2 = p + strlen(p); 

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

void handle_redirection(node_s *redir) {
    if (strcmp(redir->redirect_type, ">") == 0) {
        int fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(fd, STDOUT_FILENO);
        close(fd);
    } else if (strcmp(redir->redirect_type, ">>") == 0) {
        int fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        dup2(fd, STDOUT_FILENO);
        close(fd);
    } else if (strcmp(redir->redirect_type, "<") == 0) {
        int fd = open(redir->filename, O_RDONLY);
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
}

int execute_command(node_s *node) {
    if (!node) {
        return 0;
    }

    // Handle redirection
    node_s *child = node->first_child;
    while (child) {
        if (child->type == NODE_REDIRECT) {
            handle_redirection(child);
        }
        child = child->next_sibling;
    }

   
    child = node->first_child;
    int argc = 0;
    long max_args = 255;
    char *argv[max_args + 1];
    char *str;

    while (child) {
        if (child->type == NODE_VAR) {
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
        }
        child = child->next_sibling;
    }
    argv[argc] = NULL;

    // Execute command
    pid_t child_pid = fork();
    if (child_pid == 0) {
        do_exec(argc, argv);
        fprintf(stderr, "error: failed to execute command: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    } else if (child_pid < 0) {
        fprintf(stderr, "error: failed to fork command: %s\n", strerror(errno));
        return 0;
    }

    int status = 0;
    waitpid(child_pid, &status, 0);
    free_argv(argc, argv);

    return 1;
}