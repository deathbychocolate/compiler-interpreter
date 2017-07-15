/*
 * Phun Interpreter
 * Tami Meredith, June 2017
 */

/*
 * Constants
 */
#define SUCCESS  0
#define FAILURE -1
#define DEBUG    1  /* Set to 0 to turn off debugging messages */

/*
 * Types for lexical analysis
 */
typedef enum { sSTART, sINT, sIDENT, sSTRING, sCOMMENT } state;
typedef enum { tBEGIN, tEND, tQUOTE, tINT, tIDENT, tSTRING, tEOF } tokentype;

typedef struct {
    tokentype type;
    int       iVal;
    char     *sVal;
} token;

/*
 * Types for parsing and AST construction
 */
typedef enum { eString, eIdent, eInt, eExprList } exprtype;

typedef struct expression {
    exprtype type;
    char    *sVal;
    int      iVal;
    struct exprList *eVal;
} expr;

typedef struct exprList {
    expr *e;
    struct exprList *n;
} exprs;

/*
 * Types for evaluating
 */

 /* Symbol table:
  * Each definition is a linked list which
  * which holds the evaluation, and the
  * entire symbol table is a linked list of
  * definitions. */
typedef struct node {
    struct node *next;
    exprtype type;
    int iVal;
    char *sVal;
} node;

typedef struct st {
  char *ident; //name of identifier (ie, a)
  node *def; //identifier definition (ie, + -> 2 -> 3)
  struct st *nextdef; //the next identifier in the table
} symtab;

/*
 * Function Declarations
 */
void fatalError (char *msg);
char nextChar ();
void returnChar (char c);

void printToken (token t);
token scan ();

exprs *parse();
exprs *parseFileList (token t);
exprs *parseExprList (token t);
expr  *parseExpr (token t);

exprs *evaluate(exprs *exprList);
int inSymTab(char *ident);
symtab* addToSymTab(char *ident, int eval);
int getDef(symtab *sthead, char *ident);
exprs *addToResultList(exprs *resultList, exprs *e);


/* end of phun.h */
