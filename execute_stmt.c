#include "structure.h"//
// Created by ismail on 15/02/17.
//

#include "execute_stmt.h"
#include "structure.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "persistance.h"
#include "semantique.h"
#define debug false


//createKeyspaceStmt  *_initCreatTableSrmt(){
////    char *name;
////    ListeOptionKeyspace *liOK;
////    ListeKey *liKey;
//    createKeyspaceStmt  *stmt=(createKeyspaceStmt  *)malloc(sizeof(createKeyspaceStmt));
//    stmt->name=NULL;
//
//}


deleteStmt *_initDeleteStmt(){
    deleteStmt *stmt=(deleteStmt *)malloc(sizeof(deleteStmt));
    stmt->keyspace=NULL;
    stmt->table=NULL;
    stmt->liRelation=creerListeRelation(NULL,NULL);
    return  stmt;
}


void printRelation(objetRelation *o){
    printf("(%s ",o->name);
    char *op=NULL;
    switch (*o->anOperator){
        case op_eq:
            op=strdup("op_eq");
            break;
        case op_lessEq:
            op=strdup("op_lessEq");
            break;
        case op_less:
            op=strdup("op_less");
            break;
        case op_greater:
            op=strdup("op_greater");
            break;
        case op_greaterEq:
            op=strdup("op_greaterEq");
            break;
        case op_diff:
            op=strdup("op_diff");
            break;
    }
    printf("%s ",op);
    printTerm(o->t);
    printf(" )");
}




void printKeyspaceDesc(objetKeyspace *oks) {
        printf("%s ", oks->name);


}


objetNameValue *_createNameValue(char *name){
    objetNameValue *oc=(objetNameValue *)malloc(sizeof(objetNameValue));
    oc->name=strdup(name);
    oc->t=NULL;
    //oc->clusteringCol=false;
}



objetColumn *_create_column(){
    objetColumn *oc=(objetColumn *)malloc(sizeof(objetColumn));
    oc->_static=false;
    oc->partitionKey=false;
    //oc->clusteringCol=false;
}

objetColumn *_create_column_with_name(char *name){
    objetColumn *oc=(objetColumn *)malloc(sizeof(objetColumn));
    oc->_static=false;
    oc->partitionKey=false;
    oc->name=strdup(name);
    //oc->clusteringCol=false;
}

void printColumn(objetColumn *oC){
    printf("%s: ", oC->name);
    if(oC->_static){printf("static ");}
//    if(oC->clusteringCol){printf("culstreingCol ");}
    if(oC->partitionKey){printf("partitionKey ");}
    printf("type");
    printf("[");
    printType(oC->type);
    printf("]");

}
//native_typeE,
//map_typeE,
//set_typeE,
//list_typeE,
//udt_typeE,
//tuple_typeE,
//custom_typeE
void printType(cql_type *t){
    switch (t->type){
        case native_typeE:
            printf("native_type :");
            printf("%d ",t->node.native);
            break;
        case map_typeE:
            printf("map :");
            printf("<");
            printType(t->node.map.champ1) ;
            printf(",");
            printType(t->node.map.champ2) ;
            printf(">");
            break;
        case set_typeE:
            printf("set :");
            printf("<");
            printType(t->node.list.champ) ;
            printf(">");
            break;
        case list_typeE:
            printf("list :");
            printf("<");
            printType(t->node.list.champ) ;
            printf(">");
            break;
        case custom_typeE:
            printf("custom :");
            printf("%s ",t->node.costum.name);
            break;
        default:
            printf("default ");
    }
}


void printOptionTable(optionTable *oT){
    switch (oT->type){
        case option_keyspace:
            afficherOptionKeyspace(oT->node.optionKey);
            break;
        case compact_storage:
            printf("comapct ");
            break;
        case culstring_order:
            printf("order");
            break;
    }

}







void afficherConstant(cqlConstant constant){
    printf("%d\n", constant.type);
    switch(constant.type){
        case string_const:
            printf("%s\n", constant.node.string);
            break;
        case integer_const:
            printf("%d\n", constant.node.integer);
            break;
    }


}


void afficherOptionKeyspace(optionKeyspace *oks){
    printf("%s\n", oks->idf);
    printf("%d\n", oks->type);
    switch(oks->type){
        case idf_opt:
            printf("%s\n", (oks->node).idf_value);
            break;
        case constant_opt:
            afficherConstant((oks->node).constant);
            break;
        case map_opt:
            listerListeMap((oks->node).map,printMap);
    }
}


//void printList(objetList *oL){
//	printTerm(oL->champ);
//}

void printTerm(term *t){
    switch(t->type){
        case constant_term:
            afficherConstant((t->node).constant);
            break;
        case list_term:
            listerListeList((t->node).list,printTerm);
            break;
        case set_term:
            listerListeList((t->node).list,printTerm);
            break;
    }

}


void printMap(objetMap *oM){
    printTerm(oM->champ1);
    printTerm(oM->champ2);
}


void printKeyspace(objetKeyspace *oks){
    printf("%s\n", oks->name);
    printf("[");
    listerListeOptionKeyspace(oks->liOK,afficherOptionKeyspace);
    listerListeTable(oks->liTab,printTable);
    printf("]");
}
void printString(char *name){
    printf(" %s ",name);
}

void printNameValue(objetNameValue *objet){
    printf("(%s ,",objet->name);
    printTerm(objet->t);
    printf(")");
}

void printTable(objetTable *ot){
    printf("%s   ",ot->name);
    printf("(");
    listerListeColumn(ot->liCol,printColumn);
    listerListeOptionTable(ot->liOT,printOptionTable);
    printf("\npartitionKey:(");
    listerListeName(ot->liPartitionKey,printString);
    printf(")\n");
    printf("\nclustringOrder:(");
    listerListeName(ot->liClustringOrder,printString);
    printf(")\n");
    printf(")");
}





// int comparerKeyspace(objetKeyspace* objet1, objetKeyspace* objet2)
// {
// 	return strcmp(objet1->name,objet2->name);
// }


// char* toStringKeyspace(objetKeyspace *oks){
// 	return oks->name;
// }






objetKeyspace *_createKeyspace(char* name){
    objetKeyspace *oks=(objetKeyspace *)malloc(sizeof(objetKeyspace));
    oks->name=(char *)malloc(strlen(name)*sizeof(char)+1);
    strcpy(oks->name,name);
    oks->liTab=creerListeTable(NULL,NULL);
    oks->liOK=creerListeOptionKeyspace(NULL,NULL);
    return oks;
}



objetTable *_createTable(char* name){
    objetTable *oks=(objetTable *)malloc(sizeof(objetTable));
    oks->name=(char *)malloc(strlen(name)*sizeof(char)+1);
    strcpy(oks->name,name);
    oks->liCol=creerListeColumn(NULL,NULL);
    oks->liOT=creerListeOptionTable(NULL,NULL);
    oks->liPartitionKey=creerListeName(NULL,NULL);
    oks->liClustringOrder=creerListeName(NULL,NULL);
    return oks;
}

//objetTable *createTable(tableName *tN,objetKeyspace *currentKeyspace){
//    if(tN->nameKeyspace==NULL){
//        if(currentKeyspace->name==NULL){
//
//        }else{
//
//        }
//    }else{
//
//    }
//    objetTable *oT=(objetTable*)malloc(sizeof(objetTable));
//    oT->liCol=creerListeColumn(NULL,NULL);
//    oT->liOT=creerListeOptionTable(NULL,NULL);
//}






// void afficherKeyspace(objetKeyspace *oks){
// 	printf("Keyspace :%s\n", oks->name);
// 	printf("arr_table :[");
// 	//listerListeTable(oks->liTab,afficherTable);
// 	printf("]");


// }

tableName *_create_tableName(){
    tableName *tN=(tableName *)malloc(sizeof(tableName));
    tN->nameKeyspace=NULL;
    tN->nameTable=NULL;
}





void _createKeyspaceStmt(createKeyspaceStmt *cKS){
    objetKeyspace *objetChercheKey=_createKeyspace(cKS->name);
    objetKeyspace *objetTrouveKey=chercherUnobjetKeyspace(cKS->liKey,objetChercheKey);
    free(objetChercheKey);
    if(objetTrouveKey==NULL){
        objetKeyspace *objet=_createKeyspace(cKS->name);
        ouvrirListeOptionKeyspace(cKS->liOK);
        while(!finListeOptionKeyspace(cKS->liOK)){
            optionKeyspace *ok=objetCourantOptionKeyspace(cKS->liOK);
            insererEnFinDeListeOptionKeyspace(objet->liOK,ok);
        }
//        recopierListeOptionKeyspace(objet->liOK,cKS->liOK);
        insererEnFinDeListeKey(cKS->liKey,objet);
    }

}


void _alterKeyspaceStmt(alterKeyspaceStmt *aKS){

    UnionOptionKeyspace(aKS->key->liOK,aKS->liOK);

}


void UnionOptionKeyspace(ListeOptionKeyspace *li,ListeOptionKeyspace *li2) {
    booleen existDurableWrite=faux;
    ouvrirListeOptionKeyspace(li);
    while (!finListeOptionKeyspace(li)) {
        optionKeyspace *ch = objetCourantOptionKeyspace(li);
        if (strcmp(ch->idf, "durable_writes") == 0) {
            ouvrirListeOptionKeyspace(li2);
            while (!finListeOptionKeyspace(li2)) {
                optionKeyspace *ch2 = objetCourantOptionKeyspace(li2);
                if (strcmp(ch2->idf, "durable_writes") == 0) {
                    *ch = *ch2;
                }
            }
        } else if (strcmp(ch->idf, "replication") == 0) {
            if(debug){printf("replication1\n");}
            ouvrirListeOptionKeyspace(li2);
            while (!finListeOptionKeyspace(li2)) {
                optionKeyspace *ch2 = objetCourantOptionKeyspace(li2);
                if (strcmp(ch2->idf, "replication") == 0) {
                    if(debug){printf("replication2\n");}

                    *ch = *ch2;
                }
            }
        }
    }
    ouvrirListeOptionKeyspace(li);
    while (!finListeOptionKeyspace(li)) {
        optionKeyspace *ch = objetCourantOptionKeyspace(li);
        if (strcmp(ch->idf, "durable_writes") == 0) {
            existDurableWrite = vrai;
        }
    }
    if(!existDurableWrite){
        ouvrirListeOptionKeyspace(li2);
        while (!finListeOptionKeyspace(li2)) {
            optionKeyspace *ch2 = objetCourantOptionKeyspace(li2);
            if (strcmp(ch2->idf, "durable_writes") == 0) {
                insererEnFinDeListeOptionKeyspace(li,ch2);
            }
        }
    }
}

void _dropKeyspaceStmt(dropKeyspaceStmt *dKS){

    if(dKS->key!=NULL) {
        extraireUnobjetKeyspace(dKS->likey, dKS->key);
        _dropDirectoryKeyspace(dKS->key->name);
        free(dKS->key);
    }
}



void _dropTableStmt(dropTableStmt *stmt){

    if(stmt->table!=NULL) {
        extraireUnobjetTable(stmt->keyspace->liTab,stmt->table);
        _dropDirectoryTable(stmt->keyspace->name,stmt->table->name);
        free(stmt->table);
    }
}


void _truncatStmt(truncatStmt *stmt){
    if(stmt->table!=NULL) {
        _dropDirectoryFichier(stmt->keyspace->name,stmt->table->name);
    }
}

void _useKeyspaceStmt(useKeyspaceStmt *uKS){
    *(uKS->currentKeyspace)=*(uKS->useKeyspace);
}

void _descripe_Keyspace(descripeKeyspaceStmt *dKS){
    listerListeKey(dKS->liKey,printKeyspaceDesc);
    printf("\n");
}

void _deleteStmt(deleteStmt *stmt){
    _delete_persistance(stmt->keyspace->name,stmt->table->name,stmt->table->liPartitionKey,stmt->liRelation);
}


void _updateStmt(updateStmt *stmt) {
    ListeName *liStatic=creerListeName(NULL,NULL);
    ouvrirListeColumn(stmt->table->liCol);
    while(!finListeColumn(stmt->table->liCol)){
        objetColumn *oc=objetCourantColumn(stmt->table->liCol);
        if(oc->_static){
            insererEnFinDeListeName(liStatic,oc->name);
        }

    }

    ouvrirListeRelation(stmt->liRelation);
    while(!finListeRelation(stmt->liRelation)){
        objetRelation *objet=objetCourantRelation(stmt->liRelation);
        objetNameValue *newObjet=(objetNameValue *)malloc(sizeof(objetNameValue));
        newObjet->name=strdup(objet->name);
        newObjet->t=objet->t;
        insererEnFinDeListeNameValue(stmt->liNameValue,newObjet);
    }



    _update_persistance(stmt->keyspace->name, stmt->table->name, stmt->table->liPartitionKey,
                        stmt->table->liClustringOrder, liStatic,stmt->liNameValue, stmt->ifExist);
}


void _insertStmt(insertStmt *stmt) {
    ListeNameValue *liData = creerListeNameValue(NULL, NULL);
    ouvrirListeList(stmt->liValue);
    ouvrirListeName(stmt->liName);
    while (!finListeName(stmt->liName)) {
        char *name = objetCourantName(stmt->liName);
        term *term1 = objetCourantList(stmt->liValue);
        objetNameValue *objet = (objetNameValue *) malloc(sizeof(objetNameValue));
        objet->name = strdup(name);
        objet->t = term1;
        insererEnFinDeListeNameValue(liData, objet);
    }

    ListeName *liStatic=creerListeName(NULL,NULL);
    ouvrirListeColumn(stmt->table->liCol);
    while(!finListeColumn(stmt->table->liCol)){
        objetColumn *oc=objetCourantColumn(stmt->table->liCol);
        if(oc->_static){
            insererEnFinDeListeName(liStatic,oc->name);
        }

    }


    _insert_persistance(stmt->keyspace->name, stmt->table->name, stmt->table->liPartitionKey,
                        stmt->table->liClustringOrder, liStatic, liData, stmt->ifNotExist);
}


void _afficherData(ListeData *liData,ListeSelector *liSelector){
    size_t len[liSelector->nbElt];
    int i=0;
    int j=0;
    ouvrirListeSelector(liSelector);
    while(!finListeSelector(liSelector)){
        objetSelector *objetSelector1=objetCourantSelector(liSelector);
        if(objetSelector1->as==NULL){
           len[i]=strlen(objetSelector1->name);
            if(debug){printf("name:%s\n",objetSelector1->name);}
        }else{
            len[i]=strlen(objetSelector1->as);
            if(debug){printf("name:%s,as:%s",objetSelector1->name,objetSelector1->as);}
        }
        i++;
    }

    ouvrirListeData(liData);
    while(!finListeData(liData)){
        i=0;
        objetData *objetData1=objetCourantData(liData);
        ouvrirListeList(objetData1->data);
        while(!finListeList(objetData1->data)){
            term *term1=objetCourantList(objetData1->data);
            if(len[i]<strlen(_term_value(term1))){
                len[i]=strlen(_term_value(term1));
            }
            if(debug){printf("%s\n",_term_value(term1));}
            i++;
        }
    }


    if(debug){
        for(i=0;i<liSelector->nbElt;i++){
            printf("%d\n",len[i]);
        }
    }
    i=0;
    printf("|");
    ouvrirListeSelector(liSelector);
    while(!finListeSelector(liSelector)){
        objetSelector *objetSelector1=objetCourantSelector(liSelector);
        if(objetSelector1->as==NULL){
            printf(" %s ",objetSelector1->name);
            for(j=0;j<len[i]-strlen(objetSelector1->name);j++){
                printf(" ");
            }
        }else{
            printf(" %s ",objetSelector1->as);
            for(j=0;j<len[i]-strlen(objetSelector1->as);j++){
                printf(" ");
            }
        }
        i++;
        printf("|");
    }

  //  printf("\n|");
    printf("\n+");
    for(i=0;i<liSelector->nbElt;i++){
        for(j=0;j<len[i]+2;j++){
            printf("-");
        }
        printf("+");
    }
//    printf("\n");

    ouvrirListeData(liData);
    while(!finListeData(liData)){
        i=0;
        printf("\n");
        objetData *objetData1=objetCourantData(liData);
        ouvrirListeList(objetData1->data);
        printf("|");
        while(!finListeList(objetData1->data)){
            term *term1=objetCourantList(objetData1->data);
            printf(" %s ",_term_value(term1));
            for (j=0;j<len[i]-strlen(_term_value(term1));j++){
                printf(" ");
            }
            printf("|");
            i++;
        }
    }
    printf("\n");
    printf("(rows  %d)",liData->nbElt);
    printf("\n");

}


void _selectStmt(selectStmt *stmt1){
  if(stmt1->type==all){
        ouvrirListeColumn(stmt1->table->liCol);
        while(!finListeColumn(stmt1->table->liCol)){
            objetColumn *objetColumn1=objetCourantColumn(stmt1->table->liCol);
            objetSelector *objetSelector1=(objetSelector *)malloc(sizeof(objetSelector));
            objetSelector1->as=NULL;
            objetSelector1->name=strdup(objetColumn1->name);
            insererEnFinDeListeSelector(stmt1->liSelector,objetSelector1);
        }

    }

    _select_persistance(stmt1->keyspace->name,stmt1->table->name,stmt1->liRelation,stmt1->liSelector,stmt1->limit);
}







void _createTableStmt(createTableStmt *cts){
    if(cts->objetKeyspace1!=NULL){
        objetTable *objetCherche=_createTable(cts->name);
        objetTable *objetTrouve=chercherUnobjetTable(cts->objetKeyspace1->liTab,objetCherche);
        free(objetCherche);
        if(objetTrouve==NULL){
            if(debug){printf("table\n");}
            objetTable *table=_createTable(cts->name);
            ouvrirListeColumn(cts->liCol);
            while(!finListeColumn(cts->liCol)){
                objetColumn *oc=objetCourantColumn(cts->liCol);
                insererEnFinDeListeColumn(table->liCol,oc);
            }
            ouvrirListeName(cts->liPartitionKey);
            while(!finListeName(cts->liPartitionKey)){
                char *oc=objetCourantName(cts->liPartitionKey);
                insererEnFinDeListeName(table->liPartitionKey,oc);
            }
            ouvrirListeName(cts->liClustringOrder);
            while(!finListeName(cts->liClustringOrder)){
                char *oc=objetCourantName(cts->liClustringOrder);
                insererEnFinDeListeName(table->liClustringOrder,oc);
            }
            if(cts->liOptionTable!=NULL){
                ouvrirListeOptionTable(cts->liOptionTable);
                while(!finListeOptionTable(cts->liOptionTable)){
                    optionTable *oc=objetCourantOptionTable(cts->liOptionTable);
                    insererEnFinDeListeOptionTable(table->liOT,oc);
                }
            }



//            recopierListeColumn(table->liCol,cts->liCol);
//            recopierListeOptionTable(table->liOT,cts->liOptionTable);
//            *table->liCol=*cts->liCol;
//           *table->liOT=*cts->liOptionTable;
            insererEnFinDeListeTable(cts->objetKeyspace1->liTab,table);
        }
    }
}




void _describeTableStmt(describeTableStmt *stmt){
    int n=0;
    if(stmt->li==NULL){
        ouvrirListeTable (stmt->objetKey->liTab);
        while (!finListeTable (stmt->objetKey->liTab)) {
            objetTable* ch = objetCourantTable (stmt->objetKey->liTab);
            if(n<6){
                printf("%s  ",ch->name);
            }else{
                printf("%s\n",ch->name);
                n=0;
            }
             n++;
        }
        printf("\n");
    }else{
        ouvrirListeKey(stmt->li);
        while(!finListeKey(stmt->li)){
            objetKeyspace* objet=objetCourantKey(stmt->li);
            ouvrirListeTable(objet->liTab);
            printf("KEYSPACE : %s",objet->name);
            printf("\n");
            for(n=0;n<strlen(objet->name)+11;n++){
                printf("-");
            }
            printf("\n");
            if(finListeTable(objet->liTab)){
                printf("<empty>");
            }
            while(!finListeTable(objet->liTab)){
                objetTable *ch=objetCourantTable(objet->liTab);
                printf("%s  ",ch->name);
            }
            printf("\n");
        }
        printf("\n");

    }
}


void _excuteStmt(stmt *s){
    switch(s->type){
        case create_keyspace:
            _createKeyspaceStmt(&(s->node.createKeyspace));
            break;
        case alter_keyspace:
            _alterKeyspaceStmt(&(s->node.alterKeyspace));
            break;
        case drop_keyspace :
            _dropKeyspaceStmt(&(s->node.dropKeyspace));
            break;
        case use_keyspace :
            _useKeyspaceStmt(&(s->node.useKeyspace));
            break;
        case descripe_keyspace :
            _descripe_Keyspace(&(s->node.descripeKeyspace));
            break;
        case create_table:
            _createTableStmt(&(s->node.createTable));
            if(debug){printf("create_table");}
            break;
        case insert_into:
            _insertStmt(&(s->node.insert));
            break;
        case delete_stmt:
            _deleteStmt(&(s->node._delete));
            break;
        case select_stmt:
            _selectStmt(&(s->node.select));
            break;
        case update_stmt:
            _updateStmt(&(s->node.update));
            break;
        case drop_table:
            _dropTableStmt(&(s->node.dropTable));
            break;
        case truncate_table:
            _truncatStmt(&(s->node.truncat));
            break;
        case describe_table:
            _describeTableStmt(&(s->node.describeTable));
            break;
        default:
            if(debug){printf("default");}
            break;
    }

}