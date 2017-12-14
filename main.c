#include <time.h>
#include "lexical.h"
#include "parser.h"
#include "symbol_table.h"
#include "errors.h"
#include "stack_machine.h"

int main(int argc, char **argv){
    clock_t start_time, end_time;
    double cpu_time_used;
    if(argc > 1){
         start_time = clock();
         open_stack_file();
         readFile(argv[1]);
         end_time = clock();
         cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
         printf("Executado em %f segundos.\n", cpu_time_used);
    }else{
       error_message(ERROR_PASS_FILE);
    }
    return 0;
}
