#ifndef WORM_PARSE_DATA_H
#define WORM_PARSE_DATA_H

typedef struct {
    Zinc_error_list* errors;
    Worm_lex_data* ld;
    Worm_token* lookahead;
} Worm_parse_data;

void Worm_parse_data_init(Worm_parse_data* pd, Worm_lex_data* ld, Zinc_error_list* errors);
void Worm_parse_data_create(Worm_parse_data** pd, Worm_lex_data* ld, Zinc_error_list* errors);

#endif
