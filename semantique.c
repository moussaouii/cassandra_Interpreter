//
// Created by ismail on 01/01/17.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "semantique.h"
#include "error.h"
#include "execute_stmt.h"
#include <math.h>
#include "structure.h"




#define debug false

boolean _valide_option_replication(optionKeyspace *ch,char *name,int line){
    boolean result;
    if(debug){printf("%s",ch->idf);}
        if(strcmp(ch->idf,"replication")==0){
            if(debug){printf("reeeeeeeeeeeeeeeeeeeeee");}
            if(ch->type==map_opt) {
                ouvrirListeMap(ch->node.map);
                if (!finListeMap(ch->node.map)) {
                    objetMap *oM = objetCourantMap(ch->node.map);
                    if (oM->champ1->type ==constant_term) {
                        if (oM->champ1->node.constant.type ==string_const) {
                            if (strcmp(oM->champ1->node.constant.node.string, "'class'") == 0) {
                                if (oM->champ2->type ==constant_term) {
                                    if (oM->champ2->node.constant.type == string_const) {
                                        if (strcmp(oM->champ2->node.constant.node.string, "'SimpleStrategy'") == 0) {
                                            if (!finListeMap(ch->node.map)) {
                                                oM = objetCourantMap(ch->node.map);
                                                if (oM->champ1->type == constant_term) {
                                                    if (oM->champ1->node.constant.type == string_const) {
                                                        if (strcmp(oM->champ1->node.constant.node.string,
                                                                   "'replication_factor'") ==
                                                            0) {
                                                            if(oM->champ2->type==constant_term){
                                                                if (oM->champ2->node.constant.type == integer_const) {
                                                                    result=true;
                                                                    if(!finListeMap(ch->node.map)){
                                                                        _creerErreur(SyntaxError,line,NULL,"after 'replication_factor' ");
                                                                        result=false;

                                                                    }
                                                                } else {
                                                                    char *msg=(char*)malloc((70)*sizeof(char));
                                                                    sprintf(msg,"after 'replication_factor'");
                                                                    _creerErreur(SyntaxError,line,NULL,msg);
                                                                    result = false;
                                                                }
                                                            } else{
                                                                char *msg=(char*)malloc((70)*sizeof(char));
                                                                sprintf(msg,"after 'replication_factor'");
                                                                _creerErreur(SyntaxError,line,NULL,msg);
                                                                result=false;
                                                            }
                                                        } else {
                                                            _creerErreur(MissingSubOption,line,"'replication_factor'",NULL);
                                                            result = false;
                                                        }
                                                    } else {
                                                        _creerErreur(MissingSubOption,line,"'replication_factor'",NULL);
                                                        result = false;
                                                    }
                                                } else {
                                                    _creerErreur(MissingSubOption,line,"'replication_factor'",NULL);
                                                    result = false;
                                                }
                                            } else {
                                                _creerErreur(MissingSubOption,line,"'replication_factor'",NULL);
                                                result = false;
                                            }
                                        } else if (strcmp(oM->champ2->node.constant.node.string,"'NetworkTopologyStrategy'") == 0) {
                                            if(debug){printf("NetworkTopologyStrategy");}
                                            if(finListeMap(ch->node.map)){
                                                result=true;
                                            }
                                            while (!finListeMap(ch->node.map)) {
                                                oM = objetCourantMap(ch->node.map);
                                                if (oM->champ1->type == constant_term) {
                                                    if(debug){printf("map");}
                                                    if (oM->champ1->node.constant.type == string_const) {
                                                        if (oM->champ2->type == constant_term) {
                                                            if (oM->champ2->node.constant.type == integer_const) {
                                                                result = true;
                                                            } else {
                                                                result = false;
                                                                break;
                                                            }
                                                        } else {
                                                            result = false;
                                                            break;
                                                        }
                                                    } else {
                                                        result = false;
                                                        break;
                                                    }
                                                } else {
                                                    result = false;
                                                    break;
                                                }
                                            }
                                            if(!result){
                                                char *msg=(char*)malloc((70)*sizeof(char));
                                                sprintf(msg,"after 'NetworkTopologyStrategy'");
                                                _creerErreur(SyntaxError,line,NULL,msg);
                                                result = false;
                                            }

                                        } else {
                                            if(debug){printf("sssssssss\n");}
                                            _creerErreur(UndefinedClass,line,oM->champ2->node.constant.node.string,NULL);
                                            result = false;
                                        }

                                    } else {
                                        char *msg=(char*)malloc((50)*sizeof(char));
                                        sprintf(msg,"after class");
                                        _creerErreur(SyntaxError,line,NULL,msg);
                                        result = false;
                                    }
                                }else{
                                    char *msg=(char*)malloc((50)*sizeof(char));
                                    sprintf(msg,"after class");
                                    _creerErreur(SyntaxError,line,NULL,msg);
                                    result=false;
                                }
                            } else {
                                _creerErreur(MissingSubOption,line,"class",NULL);
                                result = false;
                            }
                        } else {
                            _creerErreur(MissingSubOption,line,"class",NULL);
                            result = false;
                        }
                    } else {
                        _creerErreur(MissingSubOption,line,"class",NULL);
                        result = false;
                    }
                } else{
                    _creerErreur(MissingSubOption,line,"class",NULL);
                    result=false;
                }
            } else{
                char *msg=(char*)malloc((strlen(ch->idf)+50)*sizeof(char));
                sprintf(msg,"after %s",ch->idf);
                _creerErreur(SyntaxError,line,NULL,msg);
                result=false;
            }
        } else{
            if(strcmp(ch->idf,"durable_writes")==0){
                _creerErreur(AlreadyDeclaredOption,line,ch->idf,NULL);

            }else{
                _creerErreur(UnknownOption,line,ch->idf,NULL);
            }
            result=false;
        }
    return result;
}


boolean _valide_option_durable_write(optionKeyspace *ch,char *name,int line){
    boolean result;
        if(strcmp(ch->idf,"durable_writes")==0){
            if(ch->type==constant_opt) {
                if(ch->node.constant.type==boolean_const) {
                    result=true;
                }else{
                    char *msg=(char*)malloc((strlen(ch->idf)+50)*sizeof(char));
                    sprintf(msg,"after %s",ch->idf);
                    _creerErreur(SyntaxError,line,NULL,msg);
                    result=false;
                }
            }else{
                char *msg=(char*)malloc((strlen(ch->idf)+50)*sizeof(char));
                sprintf(msg,"after %s",ch->idf);
                _creerErreur(SyntaxError,line,NULL,msg);
                result=false;
            }
        }
        else{
            if(strcmp(ch->idf,"replication")==0){
                _creerErreur(AlreadyDeclaredOption,line,ch->idf,NULL);

            }else{
                _creerErreur(UnknownOption,line,ch->idf,NULL);
            }
            result=false;
        }
    return result;
}


boolean _valide_option_keyspace_create(ListeOptionKeyspace *li,char *name,int line){
    boolean result;
    ouvrirListeOptionKeyspace (li);
    if (!finListeOptionKeyspace (li)) {
        optionKeyspace* ch = objetCourantOptionKeyspace (li);
        if(strcmp(ch->idf,"durable_writes")==0) {
            if(debug){printf("durable writwes\n");}
            if(_valide_option_durable_write(ch, name, line)){
                if(debug){printf("valide durable writwes\n");}
                if(!finListeOptionKeyspace (li)){
                    ch = objetCourantOptionKeyspace (li);
                    if(_valide_option_replication(ch,name,line)){
                        result=true;
                    }else{
                        result=false;
                    }
                }else{
                    _creerErreur(MissingOptionReplication,line,name,NULL);
                    result=false;
                }
            }
            else{
                result=false;
            }
        }else if (strcmp(ch->idf, "replication") == 0) {
            if(debug){printf("replication\n");}
            if( _valide_option_replication(ch, name, line)){
                if(debug){printf("valide option replication\n");}
                if(!finListeOptionKeyspace (li)){
                    ch = objetCourantOptionKeyspace (li);
                    if(_valide_option_durable_write(ch,name,line)){
                        result=true;
                    }else{
                        result=false;
                    }
                }else{
                    if(debug){printf("111111111111");}
                    result=true;
                }
            }else{
                result=false;
            }
        } else {
            _creerErreur(UnknownOption,line,ch->idf,NULL);
            result = false;
        }
    }else{
        _creerErreur(MissingOptionReplication,line,name,NULL);
        result=false;
    }




    return result;
}




boolean _valide_option_keyspace_alter(ListeOptionKeyspace *li,char *name,int line){
    boolean result;
    ouvrirListeOptionKeyspace (li);
    if (!finListeOptionKeyspace (li)) {
        optionKeyspace* ch = objetCourantOptionKeyspace (li);
        if(strcmp(ch->idf,"durable_writes")==0) {
            if(debug){printf("durable writwes\n");}
            if(_valide_option_durable_write(ch, name, line)){
                if(debug){printf("valide durable writwes\n");}
                if(!finListeOptionKeyspace (li)){
                    ch = objetCourantOptionKeyspace (li);
                    if(_valide_option_replication(ch,name,line)){
                        result=true;
                    }else{
                        result=false;
                    }
                }else{

                    result=true;
                }
            }
            else{
                result=false;
            }
        }else if (strcmp(ch->idf, "replication") == 0) {
            if(debug){printf("replication\n");}
            if( _valide_option_replication(ch, name, line)){
                if(debug){printf("valide option replication\n");}
                if(!finListeOptionKeyspace (li)){
                    ch = objetCourantOptionKeyspace (li);
                    if(_valide_option_durable_write(ch,name,line)){
                        result=true;
                    }else{
                        result=false;
                    }
                }else{
                    if(debug){printf("111111111111");}
                    result=true;
                }
            }else{
                result=false;
            }
        } else {
            _creerErreur(UnknownOption,line,ch->idf,NULL);
            result = false;
        }
    }else{
        _creerErreur(MissingOptionReplication,line,name,NULL);
        result=false;
    }



    return result;
}



objetKeyspace *trouverKeyspace(ListeKey *liKey,tableName *tN,objetKeyspace *currentKeyspace,int yylineno){
    if(tN->nameKeyspace==NULL){
        if(currentKeyspace->name==NULL){
            char *msg=malloc(150*sizeof(char));
            strcpy(msg,"No keyspace has been specified. USE a keyspace, or explicitly specify keyspace.tablename");
            _creerErreur(InvalidRequest,yylineno,NULL,msg);
            return  NULL;
        }else{
            return currentKeyspace;
        }
    }else{
        objetKeyspace *objetChercheKey=_createKeyspace(tN->nameKeyspace);
        objetKeyspace *objetTrouveKey=chercherUnobjetKeyspace(liKey,objetChercheKey);
        free(objetChercheKey);
        if(objetTrouveKey==NULL){
            _creerErreur(UnknownKeyspace,yylineno,tN->nameKeyspace,NULL);
            return NULL;
        } else{
            return  objetTrouveKey;

        }
    }
}


boolean _valide_insert_nbr_names_values(ListeName *liName,ListeList *liValue,int line){
    int nb_names=0;
    int nb_values=0;
    boolean result;
    ouvrirListeName(liName);
    while(!finListeName(liName)) {
        char *name = objetCourantName(liName);
        nb_names++;
    }
    ouvrirListeList(liValue);
    while(!finListeList(liValue)) {
        term *t = objetCourantList(liValue);
        nb_values++;
    }
    if(nb_names!=nb_values){
        char *msg=strdup("Unmatched column names/values");
        _creerErreur(InvalidRequest,line,NULL,msg);
        free(msg);
        result=false;
        //printf("nb_names:%d , nb_values:%d",nb_names,nb_values);
    }
    else{
        result=true;
    }
    return result;
}

boolean  _valide_insert_primary_key(ListeName *liName,ListeList *liValue,ListeName* liPartitionKey,ListeName *liClustringOrder,int line){
    boolean keyExits=false;
    boolean result=true;
    ouvrirListeName(liPartitionKey);
    while (!finListeName(liPartitionKey)) {
        char *nameKey = objetCourantName(liPartitionKey);
            keyExits=false;
            ouvrirListeName(liName);
            ouvrirListeList(liValue);
            while(!finListeName(liName)) {
                char *name = objetCourantName(liName);
                term *t=objetCourantList(liValue);
                if(strcmp(name,nameKey)==0){
                    if(t->node.constant.type==null_const){
                        //erreur primary key should not be null
                        _creerErreur(InvalidValueNull,line,name,NULL);
                    }
                    keyExits=true;
                    break;
                }
            }
            if(!keyExits){
                _creerErreur(MissingPrimaryKey,line,nameKey,NULL);
                result=false;
                break;
            }
        }

    ouvrirListeName(liClustringOrder);
    while (!finListeName(liClustringOrder)) {
        char *nameKey = objetCourantName(liClustringOrder);
        keyExits=false;
        ouvrirListeName(liName);
        ouvrirListeList(liValue);
        while(!finListeName(liName)) {
            term *t=objetCourantList(liValue);
            char *name = objetCourantName(liName);
            if(strcmp(name,nameKey)==0){
                if(t->node.constant.type==null_const){
                    //erreur primary key should not be null
                   _creerErreur(InvalidValueNull,line,name,NULL);
                }
                keyExits=true;
                break;
            }
        }
        if(!keyExits){
            _creerErreur(MissingPrimaryKey,line,nameKey,NULL);
            result=false;
            break;
        }
    }

    _multiple_declaration(liName,line,"List Column");
    return result;
}

//map_term,
//set_term,
//list_term,
//constant_term,
//tuple_term

//ascii_native,
//bigint_native ,
//blob_native ,
//boolean_native ,
//counter_native ,
//date_native ,
//decimal_native ,
//double_native ,
//float_native ,
//inet_native ,
//int_native ,
//smallint_native ,
//text_native ,
//time_native,
//timestamp_native ,
//timeuuid_native ,
//tinyint_native ,
//uuid_native ,
//varchar_native ,
//varint_native

//string_const , integer_const , float_const ,
//boolean_const , uuid_const , blob_const , null_const
boolean _valide_type(term *t,cql_type *ctype, int line){
    boolean result=false;
    switch (ctype->type){
        case native_typeE:
            if(ctype->node.native==ascii_native && t->type==constant_term){
                if(t->node.constant.type==string_const){
                    result=true;
                }else{
                    result=false;
                }
            }else if(ctype->node.native==bigint_native){
                if(t->node.constant.type==integer_const){
                    result=true;
                }else{
                    result=false;
                }
            }else if(ctype->node.native==blob_native){
                if(t->node.constant.type==blob_const){
                    result=true;
                }else{
                    result=false;
                }
            }else if(ctype->node.native==boolean_native){
                if(t->node.constant.type==boolean_const){
                    result=true;
                }else{
                    result=false;
                }
            }else if(ctype->node.native==counter_native){
                if(t->node.constant.type==integer_const){
                    result=true;
                }else{
                    result=false;
                }
            }else if(ctype->node.native==date_native){
                if(t->node.constant.type==string_const){
                    result=true;
                }else{
                    result=false;
                }
            }else if(ctype->node.native==decimal_native){
                if(t->node.constant.type==integer_const || t->node.constant.type==float_const){
                    result=true;
                }else{
                    result=false;
                }
            }else if(ctype->node.native==double_native){
                if(t->node.constant.type==integer_const || t->node.constant.type==float_const){
                    result=true;
                }else{
                    result=false;
                }
            }else if(ctype->node.native==float_native){
                if(t->node.constant.type==integer_const || t->node.constant.type==float_const){
                    result=true;
                }else{
                    result=false;
                }
            }else if(ctype->node.native==inet_native){
                if(t->node.constant.type==string_const){
                    result=true;
                }else{
                    result=false;
                }
            }else if(ctype->node.native==int_native){
                if(t->node.constant.type==integer_const){
                    result=true;
                }else{
                    result=false;
                }
            }else if(ctype->node.native==smallint_native){
                if(t->node.constant.type==integer_const){
                    result=true;
                }else{
                    result=false;
                }
            }else if(ctype->node.native==text_native){
                if(t->node.constant.type==string_const){
                    result=true;
                }else{
                    result=false;
                }
            }else if(ctype->node.native==timestamp_native){
                if(t->node.constant.type==integer_const){
                    result=true;
                }
                else if(t->node.constant.type==string_const){
                    result=true;
                }else{
                    result=false;
                }
            }else if(ctype->node.native==tinyint_native){
                if(t->node.constant.type==integer_const){
                    result=true;
                }else{
                    result=false;
                }
            }else if(ctype->node.native==uuid_native){
                if(t->node.constant.type==uuid_const){
                    result=true;
                }else{
                    result=false;
                }
            }else if(ctype->node.native==varchar_native){
                if(t->node.constant.type==string_const){
                    result=true;
                }else{
                    result=false;
                }
            }else if(ctype->node.native==varint_native) {
                if (t->node.constant.type == integer_const) {
                    result = true;
                } else {
                    result = false;
                }
            }else{
                result=false;
            }
            break;
        case map_typeE:
            if(t->type==map_term){
                ouvrirListeMap(t->node.map);
                while(!finListeMap(t->node.map)){
                    objetMap *om=objetCourantMap(t->node.map);
                    if(_valide_type(om->champ1,ctype->node.map.champ1,line)){
                        if(_valide_type(om->champ2,ctype->node.map.champ2,line)){
                            result=true;
                        }
                        else{
                            result=false;
                            goto sort;
                        }
                    }else{
                        result=false;
                        goto  sort;
                    }
                }
            } else{
                result=false;
            }
            sort:
            break;
        case set_typeE:
            if(t->type==set_term){
                ouvrirListeList(t->node.list);
                while(!finListeList(t->node.list)){
                    term *term1=objetCourantList(t->node.list);
                    if(_valide_type(term1,ctype->node.list.champ,line)){
                        result=true;
                    }else{
                        result=false;
                        break;
                    }
                }
            } else{
                result=false;
            }
            break;
        case list_typeE:
            if(t->type==list_term){
                ouvrirListeList(t->node.list);
                while(!finListeList(t->node.list)){
                    term *term1=objetCourantList(t->node.list);
                    if(_valide_type(term1,ctype->node.list.champ,line)){
                        result=true;
                    }else{
                        result=false;
                        break;
                    }
                }
            } else{
                result=false;
            }
            break;
        case udt_typeE :
            result=true;
            break;
        case tuple_typeE:
            if(t->type==tuple_term){
                ouvrirListeList(t->node.list);
                while(!finListeList(t->node.list)){
                    term *term1=objetCourantList(t->node.list);
                    if(_valide_type(term1,ctype->node.list.champ,line)){
                        result=true;
                    }else{
                        result=false;
                        break;
                    }
                }
            } else{
                result=false;
            }
            break;
        case custom_typeE:
            result=true;
            break;
    }
    return result;
}



void _valide_insert(ListeName *liName,ListeList *liValue,objetTable *table, int line) {
    boolean ColExistsInTable=false;
    if(_valide_insert_nbr_names_values(liName,liValue,line)){
        if(_valide_insert_primary_key(liName,liValue,table->liPartitionKey,table->liClustringOrder,line)){
            ouvrirListeName(liName);
            ouvrirListeList(liValue);
            while(!finListeName(liName)){
                char *name=objetCourantName(liName);
                term *term1=objetCourantList(liValue);
                ColExistsInTable=false;
                ouvrirListeColumn(table->liCol);
                while (!finListeColumn(table->liCol)){
                    objetColumn *oc=objetCourantColumn(table->liCol);
                    if(strcmp(name,oc->name)==0){
                        if(debug){printTerm(term1);
                        printType(oc->type);}
                        if(!(term1->type==constant_term && term1->node.constant.type==null_const)){
                            if(_valide_type(term1,oc->type,line)){
                                //printf("valide:%s ,%d ",oc->name,term1->type);
                            }
                            else{
                               // char *type1=_type_name(oc->type);
                               //char *value=_term_value(term1);
                                char *nameCol=(char *)malloc((strlen(oc->name)+strlen(_type_name(oc->type))+strlen("'' of type ")+1)*sizeof(char));
                                sprintf(nameCol,"'%s' of type %s",oc->name,_type_name(oc->type));
                               // printf("type:%s value:%s\n",type1,value);
                                _creerErreur(InvalideTypeInsert,line,_term_value(term1),nameCol);
                                //printf("non valide:%s ,%d ",oc->name,term1->type);
                                goto sort1;
                            }
                        }
                        sort1:
                        ColExistsInTable=true;
                        goto sort2;
                    }
                }
                sort2:
                if(!ColExistsInTable){
                    _creerErreur(UndefinedColumn,line,name,NULL);
                }
            }
        }
    }

}
//map_term,
//set_term,
//list_term,
//constant_term,
//tuple_term

char *_term_value(term *t){
    char *result;
    char *resultTmp;
    boolean premierFois=true;
    size_t len=0;
    int nb=0;
    switch (t->type){
        case constant_term:
            switch(t->node.constant.type){
                case string_const:
                    result=strdup(t->node.constant.node.string);
                    break;
                case integer_const:
                    result=(char *)malloc(50*sizeof(char));
                    sprintf(result,"%d",t->node.constant.node.integer);
                    break;
                case float_const:
                    result=(char *)malloc(50*sizeof(char));
                    sprintf(result,"%lf",t->node.constant.node._float);
                    break;
                case boolean_const:
                    if(t->node.constant.node.integer==0){
                        result=strdup("false");
                    }else{
                        result=strdup("true");
                    }
                    break;
                case uuid_const:
                    result=strdup(t->node.constant.node.string);
                    break;
                case blob_const:
                    result=strdup(t->node.constant.node.string);
                    break;
                case null_const:
                    result=strdup("null");
                    break;
            }
            break;
        case map_term:
            ouvrirListeMap(t->node.map);
            while(!finListeMap(t->node.map)){
                objetMap *om=objetCourantMap(t->node.map);
                len=len+strlen((_term_value(om->champ1)))+strlen((_term_value(om->champ2)));
                nb++;
            }
            result=(char *)malloc((len+6*nb+3)*sizeof(char));
            sprintf(result,"{");
            ouvrirListeMap(t->node.map);
            while(!finListeMap(t->node.map)){
                if(premierFois){
                    premierFois=false;
                }else{
                    strcat(result,",");
                }
                objetMap *om=objetCourantMap(t->node.map);
                resultTmp=(char *)malloc((strlen(_term_value(om->champ1))+strlen(_term_value(om->champ2))+5)*sizeof(char));
                sprintf(resultTmp,"%s:%s",_term_value(om->champ1),_term_value(om->champ2));
                strcat(result,resultTmp);

            }
            strcat(result,"}");
            break;
        case set_term:
            ouvrirListeList(t->node.list);
            while(!finListeList(t->node.list)){
                term *term1=objetCourantList(t->node.list);
                len=len+strlen(_term_value(term1));
                nb++;
            }
            result=(char *)malloc((len+6*nb+3)*sizeof(char));
            sprintf(result,"{");
            ouvrirListeList(t->node.list);
            while(!finListeList(t->node.list)){
                if(premierFois){
                    premierFois=false;
                }else{
                    strcat(result,",");
                }
                term *term1=objetCourantList(t->node.list);
                strcat(result,_term_value(term1));

            }
            strcat(result,"}");
            break;
        case list_term:
            ouvrirListeList(t->node.list);
            while(!finListeList(t->node.list)){
                term *term1=objetCourantList(t->node.list);
                len=len+strlen(_term_value(term1));
                nb++;
            }
            result=(char *)malloc((len+6*nb+3)*sizeof(char));
            sprintf(result,"[");
            ouvrirListeList(t->node.list);
            while(!finListeList(t->node.list)){
                if(premierFois){
                    premierFois=false;
                }else{
                    strcat(result,",");
                }
                term *term1=objetCourantList(t->node.list);
                strcat(result,_term_value(term1));
            }
            strcat(result,"]");
            break;
        case tuple_term:
            ouvrirListeList(t->node.list);
            while(!finListeList(t->node.list)){
                term *term1=objetCourantList(t->node.list);
                len=len+strlen(_term_value(term1));
                nb++;
            }
            result=(char *)malloc((len+6*nb+3)*sizeof(char));
            sprintf(result,"(");
            ouvrirListeList(t->node.list);
            while(!finListeList(t->node.list)){
                if(premierFois){
                    premierFois=false;
                }else{
                    strcat(result,",");
                }
                term *term1=objetCourantList(t->node.list);
                strcat(result,_term_value(term1));

            }
            strcat(result,")");
            break;
    }
    return  result;
}

char *_type_name(cql_type *t){
    char *result=NULL;
    switch (t->type){
        case native_typeE:
            switch(t->node.native){
                case ascii_native:
                    result=strdup("ascii");
                    break;
                case bigint_native:
                    result=strdup("bigint");
                    break;
                case blob_native:
                    result=strdup("blob");
                    break;
                case boolean_native:
                    result=strdup("boolean");
                    break;
                case counter_native:
                    result=strdup("counter");
                    break;
                case date_native:
                    result=strdup("date");
                    break;
                case decimal_native:
                    result=strdup("decimal");
                    break;
                case double_native:
                    result=strdup("double");
                    break;
                case float_native:
                    result=strdup("float");
                    break;
                case inet_native:
                    result=strdup("inet");
                    break;
                case int_native:
                    result=strdup("int");
                    break;
                case smallint_native:
                    result=strdup("smallint");
                    break;
                case text_native:
                    result=strdup("text");
                    break;
                case time_native:
                    result=strdup("time");
                    break;
                case timestamp_native:
                    result=strdup("timestamp");
                    break;
                case timeuuid_native:
                    result=strdup("timeuuid");
                    break;
                case tinyint_native:
                    result=strdup("tinyint");
                    break;
                case uuid_native:
                    result=strdup("uuid");
                    break;
                case varchar_native:
                    result=strdup("varchar");
                    break;
                case varint_native:
                    result=strdup("varint");
                    break;
            }
            break;
        case map_typeE:
            result=(char*)malloc((strlen(_type_name(t->node.map.champ1))+strlen(_type_name(t->node.map.champ2))+1+strlen("map<>"))*sizeof(char));
            sprintf(result,"map<%s,%s>",_type_name(t->node.map.champ1),_type_name(t->node.map.champ2));
            break;
        case set_typeE:
            result=(char*)malloc((strlen(_type_name(t->node.list.champ))+1+strlen("set<>"))*sizeof(char));
            sprintf(result,"set<%s>",_type_name(t->node.list.champ));
            break;
        case list_typeE:
            result=(char*)malloc((strlen(_type_name(t->node.list.champ))+1+strlen("list<>"))*sizeof(char));
            sprintf(result,"list<%s>",_type_name(t->node.list.champ));
            break;
        case tuple_typeE:
            result=(char*)malloc((strlen(_type_name(t->node.list.champ))+1+strlen("tuple<>"))*sizeof(char));
            sprintf(result,"tuple<%s>",_type_name(t->node.list.champ));
            break;
        case custom_typeE:
            break;
        case udt_typeE:
            break;
    }
    return result;
}
//

//}

objetTable *_chercher_table(objetKeyspace *objetKey,char *name,int line,boolean ifExsit){
    objetTable *objetchercher=_createTable(name);
    objetTable *objetTrouve=chercherUnobjetTable(objetKey->liTab,objetchercher);
    free(objetchercher);
    if(objetTrouve==NULL){
        if(!ifExsit) {
            char *nameTableComplet=(char *)malloc((strlen(name)+strlen(objetKey->name)+2)*sizeof(char));
            sprintf(nameTableComplet,"%s.%s",objetKey->name,name);
            _creerErreur(UnknownTable,line,nameTableComplet,NULL);
        }
        return NULL;
    } else{
        return objetTrouve;
    }
}





void *_chercher_table_in_keyspace(objetKeyspace *objetKey,char *name,boolean ifNotExists, int yylineno){
    objetTable *objetchercher=_createTable(name);
    objetTable *objetTrouve=chercherUnobjetTable(objetKey->liTab,objetchercher);
    free(objetchercher);
    if(objetTrouve!=NULL){
        if(!ifNotExists){
            char *nameTableComplet=(char *)malloc((strlen(name)+strlen(objetKey->name)+2)*sizeof(char));
            sprintf(nameTableComplet,"%s.%s",objetKey->name,name);
            _creerErreur(AlreadyExists,yylineno,nameTableComplet,"table");
        }
    }

}



void _valide_column_primary_key(ListeColumn *liCol,ListeName *liPartitionKey,int line,ListeName *liClusteringCol) {
    boolean existPrimaryKey=false;
    ouvrirListeName(liPartitionKey);
    if (!finListeName(liPartitionKey)) {
        ouvrirListeColumn(liCol);
        while (!finListeColumn(liCol)) {
            objetColumn *col = objetCourantColumn(liCol);
            if (col->partitionKey) {
                char *msg=(char *)malloc(120*sizeof(char));
                strcpy(msg,"Multiple PRIMARY KEYs specifed (exactly one required)");
                _creerErreur(InvalidRequest,line,NULL,msg);
            }
        }
        while (!finListeName(liPartitionKey)) {
            char *pKye = objetCourantName(liPartitionKey);
            objetColumn *objetCherche = (objetColumn *) malloc(sizeof(objetColumn));
            objetCherche->name = (char *) malloc(strlen(pKye) * sizeof(char) + 1);
            strcpy(objetCherche->name, pKye);
            objetColumn *objetTrouve = chercherUnobjetColumn(liCol, objetCherche);
            if (objetTrouve == NULL) {
                char *msg=(char *)malloc(120*sizeof(char));
                sprintf(msg," Unknown definition %s referenced in PRIMARY KEY",pKye);
                _creerErreur(InvalidRequest,line,NULL,msg);

            } else {
                if(objetTrouve->type->type!=native_typeE ){
                    _creerErreur(InvalideTypePrimaryKey,line,objetTrouve->name,NULL);
                    //ereur type primary key
                }
                objetTrouve->partitionKey = true;
            }
        }
    } else {
        ouvrirListeColumn(liCol);
        while (!finListeColumn(liCol)) {
            objetColumn *col = objetCourantColumn(liCol);
            if (col->partitionKey) {
                if(col->type->type!=native_typeE ){
                    //ereur type primary key
                    _creerErreur(InvalideTypePrimaryKey,line,col->name,NULL);
                }
                existPrimaryKey=true;
                if(debug){printf("primary key n exist pas");}
                insererEnFinDeListeName(liPartitionKey,col->name);
                break;
            }
        }
        if(!existPrimaryKey){
            char *msg=(char *)malloc(120*sizeof(char));
            strcpy(msg,"No PRIMARY KEY specifed (exactly one required)");
            _creerErreur(InvalidRequest,line,NULL,msg);

        }

    }
    ouvrirListeName(liClusteringCol);
    while (!finListeName(liClusteringCol)) {
        char *pOrder = objetCourantName(liClusteringCol);
        objetColumn *objetCherche = (objetColumn *) malloc(sizeof(objetColumn));
        objetCherche->name = (char *) malloc(strlen(pOrder) * sizeof(char) + 1);
        strcpy(objetCherche->name, pOrder);
        objetColumn *objetTrouve = chercherUnobjetColumn(liCol, objetCherche);
        free(objetCherche);
        if (objetTrouve == NULL) {
            char *msg=(char *)malloc((strlen(pOrder)+120)*sizeof(char));
            sprintf(msg,"Unknown definition %s referenced in PRIMARY KEY",pOrder);
            _creerErreur(InvalidRequest,line,NULL,msg);
       }
    }
    ListeName * liOther=creerListeName(NULL,NULL);
    char* ch;
    char* objet;
    ouvrirListeName (liPartitionKey);
    while (!finListeName (liPartitionKey) ) {
        ch = objetCourantName(liPartitionKey);
        insererEnFinDeListeName(liOther, ch);
    }
    ouvrirListeName (liClusteringCol);
    while (!finListeName (liClusteringCol) ) {
        ch = objetCourantName(liClusteringCol);
        insererEnFinDeListeName(liOther, ch);
    }


    _multiple_def_of_column(liCol,line);
   _multiple_declaration(liOther,line,"Primary Key");
    detruireListeName(liOther);
    free(liOther);

}

void _multiple_declaration(ListeName *li,int line,char *msg){
    ListeName * liOther=creerListeName(NULL,NULL);
    char* ch;
    char* objet;
    ouvrirListeName (li);
    while (!finListeName (li) ) {
        ch = objetCourantName(li);
        insererEnFinDeListeName(liOther, ch);
    }

    ouvrirListeName (liOther);
    while (!finListeName (liOther) ) {
        objet = objetCourantName(liOther);
        ouvrirListeName (li);
        while (!finListeName (li) ) {
            ch = objetCourantName (li);
            if(li->comparer (objet, ch) == 0 && objet!=ch) {
                _creerErreur(MultipleDefIn,line,objet,msg);
            }
        }

    }

    detruireListeName(liOther);
    free(liOther);
}


void _multiple_def_of_column(ListeColumn *li,int line){
    ListeColumn * liOther=creerListeColumn(NULL,NULL);
    objetColumn* ch;
    objetColumn* objet;
    ouvrirListeColumn (li);
    while (!finListeColumn (li) ) {
        ch = objetCourantColumn(li);
        insererEnFinDeListeColumn(liOther, ch);
    }

    ouvrirListeColumn (liOther);
    while (!finListeColumn (liOther) ) {
        objet = objetCourantColumn(liOther);
        ouvrirListeColumn (li);
        while (!finListeColumn (li) ) {
            ch = objetCourantColumn (li);
            if(li->comparer (objet, ch) == 0 && objet!=ch) {
                    _creerErreur(MultipleDefOfColumn,line,objet->name,NULL);
            }
        }

    }

    detruireListeColumn(liOther);
    free(liOther);
    // pointeur courant


}

void _valide_column(ListeColumn *liCol,int line) {
    int nbPrimaryKey=0;
    ouvrirListeColumn(liCol);
    while (!finListeColumn(liCol)) {
        objetColumn *col = objetCourantColumn(liCol);
        if (col->partitionKey) {
            if(col->_static){
                char *msg=(char *)malloc(120*sizeof(char));
                strcpy(msg,"Static column species cannot be part of the PRIMARY KEY");
                _creerErreur(InvalidRequest,line,NULL,msg);
                if(debug){printf("static et primary key\n");
                    printf("%s",msg);
                            ;}
            }
            nbPrimaryKey++;
        }
    }

    if(nbPrimaryKey>1){
        char *msg=(char *)malloc(120*sizeof(char));
        strcpy(msg,"Multiple PRIMARY KEYs specifed (exactly one required)");
        _creerErreur(InvalidRequest,line,NULL,msg);

    }
}

void _column_in_more_than_relation(ListeRelation *li,int line){
    ListeRelation* liOther=creerListeRelation(NULL,NULL);
    objetRelation* ch;
    objetRelation* objet;
    ouvrirListeRelation (li);
    while (!finListeRelation (li) ) {
        ch = objetCourantRelation(li);
        insererEnFinDeListeRelation(liOther, ch);
    }

    ouvrirListeRelation (liOther);
    while (!finListeRelation (liOther) ) {

        objet = objetCourantRelation(liOther);
        ouvrirListeRelation (li);
        while (!finListeRelation (li) ) {
            ch = objetCourantRelation (li);
            if(li->comparer (objet, ch) == 0 && objet!=ch) {




                    if(*(objet->anOperator)==*(ch->anOperator) ){
                        _creerErreur(MoreThanOneRestrictionForOneColumn,line,ch->name,NULL);
                    }
                    if(*(objet->anOperator)==op_eq || *(ch->anOperator)==op_eq){
                        _creerErreur(MoreThanRelationEq,line,ch->name,NULL);
                    }

            }
        }
    }

detruireListeRelation(liOther);
free(liOther);
    // pointeur courant


}




void _column_in_more_than_set(ListeNameValue *li,int line){
    ListeNameValue* liOther=creerListeNameValue(NULL,NULL);
    objetNameValue* ch;
    objetNameValue* objet;
    ouvrirListeNameValue (li);
    while (!finListeNameValue (li) ) {
        ch = objetCourantNameValue(li);
        insererEnFinDeListeNameValue(liOther, ch);
    }

    ouvrirListeNameValue (liOther);
    while (!finListeNameValue (liOther) ) {

        objet = objetCourantNameValue(liOther);
        ouvrirListeNameValue (li);
        while (!finListeNameValue (li) ) {
            ch = objetCourantNameValue (li);
            if(li->comparer (objet, ch) == 0 && objet!=ch) {
                _creerErreur(MultipleDefIn,line,objet->name," SET ");

            }
        }
    }

    detruireListeNameValue(liOther);
    free(liOther);
    // pointeur courant


}




void _valide_delete_where(objetTable *table,ListeRelation *li,int line){
//    int trouve=0;
//    void* ch;
//    // pointeur courant
//    ouvrirListeRelation (li);
//    while (!finListeRelation (li) && !trouve) {
//        ch = objetCourantRelation (li);
//        trouve = li->comparer (objetCherche, ch) == 0;
//    }
//    return trouve ? ch : NULL;

    if(debug){
        printf("valide delete where");
        listerListeRelation(li,printRelation);}
    ouvrirListeRelation(li);
    while(!finListeRelation(li) ) {
        objetRelation *objet = objetCourantRelation(li);
        objetColumn *ocCherche = _create_column_with_name(objet->name);
        objetColumn *ocTrouve = chercherUnobjetColumn(table->liCol, ocCherche);
        free(ocCherche);
        if (ocTrouve == NULL) {
            if (debug) { printf("non trouve\n"); }
            _creerErreur(UndefinedColumn, line, objet->name, NULL);
            //creerErreur
        } else {
            if (debug) { printf(" trouve\n"); }
            if (!_valide_type(objet->t, ocTrouve->type, line)) {
                char *nameCol = (char *) malloc(
                        (strlen(ocTrouve->name) + strlen(_type_name(ocTrouve->type)) + strlen("'' of type ") + 1) *
                        sizeof(char));
                sprintf(nameCol, "'%s' of type %s", ocTrouve->name, _type_name(ocTrouve->type));
                // printf("type:%s value:%s\n",type1,value);
                _creerErreur(InvalideTypeInsert, line, _term_value(objet->t), nameCol);
            } else {
                char *name = chercherUnchar(table->liPartitionKey, objet->name);
                if (name == NULL) {
                    char *name2 = chercherUnchar(table->liClustringOrder, objet->name);
                    if (name2 == NULL) {
                        _creerErreur(NonPrimaryKeyInWhere, line, objet->name, NULL);
                    }
                }
            }
        }
    }

    _column_in_more_than_relation(li,line);
    ouvrirListeName(table->liPartitionKey);
    while(!finListeName(table->liPartitionKey)){
        char *name=objetCourantName(table->liPartitionKey);
        objetRelation *relationRecherche=(objetRelation *)malloc(sizeof(objetRelation));
        relationRecherche->name=strdup(name);
        objetRelation *relationTrouve=chercherUnobjetRelation(li,relationRecherche);
        if(relationTrouve==NULL){
            _creerErreur(SomePartitionKeyMissing,line,name,NULL);
        }
        else{
            if(*(relationTrouve->anOperator)!=op_eq){
                _creerErreur(NonValideOperatorForPartitionKey,line,name,NULL);
            }else{
                if(relationTrouve->t->type==constant_term){
                    if(relationTrouve->t->node.constant.type==null_const){
                        _creerErreur(InvalidNullInCondition,line,relationTrouve->name,NULL);
                    }
                }
            }
        }
    }
}







void _valide_update_where(objetTable *table,ListeRelation *li,int line){
    ouvrirListeRelation(li);
    while(!finListeRelation(li)){
        objetRelation *objet=objetCourantRelation(li);
        objetColumn *ocCherche=_create_column_with_name(objet->name);
        objetColumn *ocTrouve=chercherUnobjetColumn(table->liCol,ocCherche);
        free(ocCherche);
        if(ocTrouve==NULL){
            _creerErreur(UndefinedColumn,line,objet->name,NULL);
            //creerErreur
        }
        else{
            if(!_valide_type(objet->t,ocTrouve->type,line)){
                char *nameCol=(char *)malloc((strlen(ocTrouve->name)+strlen(_type_name(ocTrouve->type))+strlen("'' of type ")+1)*sizeof(char));
                sprintf(nameCol,"'%s' of type %s",ocTrouve->name,_type_name(ocTrouve->type));
                // printf("type:%s value:%s\n",type1,value);
                _creerErreur(InvalideTypeInsert,line,_term_value(objet->t),nameCol);
            }else{
                char *name=chercherUnchar(table->liPartitionKey,objet->name);
                if(name==NULL) {
                    char *name2 = chercherUnchar(table->liClustringOrder, objet->name);
                    if (name2 == NULL) {
                        _creerErreur(NonPrimaryKeyInWhere, line, objet->name, NULL);
                    }
                    //_creerErreur(NonValideOperatorForPartitionKey,line,name,NULL);
                }
            }
        }
    }

    _column_in_more_than_relation(li,line);

    ouvrirListeName(table->liPartitionKey);
    while(!finListeName(table->liPartitionKey)){
        char *name=objetCourantName(table->liPartitionKey);
        objetRelation *relationRecherche=(objetRelation *)malloc(sizeof(objetRelation));
        relationRecherche->name=strdup(name);
        objetRelation *relationTrouve=chercherUnobjetRelation(li,relationRecherche);
        free(relationRecherche);
        if(relationTrouve==NULL){
            _creerErreur(SomePartitionKeyMissing,line,name,NULL);
        }
        else{
            if(*(relationTrouve->anOperator)!=op_eq){
                _creerErreur(NonValideOperatorForPartitionKey,line,name,NULL);
            }
            else{
                if(relationTrouve->t->type==constant_term){
                    if(relationTrouve->t->node.constant.type==null_const){
                        _creerErreur(InvalidNullInCondition,line,relationTrouve->name,NULL);
                    }
                }
            }

        }
    }
//    InvalideOpertorForUpdate
    ouvrirListeName(table->liClustringOrder);
    while(!finListeName(table->liClustringOrder)){
        char *name=objetCourantName(table->liClustringOrder);
        objetRelation *relationRecherche=(objetRelation *)malloc(sizeof(objetRelation));
        relationRecherche->name=strdup(name);
        objetRelation *relationTrouve=chercherUnobjetRelation(li,relationRecherche);
        free(relationRecherche);
        if(relationTrouve==NULL){
            _creerErreur(SomeClusteringMissing,line,name,NULL);
        }
        else{
            if(*(relationTrouve->anOperator)!=op_eq){
                _creerErreur(InvalideOpertorForUpdate,line,name,NULL);
            }else {
                if(relationTrouve->t->type==constant_term){
                    if(relationTrouve->t->node.constant.type==null_const){
                        _creerErreur(InvalidNullInCondition,line,relationTrouve->name,NULL);
                    }
                }
            }
        }
    }


}


void _valide_update_set(objetTable *table,ListeNameValue *li,int line){
    ouvrirListeNameValue(li);
    while(!finListeNameValue(li)){
        objetNameValue *objet=objetCourantNameValue(li);
        objetColumn *ocCherche=_create_column_with_name(objet->name);
        objetColumn *ocTrouve=chercherUnobjetColumn(table->liCol,ocCherche);
        free(ocCherche);
        if(ocTrouve==NULL){
            _creerErreur(UndefinedColumn,line,objet->name,NULL);
            //creerErreur
        }
        else{
            if(!_valide_type(objet->t,ocTrouve->type,line)){
                char *nameCol=(char *)malloc((strlen(ocTrouve->name)+strlen(_type_name(ocTrouve->type))+strlen("'' of type ")+1)*sizeof(char));
                sprintf(nameCol,"'%s' of type %s",ocTrouve->name,_type_name(ocTrouve->type));
                // printf("type:%s value:%s\n",type1,value);
                _creerErreur(InvalideTypeInsert,line,_term_value(objet->t),nameCol);
            }else{
                char *name=chercherUnchar(table->liPartitionKey,objet->name);
                if(name!=NULL) {
                    _creerErreur(PrimaryKeyInSet,line,name,NULL);
                }
                char *name2 = chercherUnchar(table->liClustringOrder, objet->name);
                if (name2 != NULL) {
                    _creerErreur(PrimaryKeyInSet,line,name,NULL);
                }
            }
        }
    }

    _column_in_more_than_set(li,line);
}

void _valide_select_where(objetTable *table,ListeRelation *li,int line) {
    //ListeName *listeName=creerListeName(NULL,NULL);
    ouvrirListeRelation(li);
    while (!finListeRelation(li)) {
        objetRelation *objet = objetCourantRelation(li);
        //insererEnFinDeListeName(listeName,objet->name);
        objetColumn *ocCherche = _create_column_with_name(objet->name);
        objetColumn *ocTrouve = chercherUnobjetColumn(table->liCol, ocCherche);
        free(ocCherche);
        if (ocTrouve == NULL) {
            _creerErreur(UndefinedColumn, line, objet->name, NULL);
            //creerErreur
        } else {
            if (!_valide_type(objet->t, ocTrouve->type, line)) {
                char *nameCol = (char *) malloc(
                        (strlen(ocTrouve->name) + strlen(_type_name(ocTrouve->type)) + strlen("'' of type ") + 1) *
                        sizeof(char));
                sprintf(nameCol, "'%s' of type %s", ocTrouve->name, _type_name(ocTrouve->type));
                // printf("type:%s value:%s\n",type1,value);
                _creerErreur(InvalideTypeInsert, line, _term_value(objet->t), nameCol);
            }else{
                if(!(objet->t->type==constant_term)){
                    _creerErreur(InvalideTypeInWhere,line,ocTrouve->name,NULL);
                }
            }
        }

        _column_in_more_than_relation(li, line);


    }
}


void _valide_selector(objetTable *table,ListeSelector * li,int line) {
    ListeName *listeName=creerListeName(NULL,NULL);
    ouvrirListeSelector(li);
    while (!finListeSelector(li)) {
        objetSelector *objet = objetCourantSelector(li);
        insererEnFinDeListeName(listeName,objet->name);
        objetColumn *ocCherche = _create_column_with_name(objet->name);
        objetColumn *ocTrouve = chercherUnobjetColumn(table->liCol, ocCherche);
        free(ocCherche);
        if (ocTrouve == NULL) {
            _creerErreur(UndefinedColumn, line, objet->name, NULL);
            //creerErreur
        }

    }
    _multiple_declaration(listeName,line,"Select Clause");
}


