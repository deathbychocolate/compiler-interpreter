
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "phun.h"

int result = 0;
char op = ' ';
int initialise = 1;
symtab *symboltable;

exprs *makeNewResultList(exprs *resultList, exprs *e) {
	if (resultList == NULL) {
		resultList = newExprList(e->e, e->n);
	}
}
expr *makeNewExpr(expr *e) {
	expr *result = malloc(sizeof(expr));
	result->type = e->type;
	result->iVal = e->iVal;
	result->sVal = e->sVal;

	if (result->type == eExprList) {
		result->eVal = makeNewResultList(e->eVal, e->eVal->e);
	}
	return result;
}

exprs *addToResultList(exprs *resultList, expr *e) {
	expr *tempExpr = makeNewExpr(e);
	if (resultList == NULL) {
		resultList = newExprList(NULL,NULL);
	}
	exprs *temp = resultList;
	while (temp->n!=NULL && temp->e != NULL)
	{
		temp = temp->n;
	}
	temp->e = tempExpr;

	return resultList;
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
exprs *evaluate(exprs *exprList) {

	expr *e = exprList->e;
	exprs *ret; //see eInt case

	switch (e->type) {
		case eString:
			printf("%s\n", e->sVal);
			break;
		case eIdent:
			//+, -, /, or *
			if (strcmp(e->sVal, "+") == 0) {
				op = '+';
				while (exprList->n != NULL) {
					exprList = exprList->n;
					e = exprList->e;
					//if (e->type == eExprList) {
					//	ret = evaluate(e->eVal);

					//}
					if(e->type==eIdent){
						if(inSymTab(e->sVal)){
							result += getDef(symboltable, e->sVal);
						}
						else{
							fatalError("Invalid recursive program!");
						}
					}
					else{
						result += e->iVal;
					}
				}
				printf(" ==>%d\n", result);
			}

			else if (strcmp(e->sVal, "-") == 0) {
				op = '-';
				while (exprList->n != NULL) {
					exprList = exprList->n;
					e = exprList->e;
					//if (e->type == eExprList) {
					//	ret = evaluate(e->eVal);

					//}
					if(e->type==eIdent){
						if(inSymTab(e->sVal)){
							result += getDef(symboltable, e->sVal);
						}
						else{
							fatalError("Invalid recursive program!");
						}
					}
					else{
						result -= e->iVal;
					}
				}
				printf(" ==>%d\n", result);
			}

			else if (strcmp(e->sVal, "*") == 0) {
				op = '*';
				while (exprList->n != NULL) {
					exprList = exprList->n;
					e = exprList->e;
					//if (e->type == eExprList) {
					//	ret = evaluate(e->eVal);

					//}
					if(e->type==eIdent){
						if(inSymTab(e->sVal)){
							result += getDef(symboltable, e->sVal);
						}
						else{
							fatalError("Invalid recursive program!");
						}
					}
					else{
						result *= e->iVal;
					}
				}
				printf(" ==>%d\n", result);
			}

			else if (strcmp(e->sVal, "/") == 0) {
				op = '/';
				while (exprList->n != NULL) {
					exprList = exprList->n;
					e = exprList->e;
					//if (e->type == eExprList) {
					//	ret = evaluate(e->eVal);

					//}
					if(e->type==eIdent){
						if(inSymTab(e->sVal)){
							result += getDef(symboltable, e->sVal);
						}
						else{
							fatalError("Invalid recursive program!");
						}
					}
					else{
						result /= e->iVal;
					}
				}
				printf(" ==>%d\n", result);
			}
			// car handling
			else if (strcmp(e->sVal, "car") == 0) {
				exprList = exprList->n;
				if (exprList->e->type == eExprList &&
					strcmp(exprList->e->eVal->e->sVal, "quote") == 0) {
					exprList = exprList->e->eVal->n;
					printItem(exprList->e);
					return exprList->n;
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
					return exprList;
				}
				else {
					fatalError("Not a valid cdr argument");
				}
			}
			// prints off a list with provided values
			else if (strcmp(e->sVal, "list") == 0) {
				exprs *resultList = NULL;
				exprList = exprList->n;
				if (exprList->e->type == eExprList &&
					(strcmp(exprList->e->eVal->e->sVal, "quote") == 0)) {
					resultList= makeNewResultList(resultList,exprList->e->eVal->n);
				}
				else if (exprList->e->type == eInt || exprList->e->type == eString) {
					resultList= makeNewResultList(resultList,exprList);
				}
				else if (exprList->e->type == eIdent && inSymTab(exprList->e->sVal)) {
					exprList->e->iVal = getDef(symboltable, exprList->e->sVal);
					exprList->e->type = eInt;
					exprList->e->sVal = NULL;
					resultList= makeNewResultList(resultList,exprList);
				}
				printf("( ");
				printResultList(resultList);
				printf(")\n");
				// Redirect pointer to end of list
				while (exprList->n != NULL) {
					exprList = exprList->n;
				}
			}

			//cons case
			else if (strcmp(e->sVal, "cons")==0) {
				exprs *resultList = NULL;
				exprList = exprList->n;
				e = exprList->e;
				if (e->type == eExprList) {
					if (e->eVal->e->type == eIdent
						&& strcmp(e->eVal->e->sVal, "quote") == 0) {
						resultList= addToResultList(resultList, e->eVal->n->e);
					}
				}
				else if (e->type == eIdent) {
					if (inSymTab(e->sVal)) {
						e->iVal = getDef(symboltable, e->sVal);
						e->type = eInt;
						e->sVal = NULL;
						resultList= addToResultList(resultList, e);
					}
					else {
						fatalError("Item not found in symbol table!\n");
					}
				}
				exprList = exprList->n;
				e = exprList->e;
				if (e->type == eExprList) {
					if (e->eVal->e->type == eIdent
						&& strcmp(e->eVal->e->sVal, "quote") == 0) {
						//add the rest of this
					}
				}
				else if (e->type == eIdent) {
					if (inSymTab(e->sVal)) {
						e->iVal = getDef(symboltable, e->sVal);
						e->type = eInt;
						e->sVal = NULL;
						//add the rest of this
					}
				}
				else {
					//add the rest of this
				}
				printResultList(resultList);
				// Redirect pointer to end of list
				while (exprList->n != NULL) {
					exprList = exprList->n;
				}
			}

			//If user-defined identifier
			else if (strcmp(e->sVal, "define")==0) {
				//assign identifier to a value
				exprs *eval;
				eval = evaluate(exprList->n->n);
				symboltable = addToSymTab(exprList->n->e->sVal, eval->e->iVal);
			}
			else {
				if(inSymTab( e->sVal)) {
					exprList->e->iVal = getDef(symboltable, e->sVal);
					exprList->e->type = eInt;
					exprList->e->sVal = NULL;
					return exprList;
				}
				else {
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
					ret = evaluate(e->eVal);

				}
				switch (op)
				{
					case '+':
						break;
					case '-':
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
	return exprList;
}
