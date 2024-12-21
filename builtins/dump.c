
#include "../core/shell.h"
#include "../symbol_table/symbol_table.h"

int dump(int argc, char **argv) {
  dump_local_symtab();
  return 0;
}