/*
 * @<parser.h>::
 */
#include <stdio.h>

/*
 * standard input buffer is here: 
 */
extern FILE    *tape;

extern int      lookahead;
/*
 * Hi-level IO resources: 
 */

/*
 * gettoken is essentially the lexer method, which abstracts * a finite
 * automata archive -- implemented in lexer.c 
 */
int             gettoken(FILE * tape);

/*
 * parser modules 
 */

double          expr(void);
void            term(void);
void            fact(void);
void            match(int);

#define MAXSTACKSIZE 0x100
extern double   E_val[MAXSTACKSIZE];
extern double   T_val[MAXSTACKSIZE];

double          retrieve(const char *varname);
void            store(const char *varname, double val);

//Caras novos
void block(void);
void statement(void);
