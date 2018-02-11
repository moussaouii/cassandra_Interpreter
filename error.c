#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "boolean.h"

#define debug false

#define NBERRMAX 100
static int NBERROR = 0;
static error * ERROR /*[NBERRMAX]*/;
static booleen ExistsError=false;


#define RESET   "\033[0m"

#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */



void _afficherErreurNotFile(){
    error *e=ERROR;
    switch (e->errorT) {
        case MissingOptionReplication:
            printf(BOLDRED "ConfigurationException :  Missing mandatory option replication in CREATE KEYSPACE %s\n" RESET,
                   e->name);
            break;
        case UnknownOption:
            printf(BOLDRED "ConfigurationException :  The option %s is not supported\n"RESET, e->name);
            break;
        case AlreadyDeclaredOption:
            printf(BOLDRED "SyntaxException: Multiple definition for property %s\n"RESET, e->name);
            break;
        case MissingSubOption:
            printf(BOLDRED "ConfigurationException: Missing mandatory replication strategy %s\n"RESET, e->name);
            break;
        case UndefinedClass:
            printf(BOLDRED "ConfigurationException: Unable to find replication strategy %s\n"RESET, e->name);
            break;
        case AlreadyExists:
            printf(BOLDRED "AlreadyExists: %s %s already exists \n"RESET, e->msg, e->name);
            break;
        case InvalidRequest:
            printf(BOLDRED "InvalidRequest:  message=\"%s\" \n"RESET, e->msg);
            break;
        case SyntaxError:
            printf(BOLDRED "SyntaxException:  %s\n"RESET, e->msg);
            break;
        case UnknownKeyspace:
            printf(BOLDRED "ConfigurationException: Keyspace %s doesn't exist \n"RESET, e->name);
            break;
        case UnknownTable:
            printf(BOLDRED "ConfigurationException: table %s doesn't exist \n"RESET, e->name);
            break;
        case MissingPrimaryKey :
            printf(BOLDRED "InvalidRequest: message=\"Some partition key parts are missing: %s\"\n"RESET, e->name);
            break;
        case UndefinedColumn:
            printf(BOLDRED "InvalidRequest: message=\"Undefined column name : %s\"\n"RESET, e->name);
            break;
        case InvalideTypeInsert:
            printf(BOLDRED "InvalidRequest: message=\"Invalid value %s for column %s\"\n"RESET, e->name, e->msg);
            break;
        case InvalideTypePrimaryKey:
            printf(BOLDRED "InvalidRequest: message=\"Invalid  collection type  for PRIMARY KEY component %s\"\n"RESET,
                   e->name);
            break;
        case InvalidValueNull:
            printf(BOLDRED "InvalidRequest: message=\"Invalid value null  for PRIMARY KEY component %s\"\n"RESET,
                   e->name);
            break;
        case NonPrimaryKeyInWhere:
            printf(BOLDRED "InvalidRequest: message=\"Non PRIMARY KEY columns found in where clause: '%s'\"\n"RESET,
                   e->name);
            break;
        case NonValideOperatorForPartitionKey:
            printf(BOLDRED "InvalidRequest: message=\"Only EQ and IN relation are supported on the partition key (column '%s')\"\n"RESET,
                   e->name);
            break;
        case InvalideOpertorForUpdate:
            printf(BOLDRED "InvalidRequest: message=\"Slice restrictions are not supported on the clustering columns in UPDATE statements(column '%s')\"\n"RESET,
                   e->name);
            break;
        case SomePartitionKeyMissing:
            printf(BOLDRED "InvalidRequest: message=\"Some partition key parts are missing: '%s'\"\n"RESET, e->name);
            break;
        case SomeClusteringMissing:
            printf(BOLDRED "InvalidRequest: message=\"Some clustering keys are missing: '%s'\"\n"RESET, e->name);
            break;
        case InvalidNullInCondition:
            printf(BOLDRED "InvalidRequest: message=\"Invalid null value in condition for column '%s'\"\n"RESET,
                   e->name);
            break;
        case MoreThanOneRestrictionForOneColumn:
            printf(BOLDRED "InvalidRequest: message=\"More than one restriction was found for the start bound on '%s'\"\n"RESET,
                   e->name);
            break;
        case MoreThanRelationEq:
            printf(BOLDRED "InvalidRequest: message=\"%s cannot be restricted by more than one relation if it includes an Equal\"\n"RESET,
                   e->name);
            break;
        case MultipleDefOfColumn:
            printf(BOLDRED "InvalidRequest: message=\"Multiple definition of identifier %s\"\n"RESET, e->name);
            break;
        case MultipleDefIn:
            printf(BOLDRED "InvalidRequest: message=\"Multiple definition of identifier %s in %s\"\n"RESET, e->name,
                   e->msg);
            break;
        case PrimaryKeyInSet:
            printf(BOLDRED "InvalidRequest: message=\"PRIMARY KEY part %s found in SET part\"\n"RESET, e->name);
            break;
        case InvalideTypeInWhere:
            printf(BOLDRED "InvalidRequest: message=\"Invalide collection Type for column %s In where \"\n"RESET,
                   e->name);
            break;
    }
}

void _afficherErreur(/*error *e*/){
    error *e=ERROR;
	switch (e->errorT){
        case MissingOptionReplication:
            printf(BOLDRED "ConfigurationException : line %d Missing mandatory option replication in CREATE KEYSPACE %s\n" RESET, e->line,e->name);
            break;
        case UnknownOption:
            printf(BOLDRED "ConfigurationException : line %d The option %s is not supported\n"RESET,e->line,e->name);
            break;
        case AlreadyDeclaredOption:
            printf(BOLDRED "SyntaxException: line: %d Multiple definition for property %s\n"RESET,e->line,e->name);
            break;
        case MissingSubOption:
            printf(BOLDRED "ConfigurationException: line: %d Missing mandatory replication strategy %s\n"RESET,e->line,e->name);
            break;
        case UndefinedClass:
            printf(BOLDRED "ConfigurationException: line: %d Unable to find replication strategy %s\n"RESET,e->line,e->name);
            break;
        case AlreadyExists:
            printf(BOLDRED "AlreadyExists: line: %d %s %s already exists \n"RESET,e->line,e->msg,e->name);
            break;
        case InvalidRequest:
            printf(BOLDRED "InvalidRequest: line: %d  message=\"%s\" \n"RESET,e->line,e->msg);
            break;
        case SyntaxError:
            printf(BOLDRED "SyntaxException: line: %d  %s\n"RESET,e->line,e->msg);
            break;
        case UnknownKeyspace:
            printf(BOLDRED "ConfigurationException: line: %d Keyspace %s doesn't exist \n"RESET,e->line,e->name);
            break;
        case UnknownTable:
            printf(BOLDRED "ConfigurationException: line: %d table %s doesn't exist \n"RESET,e->line,e->name);
            break;
        case MissingPrimaryKey :
            printf(BOLDRED "InvalidRequest: line: %d message=\"Some partition key parts are missing: %s\"\n"RESET,e->line,e->name);
            break;
        case UndefinedColumn:
            printf(BOLDRED "InvalidRequest: line: %d message=\"Undefined column name : %s\"\n"RESET,e->line,e->name);
            break;
        case InvalideTypeInsert:
            printf(BOLDRED "InvalidRequest: line: %d message=\"Invalid value %s for column %s\"\n"RESET,e->line,e->name,e->msg);
            break;
        case InvalideTypePrimaryKey:
            printf(BOLDRED "InvalidRequest: line: %d message=\"Invalid  collection type  for PRIMARY KEY component %s\"\n"RESET,e->line,e->name);
            break;
        case InvalidValueNull:
            printf(BOLDRED "InvalidRequest: line: %d message=\"Invalid value null  for PRIMARY KEY component %s\"\n"RESET,e->line,e->name);
            break;
        case NonPrimaryKeyInWhere:
            printf(BOLDRED "InvalidRequest: line: %d message=\"Non PRIMARY KEY columns found in where clause: '%s'\"\n"RESET,e->line,e->name);
            break;
        case NonValideOperatorForPartitionKey:
            printf(BOLDRED "InvalidRequest: line: %d message=\"Only EQ and IN relation are supported on the partition key (column '%s')\"\n"RESET,e->line,e->name);
            break;
        case InvalideOpertorForUpdate:
            printf(BOLDRED "InvalidRequest: line: %d message=\"Slice restrictions are not supported on the clustering columns in UPDATE statements(column '%s')\"\n"RESET,e->line,e->name);
            break;
        case SomePartitionKeyMissing:
            printf(BOLDRED "InvalidRequest: line: %d message=\"Some partition key parts are missing: '%s'\"\n"RESET,e->line,e->name);
            break;
        case SomeClusteringMissing:
            printf(BOLDRED "InvalidRequest: line: %d message=\"Some clustering keys are missing: '%s'\"\n"RESET,e->line,e->name);
            break;
        case InvalidNullInCondition:
            printf(BOLDRED "InvalidRequest: line: %d message=\"Invalid null value in condition for column '%s'\"\n"RESET,e->line,e->name);
            break;
        case MoreThanOneRestrictionForOneColumn:
            printf(BOLDRED "InvalidRequest: line: %d message=\"More than one restriction was found for the start bound on '%s'\"\n"RESET,e->line,e->name);
            break;
        case MoreThanRelationEq:
            printf(BOLDRED "InvalidRequest: line: %d message=\"%s cannot be restricted by more than one relation if it includes an Equal\"\n"RESET,e->line,e->name);
            break;
        case MultipleDefOfColumn:
            printf(BOLDRED "InvalidRequest: line: %d message=\"Multiple definition of identifier %s\"\n"RESET,e->line,e->name);
            break;
        case MultipleDefIn:
            printf(BOLDRED "InvalidRequest: line: %d message=\"Multiple definition of identifier %s in %s\"\n"RESET,e->line,e->name,e->msg);
            break;
        case PrimaryKeyInSet:
            printf(BOLDRED "InvalidRequest: line: %d message=\"PRIMARY KEY part %s found in SET part\"\n"RESET,e->line,e->name);
            break;
        case InvalideTypeInWhere:
            printf(BOLDRED "InvalidRequest: line: %d message=\"Invalide collection Type for column %s In where \"\n"RESET,e->line,e->name);
            break;
//        InvalidRequest: Error from server: code=2200 [Invalid query] message="Multiple definition of identifier b"

    }
}


int _nombreErreurs(){
	return NBERROR;
}

void _creerErreur(errorType et, int line, char* name,char *msg){
    if(!_existsError()){
        ERROR/*[NBERROR++]*/=(error *)_intialseErreur(et, line, name,msg);
    }
    ExistsError=true;
}


booleen _existsError(){
    return ExistsError;
}

void reintialseErreur(){
    ExistsError=false;
}


error* _intialseErreur(errorType et, int line, char * name,char* msg){
	error * e = (error*) malloc (sizeof (error) );
    if(name!=NULL){
        e->name = (char *) malloc (strlen(name)*sizeof(char)+1);
        strcpy(e->name, name);
    }else{
        e->name=NULL;
    }
    if(msg!=NULL){
        e->msg = (char *) malloc (strlen(msg)*sizeof(char)+1);
        strcpy(e->msg, msg);
    }else{
        e->msg=NULL;
    }



	e->line = line;
	e->errorT = et;
	return e;
}

//void _afficherErreurs(){
//	int i= 0;
//	while (i < NBERROR) {
//		_afficherErreur(ERROR[i]);
//		i++;
//	}
//}


