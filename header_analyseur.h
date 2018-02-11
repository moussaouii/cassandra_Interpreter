//
// Created by ismail on 13/02/17.
//

#ifndef HEADER_ANALYSEUR_H
#define HEADER_ANALYSEUR_H

#include "boolean.h"
#include "list.h"
boolean _idf();
boolean _term(term *t);
boolean _term_without_idf();
boolean _constant(cqlConstant *);
boolean _literal(term *);
boolean _bind_marker();
boolean _function_call();
boolean _terms_func_call();
boolean _terms_func_call_aux();
boolean _map_set_udt(term *);
boolean _udt_literal();
boolean _udt_literal_aux();
boolean _terms_map(ListeMap *);
boolean _terms_map_aux(ListeMap *);
boolean _terms_set(ListeList *li);
boolean _terms_set_aux(ListeList *li);
boolean _list(ListeList *);
boolean _terms_list(ListeList *);
boolean _terms_list_aux(ListeList *);
boolean _hint_type_tuple(term *t);
boolean _tuple_literal(ListeList *);
boolean _tuple_literal_aux(ListeList *);
boolean _tuple_literal_main(ListeList *);
boolean _if_not_exists();
boolean _if_exists();
boolean _type(cql_type *ct);
boolean _native_type(cql_type *ct);
boolean _collection_type(cql_type *ct);
boolean _tuple_type();
boolean _types_tuple();
boolean _types_tuple_aux();
boolean _user_defined_type();
boolean _create_type_stmt();
boolean _create_type_stmt_aux();
boolean _field_definitions();
boolean _field_definitions_aux();
boolean _field_definition();
boolean _alter_type_stmt();
boolean _alter_type_modification();
boolean _list_idf_to();
boolean _list_idf_to_aux();
boolean _drop_type_stmt();

boolean _table_name(tableName *tN);

//option Keyspace
boolean _option(optionKeyspace *ok);
boolean _options(ListeOptionKeyspace *li);
boolean _list_options(ListeOptionKeyspace *li);
boolean _list_options_aux(ListeOptionKeyspace *li);



boolean _create_keyspace_stmt();
boolean _use_stmt();
boolean _alter_keyspace_stmt();
boolean _drop_keyspace_stmt();
boolean _create_table_stmt();
boolean _create_table_stmt_aux(boolean ifNotExists);
boolean _create_table_table_option();
boolean _column_definitions(ListeColumn *li);
boolean _list_column_definitions(ListeColumn *li);
boolean _list_column_definitions_aux(ListeColumn *li);
boolean _column_definition(objetColumn *oc);
boolean _primary_key(ListeName *liPartitionKey,ListeName *liClusteringCol);
boolean _partition_key(ListeName *li);
boolean _column_names_main(ListeName *li);
boolean _column_names(ListeName *li);
boolean _column_names_aux(ListeName *li);
boolean _clustering_columns(ListeName *li);
boolean _table_options(ListeOptionTable *li);
boolean _list_table_options(ListeOptionTable *li);
boolean _list_table_options_aux(ListeOptionTable *li);
boolean _table_option(optionTable *oT);
boolean _clustering_order();
boolean _cloumn_orders();
boolean _cloumn_orders_aux();
boolean _cloumn_order();
boolean _alter_table_stmt();
boolean _alter_table_instruction();
boolean _list_column_types();
boolean _list_column_types_aux();
boolean _column_names_alter();
boolean _column_names_alter_aux();
boolean _drop_table_stmt();
boolean _truncate_stmt();
boolean _select_stmt();
boolean _select_aux();
boolean _select_where();
boolean _select_group_by();
boolean _select_order_by();
boolean _select_pre_partition();
boolean _select_limit();
boolean _select_allow_filtering();
boolean _select_clause(ListeSelector *li);
boolean _selectors_as(ListeSelector *li);
boolean _selectors_as_aux(ListeSelector *li);
boolean _selector_as(objetSelector *);
boolean _selector(objetSelector *);
boolean _selectors_main();
boolean _selectors();
boolean _selectors_aux();
boolean _where_clause(ListeRelation *li);
boolean _relations(ListeRelation *li);
boolean _relations_aux(ListeRelation *li);
boolean _relation(objetRelation *o);
boolean _group_by_clause();
boolean _ordering_clause();
boolean _column_names_ordring();
boolean _column_names_ordring_aux();
boolean _column_name_asc_desc();
boolean _operator(_operatorEnum *);
boolean _insert_stmt();
boolean _insert_if_not_exists();
boolean _insert_using();
boolean _update_parameters();
boolean _update_parameters_aux();
boolean _names_values();
boolean _json_clause();
boolean _column_names_values(ListeName *li);
boolean _update_stmt();
boolean _update_using();
boolean _update_set();
boolean _update_parameter();
boolean _update_if();
boolean _assignment(objetNameValue *);
boolean _assignments(ListeNameValue *);
boolean _assignments_aux(ListeNameValue *);
boolean _simple_selection();
boolean _simple_selection_aux();
boolean _condition();
boolean _conditions();
boolean _conditions_aux();
boolean _delete_stmt();
boolean _delete_from();
boolean _delete_where();
boolean _simple_selections();
boolean _simple_selections_aux();
boolean _batch_stmt();
boolean _batch_using();
boolean _batch_modification();
boolean _modification_statements();
boolean _modification_statements_aux();
boolean _modification_statement();
boolean _statement();
boolean _create_stmts();
boolean _alter_stmts();
boolean _drop_stmts();
boolean _statements_main();
boolean _statements();
boolean _statements_aux();

boolean _descripe_stmt();
boolean _descripe();


boolean _descripe_tables();
#endif //HEADER_ANALYSEUR_H
