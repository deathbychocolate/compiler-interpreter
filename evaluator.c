
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "phun.h"

int result = 0;
char op = ' ';

void hi()
{
	printf("I AM AT %d\n" + __LINE__);
}

int evaluate(exprs *exprList) {

	expr *e = exprList->e;

	switch (e->type) {
		case eString:
			printf("%s\n", e->sVal);
			hi();
			break;
		case eIdent:
			if (strcmp(e->sVal, "+") == 0) {
				result = 0;
				op = '+';
				result = evaluate(exprList->n);
			}

			if (strcmp(e->sVal, "-") == 0) {
				result = 0;
				op = '-';
				result = evaluate(exprList->n);
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
				evaluate(e->eVal);
			}
			break;
		default:
			fatalError("Unknown type");
			result = 0; /* dead code, assignment shouldn't happen */
			break;
	}
	if (e->sVal != NULL) {
		printf("Node evaluated to: %s\n", e->sVal);
	}

	if (exprList->n != NULL) {
		evaluate(exprList->n);
	}
	return (result);
}
