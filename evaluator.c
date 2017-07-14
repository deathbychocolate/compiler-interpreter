
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "phun.h"

int result = 0;
char op = ' ';
int initialise = 1;
exprs *resultList;
symtab *symboltable;

void addToResultList(exprs *e) {
	if (resultList == NULL) {
		resultList = newExprList(e->e, e->n);
	}
}

void printResultList(exprs *list){

	exprs *temp = list;
	while (temp != NULL)
	{
		if (temp->e->type == eInt) {
			printf(" %d ", temp->e->iVal);
		}
		else if (temp->e->type == eIdent ||
			temp->e->type == eString)
		{
			printf(" %s ", temp->e->sVal);
		}
		else if (temp->e->type == eExprList) {
			printResultList(temp->e->eVal->n);
		}
		temp = temp->n;
	}

}

//check if the given identifier is in the symbol table
int inSymTab( char *id) {
	symtab *temp = symboltable;
	while (temp!=NULL && temp->ident != NULL) {
		if (strcmp(temp->ident,id) == 0) {
			return 1;
		}
		else if(temp->nextdef != NULL) {
			temp = temp->nextdef;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

//adds an evaluation of an identifier to symbol table
symtab* addToSymTab( char *id, int eval) {
	if (symboltable == NULL) {
		symboltable = malloc(sizeof(symtab));
		symboltable->def = NULL;
		symboltable->ident = NULL;
		symboltable->nextdef = NULL;
	}
	symtab *cursor = symboltable;
	if (cursor->def == NULL) {
		node *newNode = malloc(sizeof(node));
		newNode->iVal = eval;
		newNode->type = eInt;

		symtab *newdef = malloc(sizeof(symtab));
		newdef->ident = id;
		newdef->def = newNode;
		newdef->nextdef = NULL;
		symboltable = newdef;
	}
	else {
		while (cursor->nextdef != NULL) {
			cursor = cursor->nextdef;
		}

		node *newNode = malloc(sizeof(node));
		newNode->iVal = eval;
		newNode->type = eInt;

		symtab *newdef = malloc(sizeof(symtab));
		cursor->nextdef = newdef;
		newdef->ident = id;
		newdef->def = newNode;
	}
	return symboltable;
}

//gets definition of identifier in symbol table
int getDef(symtab *sthead, char *ident) {

	symtab *cursor = sthead;

	while(cursor != NULL) {

		if(strcmp(cursor->ident, ident) == 0) {
			return cursor->def->iVal;
		}
		cursor = cursor->nextdef;

	}

	//This should not happen! We already checked if
	//the value existed in the symtab
	fatalError("Something went wrong!");
	return 0;

}

void printItem(expr *e) {
	if (e->type == eInt) {
		printf(" ==>%d\n", e->iVal);
	}
	else if (e->type == eIdent ||
		e->eVal->e->type == eString)
	{
		printf(" ==>%s\n", e->sVal);
	}
}

//evaluate the expression
//the symtab variable "symboltable" is created in main()
int evaluate(exprs *exprList) {

	expr *e = exprList->e;

	switch (e->type) {
		case eString:
			printf("%s\n", e->sVal);
			break;
		case eIdent:
			//+, -, /, or *
			if (strcmp(e->sVal, "+") == 0) {
				result = 0;
				op = '+';
				result = evaluate(exprList->n);
			}

			else if (strcmp(e->sVal, "-") == 0) {
				result = 0;
				op = '-';
				result = evaluate(exprList->n);
			}
			// car handling
			else if (strcmp(e->sVal, "car") == 0) {
				exprList = exprList->n;
				if (exprList->e->type == eExprList &&
					strcmp(exprList->e->eVal->e->sVal, "quote") == 0) {
					exprList = exprList->e->eVal->n;
					printItem(exprList->e);
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
			// prints off a list with provided values
			else if (strcmp(e->sVal, "list") == 0) {
				while (exprList->n != NULL) {
					exprList = exprList->n;
					if (exprList->e->type == eExprList &&
						(strcmp(exprList->e->eVal->e->sVal, "quote") == 0)) {
						addToResultList(exprList->e->eVal->n);
					}
					else if (exprList->e->type == eInt || exprList->e->type == eString) {
						addToResultList(exprList);
					}
					else if (exprList->e->type == eIdent && inSymTab(exprList->e->sVal)) {
						exprList->e->iVal = getDef(symboltable, exprList->e->sVal);
						exprList->e->type = eInt;
						exprList->e->sVal = NULL;
						addToResultList(exprList);
					}
				}
				printf("( ");
				printResultList(resultList);
				printf(")\n");
			}
			//If user-defined identifier
			else if (strcmp(e->sVal, "define")==0) {
				//assign identifier to a value
				int eval = evaluate(exprList->n->n);
				symboltable = addToSymTab(exprList->n->e->sVal, eval);
			}
			else {
				if(inSymTab( e->sVal)) {
					return getDef(symboltable, e->sVal);
				}
				else {
					printf(e->sVal);
					fatalError("Uninitialized identifier");
				}
			}

			break;
		case eInt:
			result = e->iVal;
			while (exprList->n != NULL) {
				exprList = exprList->n;
				e = exprList->e;
				if (e->type == eExprList) {
					result = evaluate(e->eVal);
				}
				switch (op)
				{
					case '+':
						result += evaluate(exprList);
						break;
					case '-':
						result -= evaluate(exprList);
						break;
					default:
						break;
				}
			}
			break;
		case eExprList:
			if (e->eVal != NULL) {
				evaluate( e->eVal);
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
		evaluate(exprList->n);
	}
	return (result);
}
