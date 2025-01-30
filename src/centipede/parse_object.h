#ifndef CENTIPEDE_PARSE_OBJECT_STMTS_H_
#define CENTIPEDE_PARSE_OBJECT_STMTS_H_

void Cent_parse_object_finish(Cent_parse_data* pd, Cent_token* id, Cent_ast* n);
Cent_ast* Cent_parse_follow_on(Cent_parse_data* pd);

#endif
