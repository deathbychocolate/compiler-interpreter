
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "phun.h"

int result = 0;
char op = ' ';
int initialise = 1;

void hi()
{
	printf("I AM AT %d\n" + __LINE__);
}

//check if the given identifier is in the symbol table
int inSymTab(symtab *st, char *id) {
	while (st != NULL) {
		if (strcmp(st->ident,id) == 0) {
			return 1;
		}
		else {
			st = st->nextdef;
		}
	}
	return 0;
}

//adds an evaluation of an identifier to symbol table
int addToSymTab(symtab *sthead, char *id, int eval) {
	symtab *cursor = sthead;
	while(cursor->nextdef != NULL)
		cursor = cursor->nextdef;

	node *newNode = malloc(sizeof(node));
	newNode->iVal = eval;
	newNode->type = eInt;

	symtab *newdef = malloc(sizeof(symtab));
	cursor->nextdef = newdef;
	newdef->ident = id;
	newdef->def = newNode;

	return 1;
}

//gets definition of identifier in symbol table
int getDef(symtab *sthead, char *ident) {

	symtab *cursor = sthead;

	while(cursor->nextdef != NULL) {
		cursor = cursor->nextdef;

		if(strcmp(cursor->ident, ident) == 0) {
			return cursor->def->iVal;
		}
	}

	//This should not happen! We already checked if
	//the value existed in the symtab
	fatalError("Something went wrong!");
	return 0;

}

void printItem(exprs *exprList) {
	if (exprList->e->type == eInt) {
		printf(" ==>%d\n", exprList->e->iVal);
	}
	else if (exprList->e->type == eIdent ||
		exprList->e->eVal->e->type == eString)
	{
		printf(" ==>%s\n", exprList->e->sVal);
	}
}

//evaluate the expression
//the symtab variable "symboltable" is created in main()
int evaluate(symtab *symboltable, exprs *exprList) {

	expr *e = exprList->e;

	switch (e->type) {
		case eString:
			printf("%s\n", e->sVal);
			hi();
			break;
		case eIdent:
			//+, -, /, or *
			if (strcmp(e->sVal, "+") == 0) {
				result = 0;
				op = '+';
				result = evaluate(symboltable, exprList->n);
			}

			else if (strcmp(e->sVal, "-") == 0) {
				result = 0;
				op = '-';
				result = evaluate(symboltable, exprList->n);
			}
			else if (strcmp(e->sVal, "car") == 0) {
				exprList = exprList->n;
				if (exprList->e->type == eExprList &&
					strcmp(exprList->e->eVal->e->sVal, "quote") == 0) {
					exprList = exprList->e->eVal->n;
					printItem(exprList->e->eVal);
				}

				else{
					fatalError("Not a valid car argument");
				}
			}
			// cdr handling
			else if (strcmp(e->sVal, "cdr") == 0) {
				exprList = exprList->n;
				if (exprList->e->type == eExprList &&
					strcmp(exprList->e->eVal->e->sVal, "quote") == 0) {
					exprList = exprList->e->eVal->n;
					exprList = exprList->e->eVal->n;
					printItem(exprList);
					while (exprList->n != NULL) {
						exprList = exprList->n;
						printItem(exprList);
					}
				}

				else {
					fatalError("Not a valid cdr argument");
				}
			}
			//If user-defined identifier
			else if (strcmp(e->sVal, "define")==0) {
				//assign identifier to a value
				printf("%s\n", exprList->n->e->sVal);
				addToSymTab(symboltable, exprList->n->e->sVal, evaluate(symboltable, exprList->n->n));
			}
			else {
				if(inSymTab(symboltable, e->sVal)) {
					return getDef(symboltable, e->sVal);
				}
				else {
					fatalError("Uninitialized identifier");
				}
				printf("%s\n", e->sVal);
			}

			break;
		case eInt:
			result = e->iVal;
			while (exprList->n != NULL) {
				exprList = exprList->n;
				e = exprList->e;
				if (e->type == eExprList) {
					result = evaluate(symboltable, e->eVal);
				}
				switch (op)
				{
					case '+':
						result += evaluate(symboltable, exprList);
						break;
					case '-':
						result -= evaluate(symboltable, exprList);
						break;
					default:
						break;
				}
			}
			break;
		case eExprList:
			if (e->eVal != NULL) {
				evaluate(symboltable, e->eVal);
			}
			break;
		default:
			fatalError("Unknown type");
			result = 0; /* dead code, assignment shouldn't happen */
			break;
	}

	if (e->type == eIdent &&
		e->sVal != NULL) {
		printf("Node evaluated to: %s\n", e->sVal);
	}

	if (exprList->n != NULL) {
		evaluate(symboltable, exprList->n);
	}
	return (result);
}
