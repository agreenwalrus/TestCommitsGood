/*
definition of global variable for exchanging information between lemon and flex
*/


#ifndef YYSTYPE

typedef union {
  char *charValue;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif


/* extern YYSTYPE yylval; */
YYSTYPE yylval; 
