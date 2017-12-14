#include "lexical.h"
#include "parser.h"
#include "table.h"
#include "sintatic_erros.h"
#include "stacksemantic.h"

int main(int argc, char **argv){
    if(argc > 1){
         openStackFile();
         readFile(argv[1]);
    }else{
       sintatic_erro(ERROR_PASS_FILE);
    }
}
