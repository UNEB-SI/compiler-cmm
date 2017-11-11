#ifndef PARSER_H
#define PARSER_H

extern Token token, last_token;
extern int line_number;

void prog();
int isType();
void types_param();
int cmd();
void opc_p_types();

#endif // PARSER
