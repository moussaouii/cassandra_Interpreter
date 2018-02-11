
#include "persistance.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "execute_stmt.h"
#include "src/jansson.h"
#include "structure.h"
#include <unistd.h>
#include <dirent.h>
#include <stdint.h>
#include "semantique.h"
#include <inttypes.h>
#include "murmur3.h"

#define  debug false


//persistance data







void _load_all_data(json_t *json_arr ,char *nameKeyspace,char* nameTable){
//    int json_array_extend(json_t *array, json_t *other_array)
    json_t *root;
    json_error_t error;
    char *path=(char *)malloc((strlen(nameKeyspace)+strlen(nameTable)+1+strlen("./data/node1//data.json"))*sizeof(char));
    sprintf(path,"./data/%s/%s/node1data.json",nameKeyspace,nameTable);
    root = json_load_file(path,0, &error);
    if(root) {
        json_array_extend(json_arr,root);
    }
    sprintf(path,"./data/%s/%s/node2data.json",nameKeyspace,nameTable);
    root = json_load_file(path,0, &error);
    if(root) {
        json_array_extend(json_arr,root);
    }
    sprintf(path,"./data/%s/%s/node3data.json",nameKeyspace,nameTable);
    root = json_load_file(path,0, &error);
    if(root) {
        json_array_extend(json_arr,root);
    }
    sprintf(path,"./data/%s/%s/node4data.json",nameKeyspace,nameTable);
    root = json_load_file(path,0, &error);
    if(root) {
        json_array_extend(json_arr, root);
    }
}

//char *nameKeyspace,char* nameTable,ListeName *liPartitionKey,ListeName *liClustring,ListeName *liStatic,ListeNameValue *liData,boolean ifNotexist

void _select_persistance(char *nameKeyspace,char* nameTable,ListeRelation *liRelation,ListeSelector *liSelector,int limit){
    int limitTem=0;
    json_t *json_data_temp=json_array();
    json_t *json_data=json_array();
    _load_all_data(json_data_temp,nameKeyspace,nameTable);
    size_t i;
    size_t size = json_array_size(json_data_temp);
    for (i = 0; i < size; i++) {
        json_t *json_row = json_array_get(json_data_temp, i);
        if(_verfier_relations(json_row,liRelation)){
            json_array_append(json_data,json_row);
        }
    }



    ListeData *liData=creerListeData(NULL,NULL);
   size = json_array_size(json_data);
    for (i = 0; i < size; i++) {
        if(debug){printf("entre\n");}
        if(limitTem>=limit && limit!=-1){ break;}
        json_t *json_row = json_array_get(json_data, i);
        ListeList *liList=creerListeList(NULL,NULL);
        ouvrirListeSelector(liSelector);
        while(!finListeSelector(liSelector)){
            if(debug){printf("entre2\n");}
            objetSelector *objet=objetCourantSelector(liSelector);
            term *t=(term *)malloc(sizeof(term));
            _loadTerm(json_object_get(json_row,objet->name),t);
            insererEnFinDeListeList(liList,t);
        }
        objetData *objetData1=(objetData*)malloc(sizeof(objetData));
        objetData1->data=liList;
        insererEnFinDeListeData(liData,objetData1);
        limitTem++;
    }
    _afficherData(liData,liSelector);
}
boolean _verfier_relation(term  *t1,term *t2,_operatorEnum *_operator){
    double result2;
    boolean result=false;
    switch (t1->type){
        case constant_term:
            switch (t1->node.constant.type){
                case string_const:
                case blob_const:
                case uuid_const:
                    result=_verfier_relation_aux(strcmp(t1->node.constant.node.string,t2->node.constant.node.string),*_operator);
                    break;
                case integer_const:
                    result=_verfier_relation_aux(t1->node.constant.node.integer-t2->node.constant.node.integer,*_operator);
                    break;
                case float_const:
                    result2=t1->node.constant.node._float-t2->node.constant.node._float;
                    if(result2>0){
                        result=_verfier_relation_aux(1,*_operator);
                    }else if(result2==0){
                        result=_verfier_relation_aux(0,*_operator);
                    }else{
                        result=_verfier_relation_aux(-1,*_operator);
                    }
                    break;
                case null_const:
                    result=false;
                    break;
            }
            break;
    }
    return  result;
}


boolean _verfier_relation_aux(int nb,_operatorEnum op){
    boolean result=false;
    switch(op){
        case op_eq:
            if(nb==0){
                result=true;
            }else{
                result=false;
            }
            break;
        case op_diff:
            if(nb!=0){
                result=true;
            }else{
                result=false;
            }
            break;
        case op_greater:
            if(nb>0){
                result=true;
            }else{
                result=false;
            }
            break;
        case op_greaterEq:
            if(nb>=0){
                result=true;
            }else{
                result=false;
            }
            break;
        case op_less:
            if(nb<0){
                result=true;
            }else{
                result=false;
            }
            break;
        case op_lessEq:
            if(nb<0){
                result=true;
            }else{
                result=false;
            }
            break;
    }
    return  result;
}


boolean _verfier_relations(json_t* json_row,ListeRelation *liRelation) {
    boolean result=true;
    ouvrirListeRelation(liRelation);
    while (!finListeRelation(liRelation)) {
        objetRelation *objetRelation1 = objetCourantRelation(liRelation);
        term *term1 = (term *) malloc(sizeof(term));
        _loadTerm(json_object_get(json_row, objetRelation1->name), term1);
        if (_verfier_relation(term1, objetRelation1->t, objetRelation1->anOperator)) {
            if(debug){printf(" term verfier(%s)%s : %s\n",objetRelation1->name,_term_value(term1),_term_value(objetRelation1->t));}
            result=true;
        }
        else{
            result=false;
            break;
        }
    }
    return result;
}

boolean _delete_persistance_aux(json_t *json_data,char *nameKeyspace,char* nameTable,ListeRelation *liRelation){
    boolean dataModifie=false;
    size_t i;
    size_t size = json_array_size(json_data);
    for (i = 0; i < size; i++) {
        json_t *json_row = json_array_get(json_data, i);
        if(_verfier_relations(json_row,liRelation)){
            json_array_remove(json_data,i);
            dataModifie=true;
        }
    }
    return  dataModifie;
}

void _delete_persistance(char *nameKeyspace,char* nameTable,ListeName *liPartitionKey,ListeRelation *liRelation) {
    char *nameNode;
    //construire string for hach
    boolean premierFois=true;
    char *ForHach=NULL;
    int len=0;
    ListeName *liForHach=creerListeName(NULL,NULL);
    ouvrirListeName(liPartitionKey);
    while(!finListeName(liPartitionKey)){
        char *name=objetCourantName(liPartitionKey);
        objetRelation* objetChercher=(objetRelation *)malloc(sizeof(objetRelation));
        objetChercher->name=strdup(name);
        objetRelation* objetTrouve=chercherUnobjetRelation(liRelation,objetChercher);
        free(objetChercher);
        if(objetTrouve!=NULL){
            insererEnFinDeListeName(liForHach,termToString(objetTrouve->t));
            len=len+(int)strlen(termToString(objetTrouve->t));
        }
    }
    len++;
    ForHach=(char *)malloc(len*sizeof(char));
    ouvrirListeName(liForHach);
    while (!finListeName(liForHach)){
        char *name=objetCourantName(liForHach);
        if(premierFois){
            sprintf(ForHach,"%s",name);
            premierFois=false;
        }else{
            strcat(ForHach,name);
        }
    }
    // hachage
    uint32_t hash[4];                /* Output for the hash */
    uint32_t seed = 42;              /* Seed value for hash */
    MurmurHash3_x86_32(ForHach, strlen(ForHach), seed, hash);
    if(hash[0]<=(UINT32_MAX/4) ){
        nameNode=strdup("node1");
    }else if(hash[0]<=(UINT32_MAX/2)){
        nameNode=strdup("node2");
    }else if(hash[0]<=3*(UINT32_MAX/4)){
        nameNode=strdup("node3");
    }else{
        nameNode=strdup("node4");
    }
    if(debug){printf("\n%s\n",nameNode);}
    json_t *root;
    json_error_t error;
    boolean plusGrand=false;
    boolean insert=false;
    int nbTermEq=0;
    int nbTermPEq=0;
    char *path=(char *)malloc((strlen(nameKeyspace)+strlen(nameTable)+1+strlen("./data/node1//data.json"))*sizeof(char));
    sprintf(path,"./data/%s/%s/%sdata.json",nameKeyspace,nameTable,nameNode);
    root = json_load_file(path,0, &error);
    if(root) {
        if(_delete_persistance_aux(root,nameKeyspace,nameTable,liRelation)){
            json_dump_file(root,path, JSON_INDENT(4));
            if(debug){printf("deleteeeee");}
        }
    }
}
//    json_t *root;
//    json_error_t error;
//    char *path=(char *)malloc((strlen(nameKeyspace)+strlen(nameTable)+1+strlen("./data/node1//data.json"))*sizeof(char));
//    sprintf(path,"./data/%s/%s/node1data.json",nameKeyspace,nameTable);
//    root = json_load_file(path,0, &error);
//    if(root) {
//       if(_delete_persistance_aux(root,nameKeyspace,nameTable,liRelation)){
//           json_dump_file(root,path, JSON_INDENT(4));
//       }
//    }
//    sprintf(path,"./data/%s/%s/node2data.json",nameKeyspace,nameTable);
//    root = json_load_file(path,0, &error);
//    if(root) {
//        if(_delete_persistance_aux(root,nameKeyspace,nameTable,liRelation)){
//            json_dump_file(root,path, JSON_INDENT(4));
//        }
//    }
//    sprintf(path,"./data/%s/%s/node3data.json",nameKeyspace,nameTable);
//    root = json_load_file(path,0, &error);
//    if(root) {
//        if(_delete_persistance_aux(root,nameKeyspace,nameTable,liRelation)){
//            json_dump_file(root,path, JSON_INDENT(4));
//        }
//    }
//    sprintf(path,"./data/%s/%s/node4data.json",nameKeyspace,nameTable);
//    root = json_load_file(path,0, &error);
//    if(root) {
//        if(_delete_persistance_aux(root,nameKeyspace,nameTable,liRelation)){
//            json_dump_file(root,path, JSON_INDENT(4));
//        }
//    }
//}








void _insert_persistance(char *nameKeyspace,char* nameTable,ListeName *liPartitionKey,ListeName *liClustring,ListeName *liStatic,ListeNameValue *liData,boolean ifNotexist)
{
    //create directory
    char *pathDir=(char *)malloc((strlen("./data/")+1)*sizeof(char));
    sprintf(pathDir,"./data/%s",nameKeyspace);
    mkdir(pathDir, S_IRWXU);
    free(pathDir);
    pathDir=(char *)malloc((strlen("./data//")+1)*sizeof(char));
    sprintf(pathDir,"./data/%s/%s",nameKeyspace,nameTable);
    mkdir(pathDir, S_IRWXU);
    free(pathDir);

    char *nameNode;
    //construire string for hach
    boolean premierFois=true;
    char *ForHach=NULL;
    int len=0;
    ListeName *liForHach=creerListeName(NULL,NULL);
    ouvrirListeName(liPartitionKey);
    while(!finListeName(liPartitionKey)){
        char *name=objetCourantName(liPartitionKey);
        objetNameValue* objetChercher=_createNameValue(name);
        objetNameValue* objetTrouve=chercherUnobjetNameValue(liData,objetChercher);
        free(objetChercher);
        if(objetTrouve!=NULL){
            insererEnFinDeListeName(liForHach,termToString(objetTrouve->t));
            len=len+(int)strlen(termToString(objetTrouve->t));
        }
    }
    len++;
    ForHach=(char *)malloc(len*sizeof(char));
    ouvrirListeName(liForHach);
    while (!finListeName(liForHach)){
        char *name=objetCourantName(liForHach);
        if(premierFois){
            sprintf(ForHach,"%s",name);
            premierFois=false;
        }else{
            strcat(ForHach,name);
        }
    }

    // hachage
    uint32_t hash[4];                /* Output for the hash */
    uint32_t seed = 42;              /* Seed value for hash */
    //printf("Input: \"%s\"\n", ForHach);
    MurmurHash3_x86_32(ForHach, strlen(ForHach), seed, hash);
//    if(debug){printf("x86_32:  %08x\n", hash[0]);}
////    char *strHach=(char *)malloc(120*sizeof(char));
////    snprintf(strHach, sizeof strHach, "%lu", (unsigned long)hash[0]);
//    if(debug){printf("convert :%s\n",strHach);}
//    long long int numHach=atoi(strHach);
//    if(debug){printf("%lld\n",numHach);}
    if(hash[0]<=(UINT32_MAX/4) ){
        nameNode=strdup("node1");
    }else if(hash[0]<=(UINT32_MAX/2)){
        nameNode=strdup("node2");
    }else if(hash[0]<=3*(UINT32_MAX/4)){
        nameNode=strdup("node3");
    }else{
        nameNode=strdup("node4");
    }
    if(debug){printf("\n%s\n",nameNode);}
    if(debug){listerListeName(liClustring,printString);}
    if(debug){printf("___________________________________");}
    if(debug){listerListeName(liPartitionKey,printString);}
    json_t *root;
    json_error_t error;
    boolean plusGrand=false;
    boolean insert=false;
    int nbTermEq=0;
    int nbTermPEq=0;
    char *path=(char *)malloc((strlen(nameKeyspace)+strlen(nameTable)+1+strlen("./data/node1//data.json"))*sizeof(char));
    sprintf(path,"./data/%s/%s/%sdata.json",nameKeyspace,nameTable,nameNode);
    root = json_load_file(path,0, &error);
    if(root) {
        if(debug){printf("nb element %d\n",liClustring->nbElt);}
        size_t i;
        size_t size = json_array_size(root);
        for (i = 0; i < size; i++) {
            nbTermEq = 0;
            nbTermPEq = 0;
            plusGrand = false;
            json_t *json_row = json_array_get(root, i);
            ouvrirListeName(liClustring);
            while (!finListeName(liClustring) ) {
                char *namec = objetCourantName(liClustring);
                if(debug) {printf("liclustring %s\n",namec);}
                term *term1 = (term *) malloc(sizeof(term));
                _loadTerm(json_object_get(json_row, namec), term1);
                objetNameValue *objCherech = _createNameValue(namec);
                objetNameValue *objTrouve = chercherUnobjetNameValue(liData, objCherech);
                free(objCherech);
                if (_comparerTerm(term1, objTrouve->t) > 0) {
                    plusGrand = true;
                    break;
                } else if (_comparerTerm(term1, objTrouve->t) == 0) {
                    if(debug){printf("compare\n");}
                    nbTermEq++;
                }else{
                    break;
                }
            }
            if (plusGrand) {
                    if(debug){printf("shift %d",(int)i);}
                    json_t *json_row_data = json_object();
                    _writeData(json_row_data, liData);
                    json_array_insert(root, i, json_row_data);
                    json_decref(json_row_data);
                    insert=true;
                //}
            } else if (liClustring->nbElt == nbTermEq) {
                if(debug){printf("liClustring->nbElt == nbTermEq\n");}
                ouvrirListeName(liPartitionKey);
                while (!finListeName(liPartitionKey)) {
                    char *name = objetCourantName(liPartitionKey);
                    term *term1 = (term *) malloc(sizeof(term));
                    _loadTerm(json_object_get(json_row, name), term1);
                    objetNameValue *objCherech = _createNameValue(name);
                    objetNameValue *objTrouve = chercherUnobjetNameValue(liData, objCherech);
                    free(objCherech);
                    if (_comparerTerm(term1, objTrouve->t) == 0) {
                        nbTermPEq++;
                    }
                }
                if(nbTermPEq==liPartitionKey->nbElt){
                    if(!ifNotexist){
                        if(debug){printf("remplace %d",(int)i);}
                        json_t *json_row_data = json_object();
                        _writeData(json_row_data, liData);
                        json_array_set(root, i, json_row_data);
                        json_decref(json_row_data);
                    }
                    insert=true;
                }
            }

            if(insert){ break;}
            }
        if(!insert){
            json_t *json_row_data = json_object();
            _writeData(json_row_data, liData);
            json_array_append(root, json_row_data);
            json_decref(json_row_data);
            if(debug){printf("append a la fin");}
        }
             json_dump_file(root, path, JSON_INDENT(4));
            json_decref(root);
        } else{

            json_t *json_arr=json_array();
            json_t *json_row_data=json_object();
            _writeData(json_row_data,liData);
            json_array_append(json_arr,json_row_data);
            json_decref(json_row_data);
            json_dump_file(json_arr,path, JSON_INDENT(4));
        }
}





void _update_persistance(char *nameKeyspace,char* nameTable,ListeName *liPartitionKey,ListeName *liClustring,ListeName *liStatic,ListeNameValue *liData,boolean ifExist)
{

    //create directory
    char *pathDir=(char *)malloc((strlen("./data/")+1)*sizeof(char));
    sprintf(pathDir,"./data/%s",nameKeyspace);
    mkdir(pathDir, S_IRWXU);
    free(pathDir);
    pathDir=(char *)malloc((strlen("./data//")+1)*sizeof(char));
    sprintf(pathDir,"./data/%s/%s",nameKeyspace,nameTable);
    mkdir(pathDir, S_IRWXU);
    free(pathDir);

    char *nameNode;
    //construire string for hach
    boolean premierFois=true;
    char *ForHach=NULL;
    int len=0;
    ListeName *liForHach=creerListeName(NULL,NULL);
    ouvrirListeName(liPartitionKey);
    while(!finListeName(liPartitionKey)){
        char *name=objetCourantName(liPartitionKey);
        objetNameValue* objetChercher=_createNameValue(name);
        objetNameValue* objetTrouve=chercherUnobjetNameValue(liData,objetChercher);
        free(objetChercher);
        if(objetTrouve!=NULL){
            insererEnFinDeListeName(liForHach,termToString(objetTrouve->t));
            len=len+(int)strlen(termToString(objetTrouve->t));
        }
    }
    len++;
    ForHach=(char *)malloc(len*sizeof(char));
    ouvrirListeName(liForHach);
    while (!finListeName(liForHach)){
        char *name=objetCourantName(liForHach);
        if(premierFois){
            sprintf(ForHach,"%s",name);
            premierFois=false;
        }else{
            strcat(ForHach,name);
        }
    }

    // hachage
    uint32_t hash[4];                /* Output for the hash */
    uint32_t seed = 42;              /* Seed value for hash */
    //printf("Input: \"%s\"\n", ForHach);
    MurmurHash3_x86_32(ForHach, strlen(ForHach), seed, hash);
//    if(debug){printf("x86_32:  %08x\n", hash[0]);}
////    char *strHach=(char *)malloc(120*sizeof(char));
////    snprintf(strHach, sizeof strHach, "%lu", (unsigned long)hash[0]);
//    if(debug){printf("convert :%s\n",strHach);}
//    long long int numHach=atoi(strHach);
//    if(debug){printf("%lld\n",numHach);}
    if(hash[0]<=(UINT32_MAX/4) ){
        nameNode=strdup("node1");
    }else if(hash[0]<=(UINT32_MAX/2)){
        nameNode=strdup("node2");
    }else if(hash[0]<=3*(UINT32_MAX/4)){
        nameNode=strdup("node3");
    }else{
        nameNode=strdup("node4");
    }
    if(debug){printf("\n%s\n",nameNode);}
    json_t *root;
    json_error_t error;
    boolean plusGrand=false;
    boolean insert=false;
    int nbTermEq=0;
    int nbTermPEq=0;
    char *path=(char *)malloc((strlen(nameKeyspace)+strlen(nameTable)+1+strlen("./data/node1//data.json"))*sizeof(char));
    sprintf(path,"./data/%s/%s/%sdata.json",nameKeyspace,nameTable,nameNode);
    root = json_load_file(path,0, &error);
    if(root) {


        size_t i;
        size_t size = json_array_size(root);
        for (i = 0; i < size; i++) {
            nbTermEq = 0;
            nbTermPEq = 0;
            plusGrand = false;

            //listerListeName(liClustring,printString);
            json_t *json_row = json_array_get(root, i);
            ouvrirListeName(liClustring);
            while (!finListeName(liClustring)) {
                char *namec = objetCourantName(liClustring);
                term *term1 = (term *) malloc(sizeof(term));
                _loadTerm(json_object_get(json_row, namec), term1);
                objetNameValue *objCherech = _createNameValue(namec);
                objetNameValue *objTrouve = chercherUnobjetNameValue(liData, objCherech);
                free(objCherech);
               // printf("comparer :%d\n",_comparerTerm(term1, objTrouve->t) );
                if (_comparerTerm(term1, objTrouve->t) >0) {
                    plusGrand = true;
                    break;
                } else if (_comparerTerm(term1, objTrouve->t) == 0) {
                    nbTermEq++;
                }else{
                    break;
                }
            }

            if (plusGrand) {
                    if(debug){printf("shift %d",(int)i);}
                    json_t *json_row_data = json_object();
                    _writeData(json_row_data, liData);
                    json_array_insert(root, i, json_row_data);
                    json_decref(json_row_data);
                    insert=true;
            } else if (liClustring->nbElt == nbTermEq) {
                ouvrirListeName(liPartitionKey);
                while (!finListeName(liPartitionKey)) {
                    char *name = objetCourantName(liPartitionKey);
                    term *term1 = (term *) malloc(sizeof(term));
                    _loadTerm(json_object_get(json_row, name), term1);
                    objetNameValue *objCherech = _createNameValue(name);
                    objetNameValue *objTrouve = chercherUnobjetNameValue(liData, objCherech);
                    free(objCherech);
                    if (_comparerTerm(term1, objTrouve->t) == 0) {
                        nbTermPEq++;
                    }
                }
                if(nbTermPEq==liPartitionKey->nbElt){
                    if (!ifExist) {
                        if (debug) { printf("remplace %d", (int) i); }
                        json_t *json_row_data = json_object();
                        _writeData(json_row_data, liData);
                        json_array_set(root, i, json_row_data);
                        json_decref(json_row_data);
                    }
                    insert = true;
                }
            }

            if(insert){ break;}
        }
        if(!insert){
            if(!ifExist){
                json_t *json_row_data = json_object();
                _writeData(json_row_data, liData);
                json_array_append(root, json_row_data);
                json_decref(json_row_data);
                if(debug){printf("append a la fin");}
            }
        }
        json_dump_file(root, path, JSON_INDENT(4));
        json_decref(root);
    } else{
        if(!ifExist){
            json_t *json_arr=json_array();
            json_t *json_row_data=json_object();
            _writeData(json_row_data,liData);
            json_array_append(json_arr,json_row_data);
            json_decref(json_row_data);
            json_dump_file(json_arr,path, JSON_INDENT(4));
        }

    }
}





//            nbTermEq=0;
////            plusGrand=false;
//            json_t *json_row=json_array_get(root,i);
//            ouvrirListeName(liPrimaryKey);
//            while(!finListeName(liPrimaryKey)){
//                char *name=objetCourantName(liPrimaryKey);
//                term *term1=(term *)malloc(sizeof(term));
//                _loadTerm(json_object_get(json_row,name),term1);
//                term *termNew=_getTerm(liValue,liName,name);
//                if(_comparerTerm(term1,termNew)>0){
//                    plusGrand=true;
//                    break;
//                }else if(_comparerTerm(term1,termNew)==0){
//                    nbTermEq++;
//                }
//            }
//            if(plusGrand){
//                if(!ifNotexist){
//                    json_t *json_row_data=json_object();
//                    _writeData(json_row_data,liName,liValue);
//                    json_array_insert(root,i,json_row_data);
//                    json_decref(json_row_data);
//                }
//            }else if(liPrimaryKey->nbElt==nbTermEq){
//                json_t *json_row_data=json_object();
//                _writeData(json_row_data,liName,liValue);
//                json_array_set(root,i,json_row_data);
//                json_decref(json_row_data);
//            }
//
//        }
//        json_dump_file(root,path, JSON_INDENT(4));
//        json_decref(root);
//    }
//    else{
//        json_t *json_arr=json_array();
//        json_t *json_row_data=json_object();
//        _writeData(json_row_data,liName,liValue);
//        json_array_append(json_arr,json_row_data);
//        json_decref(json_row_data);
//        json_dump_file(json_arr,path, JSON_INDENT(4));
//    }



char *termToString(term *t){
    char *result=NULL;
    switch (t->type){
        case constant_term:
            switch (t->node.constant.type){
                case string_const:
                case blob_const:
                case uuid_const:
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
            }
            break;
    }
    return result;
}

int _comparerTerm(term *term1,term* term2){
    int result=0;
    double result2=0;
    switch (term1->type){
        case constant_term:
            switch (term1->node.constant.type){
                case string_const:
                case blob_const:
                case uuid_const:
                    result=strcmp(term1->node.constant.node.string,term2->node.constant.node.string);
                    break;
                case integer_const:
                    result=term1->node.constant.node.integer-term2->node.constant.node.integer;
                    break;
                case float_const:
                    result2=term1->node.constant.node._float-term2->node.constant.node._float;
                    if(result2>0){
                        result= 1;
                    }else if(result2==0){
                        result=0;
                    }else{
                        result=-1;
                    }
                    break;
            }
            break;
    }
    return result;
}
//term *_getTerm(ListeList *liValue,ListeName *liName,char *name){
//    ouvrirListeList(liValue);
//    ouvrirListeName(liName);
//    while(!finListeName(liName)){
//        char *name1=objetCourantName(liName);
//        term *t=objetCourantList(liValue);
//        if(strcmp(name1,name)==0){
//            return t;
//        }
//
//    }
//    return NULL;
//}


void _writeData(json_t *json_row_data,ListeNameValue *liData){
    ouvrirListeNameValue(liData);
    while(!finListeNameValue(liData)){
        objetNameValue *obj=objetCourantNameValue(liData);
        json_t *json_ob=json_object();
        _writeTerm(json_ob,obj->t);
        json_object_set_new(json_row_data,obj->name,json_ob);
    }
}


//load
ListeKey *_load(){
    json_t *root;
    json_error_t error;
    ListeKey *liKey=creerListeKey(NULL,NULL);
    root = json_load_file("./data/arr_keyspace.json",0, &error);
    if(root) {
        size_t i;
        size_t size = json_array_size(root);
        for (i = 0; i < size; i++) {
            json_t *json_key=json_array_get(root,i);
            objetKeyspace *key=_createKeyspace(json_string_value(json_key));
            _loadArrOptionKey(json_string_value(json_key),key->liOK);
            _loadArrTable(json_string_value(json_key),key->liTab);
            insererEnFinDeListeKey(liKey,key);

        }
        json_decref(root);
    }
    return liKey;
}


void _loadArrTable(char *name_key,ListeTable* li){
    json_t *root;
    json_error_t error;
    char *path=(char *)malloc((strlen("./data//arr_table.json")+strlen(name_key)+1)*sizeof(char));
    sprintf(path,"./data/%s/arr_table.json",name_key);
    root = json_load_file(path,0, &error);
    free(path);
    if(root) {
        size_t i;
        size_t size = json_array_size(root);
        for (i = 0; i < size; i++) {
            json_t *json_table=json_array_get(root,i);
            objetTable *table=_createTable(json_string_value(json_table));
            char *pathTable=(char *)malloc((strlen("./data//")+strlen(name_key)+strlen(table->name)+1)*sizeof(char));
            sprintf(pathTable,"./data/%s/%s",name_key,table->name);
                    if(debug){printf("path:%s\n",pathTable);}
            _loadArrOptionTable(pathTable,table->liOT);
            _loadArrColumn(pathTable,table->liCol,table->liPartitionKey,table->liClustringOrder);
            insererEnFinDeListeTable(li,table);
            free(pathTable);
        }
        json_decref(root);
    }
    else{

        //printf("msg:%s , source:%s  line:%d  column:%d  position:%d",error.text,error.source,error.line,error.column,error.position);
    }
}


void _loadArrColumn(char *pathTable,ListeColumn *li,ListeName *liPartitionKey,ListeName *liClustringOrder){
    if(debug){printf("path after :%s\n",pathTable);}
    json_t *root;
    json_error_t error;
    char *path=(char *)malloc((strlen("/arr_column.json")+strlen(pathTable)+1)*sizeof(char));
    sprintf(path,"%s/arr_column.json",pathTable);
    root = json_load_file(path,0, &error);
    free(path);
    if(root) {
        size_t i;
        size_t size1= json_array_size(json_object_get(root,"column"));
        for (i = 0; i < size1; i++) {
            json_t *json_column=json_array_get(json_object_get(root,"column"),i);
            objetColumn *o=(objetColumn *)malloc(sizeof(objetColumn));
            _loadColumn(json_column,o);
            insererEnFinDeListeColumn(li,o);

        }
        size_t size2 = json_array_size(json_object_get(root,"partition"));
        for (i = 0; i < size2; i++) {
            json_t *json_name=json_array_get(json_object_get(root,"partition"),i);
            char *o=strdup(json_string_value(json_name));
            insererEnFinDeListeName(liPartitionKey,o);

        }
        size_t size3 = json_array_size(json_object_get(root,"clustring"));
        for (i = 0; i < size3; i++) {
            json_t *json_name=json_array_get(json_object_get(root,"clustring"),i);
            char *o=strdup(json_string_value(json_name));
            insererEnFinDeListeName(liClustringOrder,o);

        }
        json_decref(root);
    }
    else{

      //  printf("msg:%s , source:%s  line:%d  column:%d  position:%d",error.text,error.source,error.line,error.column,error.position);
    }
}

void _loadColumn(json_t* json_column,objetColumn *column){
    column->name=strdup(json_string_value(json_object_get(json_column,"name")));
    column->_static=(boolean)json_boolean_value(json_object_get(json_column,"static"));
    column->partitionKey=(boolean)json_boolean_value(json_object_get(json_column,"partitionKey"));
//    column->clusteringCol=(boolean)json_boolean_value(json_object_get(json_column,"clusteringCol"));
    column->type=(cql_type *)malloc(sizeof(cql_type));
//    column->type->type=native_typeE;
//    column->type->node.native=ascii_native;
    _loadType(json_object_get(json_column,"type"),column->type);
    if(debug){
        printf("type:%d\nnode:%d",column->type->type,column->type->node);
    }
}

void _loadType(json_t* json_type,cql_type *cqlType){
    cqlType->type=(type_cql_type)json_integer_value(json_object_get(json_type,"type"));
    json_t* node=json_object_get(json_type,"node");
    //cqlType->node.native=(native_type)json_integer_value(node);
    switch (cqlType->type){
        case native_typeE:
            cqlType->node.native=(native_type)json_integer_value(node);
//            if(debug){printf("%d\n",cqlType->node.native);}
            break;
        case map_typeE:
            cqlType->node.map.champ1=(cql_type *)malloc(sizeof(cql_type));
            cqlType->node.map.champ2=(cql_type *)malloc(sizeof(cql_type));
            _loadType(json_object_get(node,"champ1"),cqlType->node.map.champ1);
            _loadType(json_object_get(node,"champ2"),cqlType->node.map.champ2);
            break;
        case set_typeE:
        case list_typeE:
            cqlType->node.list.champ=(cql_type *)malloc(sizeof(cql_type));
            _loadType(node,cqlType->node.list.champ);
            break;
        case udt_typeE:
        case tuple_typeE :
            break;
        case custom_typeE:
            cqlType->node.costum.name=strdup(json_string_value(node));
            break;
    }

}


void _loadArrOptionTable(char *pathTable,ListeOptionTable *li){
    json_t *root;
    json_error_t error;
    char *path=(char *)malloc((strlen("/arr_option.json")+strlen(pathTable)+1)*sizeof(char));
    sprintf(path,"%s/arr_option.json",pathTable);
    root = json_load_file(path,0, &error);
    free(path);
    if(root) {
        size_t i;
        size_t size = json_array_size(root);
        for (i = 0; i < size; i++) {
            json_t *json_option=json_array_get(root,i);
            optionTable *option=(optionTable *)malloc(sizeof(optionTable));
            _loadOptionTable(json_option,option);
            insererEnFinDeListeOptionTable(li,option);

        }
        json_decref(root);

    }
    else{

       // printf("msg:%s , source:%s  line:%d  column:%d  position:%d",error.text,error.source,error.line,error.column,error.position);
    }
}

void _loadOptionTable(json_t *json_option,optionTable *option){
    option->type=(type_option_table)json_integer_value(json_object_get(json_option,"type"));
    json_t *node=json_object_get(json_option,"node");
    switch (option->type){
//        option_keyspace,
//                compact_storage,
//                culstring_order,
        case option_keyspace:
            option->node.optionKey=(optionKeyspace *)malloc(sizeof(optionKeyspace));
            _loadOptionKey(node,option->node.optionKey);
            break;
        case compact_storage:
            break;
        case culstring_order:
            break;
    }

}

void _loadArrOptionKey(char *name_key,ListeOptionKeyspace *li){
    json_t *root;
    json_error_t error;
    char *path=(char *)malloc((strlen("./data//arr_option.json")+strlen(name_key)+1)*sizeof(char));
    sprintf(path,"./data/%s/arr_option.json",name_key);
    root = json_load_file(path,0, &error);
    free(path);
    if(root) {
        size_t i;
        size_t size = json_array_size(root);
        for (i = 0; i < size; i++) {
            json_t *json_option=json_array_get(root,i);
            optionKeyspace *option_key=(optionKeyspace *)malloc(sizeof(optionKeyspace));
            _loadOptionKey(json_option,option_key);
            insererEnFinDeListeOptionKeyspace(li,option_key);
        }
        json_decref(root);
    }
}


void _loadOptionKey(json_t *json_option,optionKeyspace *option ){
    option->type=(type_option)json_integer_value(json_object_get(json_option,"type"));
    option->idf=strdup(json_string_value(json_object_get(json_option,"idf")));
    json_t *node=json_object_get(json_option,"node");
    switch (option->type){
        case map_opt:
            option->node.map=creerListeMap(NULL,NULL);
            _loadMap(node,option->node.map);
            break;
        case idf_opt:
            option->node.idf_value=strdup(json_string_value(node));
            break;
        case constant_opt:
            _loadConstant(node,&(option->node.constant));
            break;
    }

}

void _loadConstant(json_t *json_constant,cqlConstant *constant){
    constant->type=(type_constant)json_integer_value(json_object_get(json_constant,"type"));
    switch (constant->type){
        case blob_const:
        case uuid_const:
        case string_const:
            constant->node.string=strdup(json_string_value(json_object_get(json_constant,"node")));
            break;
        case boolean_const:
        case integer_const:
            constant->node.integer=(int)json_integer_value(json_object_get(json_constant,"node"));
            break;
        case float_const:
            constant->node._float=json_real_value(json_object_get(json_constant,"node"));
            break;
        default:
            break;
    }
}


void _loadMap(json_t *json_map,ListeMap *map){
    size_t i;
    size_t size = json_array_size(json_map);
    for (i = 0; i < size; i++) {
        objetMap *om=(objetMap *)malloc(sizeof(objetMap));
        term *term1=(term *)malloc(sizeof(term));
        term *term2=(term *)malloc(sizeof(term));
        json_t *json_term=json_array_get(json_map,i);
        _loadTerm(json_object_get(json_term,"champ1"),term1);
        om->champ1=term1;
        _loadTerm(json_object_get(json_term,"champ2"),term2);
        om->champ2=term2;
        insererEnFinDeListeMap(map,om);
    }

}


void _loadList(json_t *json_list,ListeList *list){
    size_t i;
    size_t size = json_array_size(json_list);
    for (i = 0; i < size; i++) {
        term *t=(term *)malloc(sizeof(term));
        json_t *json_term=json_array_get(json_list,i);
        _loadTerm(json_term,t);
        insererEnFinDeListeList(list,t);

    }

}


void _loadTerm(json_t *json_term,term* term){
    if(json_term){
        term->type=(type_term)json_integer_value(json_object_get(json_term,"type"));
        switch (term->type){
            case map_term:
                term->node.map=creerListeMap(NULL,NULL);
                _loadMap(json_object_get(json_term,"node"),term->node.map);
                break;
            case tuple_term:
            case set_term:
            case list_term:
                term->node.list=creerListeList(NULL,NULL);
                _loadList(json_object_get(json_term,"node"),term->node.list);
                break;
            case constant_term:
                _loadConstant(json_object_get(json_term,"node"),&(term->node.constant));
                break;
        }
    }else{
        term->type=constant_term;
        term->node.constant.type=null_const;
    }

}

//write
void _write(ListeKey *liKey){
    char *path = strdup("./data/");
    mkdir(path, S_IRWXU);
    free(path);
    json_t *arr_key = json_array();
    ouvrirListeKey (liKey);
    while (!finListeKey (liKey)) {
        objetKeyspace *objetKey = objetCourantKey(liKey);
        json_array_append_new(arr_key,json_string(objetKey->name));
        char *pathKeyspace=(char *)malloc((strlen(objetKey->name)+strlen("./data/")+1)*sizeof(char));
        sprintf(pathKeyspace,"./data/%s",objetKey->name);
        _writeArrOptionKey(pathKeyspace,objetKey->liOK);
        _writeArrTable(pathKeyspace,objetKey->liTab);
        free(pathKeyspace);
    }
    char *pathKeys=strdup("./data/arr_keyspace.json");
    json_dump_file(arr_key,pathKeys, JSON_INDENT(4));
    free(pathKeys);
}




void _writeArrTable(char *pathKeyspace,ListeTable *li){
    json_t *arr_table = json_array();
    ouvrirListeTable (li);
    while (!finListeTable (li)) {
        objetTable *ot = objetCourantTable(li);
        json_array_append_new(arr_table,json_string(ot->name));
        char *pathTable=(char *)malloc((strlen(pathKeyspace)+strlen(ot->name)+2)*sizeof(char));
        sprintf(pathTable,"%s/%s",pathKeyspace,ot->name);
        _writeArrOptionTable(pathTable,ot->liOT);
        _writeArrColumn(pathTable,ot->liCol,ot->liPartitionKey,ot->liClustringOrder);
        free(pathTable);
    }
    mkdir(pathKeyspace, S_IRWXU);
    char *pathArrTable=(char *)malloc((strlen("arr_table.json")+strlen(pathKeyspace)+1)*sizeof(char));
    sprintf(pathArrTable,"%s/arr_table.json",pathKeyspace);
    json_dump_file(arr_table,pathArrTable, JSON_INDENT(4));
    free(pathArrTable);
    json_decref(arr_table);
}


void _writeArrOptionTable(char *pathTable,ListeOptionTable* li){
    json_t *arr_option = json_array();
    ouvrirListeOptionTable (li);
    while (!finListeOptionTable (li)) {
        optionTable *ok = objetCourantOptionTable(li);
        json_t *json_ok=json_object();
        _writeOptionTable(json_ok,ok);
        json_array_append(arr_option,json_ok);
        json_decref(json_ok);
    }
    mkdir(pathTable, S_IRWXU);
    char *pathOption=(char *)malloc((strlen("arr_option.json")+strlen(pathTable)+1)*sizeof(char));
    sprintf(pathOption,"%s/arr_option.json",pathTable);
    json_dump_file(arr_option,pathOption, JSON_INDENT(4));
    free(pathOption);
    json_decref(arr_option);
}

void _writeOptionTable(json_t *json_ot,optionTable *ot){
    json_object_set_new(json_ot,"type",json_integer(ot->type));
    json_t *obj=NULL;
    switch(ot->type){
        case  option_keyspace:
            obj=json_object();
            _writeOptionKey(obj,ot->node.optionKey);
            break;
        case culstring_order:
            //obj=json_string(ok->node.idf_value);
            break;
        default:
            break;
    }
    json_object_set(json_ot,"node",obj);
    json_decref(obj);
}

void _writeArrOptionKey(char *pathKeyspace,ListeOptionKeyspace *li){
    json_t *arr_option_key = json_array();
    ouvrirListeOptionKeyspace (li);
    while (!finListeOptionKeyspace (li)) {
        optionKeyspace *ok = objetCourantOptionKeyspace(li);
        json_t *json_ok=json_object();
        _writeOptionKey(json_ok,ok);
        json_array_append(arr_option_key,json_ok);
        json_decref(json_ok);
    }
    mkdir(pathKeyspace, S_IRWXU);
    char *pathOptionKey=(char *)malloc((strlen("/arr_option.json")+strlen(pathKeyspace)+1)*sizeof(char));
    sprintf(pathOptionKey,"%s/arr_option.json",pathKeyspace);
    json_dump_file(arr_option_key,pathOptionKey, JSON_INDENT(4));
    free(pathOptionKey);
    json_decref(arr_option_key);
}

void _writeOptionKey(json_t *json_ok,optionKeyspace *ok){
    json_object_set_new(json_ok,"idf",json_string(ok->idf));
    json_object_set_new(json_ok,"type",json_integer(ok->type));
    json_t *obj=NULL;
    switch(ok->type){
        case  map_opt:
            obj=json_array();
            _writeMap(obj,ok->node.map);
            break;
        case idf_opt:
            obj=json_string(ok->node.idf_value);
            break;
        case constant_opt:
            obj=json_object();
            _writeConstant(obj,ok->node.constant);
            break;
    }
    json_object_set(json_ok,"node",obj);
    json_decref(obj);
}


void _writeMap(json_t* map,ListeMap* li){
    ouvrirListeMap (li);
    while (!finListeMap (li)) {
        json_t *term1=json_object();
        json_t *term2=json_object();
        json_t *obj=json_object();
        objetMap *om = objetCourantMap(li);
        _writeTerm(term1,om->champ1);
        json_object_set(obj,"champ1",term1);
        _writeTerm(term2,om->champ2);
        json_object_set(obj,"champ2",term2);
        json_array_append(map,obj);
        json_decref(term1);
        json_decref(term2);
        json_decref(obj);
    }

}


void _writeList(json_t *json_list,ListeList *li){
    ouvrirListeList (li);
    while (!finListeList (li)) {
        json_t *json_term=json_object();
        term *t= objetCourantList(li);
        _writeTerm(json_term,t);
        json_array_append(json_list,json_term);
    }

}


void _writeTerm(json_t *json_term,term *term1){
    json_object_set_new(json_term,"type",json_integer(term1->type));
    json_t *obj=NULL;
    switch (term1->type){
        case map_term:
            obj=json_array();
            _writeMap(obj,term1->node.map);
            break;
        case set_term:
        case list_term:
            obj=json_array();
            _writeList(obj,term1->node.list);
            break;
        case constant_term:
            obj=json_object();
            _writeConstant(obj,term1->node.constant);
            break;
    }
    json_object_set(json_term,"node",obj);
    json_decref(obj);
}

void _writeConstant(json_t* json_constant,cqlConstant constant){
    json_object_set_new(json_constant,"type",json_integer(constant.type));
    json_t *obj=NULL;
    switch (constant.type){
        case uuid_const:
        case blob_const:
        case string_const:
            obj=json_string(constant.node.string);
            break;
        case boolean_const:
        case integer_const:
            obj=json_integer(constant.node.integer);
            break;
        case float_const:
            obj=json_real(constant.node._float);
            break;
        default:
            break;
    }
    json_object_set(json_constant,"node",obj);
    json_decref(obj);
}


void _writeArrColumn(char *pathTable,ListeColumn *li,ListeName *liPartitionKey,ListeName *liClustringOrder){
    json_t *obj = json_object();
    json_t *arr_column = json_array();
    ouvrirListeColumn (li);
    while (!finListeColumn (li)) {
        objetColumn *oc = objetCourantColumn(li);
        json_t *json_oc=json_object();
        _writeColumn(json_oc,oc);
        json_array_append(arr_column,json_oc);
        json_decref(json_oc);
    }
    json_object_set(obj,"column",arr_column);
    json_decref(arr_column);
    json_t *arr_partition = json_array();
    ouvrirListeName (liPartitionKey);
    while (!finListeName (liPartitionKey)) {
        char *name = objetCourantName(liPartitionKey);
        json_array_append_new(arr_partition,json_string(name));
    }
    json_object_set(obj,"partition",arr_partition);
    json_decref(arr_partition);
    json_t *arr_clustringOrder = json_array();
    ouvrirListeName (liClustringOrder);
    while (!finListeName (liClustringOrder)) {
        char *name = objetCourantName(liClustringOrder);
        json_array_append_new(arr_clustringOrder,json_string(name));
    }
    json_object_set(obj,"clustring",arr_clustringOrder);
    json_decref(arr_clustringOrder);
    mkdir(pathTable, S_IRWXU);
    char *pathColumn=(char *)malloc((strlen("/arr_column.json")+strlen(pathTable)+1)*sizeof(char));
    sprintf(pathColumn,"%s/arr_column.json",pathTable);
    json_dump_file(obj,pathColumn, JSON_INDENT(4));
    free(pathColumn);
    json_decref(arr_column);
}


void _writeColumn(json_t *json_oc,objetColumn *oc){
    json_object_set_new(json_oc,"name",json_string(oc->name));
    json_object_set_new(json_oc,"static",json_boolean(oc->_static));
    //json_object_set_new(json_oc,"clustringCol",json_boolean(oc->clusteringCol));
    json_object_set_new(json_oc,"partitionKey",json_boolean(oc->partitionKey));
    json_t *json_type=json_object();
    _writeType(json_type,oc->type);
    json_object_set(json_oc,"type",json_type);
    json_decref(json_type);
}


void _writeType(json_t *json_type,cql_type *type){
    json_object_set_new(json_type,"type",json_integer(type->type));
    json_t *obj=NULL;
    switch (type->type){
        case native_typeE:
            obj=json_integer(type->node.native);
            break;
        case map_typeE:
            obj=json_object();
            json_t* champ1=json_object();
            json_t* champ2=json_object();
            _writeType(champ1,type->node.map.champ1);
            _writeType(champ2,type->node.map.champ1);
            json_object_set(obj,"champ1",champ1);
            json_object_set(obj,"champ2",champ2);
            json_decref(champ1);
            json_decref(champ2);
            break;
        case set_typeE:
            obj=json_object();
            _writeType(obj,type->node.list.champ);
        case list_typeE:
            obj=json_object();
            _writeType(obj,type->node.list.champ);
            break;
        case udt_typeE:
            obj=json_object();
            _writeType(obj,type->node.list.champ);
            break;
        case tuple_typeE:
           obj=json_object();
            break;
        case custom_typeE:
            obj=json_string(type->node.costum.name);
            break;
    }
    json_object_set(json_type,"node",obj);
    json_decref(obj);
}



//gestion directory

void _dropDirectoryKeyspace(char *name){
    char * path=(char *)malloc((strlen(name)+strlen("./data/")+1)*sizeof(char));
    sprintf(path,"./data/%s",name);
    remove_directory(path);
    free(path);
}
void _dropDirectoryFichier(char *keyspace,char *table){
    int i;
    for(i=1;i<5;i++){
        char * path=(char *)malloc((strlen(keyspace)+strlen(table)+strlen("./data//node1data.json")+1)*sizeof(char));
        sprintf(path,"./data/%s/%s/node%ddata.json",keyspace,table,i);
        remove(path);
        free(path);
    }
}



void _dropDirectoryTable(char *keyspace,char *table){
    char * path=(char *)malloc((strlen(keyspace)+strlen(table)+strlen("./data//")+1)*sizeof(char));
    sprintf(path,"./data/%s/%s",keyspace,table);
    remove_directory(path);
    free(path);
}

int remove_directory(const char *path)
{
    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;
    if (d)
    {
        struct dirent *p;

        r = 0;

        while (!r && (p=readdir(d)))
        {
            int r2 = -1;
            char *buf;
            size_t len;

            /* Skip the names "." and ".." as we don't want to recurse on them. */
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
            {
                continue;
            }

            len = path_len + strlen(p->d_name) + 2;
            buf = malloc(len);

            if (buf)
            {
                struct stat statbuf;

                snprintf(buf, len, "%s/%s", path, p->d_name);

                if (!stat(buf, &statbuf))
                {
                    if (S_ISDIR(statbuf.st_mode))
                    {
                        r2 = remove_directory(buf);
                    }
                    else
                    {
                        r2 = unlink(buf);
                    }
                }

                free(buf);
            }

            r = r2;
        }

        closedir(d);
    }
    if (!r)
    {
        r = rmdir(path);
    }
    return r;
}



