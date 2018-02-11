
#ifndef SEMANTIQUE_H
#define SEMANTIQUE_H

#include "list.h"
#include "boolean.h"
boolean _valide_option_replication(optionKeyspace *ch,char *name,int line);
boolean _valide_option_durable_write(optionKeyspace *ch,char *name,int line);
boolean _valide_option_keyspace_create(ListeOptionKeyspace *li,char *name,int line);
boolean _valide_option_keyspace_alter(ListeOptionKeyspace *li,char *name,int line);

objetKeyspace *trouverKeyspace(ListeKey *liKey,tableName *tN,objetKeyspace *currentKeyspace,int yylineno);
void _valide_column_primary_key(ListeColumn *liCol,ListeName *liPartitionKey,int line,ListeName *liClusteringCol);
void _valide_column(ListeColumn *liCol,int line);

void *_chercher_table_in_keyspace(objetKeyspace *objetKey,char *name,boolean ifNotExists, int yylineno);
objetTable *_chercher_table(objetKeyspace *objetKey,char *name,int line,boolean ifExsit);



void _valide_select_where(objetTable *table,ListeRelation *li,int line);


void _valide_selector(objetTable *table,ListeSelector * li,int line);


boolean _valide_insert_nbr_names_values(ListeName *liName,ListeList *liValue,int line);


boolean  _valide_insert_primary_key(ListeName *liName,ListeList *liValue,ListeName* liPartitionKey,ListeName *liClustringOrder,int line);
boolean _valide_type(term *t,cql_type *ctype, int line);
void _valide_insert(ListeName *liName,ListeList *liValue,objetTable *table, int line);


char *_term_value(term *t);


void _valide_delete_where(objetTable *table,ListeRelation *li,int line);

void _valide_update_where(objetTable *table,ListeRelation *li,int line);
void _column_in_more_than_set(ListeNameValue *li,int line);
void _valide_update_set(objetTable *table,ListeNameValue *li,int line);
void _column_in_more_than_relation(ListeRelation *li,int line);
char *_type_name(cql_type *t);

void _multiple_def_of_column(ListeColumn *li,int line);

void _multiple_declaration(ListeName *li,int line,char *msg);
#endif
