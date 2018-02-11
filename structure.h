#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "boolean.h"
/*=========================================ListName==========================================*/
typedef struct element_name{
    char* reference;
    struct element_name* suivant;
} ElementName;

typedef struct {
    ElementName* premier; /*premier élément de la liste*/
    ElementName* dernier; /*dernier élément de la liste*/
    ElementName* courant; /* élément en cours de traitement (parcours de liste) */
    int nbElt; /* nombre des element */
    char* (*toString) (char*);
    int (*comparer) (char*,char*);
} ListeName;



/*================================struct=Type=======================================*/
//pointeur sur cql_type
typedef struct cql_type * pCqlType;
//native type
typedef enum {
    ascii_native,
    bigint_native ,
    blob_native ,
    boolean_native ,
    counter_native ,
    date_native ,
    decimal_native ,
    double_native ,
    float_native ,
    inet_native ,
    int_native ,
    smallint_native ,
    text_native ,
    time_native,
    timestamp_native ,
    timeuuid_native ,
    tinyint_native ,
    uuid_native ,
    varchar_native ,
    varint_native
} native_type;

//collection_type
typedef struct {
    pCqlType champ1;
    pCqlType champ2;
} map_type;

typedef struct {
    pCqlType champ;
} list_type;

typedef struct {
    pCqlType champ;
} set_type;

//costum_type
typedef struct {
    char* name;
} costum_type;

//tuple_type
typedef struct{
    pCqlType champ;
}objetTupleT;


typedef struct element_TupleT{
    objetTupleT* reference;
    struct element_TupleT* suivant;
} ElementTupleT;

typedef struct {
    ElementTupleT* premier; /*premier élément de la liste*/
    ElementTupleT* dernier; /*dernier élément de la liste*/
    ElementTupleT* courant;  //élément en cours de traitement (parcours de liste)
    int nbElt; /* nombre des element */
    char* (*toString) (objetTupleT*);
    int (*comparer) (objetTupleT*,objetTupleT*);
} ListeTupleT;


typedef struct {
    ListeTupleT *li;
} tuple_type;

//udt_type
typedef struct{
    char* name;
    pCqlType type;
}objetTypesUdt;


typedef struct element_typesudt{
    objetTypesUdt* reference;
    struct element_typesudt* suivant;
} ElementTypesUdt;

typedef struct {
    ElementTypesUdt* premier; /*premier élément de la liste*/
    ElementTypesUdt* dernier; /*dernier élément de la liste*/
    ElementTypesUdt* courant; /* élément en cours de traitement (parcours de liste) */
    int nbElt; /* nombre des element */
    char* (*toString) (objetTypesUdt*);
    int (*comparer) (objetTypesUdt*,objetTypesUdt*);
} ListeTypesUdt;

typedef struct {
    char *name;
    ListeTypesUdt *liTU;
}udt_type;

//type type
typedef enum
{
    native_typeE,
    map_typeE,
    set_typeE,
    list_typeE,
    udt_typeE,
    tuple_typeE,
    custom_typeE
}type_cql_type;

typedef union{
    native_type native;
    map_type map;
    list_type list;
    //set_type set;
    costum_type costum;
    tuple_type tuple;
    udt_type udt;
}nodeType;


typedef struct cql_type{
    type_cql_type type;
    nodeType node;
}cql_type;

/*==================================struct=List=type=======================================*/

//PS:pour un keyspace
typedef struct{
    ListeTypesUdt *liTU;
}objetUdt;


typedef struct element_udt{
    objetUdt* reference;
    struct element_udt* suivant;
} ElementUdt;

typedef struct {
    ElementUdt* premier; /*premier élément de la liste*/
    ElementUdt* dernier; /*dernier élément de la liste*/
    ElementUdt* courant; /* élément en cours de traitement (parcours de liste) */
    int nbElt; /* nombre des element */
    char* (*toString) (objetUdt*);
    int (*comparer) (objetUdt*,objetUdt*);
} ListeUdt;

/*========================+==============struct=Term==================================*/
//const
typedef enum{
    string_const , integer_const , float_const ,
    boolean_const , uuid_const , blob_const , null_const
}type_constant;

typedef union{
    int integer;
    double _float;
    //int boolean;
    char *string;
    //char *uuid;
    //char* blob;
    //int _null;
}nodeConstant;

typedef struct {
    type_constant type;
    nodeConstant node;
}cqlConstant;


//pointeur sur term
struct term;
typedef struct term * pTerm;

//map

typedef struct{
    pTerm champ1;
    pTerm champ2;
}objetMap;


typedef struct element_map{
    objetMap* reference;
    struct element_map *suivant;
}ElementMap;

typedef struct {
    ElementMap* premier; /*premier élément de la liste*/
    ElementMap* dernier; /*dernier élément de la liste*/
    ElementMap* courant; /* élément en cours de traitement (parcours de liste) */
    int nbElt; /* nombre des element */
    char* (*toString) (objetMap*);
    int (*comparer) (objetMap*,objetMap*);
}ListeMap;

//List
//on utilise la meme liste pour list et set on change seulement le type ;


typedef struct element_list{
    pTerm reference;
    struct element_list* suivant;
}ElementList;

typedef struct {
    ElementList* premier; /*premier élément de la liste*/
    ElementList* dernier; /*dernier élément de la liste*/
    ElementList* courant; /* élément en cours de traitement (parcours de liste) */
    int nbElt; /* nombre des element */
    char* (*toString) (pTerm);
    int (*comparer) (pTerm,pTerm);
}ListeList;


//Set
typedef struct{
    pTerm champ;
}objetSet;


typedef struct element_set{
    objetSet* reference;
    struct element_set* suivant;
}ElementSet;

typedef struct {
    ElementSet* premier; /*premier élément de la liste*/
    ElementSet* dernier; /*dernier élément de la liste*/
    ElementSet* courant; /* élément en cours de traitement (parcours de liste) */
    int nbElt; /* nombre des element */
    char* (*toString) (objetSet*);
    int (*comparer) (objetSet*,objetSet*);
}ListeSet;

//Tuple
typedef struct{
    pTerm champ;
}objetTuple;


typedef struct element_tuple{
    objetTuple* reference;
    struct element_tuple* suivant;
}ElementTuple;

typedef struct {
    ElementTuple* premier; /*premier élément de la liste*/
    ElementTuple* dernier; /*dernier élément de la liste*/
    ElementTuple* courant; /* élément en cours de traitement (parcours de liste) */
    int nbElt; /* nombre des element */
    char* (*toString) (objetTuple*);
    int (*comparer) (objetTuple*,objetTuple*);
}ListeTuple;


//il manque function_call | type_hint | bind_marker | udt_literal
typedef enum {
    map_term,
    set_term,
    list_term,
    constant_term,
    tuple_term
}type_term;

typedef union{
    ListeList *list;
    //ListeList *set;
    ListeMap *map;
    //ListeTuple *tuple;
    cqlConstant constant;
}nodeTerm;


typedef struct term{
    type_term type;
    nodeTerm node;
}term;


/*=================================Option=Keyspace==================================*/
typedef enum
{
    map_opt,
    idf_opt,
    constant_opt
}type_option;


typedef union{
    ListeMap *map;
    char* idf_value;
    cqlConstant constant;
}nodeOptionKeyspace;

typedef struct {
    type_option type;
    char *idf;//key
    nodeOptionKeyspace node;
}optionKeyspace;

//list_option_keyspace

typedef struct element_option_keyspace{
    optionKeyspace* reference;
    struct element_option_keyspace* suivant;
} ElementOptionKeyspace;

typedef struct {
    ElementOptionKeyspace* premier; /*premier élément de la liste*/
    ElementOptionKeyspace* dernier; /*dernier élément de la liste*/
    ElementOptionKeyspace* courant; /* élément en cours de traitement (parcours de liste) */
    int nbElt; /* nombre des element */
    char* (*toString) (optionKeyspace*);
    int (*comparer) (optionKeyspace*,optionKeyspace*);
} ListeOptionKeyspace;

/*=================================Option=Table====================================*/
//column_clustering_order
typedef struct{
    char* name;
    int ASC;
}objetColClustring;


typedef struct element_column_clustring{
    objetTuple* reference;
    struct element_column_clustring* suivant;
}ElementColClustring;

typedef struct {
    ElementColClustring* premier; /*premier élément de la liste*/
    ElementColClustring* dernier; /*dernier élément de la liste*/
    ElementColClustring* courant; /* élément en cours de traitement (parcours de liste) */
    int nbElt; /* nombre des element */
    char* (*toString) (objetColClustring*);
    int (*comparer) (objetColClustring*,objetColClustring*);
}ListeColClustring;


typedef enum
{
    option_keyspace,
    compact_storage,
    culstring_order,
}type_option_table;

typedef union{
    optionKeyspace *optionKey;
    ListeTuple *culstringOrder;
}nodeOptionTable;


typedef struct {
    type_option_table type;
    nodeOptionTable node;
}optionTable;

//list_option_table


typedef struct element_option_table{
    optionTable* reference;
    struct element_option_table* suivant;
} ElementOptionTable;

typedef struct {
    ElementOptionTable* premier; /*premier élément de la liste*/
    ElementOptionTable* dernier; /*dernier élément de la liste*/
    ElementOptionTable* courant; /* élément en cours de traitement (parcours de liste) */
    int nbElt; /* nombre des element */
    char* (*toString) (optionTable*);
    int (*comparer) (optionTable*,optionTable*);
} ListeOptionTable;


/*================================struct=Column=======================================*/

typedef struct{
    char* name;
    cql_type *type;
    boolean _static;
    boolean partitionKey;
   // boolean clusteringCol;
}objetColumn;


typedef struct element_col{
    objetColumn* reference;
    struct element_col* suivant;
} ElementColumn;

typedef struct {
    ElementColumn* premier; /*premier élément de la liste*/
    ElementColumn* dernier; /*dernier élément de la liste*/
    ElementColumn* courant; /* élément en cours de traitement (parcours de liste) */
    int nbElt; /* nombre des element */
    char* (*toString) (objetColumn*);
    int (*comparer) (objetColumn*,objetColumn*);
} ListeColumn;


/*====================================struct=Table====================================*/
typedef struct{
    char* name;
    ListeColumn *liCol;
    ListeName *liPartitionKey;
    ListeName *liClustringOrder;
    ListeOptionTable *liOT;
}objetTable;


typedef struct element_tab{
    objetTable* reference;
    struct element_tab* suivant;
} ElementTable;

typedef struct {
    ElementTable* premier; /*premier élément de la liste*/
    ElementTable* dernier; /*dernier élément de la liste*/
    ElementTable* courant; /* élément en cours de traitement (parcours de liste) */
    int nbElt; /* nombre des element */
    char* (*toString) (objetTable*);
    int (*comparer) (objetTable*,objetTable*);
} ListeTable;
/*====================================struct=Keyspace==================================*/

typedef struct{
    char* name;
    ListeTable *liTab;
    ListeOptionKeyspace *liOK;
}objetKeyspace;


typedef struct element_key{
    objetKeyspace* reference;
    struct element_key* suivant;
}ElementKey;

typedef struct {
    ElementKey* premier; /*premier élément de la liste*/
    ElementKey* dernier; /*dernier élément de la liste*/
    ElementKey* courant; /* élément en cours de traitement (parcours de liste) */
    int nbElt; /* nombre des element */
    char* (*toString) (objetKeyspace*);
    int (*comparer) (objetKeyspace*,objetKeyspace*);
} ListeKey;


/*====================================struct=NameValue==================================*/

typedef struct{
    char* name;
    term *t;
}objetNameValue;


typedef struct element_name_value{
    objetNameValue* reference;
    struct element_name_value* suivant;
}ElementNameValue;

typedef struct {
    ElementNameValue* premier; /*premier élément de la liste*/
    ElementNameValue* dernier; /*dernier élément de la liste*/
    ElementNameValue* courant; /* élément en cours de traitement (parcours de liste) */
    int nbElt; /* nombre des element */
    char* (*toString) (objetNameValue*);
    int (*comparer) (objetNameValue*,objetNameValue*);
} ListeNameValue;



/*====================================struct=Data==================================*/

typedef struct {
    ListeList* data;
}objetData;

typedef struct element_data{
    objetData* reference;
    struct element_name_value* suivant;
}ElementData;

typedef struct {
    ElementData* premier; /*premier élément de la liste*/
    ElementData* dernier; /*dernier élément de la liste*/
    ElementData* courant; /* élément en cours de traitement (parcours de liste) */
    int nbElt; /* nombre des element */
    char* (*toString) (objetData*);
    int (*comparer) (objetData*,objetData*);
} ListeData;



/*====================================Selector==================================*/



typedef struct{
    char* name;
    char* as;
}objetSelector;


typedef struct element_selector{
    objetSelector* reference;
    struct element_selector* suivant;
}ElementSelector;

typedef struct {
    ElementSelector* premier; /*premier élément de la liste*/
    ElementSelector* dernier; /*dernier élément de la liste*/
    ElementSelector* courant; /* élément en cours de traitement (parcours de liste) */
    int nbElt; /* nombre des element */
    char* (*toString) (objetSelector*);
    int (*comparer) (objetSelector*,objetSelector*);
} ListeSelector;



/*====================================Where=Clause==================================*/
//'=' | '<' | '>' | '<=' | '>=' | '!='
//"<="	{return _LESSEQ ;}
//">="	{return _GREATEREQ;}
//"!="	{return _DIFF;}
//"<"		{return _LESS ;}
//">"		{return _GREATER;}
typedef enum {
    op_eq,
    op_less,
    op_greater,
    op_lessEq,
    op_greaterEq,
    op_diff
}_operatorEnum;


typedef struct{
    char* name;
    _operatorEnum *anOperator;
    term *t;
}objetRelation;


typedef struct element_relation{
    objetRelation* reference;
    struct element_relation* suivant;
}ElementRelation;

typedef struct {
    ElementRelation* premier; /*premier élément de la liste*/
    ElementRelation* dernier; /*dernier élément de la liste*/
    ElementRelation* courant; /* élément en cours de traitement (parcours de liste) */
    int nbElt; /* nombre des element */
    char* (*toString) (objetRelation*);
    int (*comparer) (objetRelation*,objetRelation*);
} ListeRelation;



/*=========================================Statment==========================================*/

typedef struct {
    char *name;
    ListeOptionKeyspace *liOK;
    ListeKey *liKey;
}createKeyspaceStmt;


typedef struct {
    objetKeyspace *key;
    ListeOptionKeyspace *liOK;
}alterKeyspaceStmt;

typedef struct {
    objetKeyspace *key;
    ListeKey *likey;
}dropKeyspaceStmt;


typedef struct {
    objetKeyspace *currentKeyspace;
    objetKeyspace *useKeyspace;
}useKeyspaceStmt;

typedef struct {
    ListeKey *liKey;
}descripeKeyspaceStmt;

//create_table
typedef struct {
    char* nameKeyspace;
    char *nameTable;
}tableName;

typedef struct {
    objetKeyspace *objetKeyspace1;
    char *name;
    ListeColumn *liCol;
    ListeName *liPartitionKey;
    ListeName *liClustringOrder;
    ListeOptionTable *liOptionTable;
}createTableStmt;


typedef struct {
    objetKeyspace *objetKey;
    ListeKey *li;
}describeTableStmt;



typedef struct {
    objetKeyspace *keyspace;
    objetTable *table;
}dropTableStmt;


typedef struct {
    objetKeyspace *keyspace;
    objetTable *table;
}truncatStmt;


typedef struct {
    objetKeyspace *keyspace;
    objetTable *table;
    ListeName *liName;
    ListeList *liValue;
    boolean ifNotExist;
    //ListeName *partitionKey;
    //ListeName *clustringOrder;
    //ListeUpdateParm
}insertStmt;


typedef struct {
    objetKeyspace *keyspace;
    objetTable *table;
    ListeRelation *liRelation;
}deleteStmt;





typedef struct {
    boolean ifExist;
    objetKeyspace *keyspace;
    objetTable *table;
    ListeRelation *liRelation;
    ListeNameValue *liNameValue;
}updateStmt;

typedef enum{
    all,
    notAll
}type_selector;

typedef struct {
    type_selector type;
    int limit;
    objetKeyspace *keyspace;
    objetTable *table;
    ListeRelation *liRelation;
    ListeSelector *liSelector;
}selectStmt;


typedef enum
{
    create_keyspace,
    alter_keyspace,
    drop_keyspace,
    use_keyspace,
    descripe_keyspace,
    create_table,
    insert_into,
    delete_stmt,
    update_stmt,
    default_stmt,
    select_stmt,
    alter_table,
    drop_table,
    truncate_table,
    describe_table
}stmt_type;

typedef union{
    createKeyspaceStmt createKeyspace;
    alterKeyspaceStmt  alterKeyspace;
    dropKeyspaceStmt dropKeyspace;
    useKeyspaceStmt useKeyspace;
    descripeKeyspaceStmt descripeKeyspace;
    createTableStmt createTable;
    insertStmt insert;
    deleteStmt _delete;
    updateStmt update;
    selectStmt select;
    describeTableStmt describeTable;
    truncatStmt truncat;
    dropTableStmt dropTable;

}nodeStmt;


typedef struct stmt{
    stmt_type type;
    nodeStmt node;
}stmt;












#endif