# Sea Shell   

## Table of Contents
1. [Introduction](#introduction)
2. [Architecture](#architecture)
   - [Lexer](#the-lexer)
   - [Parser](#the-parser)
   - [Executor](#the-executor)
   - [Symbol Table](#symbol-table)
3. [Demo](#Demo)
4. [Compilation](#Compilation )

---

## Introduction

A shell is a command-line interface that enables users to interact with the operating system by typing commands. It interprets and executes these commands, allowing for efficient task execution, automation, and system management, particularly in UNIX-like systems.

This project implements a custom shell, providing core functionalities such as input scanning, command parsing, and process execution. It’s designed as a simplified version of shells like Bash, offering valuable insights into shell operations, command execution, and process management.

## Architecture

The architecture of the shell comprises several key components that work together to parse and execute commands.

![shell-architecture-2024-10-08-1714](https://github.com/user-attachments/assets/56f5e2e4-3d1d-4bda-b580-ac3e12e7b03e)

### The Lexer

The **Lexer** (or Lexical Analyzer) reads user input and breaks it down into tokens for the parser.

![shell-lexer-2024-10-08-1714](https://github.com/user-attachments/assets/42d58a69-9d85-44f8-ad43-3c6cadd0fc65)

#### Key Functions

- **Input Scanning**: Reads user input and processes it character by character.
  - **Function: `get_next_char()`** – Retrieves the next character from the input stream.
- **Tokenization**: Splits input into tokens representing commands, arguments, operators, or special characters.
  - **Function: `tokenize()`** – Processes the input to return distinct tokens.

#### Example Command
```bash
echo "Hello, World!" > output.txt
```
Tokens generated:
- **Command**: `echo`
- **String Argument**: `"Hello, World!"`
- **Redirection Operator**: `>`
- **File Argument**: `output.txt`

### The Parser

The **Parser** transforms the tokens from the lexer into a structured representation, typically an Abstract Syntax Tree (AST).

![shell-ast-2024-10-08-1714](https://github.com/user-attachments/assets/10fca070-2418-4688-8c96-46cb4cb7c5eb)

#### Key Functions

- **parse_simple_command** – Parses a simple shell command.
- **new_node** – Creates nodes in the AST.
- **set_node_val_str** – Assigns a string value to a node.
- **add_child** – Adds child nodes to build the AST.
- **free_node_tree** – Recursively frees nodes for cleanup.

### The Executor

The **Executor** converts parsed commands into system calls that interface with the operating system.

![shell-executor-2024-10-08-1714](https://github.com/user-attachments/assets/9003ecf3-e5b0-4928-b6b8-8a2c47b4b09c)

#### Key Features

- **Command Execution**: Executes external commands using `fork()` and `execvp()`.
- **Built-in Commands**: Detects and executes built-in commands (e.g., `cd`, `exit`).
- **Process Management**: Uses `fork()` and `waitpid()` to manage processes.
- **Input/Output Redirection**: Redirects streams using `dup2`.
- **Pipes**: Sets up and manages data flow between commands connected by pipes.
- **Background Execution**: Supports background processes (commands ending with `&`).

### Symbol Table

The **Symbol Table** manages variables and functions within the shell.

![shell-symbol-table-2024-10-08-1714](https://github.com/user-attachments/assets/0371110f-d037-454f-b0b3-35d25b95a60f)

#### Components

1. **Symbol Table Entry (`symtab_entry_s`)**: Represents a symbol, such as a variable or function.
2. **Symbol Table (`symtab_s`)**: A scope that contains multiple symbols.
3. **Symbol Table Stack (`symtab_stack_s`)**: Manages multiple symbol tables, supporting local and global scopes.

---

## Demo 

![shell_demo2](https://github.com/user-attachments/assets/adb675f7-dac8-4100-aea7-e7771b901976)

![shell_demo3](https://github.com/user-attachments/assets/a80c890d-9028-4519-8d91-357b60d40fd6)

![shell_demo4](https://github.com/user-attachments/assets/bb48978f-4550-4aff-97cf-7451f33cbc86)

---

## Compilation 

To compile and run the shell, use the following commands. Ensure you have `gcc` installed.

### Steps to Compile

1. Clone the repository to your local machine.
2. Navigate to the project directory.
3. Compile the code using the following `gcc` command:

```bash
   gcc -o shell \
       builtins/builtins.c builtins/dump.c \
       core/main.c core/prompt.c \
       executor/executor.c \
       input/input_stream.c \
       lexer/lexer.c \
       parser/parser.c parser/node.c \
       symbol_table/symbol_table.c 
       core/initsh.c

```
---

### Running the Shell
After compilation, run the shell with:

```bash
./shell
```
You can now start entering commands in the shell interface
