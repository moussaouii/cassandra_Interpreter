//
// Created by ismail on 17/02/17.
//

#ifndef CASSANDRA1_PERSISTANCE_H
#define CASSANDRA1_PERSISTANCE_H

#include "list.h"
#include "src/jansson.h"

void _update_persistance(char *nameKeyspace,char* nameTable,ListeName *liPartitionKey,ListeName *liClustring,ListeName *liStatic,ListeNameValue *liData,boolean ifExist);
boolean _verfier_relation(term  *t1,term *t2,_operatorEnum *_operator);
boolean _verfier_relation_aux(int nb,_operatorEnum op);
boolean _verfier_relations(json_t* json_row,ListeRelation *liRelation);

boolean _delete_persistance_aux(json_t *json_data,char *nameKeyspace,char* nameTable,ListeRelation *liRelation);

void _delete_persistance(char *nameKeyspace,char* nameTable,ListeName *liPartitionKey,ListeRelation *liRelation);

void _insert_persistance(char *nameKeyspace,char* nameTable,ListeName *liParitionKey,ListeName *liClustring,ListeName *liStatic,ListeNameValue *li,boolean ifNotexist);

char *termToString(term *t);
int _comparerTerm(term *term1,term* term2);
term *_getTerm(ListeList *liValue,ListeName *liName,char *name);

void _writeData(json_t *json_row_data,ListeNameValue *liData);
//load
ListeKey *_load();
void _loadArrTable(char *name_key,ListeTable* li);
void _loadArrColumn(char *pathTable,ListeColumn *li,ListeName *liPartitionKey,ListeName *liClustringOrder);

void _loadColumn(json_t* json_column,objetColumn *column);
void _loadType(json_t* json_type,cql_type *cqlType);
void _loadArrOptionTable(char *pathTable,ListeOptionTable*li);
void _loadOptionTable(json_t *json_option,optionTable *option);

void _loadArrOptionKey(char *name_key,ListeOptionKeyspace *li);


void _loadOptionKey(json_t *json_option,optionKeyspace *option );

void _loadConstant(json_t *json_constant,cqlConstant *constant);

void _loadMap(json_t *json_map,ListeMap *map);

void _loadList(json_t *json_list,ListeList *list);
void _loadTerm(json_t *json_term,term* term);
//write
void _write(ListeKey *liKey);
void _writeArrTable(char *pathKeyspace,ListeTable *li);
void _writeArrOptionTable(char *pathTable,ListeOptionTable* li);
void _writeOptionTable(json_t *json_ot,optionTable *ot);

void _writeArrOptionKey(char *pathKeyspace,ListeOptionKeyspace *li);
void _writeOptionKey(json_t *json_ok,optionKeyspace *ok);

void _writeMap(json_t* map,ListeMap* li);


void _writeList(json_t *json_list,ListeList *li);

void _writeTerm(json_t *json_term,term *term1);
void _writeConstant(json_t* json_constant,cqlConstant constant);
void _writeArrColumn(char *pathTable,ListeColumn *li,ListeName *liPartitionKey,ListeName *liClustringOrder);


void _writeColumn(json_t *json_oc,objetColumn *oc);


void _writeType(json_t *json_type,cql_type *type);

//gestion directory

void _dropDirectoryKeyspace(char *name);
void _dropDirectoryTable(char *keyspace,char *table);

void _dropDirectoryFichier(char *keyspace,char *table);

int remove_directory(const char *path);

void _select_persistance(char *nameKeyspace,char* nameTable,ListeRelation *liRelation,ListeSelector *liSelector,int limit);

#endif //CASSANDRA1_PERSISTANCE_H
