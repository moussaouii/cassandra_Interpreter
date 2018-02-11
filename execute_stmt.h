//
// Created by ismail on 15/02/17.
//

#ifndef EXECUTE_STMT_H
#define EXECUTE_STMT_H

#include "list.h"
#include "boolean.h"


void _dropTableStmt(dropTableStmt *stmt);

void _updateStmt(updateStmt *stmt);

void printNameValue(objetNameValue *objet);
void _deleteStmt(deleteStmt *stmt);
deleteStmt *_initDeleteStmt();
void printRelation(objetRelation *o);

void _afficherData(ListeData *liData,ListeSelector *liSelector);

objetNameValue *_createNameValue(char *name);

objetColumn *_create_column_with_name(char *name);
void printString(char *name);

void printOptionTable(optionTable *oT);
void printColumn(objetColumn *oC);
void printType(cql_type *t);


objetColumn *_create_column();
void printKeyspaceDesc(objetKeyspace *oks) ;
int comparerKeyspace(objetKeyspace* objet1, objetKeyspace* objet2);
void afficherConstant(cqlConstant constant);

void afficherOptionKeyspace(optionKeyspace *oks);
void printTerm(term *t);

void printMap(objetMap *oM);
void printKeyspace(objetKeyspace *oks);



objetTable *_createTable(char* name);

objetKeyspace *_createKeyspace(char* name);



void _createKeyspaceStmt(createKeyspaceStmt *cKS);

void _alterKeyspaceStmt(alterKeyspaceStmt *aKS);

void UnionOptionKeyspace(ListeOptionKeyspace *li,ListeOptionKeyspace *li2);

void printTable(objetTable *ot);

void _dropKeyspaceStmt(dropKeyspaceStmt *dKS);


void _useKeyspaceStmt(useKeyspaceStmt *uKS);

void _descripe_Keyspace(descripeKeyspaceStmt *dKS);

void _excuteStmt(stmt *s);

void _createTableStmt(createTableStmt *cts);


void _truncatStmt(truncatStmt *stmt);

void _describeTableStmt(describeTableStmt *stmt);
tableName *_create_tableName();
#endif //CASSANDRA1_EXECUTE_STMT_H
