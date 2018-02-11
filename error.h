#ifndef ERROR_H
#define ERROR_H

#include "analyseur_cql.h"
#include "list.h"

typedef enum {
	NonDeclaredKey,
    SyntaxError,
    AlreadyDeclaredOption,
	AlreadyExists,
    MissingSubOption,
    UnknownOption,
    UndefinedClass,
    InvalidRequest,
    MissingOptionReplication,
	UnknownKeyspace,
	UnknownTable,
    MissingPrimaryKey,
    UndefinedColumn,
    InvalideTypeInsert,
	InvalideTypePrimaryKey,
    InvalidValueNull,
	NonPrimaryKeyInWhere,
	NonValideOperatorForPartitionKey,
	InvalideOpertorForUpdate,
	SomePartitionKeyMissing,
	SomeClusteringMissing,
	InvalidNullInCondition,
    MoreThanOneRestrictionForOneColumn,
    MoreThanRelationEq,
    MultipleDefOfColumn,
    MultipleDefIn,
    PrimaryKeyInSet,
    InvalideTypeInWhere

} errorType;

typedef struct {
	char *name; // nom de l’identificateur qui pose problème
	int line;
    char* msg;
	errorType errorT;
} error;



error* _intialseErreur(errorType et, int line, char * name,char *msg);
void _creerErreur(errorType et, int line, char* name,char *msg);
void _afficherErreurNotFile();
void _afficherErreur(/*error *e*/);
void _afficherErreurs();
int _nombreErreurs();
void reintialseErreur();
booleen _existsError();



#endif

