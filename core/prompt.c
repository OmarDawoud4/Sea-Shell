
#include "../symbol_table/symbol_table.h"
#include "shell.h"
#include <stdio.h>


void print_prompt_dollar() {
  struct symtab_entry_s *entry = get_symtab_entry("PS1");
  if (entry && entry->val) {
    fprintf(stderr, "%s", entry->val);
  } else {
    printf("$ ");
  }
}

void print_prompt_gt() {
  struct symtab_entry_s *entry = get_symtab_entry("PS2");
  if (entry && entry->val) {
    fprintf(stderr, "%s", entry->val);
  } else {
    printf("> ");
  }
}
