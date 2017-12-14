#ifndef ERRORS_H
#define ERROS_H

enum code_error { MISSING_SEMI_COLON = 1,MISSING_ID, MISSING_CLOSE_PAREN,
                  MISSING_OPEN_PAREN, MISSING_OPEN_KEY, MISSING_CLOSE_KEY,
                  MISSING_CMD, MISSING_EQUAL_SNG, SYMBOL_NOT_RECOG,
                  MISSING_TYPE, MISSING_COMMA, ERROR_PASS_FILE,
                  ERROR_NOT_FOUND_FILE, ERROR_NUMBER_FLOAT_FORMAT
                };

void error_message(int flag);
#endif // ERRORS
