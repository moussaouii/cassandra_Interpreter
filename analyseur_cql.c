#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include <sys/types.h>
#include "analyseur_cql.h"
#include "header_analyseur.h"
#include "semantique.h"
#include "list.h"
#include "error.h"
#include "structure.h"
#include "execute_stmt.h"
#include "persistance.h"


#define RESET   "\033[0m"

#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */

#define debug false


objetKeyspace *currentKeyspace=NULL;
stmt *currentStmt=NULL;
typetoken token; //token est un variable global contient le token courat
boolean follow_token=false ; // permet de gerer les sur lecture du token
boolean read_from_file=false;
ListeKey *liKey;

char *strlwr(char *str)
{
    if(*str=='"'){
        return str;
    }else{
        unsigned char *p = (unsigned char *)str;

        while (*p) {
            *p = tolower(*p);
            p++;
        }
    }
	return str;
}

void reintialseStmt(){
    currentStmt->type=default_stmt;
}

extern int yylex();
extern char *yytext;
extern int yylineno;
extern FILE* yyin;


void main_aux();

// _lire_token : permet de lire un token dans chaque appel
typetoken _lire_token(){
	typetoken temp;
	if (follow_token){
		follow_token = false;// RAZ du marqueur de sur-lecture
		return token; // renvoie du token déjà lu 
	}else{
		temp=(typetoken) yylex();
		if(temp==_NEW_LINE){
			token=_lire_token();
		}
	}
}

typetoken _lire_token_all(){
	typetoken temp;
	if (follow_token){
		follow_token = false;// RAZ du marqueur de sur-lecture
		return token; // renvoie du token déjà lu
	}else{
		return (typetoken) yylex();
	}
}


void printCqlsh(){

//    _afficherErreurs();
    if(currentKeyspace->name==NULL){
        printf("cqlsh>");
    }else{
        printf("cqlsh:%s>",currentKeyspace->name);
    }
	yylineno=1;
}



int main(int argc, char const *argv[]){
    char* s = NULL;
    liKey=_load();
    if(debug){listerListeKey(liKey,printKeyspace);}
    currentKeyspace=(objetKeyspace *)malloc(sizeof(objetKeyspace));
    currentKeyspace->name=NULL;
    currentStmt=(stmt *)malloc(sizeof(stmt));
    currentStmt->type=default_stmt;
    ++argv, --argc;
	if ( argc > 0 ){
		yyin = fopen( argv[0], "r" );
		read_from_file=true;

	}
	else{yyin = stdin;
        }

    main_aux();

	return 0;
}


void skip_all(){
	while(token!=_NEW_LINE && token!=_PVIRG){
		token=_lire_token_all();
	}
}


void main_aux(){
	if(!read_from_file){
		printCqlsh();
	}
	token=_lire_token();
	if(_statements()){
        _write(liKey);
		if(debug){listerListeKey(liKey,printKeyspace);};
	} else{
        if(read_from_file){
            printf(BOLDRED "SyntaxException: line: %d: no viable alternative at input '%s'\n"RESET,yylineno,yytext);
        }else{
            printf(BOLDRED "SyntaxException : no viable alternative at input '%s'\n"RESET,yytext);
        }
		skip_all();
        reintialseErreur();
		main_aux();
	}
}








/*****************************************Definitions****************************************
 identifier          ::=  unquoted_identifier | quoted_identifier | (non resirved words)  

constant ::=  string | integer | float | boolean | uuid | blob | NULL 

term          ::=  constant | literal | function_call | type_hint | bind_marker  
literal       ::=  collection_literal | udt_literal | tuple_literal
function_call ::=  identifier '(' [ term (',' term)* ] ')'
type_hint     ::=  '(' cql_type `)` term
bind_marker   ::=  '?' | ':' identifier

collection_literal ::=  map_literal | set_literal | list_literal
map_literal        ::=  '{' [ term ':' term (',' term : term)* ] '}'
set_literal        ::=  '{' [ term (',' term)* ] '}'
list_literal       ::=  '[' [ term (',' term)* ] ']'

udt_literal ::=  '{' identifier ':' term ( ',' identifier ':' term )* '}'

tuple_literal ::=  '(' term ( ',' term )* ')'


if_not_exists ::=  IF  NOT EXISTS 
if_exists ::= IF EXISTS  

*********************************************************************************************/

/******************************************************************************/
/*_idf() : _IDF_UNQUOTED | _IDF_QUOTED | _AGGREGATE | _ALL | _AS | _ASCII | 
_BIGINT | _BLOB | _BOOLEAN | _CALLED | _CLUSTERING | _COMPACT | _CONTAINS | 
_COUNTER | _CUSTOM | _DATE | _DECIMAL | _DISTINCT | _DOUBLE | _EXISTS | 
_FILTERING | _FINALFUNC | _FLOAT | _FROZEN | _FUNCTION | _FUNCTIONS | _INET | 
_INITCOND | _INPUT | _INT | _JSON | _KEY | _KEYS | _KEYSPACES | 
_LANGUAGE | _LIST | _LOGIN | _MAP | _NOLOGIN | _NOSUPERUSER | _OPTIONS | 
_PASSWORD | _PERMISSION | _PERMISSIONS | _RETURNS | _ROLE | _ROLES | _SFUNC | 
_SMALLINT | _STATIC | _STORAGE | _STYPE | _SUPERUSER | _TEXT | _TIME | 
_TIMESTAMP | _TIMEUUID | _TINYINT | _TRIGGER | _TTL | _TUPLE | _TYPE | 
_USER | _USERS | _UUID | _VALUES | _VARCHAR | _VARINT | _WRITETIME*/
/******************************************************************************/
boolean _idf(){
	boolean result;
	if(token==_IDF_UNQUOTED){
		result=true;
	}
	else if(token==_IDF_QUOTED){
		result=true;
	}
	else if(token==_AGGREGATE || token==_ALL || token==_AS || token==_ASCII || token==_BIGINT
		|| token==_BLOB || token==_BOOLEAN || token==_CALLED || token==_CLUSTERING
		|| token==_COMPACT || token==_CONTAINS || token==_COUNTER  || token==_CUSTOM
		|| token==_DATE || token==_DECIMAL || token==_DISTINCT || token==_DOUBLE || token==_DOUBLE
		|| token==_EXISTS || token==_FILTERING || token==_FINALFUNC || token==_FLOAT ||
		token==_FROZEN || token==_FUNCTION || token==_FUNCTIONS || token==_INET || token==_INITCOND
		|| token==_INPUT || token==_INT || token==_JSON || token==_KEY || token==_KEYS 
		|| token==_KEYSPACES || token==_LANGUAGE || token==_LIST || token==_LOGIN || token==_MAP
		|| token==_NOLOGIN || token==_NOSUPERUSER || token==_OPTIONS || token==_PASSWORD
		|| token==_PERMISSION || token==_PERMISSIONS || token==_RETURNS || token==_ROLE
		|| token==_ROLES || token==_SFUNC || token==_SMALLINT || token==_STATIC || 
		token==_STORAGE || token==_STYPE || token==_SUPERUSER || token==_TEXT 
		||token==_TIME || token==_TIMESTAMP || token==_TIMEUUID || token==_TINYINT
		|| token==_TRIGGER || token==_TTL || token==_TUPLE || token==_TYPE || token==_USER 
		|| token==_USERS || token==_UUID || token==_VALUES || token==_VARCHAR || token==_VARINT
		|| token==_WRITETIME){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_term() : _constant() | _literal() | _bind_marker() | _function_call()*/
/******************************************************************************/
boolean _term(term *t){
	boolean result;
	cqlConstant *cC=(cqlConstant *)malloc(sizeof(cqlConstant));
	if(_constant(cC)){
		t->type=constant_term;
		(t->node).constant=*cC;

		result=true;
	}
	else if(_literal(t)){
		result=true;
	}
	else if(_bind_marker()){
		result=true;
	}
	else if(_function_call()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_term_without_idf() : _constant() | _literal() | _bind_marker()*/
/******************************************************************************/
boolean _term_without_idf(){
	boolean result;
	if(_constant(NULL)){
		result=true;
	}
	else if(_literal(NULL)){
		result=true;
	}
	else if(_bind_marker()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_constant() : _STRING | _INTEGER | _FLOAT | _TRUE | _FALSE | _UUID | 
_BLOB | _NULL*/
/******************************************************************************/
boolean _constant(cqlConstant *cC){
	boolean result;
	if(token==_STRING){
		cC->type=string_const;
        if(debug){printf("befor :%s\n",yytext);}
		(cC->node).string=(char *)malloc(strlen(yytext)*sizeof(char)+1);
		strcpy((cC->node).string,yytext);
        if(debug){printf("after :%s\n",(cC->node).string);}
		result=true;
	}
	else if(token==_INTEGER){
		cC->type=integer_const;
		(cC->node).integer=atoi(yytext);
		result=true;
	}
	else if(token==_FLOAT){
		cC->type=float_const;
		(cC->node)._float=atof(yytext);
		result=true;
	}
	else if(token==_TRUE){
		cC->type=boolean_const;
        cC->node.integer=1;
		result=true;
	}
	else if(token==_FALSE){
		cC->type=boolean_const;
        cC->node.integer=0;
		result=true;
	}
	else if(token==_UUID){
		cC->type=uuid_const;
		(cC->node).string=(char *)malloc(strlen(yytext)*sizeof(char)+1);
		strcpy((cC->node).string,yytext);
		result=true;
	}
	else if(token==_BLOB){
		cC->type=blob_const;
		(cC->node).string=(char *)malloc(strlen(yytext)*sizeof(char)+1);
		strcpy((cC->node).string,yytext);
		result=true;
	}
	else if(token==_NULL){
		cC->type=null_const;
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_literal() : _map_set_udt() | _list() | _hint_type_tuple()*/
/******************************************************************************/
boolean _literal(term *t){
	boolean result;
	ListeList *LL=creerListeList(NULL,NULL);
	if(_map_set_udt(t)){
		result=true;
	}
	else if(_list(LL)){
		t->type=list_term;
		(t->node).list=LL;
		result=true;
	}
	else if(_hint_type_tuple(t)){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_bind_marker() : _PINTERR | _DEUXP _idf()*/
/******************************************************************************/
boolean _bind_marker(){
	boolean result;
	if(token==_PINTERR){
		result=true;
	}
	else if(token==_DEUXP){
		token=_lire_token();
		if(_idf()){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/* function_call ::= identifier '(' [ term (',' term)* ] ')' 
_est exprimee a l'aide de 3 fonctions :
_function_call() , _terms_func_call() , _terms_func_call_aux() */

/******************************************************************************/
/*_function_call() : _idf() _POPEN ( _term() _terms_func_call() _PCLOSE )*/
/******************************************************************************/
boolean _function_call(){
	boolean result;
	if(_idf()){
		token=_lire_token();
		if(token==_POPEN){
			token=_lire_token();
			if(_term(NULL)){
				token=_lire_token();
				if(_terms_func_call()){
					token=_lire_token();
					if(token==_PCLOSE){
						result=true;
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_terms_func_call() :  follow( _PCLOSE ) | _terms_func_call_aux()*/
/******************************************************************************/
boolean _terms_func_call(){
	boolean result;
	if(token==_PCLOSE){
		follow_token = true;
		result=true;
	}
	else if(_terms_func_call_aux()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_terms_func_call_aux() : _VIRG _term() _terms_func_call()*/
/******************************************************************************/
boolean _terms_func_call_aux(){
	boolean result;
	if(token==_VIRG){
		token=_lire_token();
		if(_term(NULL)){
			token=_lire_token();
			if(_terms_func_call()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/* pour rendre le grammaire LL1 on factorise map, set et udt */

/******************************************************************************/
/*_map_set_udt() : _AOPEN ( _ACLOSE | _idf() _DEUXP _term() _udt_literal() _ACLOSE | 
_term() ( _DEUXP _term() _terms_map() _ACLOSE | _VIRG _term() _terms_set() _ACLOSE | 
_ACLOSE ) )*/
/******************************************************************************/
boolean _map_set_udt(term *t){
	boolean result;
	term *t1=(term*)malloc(sizeof(term));
	term *t2=(term*)malloc(sizeof(term));
	ListeList *LL=creerListeList(NULL,NULL);
	ListeMap *LM=creerListeMap(NULL,NULL);
	objetMap* oM=(objetMap *)malloc(sizeof(objetMap));
	if(token==_AOPEN){
		token=_lire_token();
		if(token==_ACLOSE){
			result=true;
		}
		else if(_idf()){
			token=_lire_token();
			if(token==_DEUXP){
				token=_lire_token();
				if(_term(NULL)){
					token=_lire_token();
					if(_udt_literal()){
						token=_lire_token();
						if(token==_ACLOSE){
							result=true;
						}
						else{
							result=false;
						}
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else if(_term(t1)){
			token=_lire_token();
			if(token==_DEUXP){
				token=_lire_token();
				if(_term(t2)){
					oM->champ1=t1;
					oM->champ2=t2;
					insererEnFinDeListeMap(LM,oM);
					token=_lire_token();
					if(_terms_map(LM)){
						t->type=map_term;
						(t->node).map=LM;
						token=_lire_token();
						if(token==_ACLOSE){
							result=true;
						}
						else{
							result=false;
						}
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else if(token==_VIRG){
				token=_lire_token();
				if(_term(t2)){
					insererEnFinDeListeList(LL,t1);
					insererEnFinDeListeList(LL,t2);
					token=_lire_token();
					if(_terms_set(LL)){
						t->type=set_term;
						(t->node).list=LL;
						token=_lire_token();
						if(token==_ACLOSE){
							result=true;
						}
						else{
							result=false;
						}
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else if(token==_ACLOSE){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/*_udt_literal() ::=  (',' term ':' term)* 
map est exprimee a l'aide de 2 fonctions :
_terms_map() , _terms_map_aux()*/

/******************************************************************************/
/*_udt_literal() :  follow( _ACLOSE ) | _udt_literal_aux()*/
/******************************************************************************/
boolean _udt_literal(){
	boolean result;
	if(token==_ACLOSE){
		follow_token = true;
		result=true;
	}
	else if(_udt_literal_aux()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_udt_literal_aux() : _VIRG _idf() _DEUXP _term() _udt_literal()*/
/******************************************************************************/
boolean _udt_literal_aux(){
	boolean result;
	if(token==_VIRG){
		token=_lire_token();
		if(_idf()){
			token=_lire_token();
			if(token==_DEUXP){
				token=_lire_token();
				if(_term(NULL)){
					token=_lire_token();
					if(_udt_literal()){
						result=true;
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/*_terms_map() ::=  (',' term ':' term)* 
map est exprimee a l'aide de 2 fonctions :
_terms_map() , _terms_map_aux()*/

/******************************************************************************/
/*_terms_map() :  follow( _ACLOSE ) | _terms_map_aux()*/
/******************************************************************************/
boolean _terms_map(ListeMap* li){
	boolean result;
	if(token==_ACLOSE){
		follow_token = true;
		result=true;
	}
	else if(_terms_map_aux(li)){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_terms_map_aux() : _VIRG _term() _DEUXP _term() _terms_map()*/
/******************************************************************************/
boolean _terms_map_aux(ListeMap* li){
	boolean result;
	objetMap *oM=(objetMap *)malloc(sizeof(objetMap));
	term *t1=(term *)malloc(sizeof(term));
	term *t2=(term *)malloc(sizeof(term));
	if(token==_VIRG){
		token=_lire_token();
		if(_term(t1)){
			oM->champ1=t1;
			token=_lire_token();
			if(token==_DEUXP){
				token=_lire_token();
				if(_term(t2)){
					oM->champ2=t2;
					insererEnFinDeListeMap(li,oM);
					token=_lire_token();
					if(_terms_map(li)){
						result=true;
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/*_terms_set::= (',' term)* 
_terms_set() est exprimee a l'aide de 2 fonctions :
 _terms_set() , _terms_set_aux()*/

/******************************************************************************/
/*_terms_set() :  follow( _ACLOSE ) | _terms_set_aux()*/
/******************************************************************************/
boolean _terms_set(ListeList *li){
	boolean result;
	if(token==_ACLOSE){
		follow_token = true;
		result=true;
	}
	else if(_terms_set_aux(li)){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_terms_set_aux() : _VIRG _term() _terms_set()*/
/******************************************************************************/
boolean _terms_set_aux(ListeList *li){
	boolean result;
	term *t=(term *)malloc(sizeof(term));
	if(token==_VIRG){
		token=_lire_token();
		if(_term(t)){
			insererEnFinDeListeList(li,t);
			token=_lire_token();
			if(_terms_set(li)){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/*list ::= '[' (term (',' term*)? ']'
list est exprimee a l'aide de 3 fonctions :
_list() , _terms_list() , _terms_list_aux()*/

/******************************************************************************/
/*_list() : _COPEN ( _term() _terms_list() _CCLOSE | _CCLOSE )*/
/******************************************************************************/
boolean _list(ListeList *LL){
	boolean result;
	term *t=(term *)malloc(sizeof(term));
	//objetList* oL=(objetList *)malloc(sizeof(objetList));
	if(token==_COPEN){
		token=_lire_token();
		if(_term(t)){
			//oL->champ=t;
			insererEnFinDeListeList(LL,t);
			token=_lire_token();
			if(_terms_list(LL)){
				token=_lire_token();
				if(token==_CCLOSE){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else if(token==_CCLOSE){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_terms_list() :  follow( _CCLOSE ) | _terms_list_aux()*/
/******************************************************************************/
boolean _terms_list(ListeList *LL){
	boolean result;
	if(token==_CCLOSE){
		follow_token = true;
		result=true;
	}
	else if(_terms_list_aux(LL)){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_terms_list_aux() : _VIRG _term() _terms_list()*/
/******************************************************************************/
boolean _terms_list_aux(ListeList *LL){
	boolean result;
	term *t=(term *)malloc(sizeof(term));
	//objetList* oL=(objetList *)malloc(sizeof(objetList));
	if(token==_VIRG){
		token=_lire_token();
		if(_term(t)){
			//oL->champ=t;
			insererEnFinDeListeList(LL,t);
			token=_lire_token();
			if(_terms_list(LL)){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/* pour rendre le grammaire LL1 on factorise literal_tuple et  _hint_type */

/******************************************************************************/
/*_hint_type_tuple() : _POPEN ( _term() _tuple_literal() _PCLOSE | _type() _PCLOSE _term() )
*/
/******************************************************************************/
boolean _hint_type_tuple(term *t){
    t->type=tuple_term;
	boolean result;
	if(token==_POPEN){
        t->node.list=creerListeList(NULL,NULL);
		token=_lire_token();
        term *t=(term*)malloc(sizeof(term));
		if(_term(t)){
            insererEnFinDeListeList(t->node.list,t);
			token=_lire_token();
			if(_tuple_literal(t->node.list)){
				token=_lire_token();
				if(token==_PCLOSE){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else if(_type(NULL)){
			token=_lire_token();
			if(token==_PCLOSE){
				token=_lire_token();
				if(_term(NULL)){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_tuple_literal() :  follow( _PCLOSE ) | _tuple_literal_aux()*/
/******************************************************************************/
boolean _tuple_literal(ListeList *li){
	boolean result;
	if(token==_PCLOSE){
		follow_token = true;
		result=true;
	}
	else if(_tuple_literal_aux(li)){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_tuple_literal_aux() : _VIRG _term() _tuple_literal()*/
/******************************************************************************/
boolean _tuple_literal_aux(ListeList *li){
	boolean result;
	if(token==_VIRG){
		token=_lire_token();
        term *t=(term *)malloc(sizeof(term));
		if(_term(t)){
            insererEnFinDeListeList(li,t);
			token=_lire_token();
			if(_tuple_literal(li)){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_tuple_literal_main() : _POPEN _term() _tuple_literal() _PCLOSE*/
/******************************************************************************/
boolean _tuple_literal_main(ListeList *li){
	boolean result;
	if(token==_POPEN){
		token=_lire_token();
        term *t=(term *)malloc(sizeof(term));
		if(_term(t)){
            insererEnFinDeListeList(li,t);
			token=_lire_token();
			if(_tuple_literal(li)){
				token=_lire_token();
				if(token==_PCLOSE){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_if_not_exists() : _IF _NOT _EXISTS*/
/******************************************************************************/
boolean _if_not_exists(){
	boolean result;
	if(token==_IF){
		token=_lire_token();
		if(token==_NOT){
			token=_lire_token();
			if(token==_EXISTS){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_if_exists() : _IF _EXISTS*/
/******************************************************************************/
boolean _if_exists(){
	boolean result;
	if(token==_IF){
		token=_lire_token();
		if(token==_EXISTS){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/*****************************************Data Types****************************************
cql_type ::=  native_type | collection_type | user_defined_type | tuple_type | custom_type

native_type ::=  ASCII
                 | BIGINT
                 | BLOB
                 | BOOLEAN
                 | COUNTER
                 | DATE
                 | DECIMAL
                 | DOUBLE
                 | FLOAT
                 | INET
                 | INT
                 | SMALLINT
                 | TEXT
                 | TIME
                 | TIMESTAMP
                 | TIMEUUID
                 | TINYINT
                 | UUID
                 | VARCHAR
                 | VARINT

collection_type ::=  MAP '<' cql_type ',' cql_type '>'
                     | SET '<' cql_type '>'
                     | LIST '<' cql_type '>'

user_defined_type ::=  udt_name
udt_name          ::=  [ keyspace_name '.' ] identifier

create_type_statement ::=  CREATE TYPE [ IF NOT EXISTS ] udt_name
                               '(' field_definition ( ',' field_definition )* ')'
field_definition      ::=  identifier cql_type

alter_type_statement    ::=  ALTER TYPE udt_name alter_type_modification
alter_type_modification ::=  ALTER identifier TYPE cql_type
                             | ADD field_definition
                             | RENAME identifier TO identifier ( identifier TO identifier )*

drop_type_statement ::=  DROP TYPE [ IF EXISTS ] udt_name

tuple_type    ::=  TUPLE '<' cql_type ( ',' cql_type )* '>'

custom_type ::=  string

*********************************************************************************************/

/******************************************************************************/
/*_type() : _native_type() | _tuple_type() | _collection_type() | _user_defined_type() | _STRING*/
/******************************************************************************/
boolean _type(cql_type *cT){
	boolean result;
	if(_native_type(cT)){
		result=true;
	}
	else if(_tuple_type()){
		result=true;
	}
	else if(_collection_type(cT)){
		result=true;
	}
	else if(_user_defined_type()){
		result=true;
	}
	else if(token==_STRING){
        cT->type=custom_typeE;
        cT->node.costum.name=(char *)malloc(strlen(yytext)*sizeof(char)+1);
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_native_type() : _ASCII | _BIGINT | T_BLOB | _BOOLEAN | _COUNTER | _DATE | 
_DECIMAL | _DOUBLE | T_FLOAT | _INET | _INT | _SMALLINT | _TEXT | 
_TIME | _TIMESTAMP | _TIMEUUID | _TINYINT | T_UUID | _VARCHAR | _VARINT*/
/******************************************************************************/
boolean _native_type(cql_type *cT){
	boolean result;
    cT->type=native_typeE;
	if(token==_ASCII){
        cT->node.native=ascii_native;
		result=true;
	}
	else if(token==_BIGINT){
        cT->node.native=bigint_native;
		result=true;
	}
	else if(token==T_BLOB){
        cT->node.native=blob_native;
		result=true;
	}
	else if(token==_BOOLEAN){
        cT->node.native=boolean_native;
		result=true;
	}
	else if(token==_COUNTER){
        cT->node.native=counter_native;
		result=true;
	}
	else if(token==_DATE){
        cT->node.native=date_native;
		result=true;
	}
	else if(token==_DECIMAL){
        cT->node.native=decimal_native;
		result=true;
	}
	else if(token==_DOUBLE){
        cT->node.native=double_native;
		result=true;
	}
	else if(token==T_FLOAT){
        cT->node.native=float_native;
		result=true;
	}
	else if(token==_INET){
        cT->node.native=inet_native;
		result=true;
	}
	else if(token==_INT){
        cT->node.native=int_native;
		result=true;
	}
	else if(token==_SMALLINT){
        cT->node.native=smallint_native;
		result=true;
	}
	else if(token==_TEXT){
        cT->node.native=text_native;
		result=true;
	}
	else if(token==_TIME){
        cT->node.native=time_native;
		result=true;
	}
	else if(token==_TIMESTAMP){
        cT->node.native=timestamp_native;
		result=true;
	}
	else if(token==_TIMEUUID){
        cT->node.native=timeuuid_native;
		result=true;
	}
	else if(token==_TINYINT){
        cT->node.native=tinyint_native;
		result=true;
	}
	else if(token==T_UUID){
        cT->node.native=uuid_native;
		result=true;
	}
	else if(token==_VARCHAR){
        cT->node.native=varchar_native;
		result=true;
	}
	else if(token==_VARINT){
        cT->node.native=varint_native;
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_collection_type() : _LIST _LESS _type() _GREATER | _SET _LESS _type() _GREATER |
_MAP _LESS _type() _VIRG _type() _GREATER*/
/******************************************************************************/
boolean _collection_type(cql_type *ct){
	boolean result;
	if(token==_LIST){
        ct->type=list_typeE;
		token=_lire_token();
		if(token==_LESS){
			token=_lire_token();
            ct->node.list.champ=(cql_type *)malloc(sizeof(cql_type));
			if(_type(ct->node.list.champ)){
				token=_lire_token();
				if(token==_GREATER){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else if(token==_SET){
        ct->type=set_typeE;
		token=_lire_token();
		if(token==_LESS){
			token=_lire_token();
            ct->node.list.champ=(cql_type *)malloc(sizeof(cql_type));
			if(_type(ct->node.list.champ)){
				token=_lire_token();
				if(token==_GREATER){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else if(token==_MAP){
        ct->type=map_typeE;
		token=_lire_token();
		if(token==_LESS){
			token=_lire_token();
            ct->node.map.champ1=(cql_type *)malloc(sizeof(cql_type));
			if(_type(ct->node.map.champ1)){
				token=_lire_token();
				if(token==_VIRG){
					token=_lire_token();
                    ct->node.map.champ2=(cql_type *)malloc(sizeof(cql_type));
					if(_type(ct->node.map.champ2)){
						token=_lire_token();
						if(token==_GREATER){
							result=true;
						}
						else{
							result=false;
						}
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/*tuple-type : tuple '<' type (',' type)* '>'
tuple est exprimee a l'aide de 3 fonctions :
_tuple_type() , _types_tuple() , _types_tuple_aux()*/

/******************************************************************************/
/*_tuple_type() : _TUPLE _LESS _type() _types_tuple() _GREATER*/
/******************************************************************************/
boolean _tuple_type(){
	boolean result;
	if(token==_TUPLE){
		token=_lire_token();
		if(token==_LESS){
			token=_lire_token();
			if(_type(NULL)){
				token=_lire_token();
				if(_types_tuple()){
					token=_lire_token();
					if(token==_GREATER){
						result=true;
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_types_tuple() :  follow( _GREATER ) | _types_tuple_aux()*/
/******************************************************************************/
boolean _types_tuple(){
	boolean result;
	if(token==_GREATER){
		follow_token = true;
		result=true;
	}
	else if(_types_tuple_aux()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_types_tuple_aux() : _VIRG _type() _types_tuple()*/
/******************************************************************************/
boolean _types_tuple_aux(){
	boolean result;
	if(token==_VIRG){
		token=_lire_token();
		if(_type(NULL)){
			token=_lire_token();
			if(_types_tuple()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_user_defined_type() : _idf() (  follow( _PVIRG , _ALTER , _POPEN , _ADD , _RENAME )
 | _POINT _idf() )*/
/******************************************************************************/
boolean _user_defined_type(){
	boolean result;
	if(_idf()){
		token=_lire_token();
		if(token==_PVIRG || token==_ALTER || token==_POPEN || token==_ADD || token==_RENAME){
			follow_token = true;
			result=true;
		}
		else if(token==_POINT){
			token=_lire_token();
			if(_idf()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_create_type_stmt() : _if_not_exists() _create_type_stmt_aux() | _create_type_stmt_aux()*/
/******************************************************************************/
boolean _create_type_stmt(){
	boolean result;
	if(_if_not_exists()){
		token=_lire_token();
		if(_create_type_stmt_aux()){
			result=true;
		}
		else{
			result=false;
		}
	}
	else if(_create_type_stmt_aux()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_create_type_stmt_aux() : _user_defined_type() _POPEN _field_definition() _field_definitions() _PCLOSE*/
/******************************************************************************/
boolean _create_type_stmt_aux(){
	boolean result;
	if(_user_defined_type()){
		token=_lire_token();
		if(token==_POPEN){
			token=_lire_token();
			if(_field_definition()){
				token=_lire_token();
				if(_field_definitions()){
					token=_lire_token();
					if(token==_PCLOSE){
						result=true;
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/*( ',' field_definition )*
 est exprimee a l'aide de 2 fonctions : 
_field_definitions() _field_definitions_aux() */

/******************************************************************************/
/*_field_definitions() :  follow( _PCLOSE ) | _field_definitions_aux()*/
/******************************************************************************/
boolean _field_definitions(){
	boolean result;
	if(token==_PCLOSE){
		follow_token = true;
		result=true;
	}
	else if(_field_definitions_aux()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_field_definitions_aux() : _VIRG _field_definition() _field_definitions()*/
/******************************************************************************/
boolean _field_definitions_aux(){
	boolean result;
	if(token==_VIRG){
		token=_lire_token();
		if(_field_definition()){
			token=_lire_token();
			if(_field_definitions()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_field_definition() : _idf() _type()*/
/******************************************************************************/
boolean _field_definition(){
	boolean result;
	if(_idf()){
		token=_lire_token();
		if(_type(NULL)){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_alter_type_stmt() : _user_defined_type() _alter_type_modification()*/
/******************************************************************************/
boolean _alter_type_stmt(){
	boolean result;
	if(_user_defined_type()){
		token=_lire_token();
		if(_alter_type_modification()){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_alter_type_modification() : _ALTER _idf() _TYPE _type() | _ADD _field_definition() | 
_RENAME _idf() _TO _idf() _list_idf_to()*/
/******************************************************************************/
boolean _alter_type_modification(){
	boolean result;
	if(token==_ALTER){
		token=_lire_token();
		if(_idf()){
			token=_lire_token();
			if(token==_TYPE){
				token=_lire_token();
				if(_type(NULL)){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else if(token==_ADD){
		token=_lire_token();
		if(_field_definition()){
			result=true;
		}
		else{
			result=false;
		}
	}
	else if(token==_RENAME){
		token=_lire_token();
		if(_idf()){
			token=_lire_token();
			if(token==_TO){
				token=_lire_token();
				if(_idf()){
					token=_lire_token();
					if(_list_idf_to()){
						result=true;
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/* ( identifier TO identifier )* 
est exprimee a l'aide de 2 fonctions :
_list_idf_to() _list_idf_to_aux()*/

/******************************************************************************/
/*_list_idf_to() :  follow( _PVIRG ) | _list_idf_to_aux()*/
/******************************************************************************/
boolean _list_idf_to(){
	boolean result;
	if(token==_PVIRG){
		follow_token = true;
		result=true;
	}
	else if(_list_idf_to_aux()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_list_idf_to_aux() : _idf() _TO _idf() _list_idf_to()*/
/******************************************************************************/
boolean _list_idf_to_aux(){
	boolean result;
	if(_idf()){
		token=_lire_token();
		if(token==_TO){
			token=_lire_token();
			if(_idf()){
				token=_lire_token();
				if(_list_idf_to()){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_drop_type_stmt() : _if_exists() _user_defined_type() | _user_defined_type()*/
/******************************************************************************/
boolean _drop_type_stmt(){
	boolean result;
	if(_if_exists()){
		token=_lire_token();
		if(_user_defined_type()){
			result=true;
		}
		else{
			result=false;
		}
	}
	else if(_user_defined_type()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/***********************************Data Definition****************************************
keyspace_name ::=  name
table_name    ::=  [ keyspace_name '.' ] name
name          ::=  unquoted_name | quoted_name
unquoted_name ::=  re('[a-zA-Z_0-9]{1, 48}')
quoted_name   ::=  doublequote unquoted_name doublequote
column_name ::=  identifier
options ::=  option ( AND option )*
option  ::=  identifier '=' ( identifier | constant | map_literal )
********************************************************************************************/



/*table_name  ou key_space_name.table_name */

/******************************************************************************/
/*_table_name() : _idf() (  follow( _POPEN , _USING , _PVIRG , _ALTER , _ADD , _DROP , 
_WITH , _WHERE , _TO , _FROM , _JSON , _OF , _USING , _SET )
 | _POINT _idf() )*/
/******************************************************************************/
boolean _table_name(tableName *tN){
	boolean result;
    char *name1=NULL;
	if(_idf()){
        name1=(char *)malloc(strlen(yytext)*sizeof(char)+1);
        strcpy(name1,strlwr(yytext));
		token=_lire_token();
		if(token==_POPEN || token==_USING || token==_PVIRG || token==_ALTER || token==_ADD
			|| token==_DROP || token==_WITH || token==_WHERE || token==_TO || token==_FROM
			|| token==_JSON || token==_OF || token==_USING || token==_SET || token==_PVIRG){
            tN->nameTable=(char *)malloc(strlen(name1)*sizeof(char)+1);
            strcpy(tN->nameTable,name1);
			follow_token = true;
			result=true;
		}
		else if(token==_POINT){
			token=_lire_token();
			if(_idf()){
                tN->nameTable=(char *)malloc(strlen(yytext)*sizeof(char)+1);
                strcpy(tN->nameTable,strlwr(yytext));
                tN->nameKeyspace=(char *)malloc(strlen(name1)*sizeof(char)+1);
                strcpy(tN->nameKeyspace,name1);
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}


/******************************************************************************/
/*_option() : _idf() _EQ ( _idf() | _constant() | _AOPEN _term() ( _DEUXP _term()
 _terms_map() _ACLOSE | _ACLOSE ) )*/
/******************************************************************************/
boolean _option(optionKeyspace *oks){
	boolean result;
	cqlConstant *cC=(cqlConstant *)malloc(sizeof(cqlConstant));
	ListeMap *LM;
	term *t1;
	term *t2;
	objetMap *oM;
	if(_idf()){
		if(debug){printf("%s\n",yytext);}
		oks->idf=(char *)malloc(strlen(yytext)*sizeof(char)+1);
		strcpy(oks->idf,strlwr(yytext));
		token=_lire_token();
		if(token==_EQ){
			token=_lire_token();
			if(_idf()){
				oks->type=idf_opt;
				(oks->node).idf_value=(char *)malloc(strlen(yytext)*sizeof(char)+1);
				strcpy((oks->node).idf_value,strlwr(yytext));
				result=true;
			}
			else if(_constant(cC)){
				oks->type=constant_opt;
				(oks->node).constant=*cC;
				result=true;
			}
			else if(token==_AOPEN){
				if(debug){printf("aopen\n");}
                LM=creerListeMap(NULL,NULL);
                oM=(objetMap *)calloc(1,sizeof(objetMap));
                t2=(term *)malloc(sizeof(term));
                t1=(term *)malloc(sizeof(term));
				oks->type=map_opt;
				token=_lire_token();
				if(_term(t1)){
					token=_lire_token();
					oM->champ1=t1;
					if(token==_DEUXP){
						token=_lire_token();
						if(_term(t2)){
							token=_lire_token();
							oM->champ2=t2;

							insererEnFinDeListeMap(LM,oM);

							if(_terms_map(LM)){
								token=_lire_token();
								if(token==_ACLOSE){
									(oks->node).map=LM;
									result=true;
								}
								else{
									result=false;
								}
							}
							else{
								result=false;
							}
						}
						else{
							result=false;
						}
					}
					else if(token==_ACLOSE){
						result=true;
					}
					else{
						result=false;
					}
				}else if(token==_ACLOSE){
					if(debug){printf("ACLOSE");}
					(oks->node).map=LM;
                    result=true;
                }
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}
/* options : option (AND option)*
options est exprime a l'aide de 3 fonctions :
_options() _list_options() _list_options_aux() */

/******************************************************************************/
/*_options() : _option() _list_options()*/
/******************************************************************************/
boolean _options(ListeOptionKeyspace *li){
	boolean result;
	optionKeyspace *oK=(optionKeyspace *)malloc(sizeof(optionKeyspace));
	if(_option(oK)){
		insererEnFinDeListeOptionKeyspace(li,oK);
		token=_lire_token();
		if(_list_options(li)){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_list_options() :  follow( _PVIRG ) | _list_options_aux()*/
/******************************************************************************/
boolean _list_options(ListeOptionKeyspace *li){
	boolean result;
	if(token==_PVIRG){
		follow_token = true;
		result=true;
	}
	else if(_list_options_aux(li)){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_list_options_aux() : _AND _option() _list_options()*/
/******************************************************************************/
boolean _list_options_aux(ListeOptionKeyspace *li){
	boolean result;
	optionKeyspace *oK=(optionKeyspace *)malloc(sizeof(optionKeyspace));
	if(token==_AND){
		token=_lire_token();
		if(_option(oK)){
			insererEnFinDeListeOptionKeyspace(li,oK);
			token=_lire_token();
			if(_list_options(li)){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/***********************************Data Definition:KEYSPACE**********************************
create_keyspace_statement ::=  CREATE KEYSPACE [ IF NOT EXISTS ] keyspace_name WITH options

use_statement ::=  USE keyspace_name

alter_keyspace_statement ::=  ALTER KEYSPACE keyspace_name WITH options

drop_keyspace_statement ::=  DROP KEYSPACE [ IF EXISTS ] keyspace_name
*********************************************************************************************/

boolean _create_keyspace_stmt_aux(boolean ifNotExist){
	boolean result;
    currentStmt->type=create_keyspace;
	//createKeyspaceStmt *cKS=(createKeyspaceStmt *)malloc(sizeof(createKeyspaceStmt));
	if(_idf()){
        currentStmt->node.createKeyspace.liKey=liKey;
		currentStmt->node.createKeyspace.name=(char *)malloc(strlen(yytext)*sizeof(char)+1);
		strcpy(currentStmt->node.createKeyspace.name,strlwr(yytext));
		objetKeyspace *objetChercheKey=_createKeyspace(strlwr(yytext));
		objetKeyspace *objetTrouveKey=chercherUnobjetKeyspace(liKey,objetChercheKey);
		free(objetChercheKey);
		if(objetTrouveKey!=NULL){
			if(!ifNotExist){
				_creerErreur(AlreadyExists,yylineno,strlwr(yytext),"keyspace");
			}
		}
		token=_lire_token();
		if(token==_WITH){
			currentStmt->node.createKeyspace.liOK=creerListeOptionKeyspace(NULL,NULL);
			token=_lire_token();
			if(_options(currentStmt->node.createKeyspace.liOK)){
				result=true;
                if(_valide_option_keyspace_create(currentStmt->node.createKeyspace.liOK,currentStmt->node.createKeyspace.name,yylineno)){
//					if(objetTrouveKey==NULL){
//						_createKeyspaceStmt(liKey,cKS);
//					}

                }
                else{

                }

			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

boolean _create_keyspace_stmt(){
	boolean result;
	if(_if_not_exists()){
		token=_lire_token();
		if(_create_keyspace_stmt_aux(true)){
			result=true;
		}
		else{
			result=false;
		}
	}
	else if(_create_keyspace_stmt_aux(false)){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_use_stmt() : _USE _idf()*/
/******************************************************************************/
boolean _use_stmt(){
	boolean result;
    currentStmt->type=use_keyspace;
	if(token==_USE){
		token=_lire_token();
		if(_idf()){
            objetKeyspace *objetChercheKey=_createKeyspace(strlwr(yytext));
            objetKeyspace *objetTrouveKey=chercherUnobjetKeyspace(liKey,objetChercheKey);
            free(objetChercheKey);
            if(objetTrouveKey!=NULL){
                currentStmt->node.useKeyspace.currentKeyspace=currentKeyspace;
                currentStmt->node.useKeyspace.useKeyspace=objetTrouveKey;
            }
            else{
                char *msg=malloc((strlen(yytext)+150)*sizeof(char));
                sprintf(msg,"Keyspace %s does not exist",strlwr(yytext));
                _creerErreur(InvalidRequest,yylineno,NULL,msg);
            }
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_alter_keyspace_stmt() : _idf() _WITH _options()*/
/******************************************************************************/
boolean _alter_keyspace_stmt(){
	boolean result;
    currentStmt->type=alter_keyspace;
	if(_idf()){
        objetKeyspace *objetChercheKey=_createKeyspace(strlwr(yytext));
        objetKeyspace *objetTrouveKey=chercherUnobjetKeyspace(liKey,objetChercheKey);
        currentStmt->node.alterKeyspace.key=objetTrouveKey;
        free(objetChercheKey);
        if(objetTrouveKey==NULL){
            char *msg=malloc((strlen(yytext)+150)*sizeof(char));
            sprintf(msg,"Keyspace %s does not exist",strlwr(yytext));
            _creerErreur(InvalidRequest,yylineno,NULL,msg);
        }
		token=_lire_token();
		if(token==_WITH){
            currentStmt->node.alterKeyspace.liOK= creerListeOptionKeyspace(NULL,NULL);
			token=_lire_token();
			if(_options(currentStmt->node.alterKeyspace.liOK)){
                if(objetTrouveKey!=NULL){
                    if(_valide_option_keyspace_alter(currentStmt->node.alterKeyspace.liOK,currentStmt->node.alterKeyspace.key->name,yylineno)){
                       // _alterKeyspaceStmt(aKS);
                    }else{
                       // printf("false\n");
                    }
                }
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_drop_keyspace_stmt() : _if_exists() _idf() | _idf()*/
/******************************************************************************/
boolean _drop_keyspace_stmt(){
	boolean result;
    currentStmt->type=drop_keyspace;
    //dropKeyspaceStmt *dKS=(dropKeyspaceStmt *)malloc(sizeof(dropKeyspaceStmt));
	if(_if_exists()){
		token=_lire_token();
		if(_idf()){
            objetKeyspace *objetChercheKey=_createKeyspace(strlwr(yytext));
            objetKeyspace *objetTrouveKey=chercherUnobjetKeyspace(liKey,objetChercheKey);
            currentStmt->node.dropKeyspace.key=objetTrouveKey;
            currentStmt->node.dropKeyspace.likey=liKey;
            free(objetChercheKey);
//            if(objetTrouveKey!=NULL){
//                _dropKeyspaceStmt(liKey,dKS);
//            }
			result=true;
		}
		else{
			result=false;
		}
	}
	else if(_idf()){
        objetKeyspace *objetChercheKey=_createKeyspace(strlwr(yytext));
        objetKeyspace *objetTrouveKey=chercherUnobjetKeyspace(liKey,objetChercheKey);
        currentStmt->node.dropKeyspace.key=objetTrouveKey;
        currentStmt->node.dropKeyspace.likey=liKey;
        free(objetChercheKey);
        if(objetTrouveKey==NULL){
            char *msg=malloc((strlen(yytext)+150)*sizeof(char));
            sprintf(msg,"Keyspace %s does not exist",strlwr(yytext));
            _creerErreur(InvalidRequest,yylineno,NULL,msg);
        }
//        else{
//            _dropKeyspaceStmt(liKey,dKS);
//        }
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

boolean _descripe(){
	boolean result;
	if(token==_DESCRIBE){
		token=_lire_token();
		if(token==_TABLES){
            currentStmt->type=describe_table;
            if(currentKeyspace->name==NULL){
                currentStmt->node.describeTable.li=liKey;
                currentStmt->node.describeTable.objetKey=NULL;
            }else{
                currentStmt->node.describeTable.li=NULL;
                currentStmt->node.describeTable.objetKey=currentKeyspace;
            }
            result=true;
		}else if(token==_KEYSPACES){
            currentStmt->type=descripe_keyspace;
            currentStmt->node.descripeKeyspace.liKey=liKey;
            result=true;
		}
	}
	else{
		result=false;
	}
	return result;
}

//boolean _descripe_tables(){
//	boolean result;
//
//	if(token==_TABLES){
//        currentStmt->type=describe_table;
//		if(currentKeyspace->name==NULL){
//			currentStmt->node.describeTable.li=liKey;
//			currentStmt->node.describeTable.objetKey=NULL;
//		}else{
//			currentStmt->node.describeTable.li=NULL;
//			currentStmt->node.describeTable.objetKey=currentKeyspace;
//		}
//		result=true;
//	}
//	else{
//		result=false;
//	}
//	return result;
//}
//
//boolean _descripe_stmt(){
//    boolean result;
//
//        if(token==_KEYSPACES){
//            currentStmt->type=descripe_keyspace;
//            currentStmt->node.descripeKeyspace.liKey=liKey;
//            result=true;
//        }
//        else{
//            result=false;
//        }
//    return result;
//}
/*****************************Data Definition:CREATE TABLE**********************************
create_table_statement ::=  CREATE TABLE [ IF NOT EXISTS ] table_name
                            '('
                                column_definition
                                ( ',' column_definition )*
                                [ ',' PRIMARY KEY '(' primary_key ')' ]
                            ')' [ WITH table_options ]
column_definition      ::=  column_name cql_type [ STATIC ] [ PRIMARY KEY]
primary_key            ::=  partition_key [ ',' clustering_columns ]
partition_key          ::=  column_name
                            | '(' column_name ( ',' column_name )* ')'
clustering_columns     ::=  column_name ( ',' column_name )*
table_options          ::=  COMPACT STORAGE [ AND table_options ]table create :
                            | CLUSTERING ORDER BY '(' clustering_order ')' [ AND table_options ]
                            | options
clustering_order       ::=  column_name (ASC | DESC) ( ',' column_name (ASC | DESC) )*
*********************************************************************************************/

/* create table est exprimee a l'aide de 3 fonctions : _create_table_stmt()
 , _create_table_stmt_aux() , _create_table_table_option()  */

/******************************************************************************/
/*_create_table_stmt() : _create_table_stmt_aux() | _if_not_exists() _create_table_stmt_aux()*/
/******************************************************************************/
boolean _create_table_stmt(){
	boolean result;
	if(_create_table_stmt_aux(false)){
		result=true;
	}
	else if(_if_not_exists()){
		token=_lire_token();
		if(_create_table_stmt_aux(true)){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_create_table_stmt_aux() : _table_name() _POPEN _column_definitions() ( _PRIMARY _KEY _POPEN _primary_key() _PCLOSE 
_PCLOSE _create_table_table_option() | _PCLOSE _create_table_table_option() )*/
/******************************************************************************/
boolean _create_table_stmt_aux(boolean ifNotExists){
	boolean result;
    currentStmt->type=create_table;
	currentStmt->node.createTable.liOptionTable=NULL;
    tableName *tN=_create_tableName();
	if(_table_name(tN)){
        currentStmt->node.createTable.objetKeyspace1=trouverKeyspace(liKey,tN,currentKeyspace,yylineno);
        if(currentStmt->node.createTable.objetKeyspace1!=NULL){
            _chercher_table_in_keyspace(currentStmt->node.createTable.objetKeyspace1,tN->nameTable,ifNotExists,yylineno);
        }
        currentStmt->node.createTable.name=(char *)malloc(strlen(tN->nameTable)*sizeof(char)+1);
        strcpy(currentStmt->node.createTable.name,tN->nameTable);
        free(tN);
		token=_lire_token();
		if(token==_POPEN){
            currentStmt->node.createTable.liCol=creerListeColumn(NULL,NULL);
			token=_lire_token();
			if(_column_definitions(currentStmt->node.createTable.liCol)){
                if(debug){listerListeColumn(currentStmt->node.createTable.liCol,printColumn);}
                _valide_column(currentStmt->node.createTable.liCol,yylineno);
				token=_lire_token();
                currentStmt->node.createTable.liPartitionKey=creerListeName(NULL,NULL);
                currentStmt->node.createTable.liClustringOrder=creerListeName(NULL,NULL);
				if(token==_PRIMARY){
					token=_lire_token();
					if(token==_KEY){
						token=_lire_token();
						if(token==_POPEN){
							token=_lire_token();
							if(_primary_key(currentStmt->node.createTable.liPartitionKey,currentStmt->node.createTable.liClustringOrder)){
                                _valide_column_primary_key(currentStmt->node.createTable.liCol,currentStmt->node.createTable.liPartitionKey,yylineno,currentStmt->node.createTable.liClustringOrder);
                                if(debug){listerListeColumn(currentStmt->node.createTable.liCol,printColumn);}
								token=_lire_token();
								if(token==_PCLOSE){
									token=_lire_token();
									if(token==_PCLOSE){
										token=_lire_token();
										if(_create_table_table_option()){
											result=true;
										}
										else{
											result=false;
										}
									}
									else{
										result=false;
									}
								}
								else{
									result=false;
								}
							}
							else{
								result=false;
							}
						}
						else{
							result=false;
						}
					}
					else{
						result=false;
					}
				}
				else if(token==_PCLOSE){
                    _valide_column_primary_key(currentStmt->node.createTable.liCol,currentStmt->node.createTable.liPartitionKey,yylineno,currentStmt->node.createTable.liClustringOrder);
					token=_lire_token();
					if(_create_table_table_option()){
						result=true;
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_create_table_table_option() :  follow( _PVIRG ) | _WITH _table_options()*/
/******************************************************************************/
boolean _create_table_table_option(){
	boolean result;
	if(token==_PVIRG){
		follow_token = true;
		result=true;
	}
	else if(token==_WITH){
        currentStmt->node.createTable.liOptionTable=creerListeOptionTable(NULL,NULL);
		token=_lire_token();
		if(_table_options(currentStmt->node.createTable.liOptionTable)){
            if(debug){listerListeOptionTable(currentStmt->node.createTable.liOptionTable,printOptionTable);}
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/*column-definitions : '(' column-definition ( ',' column-definition )* ')'
column-definitions est exprimee a l'aide de 3 fonctions :
_column_definitions() , _list_column_definitions() , _list_column_definitions_aux() */

/******************************************************************************/
/*_column_definitions() : _column_definition() _list_column_definitions()*/
/******************************************************************************/
boolean _column_definitions(ListeColumn *li){
	boolean result;
    objetColumn *oC=_create_column();
	if(_column_definition(oC)){
        insererEnFinDeListeColumn(li,oC);
		token=_lire_token();
		if(_list_column_definitions(li)){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_list_column_definitions() :  follow( _PCLOSE ) | _list_column_definitions_aux()*/
/******************************************************************************/
boolean _list_column_definitions(ListeColumn *li){
	boolean result;
	if(token==_PCLOSE){
		follow_token = true;
		result=true;
	}
	else if(_list_column_definitions_aux(li)){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_list_column_definitions_aux() : _VIRG (  follow( _PRIMARY ) | _column_definition() _list_column_definitions() )
*/
/******************************************************************************/
boolean _list_column_definitions_aux(ListeColumn *li){
	boolean result;
    objetColumn *oC=_create_column();
	if(token==_VIRG){
		token=_lire_token();
		if(token==_PRIMARY){
			follow_token = true;
			result=true;
		}
		else if(_column_definition(oC)){
            insererEnFinDeListeColumn(li,oC);
			token=_lire_token();
			if(_list_column_definitions(li)){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_column_definition() : _idf() _type() ( _STATIC (  follow( _VIRG , _PCLOSE )
 | _PRIMARY _KEY ) | (  follow( _VIRG , _PCLOSE ) | 
_PRIMARY _KEY ) )*/
/******************************************************************************/
boolean _column_definition(objetColumn *oC){
	boolean result;
	if(_idf()){
        oC->name=(char *)malloc(strlen(yytext)*sizeof(char)+1);
        strcpy(oC->name,strlwr(yytext));
		token=_lire_token();
        oC->type=(cql_type *)malloc(sizeof(cql_type));
		if(_type(oC->type)){
			token=_lire_token();
			if(token==_STATIC){
                oC->_static=true;
				token=_lire_token();
				if(token==_VIRG || token==_PCLOSE){
					follow_token = true;
					result=true;
				}
				else if(token==_PRIMARY){
					token=_lire_token();
					if(token==_KEY){
                        oC->partitionKey=true;
						result=true;
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else if(token==_VIRG || token==_PCLOSE){
				follow_token = true;
				result=true;
			}
			else if(token==_PRIMARY){
				token=_lire_token();
				if(token==_KEY){
                    oC->partitionKey=true;
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_primary_key() : _partition_key() (  follow( _PCLOSE ) | _VIRG _clustering_columns() )
*/
/******************************************************************************/
boolean _primary_key(ListeName *liPartitionKey,ListeName *liClusteringCol){
	boolean result;

	if(_partition_key(liPartitionKey)){
		token=_lire_token();
		if(token==_PCLOSE){
			follow_token = true;
			result=true;


		}
		else if(token==_VIRG){
			token=_lire_token();
			if(_clustering_columns(liClusteringCol)){
                if(debug )
                {listerListeName(liClusteringCol,printString);}
				result=true;

			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_partition_key() : _idf() | _column_names_main()*/
/******************************************************************************/
boolean _partition_key(ListeName *li){
	boolean result;
	if(_idf()){
        char *name=(char *)malloc(strlen(yytext)*sizeof(char)+1);
        strcpy(name,strlwr(yytext));
        insererEnFinDeListeName(li,name);
		result=true;
	}
	else if(_column_names_main(li)){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/*_column_names()  : '(' column_name (',' column_name)* ')'
column-definitions est exprimee a l'aide de 3 fonctions :
_column_names() ,_list_column_names() , _list_column_names_aux() */

/******************************************************************************/
/*_column_names_main() : _POPEN _idf() _column_names() _PCLOSE*/
/******************************************************************************/
boolean _column_names_main(ListeName *li){
	boolean result;
	if(token==_POPEN){
		token=_lire_token();
		if(_idf()){
            char *name=(char *)malloc(strlen(yytext)*sizeof(char)+1);
            strcpy(name,strlwr(yytext));
            insererEnFinDeListeName(li,name);
			token=_lire_token();
			if(_column_names(li)){
				token=_lire_token();
				if(token==_PCLOSE){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_column_names() :  follow( _PCLOSE , _ORDER , _PRE , _LIMIT , _ALLOW , _PVIRG )
 | _column_names_aux()*/
/******************************************************************************/
boolean _column_names(ListeName *li){
	boolean result;
	if(token==_PCLOSE || token==_ORDER || token==_PRE || token==_LIMIT || token==_ALLOW ||
		token==_PVIRG){
		follow_token = true;
		result=true;
	}
	else if(_column_names_aux(li)){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_column_names_aux() : _VIRG _idf() _column_names()*/
/******************************************************************************/
boolean _column_names_aux(ListeName *li){
	boolean result;
	if(token==_VIRG){
		token=_lire_token();
		if(_idf()){
            char *name=(char *)malloc(strlen(yytext)*sizeof(char)+1);
            strcpy(name,strlwr(yytext));
            insererEnFinDeListeName(li,name);
			token=_lire_token();
			if(_column_names(li)){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_clustering_columns() : _idf() _column_names()*/
/******************************************************************************/
boolean _clustering_columns(ListeName *li){
	boolean result;
	if(_idf()){
        char *name=(char *)malloc(strlen(yytext)*sizeof(char)+1);
        strcpy(name,strlwr(yytext));
        insererEnFinDeListeName(li,name);
		token=_lire_token();
		if(_column_names(li)){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/*table_options() : table_option() ( AND table_option())*  
options est exprimee a l'aide de 3 fonctions 
_table_options() , _list_table_options() , _list_table_options_aux()*/

/******************************************************************************/
/*_table_options() : _table_option() _list_table_options()*/
/******************************************************************************/
boolean _table_options(ListeOptionTable *li){
	boolean result;
    optionTable *oT=(optionTable *)malloc(sizeof(optionTable));
	if(_table_option(oT)){
        insererEnFinDeListeOptionTable(li,oT);
		token=_lire_token();
		if(_list_table_options(li)){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_list_table_options() :  follow( _PVIRG ) | _list_table_options_aux()*/
/******************************************************************************/
boolean _list_table_options(ListeOptionTable *li){
	boolean result;
	if(token==_PVIRG){
		follow_token = true;
		result=true;
	}
	else if(_list_table_options_aux(li)){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_list_table_options_aux() : _AND _table_option() _list_table_options()*/
/******************************************************************************/
boolean _list_table_options_aux(ListeOptionTable *li){
	boolean result;
	if(token==_AND){
		token=_lire_token();
        optionTable *oT=(optionTable *)malloc(sizeof(optionTable));
		if(_table_option(oT)){
            insererEnFinDeListeOptionTable(li,oT);
			token=_lire_token();
			if(_list_table_options(li)){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_table_option() : _COMPACT _STORAGE | _CLUSTERING _ORDER _BY _POPEN _clustering_order() _PCLOSE | 
_option()*/
/******************************************************************************/
boolean _table_option(optionTable *oT){
	boolean result;
    optionKeyspace *oK=(optionKeyspace *)malloc(sizeof(optionKeyspace));
	if(token==_COMPACT){
		token=_lire_token();
		if(token==_STORAGE){
            oT->type=compact_storage;
			result=true;
		}
		else{
			result=false;
		}
	}
	else if(token==_CLUSTERING){
		token=_lire_token();
		if(token==_ORDER){
            oT->type=culstring_order;
			token=_lire_token();
			if(token==_BY){
				token=_lire_token();
				if(token==_POPEN){
					token=_lire_token();
					if(_clustering_order()){
						token=_lire_token();
						if(token==_PCLOSE){
							result=true;
						}
						else{
							result=false;
						}
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else if(_option(oK)){
        oT->type=option_keyspace;
        oT->node.optionKey=oK;
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_clustering_order() : _idf() ( _ASC _cloumn_orders() | _DESC _cloumn_orders() )*/
/******************************************************************************/
boolean _clustering_order(){
	boolean result;
	if(_idf()){
		token=_lire_token();
		if(token==_ASC){
			token=_lire_token();
			if(_cloumn_orders()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else if(token==_DESC){
			token=_lire_token();
			if(_cloumn_orders()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_cloumn_orders() :  follow( _PCLOSE ) | _cloumn_orders_aux()*/
/******************************************************************************/
boolean _cloumn_orders(){
	boolean result;
	if(token==_PCLOSE){
		follow_token = true;
		result=true;
	}
	else if(_cloumn_orders_aux()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_cloumn_orders_aux() : _VIRG _cloumn_order() _cloumn_orders()*/
/******************************************************************************/
boolean _cloumn_orders_aux(){
	boolean result;
	if(token==_VIRG){
		token=_lire_token();
		if(_cloumn_order()){
			token=_lire_token();
			if(_cloumn_orders()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_cloumn_order() : _idf() ( _ASC | _DESC )*/
/******************************************************************************/
boolean _cloumn_order(){
	boolean result;
	if(_idf()){
		token=_lire_token();
		if(token==_ASC){
			result=true;
		}
		else if(token==_DESC){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/*****************************Data Definition:ALTER TABLE*************************************
alter_table_statement   ::=  ALTER TABLE table_name alter_table_instruction
alter_table_instruction ::=  ALTER column_name TYPE cql_type
                             | ADD column_name cql_type ( ',' column_name cql_type )*
                             | DROP column_name ( column_name )*
                             | WITH options

*********************************************************************************************/

/******************************************************************************/
/*_alter_table_stmt() : _table_name() _alter_table_instruction()*/
/******************************************************************************/
boolean _alter_table_stmt(){
	boolean result;
	if(_table_name(NULL)){
		token=_lire_token();
		if(_alter_table_instruction()){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_alter_table_instruction() : _ALTER _idf() _TYPE _type() | _ADD _idf() _type() _list_column_types() | 
_DROP _idf() _column_names_alter() | _WITH _options()*/
/******************************************************************************/
boolean _alter_table_instruction(){
	boolean result;
	if(token==_ALTER){
		token=_lire_token();
		if(_idf()){
			token=_lire_token();
			if(token==_TYPE){
				token=_lire_token();
				if(_type(NULL)){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else if(token==_ADD){
		token=_lire_token();
		if(_idf()){
			token=_lire_token();
			if(_type(NULL)){
				token=_lire_token();
				if(_list_column_types()){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else if(token==_DROP){
		token=_lire_token();
		if(_idf()){
			token=_lire_token();
			if(_column_names_alter()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else if(token==_WITH){
		token=_lire_token();
		if(_options(NULL)){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_list_column_types() :  follow( _PVIRG ) | _list_column_types_aux()*/
/******************************************************************************/
boolean _list_column_types(){
	boolean result;
	if(token==_PVIRG){
		follow_token = true;
		result=true;
	}
	else if(_list_column_types_aux()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_list_column_types_aux() : _VIRG _idf() _type() _list_column_types()*/
/******************************************************************************/
boolean _list_column_types_aux(){
	boolean result;
	if(token==_VIRG){
		token=_lire_token();
		if(_idf()){
			token=_lire_token();
			if(_type(NULL)){
				token=_lire_token();
				if(_list_column_types()){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_column_names_alter() :  follow( _PVIRG ) | _column_names_alter_aux()*/
/******************************************************************************/
boolean _column_names_alter(){
	boolean result;
	if(token==_PVIRG){
		follow_token = true;
		result=true;
	}
	else if(_column_names_alter_aux()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_column_names_alter_aux() : _idf() _column_names_alter()*/
/******************************************************************************/
boolean _column_names_alter_aux(){
	boolean result;
	if(_idf()){
		token=_lire_token();
		if(_column_names_alter()){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/***************Data Definition:TRUNCATE TABLE***DROP TABLE*********************************
drop_table_statement ::=  DROP TABLE [ IF EXISTS ] table_name
truncate_statement ::=  TRUNCATE [ TABLE ] table_name
*********************************************************************************************/

/******************************************************************************/
/*_drop_table_stmt() : _if_exists() _table_name() | _table_name()*/
/******************************************************************************/
boolean _drop_table_stmt(){
	currentStmt->type=drop_table;
    currentStmt->node.dropTable.keyspace=NULL;
    currentStmt->node.dropTable.table=NULL;
	tableName *tn=_create_tableName();
	boolean result;
	if(_if_exists()){
		token=_lire_token();
		if(_table_name(tn)){
			currentStmt->node.dropTable.keyspace=trouverKeyspace(liKey,tn,currentKeyspace,yylineno);
			if(currentStmt->node.dropTable.keyspace!=NULL){
				currentStmt->node.dropTable.table=_chercher_table(currentStmt->node.dropTable.keyspace,tn->nameTable,yylineno,true);
			}
			result=true;
		}
		else{
			result=false;
		}
	}
	else if(_table_name(tn)){
		currentStmt->node.dropTable.keyspace=trouverKeyspace(liKey,tn,currentKeyspace,yylineno);
		if(currentStmt->node.dropTable.keyspace!=NULL){
			currentStmt->node.dropTable.table=_chercher_table(currentStmt->node.dropTable.keyspace,tn->nameTable,yylineno,false);
		}
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_truncate_stmt() : _TRUNCATE ( _TABLE _table_name() | _table_name() )*/
/******************************************************************************/
boolean _truncate_stmt(){
	currentStmt->type=truncate_table;
	currentStmt->node.truncat.keyspace=NULL;
	currentStmt->node.truncat.table=NULL;
	tableName *tn=_create_tableName();
	boolean result;
	if(token==_TRUNCATE){
		token=_lire_token();
		if(token==_TABLE){
			token=_lire_token();
			if(_table_name(tn)){
				currentStmt->node.truncat.keyspace=trouverKeyspace(liKey,tn,currentKeyspace,yylineno);
				if(currentStmt->node.truncat.keyspace!=NULL){
					currentStmt->node.truncat.table=_chercher_table(currentStmt->node.truncat.keyspace,tn->nameTable,yylineno,false);
				}
				result=true;
			}
			else{
				result=false;
			}
		}
		else if(_table_name(tn)){
			currentStmt->node.truncat.keyspace=trouverKeyspace(liKey,tn,currentKeyspace,yylineno);
			if(currentStmt->node.truncat.keyspace!=NULL){
				currentStmt->node.truncat.table=_chercher_table(currentStmt->node.truncat.keyspace,tn->nameTable,yylineno,false);
			}
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/*************************************SELECT**************************************************
select_statement ::=  SELECT [ JSON | DISTINCT ] ( select_clause | '*' )
                      FROM table_name
                      [ WHERE where_clause ]
                      [ GROUP BY group_by_clause ]
                      [ ORDER BY ordering_clause ]
                      [ PER PARTITION LIMIT (integer | bind_marker) ]
                      [ LIMIT (integer | bind_marker) ]
                      [ ALLOW FILTERING ]
select_clause    ::=  selector [ AS identifier ] ( ',' selector [ AS identifier ] )*
selector         ::=  column_name
                      | term
                      | CAST '(' selector AS cql_type ')'
                      | function_name '(' [ selector ( ',' selector )* ] ')'
                      | COUNT '(' '*' ')';;;;;;;;;;;;;;;;;;;;;;;;
where_clause     ::=  relation ( AND relation )*
relation         ::=  column_name operator term
                      '(' column_name ( ',' column_name )* ')' operator tuple_literal
                      TOKEN '(' column_name ( ',' column_name )* ')' operator term;;;;;;;;;;;;;;;;
operator         ::=  '=' | '<' | '>' | '<=' | '>=' | '!=' | IN | CONTAINS | CONTAINS KEY ;;;;;;;
group_by_clause  ::=  column_name ( ',' column_name )*
ordering_clause  ::=  column_name [ ASC | DESC ] ( ',' column_name [ ASC | DESC ] )*
*********************************************************************************************/

/* select est exprimee a l'aide de 8 fonctions : _select_stmt() , _select_aux() 
_select_where() ,  _select_group_by() , _select_order_by() , _select_pre_partition() ,
_select_limit() , _select_allow_filtering() */

/******************************************************************************/
/*_select_stmt() : _SELECT ( _JSON _select_aux() | _DISTINCT _select_aux() | _select_aux() )
*/
/******************************************************************************/
boolean _select_stmt(){
    currentStmt->type=select_stmt;
    currentStmt->node.select.liRelation=creerListeRelation(NULL,NULL);
    currentStmt->node.select.keyspace=NULL;
    currentStmt->node.select.table=NULL;
    currentStmt->node.select.liSelector=creerListeSelector(NULL,NULL);
    currentStmt->node.select.limit=-1;
	boolean result;
	if(token==_SELECT){
		token=_lire_token();
		if(token==_JSON){
			token=_lire_token();
			if(_select_aux()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else if(token==_DISTINCT){
			token=_lire_token();
			if(_select_aux()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else if(_select_aux()){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_select_aux() : _select_clause() _FROM _table_name() _select_where() | _MULT _FROM _table_name() _select_where()*/
/******************************************************************************/
boolean _select_aux(){
	boolean result;
    if(token==_MULT){
        token=_lire_token();
        if(token==_FROM){
            token=_lire_token();
            tableName *tn=_create_tableName();
            if(_table_name(tn)){
                currentStmt->node.select.keyspace=trouverKeyspace(liKey,tn,currentKeyspace,yylineno);
                if(currentStmt->node.select.keyspace!=NULL){
                    currentStmt->node.select.table=_chercher_table(currentStmt->node.select.keyspace,tn->nameTable,yylineno,false);
                }
                token=_lire_token();
                if(_select_where()){
                    currentStmt->node.select.type=all;
                    result=true;
                }
                else{
                    result=false;
                }
            }
            else{
                result=false;
            }
        }
        else{
            result=false;
        }
    }

	else if(_select_clause(currentStmt->node.select.liSelector)){
        token=_lire_token();
        if(token==_FROM){
            token = _lire_token();
            tableName *tn = _create_tableName();
            if (_table_name(tn)) {
                currentStmt->node.select.keyspace = trouverKeyspace(liKey, tn, currentKeyspace, yylineno);
                if (currentStmt->node.select.keyspace != NULL) {
                    currentStmt->node.select.table = _chercher_table(currentStmt->node.select.keyspace,
                                                                     tn->nameTable, yylineno,false);
                }
                token = _lire_token();
                if (_select_where()) {
                    currentStmt->node.select.type = notAll;
                    result = true;
                } else {
                    result = false;
                }
            } else {
                result = false;
            }
        }
        else{
            result=false;
        }
    }
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_select_where() : _WHERE _where_clause() _select_group_by() | _select_group_by()*/
/******************************************************************************/
boolean _select_where(){
    if(currentStmt->node.select.table!=NULL){
        _valide_selector(currentStmt->node.select.table,currentStmt->node.select.liSelector,yylineno);
    }

	boolean result;
	if(token==_WHERE){
		token=_lire_token();
        if(_where_clause(currentStmt->node.select.liRelation)){
            if(currentStmt->node.select.table!=NULL){
                _valide_select_where(currentStmt->node.select.table,currentStmt->node.select.liRelation,yylineno);
            }
			token=_lire_token();
			if(_select_group_by()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else if(_select_group_by()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_select_group_by() : _GROUP _BY _group_by_clause() _select_order_by() | _select_order_by()*/
/******************************************************************************/
boolean _select_group_by(){
	boolean result;
	if(token==_GROUP){
		token=_lire_token();
		if(token==_BY){
			token=_lire_token();
			if(_group_by_clause()){
				token=_lire_token();
				if(_select_order_by()){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else if(_select_order_by()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_select_order_by() : _ORDER _BY _ordering_clause() _select_pre_partition() | _select_pre_partition()*/
/******************************************************************************/
boolean _select_order_by(){
	boolean result;
	if(token==_ORDER){
		token=_lire_token();
		if(token==_BY){
			token=_lire_token();
			if(_ordering_clause()){
				token=_lire_token();
				if(_select_pre_partition()){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else if(_select_pre_partition()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_select_pre_partition() : _PER _PARTITION _LIMIT ( _INTEGER _select_limit() | _bind_marker() _select_limit() )
 | _select_limit()*/
/******************************************************************************/
boolean _select_pre_partition(){
	boolean result;
	if(token==_PER){
		token=_lire_token();
		if(token==_PARTITION){
			token=_lire_token();
			if(token==_LIMIT){
				token=_lire_token();
				if(token==_INTEGER){
                    currentStmt->node.select.limit=atoi(yytext);
                    if(currentStmt->node.select.limit<=0){
                        _creerErreur(InvalidRequest,yylineno,NULL,"LIMIT must be strictly positive");
                    }
					token=_lire_token();
					if(_select_limit()){
						result=true;
					}
					else{
						result=false;
					}
				}
				else if(_bind_marker()){
					token=_lire_token();
					if(_select_limit()){
						result=true;
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else if(_select_limit()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_select_limit() : _LIMIT ( _INTEGER _select_allow_filtering() | _bind_marker() _select_allow_filtering() ) | 
_select_allow_filtering()*/
/******************************************************************************/
boolean _select_limit(){
	boolean result;
	if(token==_LIMIT){
		token=_lire_token();
		if(token==_INTEGER){
			token=_lire_token();
			if(_select_allow_filtering()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else if(_bind_marker()){
			token=_lire_token();
			if(_select_allow_filtering()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else if(_select_allow_filtering()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_select_allow_filtering() :  follow( _PVIRG ) | _ALLOW _FILTERING*/
/******************************************************************************/
boolean _select_allow_filtering(){
	boolean result;
	if(token==_PVIRG){
		follow_token = true;
		result=true;
	}
	else if(token==_ALLOW){
		token=_lire_token();
		if(token==_FILTERING){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_select_clause() : _selector_as() _selectors_as()*/
/******************************************************************************/
boolean _select_clause(ListeSelector *li){
	boolean result;
    objetSelector *objet=(objetSelector *)malloc(sizeof(objetSelector));
    objet->as=NULL;
	if(_selector_as(objet)){
        insererEnFinDeListeSelector(li,objet);
		token=_lire_token();
		if(_selectors_as(li)){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_selectors_as() :  follow( _FROM ) | _selectors_as_aux()*/
/******************************************************************************/
boolean _selectors_as(ListeSelector *li){
	boolean result;
	if(token==_FROM){
		follow_token = true;
		result=true;
	}
	else if(_selectors_as_aux(li)){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_selectors_as_aux() : _VIRG _selector_as() _selectors_as()*/
/******************************************************************************/
boolean _selectors_as_aux(ListeSelector *li){
	boolean result;
	if(token==_VIRG){
		token=_lire_token();
        objetSelector *objet=(objetSelector *)malloc(sizeof(objetSelector));
        objet->as=NULL;
		if(_selector_as(objet)){
            insererEnFinDeListeSelector(li,objet);
			token=_lire_token();
			if(_selectors_as(li)){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_selector_as() : _selector() (  follow( _VIRG , _FROM ) | _AS _idf() )
*/
/******************************************************************************/
boolean _selector_as(objetSelector *objet){
	boolean result;
	if(_selector(objet)){
		token=_lire_token();
		if(token==_VIRG || token==_FROM){
			follow_token = true;
			result=true;
		}

		else if(token==_AS){
			token=_lire_token();
			if(_idf()){
                objet->as=strdup(strlwr(yytext));
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_selector() : _idf() (  follow( _AS , _VIRG , _FROM ) | 
_POPEN _selectors_main() _PCLOSE ) | _term_without_idf() | _CAST _POPEN _selector() _AS _type() 
_PCLOSE | _COUNT _POPEN _MULT _PCLOSE*/
/******************************************************************************/
boolean _selector(objetSelector *objet){
	boolean result;
	if(_idf()){
        objet->name=strdup(strlwr(yytext));
		token=_lire_token();
		if(token==_AS || token==_VIRG || token==_FROM){
			follow_token = true;
			result=true;
		}
		else if(token==_POPEN){
			token=_lire_token();
			if(_selectors_main()){
				token=_lire_token();
				if(token==_PCLOSE){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else if(_term_without_idf()){
		result=true;
	}
	else if(token==_CAST){
		token=_lire_token();
		if(token==_POPEN){
			token=_lire_token();
			if(_selector(NULL)){
				token=_lire_token();
				if(token==_AS){
					token=_lire_token();
					if(_type(NULL)){
						token=_lire_token();
						if(token==_PCLOSE){
							result=true;
						}
						else{
							result=false;
						}
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else if(token==_COUNT){
		token=_lire_token();
		if(token==_POPEN){
			token=_lire_token();
			if(token==_MULT){
				token=_lire_token();
				if(token==_PCLOSE){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/*[ selector ( ',' selector )* ]*/

/******************************************************************************/
/*_selectors_main() :  follow( _PCLOSE ) | _selector() _selectors()*/
/******************************************************************************/
boolean _selectors_main(){
	boolean result;
	if(token==_PCLOSE){
		follow_token = true;
		result=true;
	}
	else if(_selector(NULL)){
		token=_lire_token();
		if(_selectors()){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_selectors() :  follow( _PCLOSE ) | _selectors_aux()*/
/******************************************************************************/
boolean _selectors(){
	boolean result;
	if(token==_PCLOSE){
		follow_token = true;
		result=true;
	}
	else if(_selectors_aux()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_selectors_aux() : _VIRG _selector() _selectors()*/
/******************************************************************************/
boolean _selectors_aux(){
	boolean result;
	if(token==_VIRG){
		token=_lire_token();
		if(_selector(NULL)){
			token=_lire_token();
			if(_selectors()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_where_clause() : _relation() _relations()*/
/******************************************************************************/
boolean _where_clause(ListeRelation *li){
	boolean result;
	objetRelation *objet=(objetRelation *)malloc(sizeof(objetRelation));
	if(_relation(objet)){
		insererEnFinDeListeRelation(li,objet);
		token=_lire_token();
		if(_relations(li)){
			if(debug){listerListeRelation(li,printRelation);}
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_relations() :  follow( _GROUP , _ORDER , _PRE , _LIMIT , _ALLOW , _PVIRG )
 | _relations_aux()*/
/******************************************************************************/
boolean _relations(ListeRelation *li){
	boolean result;
	if(token==_GROUP || token==_ORDER || token==_PRE || token==_LIMIT || token==_ALLOW
		|| token==_PVIRG ){
		follow_token = true;
		result=true;
	}
	else if(_relations_aux(li)){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_relations_aux() : _AND _relation() _relations()*/
/******************************************************************************/
boolean _relations_aux(ListeRelation *li){
	boolean result;
	if(token==_AND){
		token=_lire_token();
		objetRelation *objet=(objetRelation *)malloc(sizeof(objetRelation));
		if(_relation(objet)){
			insererEnFinDeListeRelation(li,objet);
			token=_lire_token();
			if(_relations(li)){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_relation() : _idf() _operator() _term() | _POPEN _idf() _column_names() _PCLOSE _operator() _tuple_literal_main()
 | _TOKEN _POPEN _idf() _column_names() _PCLOSE _operator() _term()*/
/******************************************************************************/
boolean _relation(objetRelation *objet){
	boolean result;
	if(_idf()){
		objet->name=strdup(strlwr(yytext));
		token=_lire_token();
		objet->anOperator=(_operatorEnum *)malloc(sizeof(_operatorEnum));
		if(_operator(objet->anOperator)){
			token=_lire_token();
			objet->t=(term *)malloc(sizeof(term));
			if(_term(objet->t)){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else if(token==_POPEN){
		token=_lire_token();
		if(_idf()){
			token=_lire_token();
			if(_column_names(NULL)){
				token=_lire_token();
				if(token==_PCLOSE){
					token=_lire_token();
					if(_operator(NULL)){
						token=_lire_token();
						if(_tuple_literal_main(NULL)){
							result=true;
						}
						else{
							result=false;
						}
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else if(token==_TOKEN){
		token=_lire_token();
		if(token==_POPEN){
			token=_lire_token();
			if(_idf()){
				token=_lire_token();
				if(_column_names(NULL)){
					token=_lire_token();
					if(token==_PCLOSE){
						token=_lire_token();
						if(_operator(NULL)){
							token=_lire_token();
							if(_term(NULL)){
								result=true;
							}
							else{
								result=false;
							}
						}
						else{
							result=false;
						}
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_group_by_clause() : _idf() _column_names()*/
/******************************************************************************/
boolean _group_by_clause(){
	boolean result;
	if(_idf()){
		token=_lire_token();
		if(_column_names(NULL)){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_ordering_clause() : _column_name_asc_desc() _column_names_ordring()*/
/******************************************************************************/
boolean _ordering_clause(){
	boolean result;
	if(_column_name_asc_desc()){
		token=_lire_token();
		if(_column_names_ordring()){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_column_names_ordring() :  follow( _PRE , _LIMIT , _ALLOW , _PVIRG ) | _column_names_ordring_aux()*/
/******************************************************************************/
boolean _column_names_ordring(){
	boolean result;
	if(token==_PRE || token==_LIMIT || token==_ALLOW || token==_PVIRG){
		follow_token = true;
		result=true;
	}
	else if(_column_names_ordring_aux()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_column_names_ordring_aux() : _VIRG _column_name_asc_desc() _column_names_ordring()*/
/******************************************************************************/
boolean _column_names_ordring_aux(){
	boolean result;
	if(token==_VIRG){
		token=_lire_token();
		if(_column_name_asc_desc()){
			token=_lire_token();
			if(_column_names_ordring()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_column_name_asc_desc() : _idf() (  follow( _PRE , _LIMIT , _ALLOW , _PVIRG , _VIRG )
 | _ASC | _DESC )*/
/******************************************************************************/
boolean _column_name_asc_desc(){
	boolean result;
	if(_idf()){
		token=_lire_token();
		if(token==_PRE || token==_LIMIT || token==_ALLOW || token==_PVIRG || token==_VIRG){
			follow_token = true;
			result=true;
		}
		else if(token==_ASC){
			result=true;
		}
		else if(token==_DESC){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_operator() : _EQ | _LESS | _GREATER | _LESSEQ | _GREATEREQ | _DIFF | 
_IN | _CONTAINS (  follow( _STRING , _INTEGER , _FLOAT , _TRUE , _FALSE , _UUID , 
_BLOB , _NULL , _POPEN , _COPEN , _AOPEN , _iidf , _PINTERR , _DEUXP )
 | _KEY )*/
/******************************************************************************/
boolean _operator(_operatorEnum *o){
	boolean result;
	if(token==_EQ){
		*o=op_eq;
		result=true;
	}
	else if(token==_LESS){
		*o=op_less;
		result=true;
	}
	else if(token==_GREATER){
		*o=op_greater;
		result=true;
	}
	else if(token==_LESSEQ){
		*o=op_lessEq;
		result=true;
	}
	else if(token==_GREATEREQ){
		*o=op_greaterEq;
		result=true;
	}
	else if(token==_DIFF){
		*o=op_diff;
		result=true;
	}
	else if(token==_IN){
		result=true;
	}
	else if(token==_CONTAINS){
		token=_lire_token();
		if(token==_STRING || token==_INTEGER || token==_FLOAT || token==_TRUE 
			|| token==_FALSE || token==_UUID || token==_BLOB || token==_NULL
			|| token==_POPEN || token==_COPEN || token==_AOPEN || token==_PINTERR || token==_DEUXP){
			follow_token = true;
			result=true;
		}
		else if(_idf()){
			follow_token = true;
			result=true;
		}
		else if(token==_KEY){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/*************************************INSERT**************************************************
insert_statement ::=  INSERT INTO table_name ( names_values | json_clause )
                      [ IF NOT EXISTS ]
                      [ USING update_parameter ( AND update_parameter )* ]
names_values     ::=  names VALUES tuple_literal
json_clause      ::=  JSON string [ DEFAULT ( NULL | UNSET ) ]
names            ::=  '(' column_name ( ',' column_name )* ')'
*********************************************************************************************/

/******************************************************************************/
/*_insert_stmt() : _INSERT _INTO _table_name() ( _names_values() _insert_if_not_exists() | _json_clause() _insert_if_not_exists() )
*/
/******************************************************************************/
boolean _insert_stmt(){
    currentStmt->type=insert_into;
    currentStmt->node.insert.ifNotExist=false;
	currentStmt->node.insert.table=NULL;
	boolean result;
	if(token==_INSERT){
		token=_lire_token();
		if(token==_INTO){
			token=_lire_token();
            tableName *tn=_create_tableName();
			if(_table_name(tn)){
                currentStmt->node.insert.keyspace=trouverKeyspace(liKey,tn,currentKeyspace,yylineno);
                if(currentStmt->node.insert.keyspace!=NULL){
                   currentStmt->node.insert.table=_chercher_table(currentStmt->node.insert.keyspace,tn->nameTable,yylineno,false);
                }
				token=_lire_token();
				if(_names_values()){
					token=_lire_token();
					if(_insert_if_not_exists()){
						result=true;
					}
					else{
						result=false;
					}
				}
				else if(_json_clause()){
					token=_lire_token();
					if(_insert_if_not_exists()){
						result=true;
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_insert_if_not_exists() : _IF _NOT _EXISTS _insert_using() | _insert_using()*/
/******************************************************************************/
boolean _insert_if_not_exists(){
	boolean result;
	if(token==_IF){
		token=_lire_token();
		if(token==_NOT){
			token=_lire_token();
			if(token==_EXISTS){
                currentStmt->node.insert.ifNotExist=true;
				token=_lire_token();
				if(_insert_using()){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else if(_insert_using()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_insert_using() :  follow( _PVIRG ) | _USING _update_parameter() _update_parameters()*/
/******************************************************************************/
boolean _insert_using(){
	boolean result;
	if(token==_PVIRG){
		follow_token = true;
		result=true;
	}
	else if(token==_USING){
		token=_lire_token();
		if(_update_parameter()){
			token=_lire_token();
			if(_update_parameters()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_update_parameters() :  follow( _PVIRG , _SET , _WHERE , _INSERT , _UPDATE , _DELETE )
 | _update_parameters_aux()*/
/******************************************************************************/
boolean _update_parameters(){
	boolean result;
	if(token==_PVIRG || token==_SET || token==_WHERE || token==_INSERT || token==_UPDATE || token==_DELETE){
		follow_token = true;
		result=true;
	}
	else if(_update_parameters_aux()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_update_parameters_aux() : _AND _update_parameter() _update_parameters()*/
/******************************************************************************/
boolean _update_parameters_aux(){
	boolean result;
	if(token==_AND){
		token=_lire_token();
		if(_update_parameter()){
			token=_lire_token();
			if(_update_parameters()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_names_values() : _column_names_values() _VALUES _tuple_literal_main()*/
/******************************************************************************/
boolean _names_values(){
    currentStmt->node.insert.liName=creerListeName(NULL,NULL);
	boolean result;
	if(_column_names_values(currentStmt->node.insert.liName)){
		token=_lire_token();
		if(token==_VALUES){
			token=_lire_token();
            currentStmt->node.insert.liValue=creerListeList(NULL,NULL);


			if(_tuple_literal_main(currentStmt->node.insert.liValue)){
                if(currentStmt->node.insert.table!=NULL){
                    _valide_insert(currentStmt->node.insert.liName,currentStmt->node.insert.liValue,currentStmt->node.insert.table,yylineno);
                }
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_json_clause() : _JSON _STRING (  follow( _IF , _USING , _PVIRG )
 | _DEFAULT ( _NULL | _UNSET ) )*/
/******************************************************************************/
boolean _json_clause(){
	boolean result;
	if(token==_JSON){
		token=_lire_token();
		if(token==_STRING){
			token=_lire_token();
			if(token==_IF || token==_USING || token==_PVIRG ){
				follow_token = true;
				result=true;
			}
			else if(token==_DEFAULT){
				token=_lire_token();
				if(token==_NULL){
					result=true;
				}
				else if(token==_UNSET){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_column_names_values() : _POPEN _idf() _column_names() _PCLOSE*/
/******************************************************************************/
boolean _column_names_values(ListeName *li){
	boolean result;
	if(token==_POPEN){
		token=_lire_token();
		if(_idf()){
            char *name=strdup(strlwr(yytext));
            insererEnFinDeListeName(li,name);
			token=_lire_token();
			if(_column_names(li)){
				token=_lire_token();
				if(token==_PCLOSE){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/*************************************UPDATE**************************************************
update_statement ::=  UPDATE table_name
                      [ USING update_parameter ( AND update_parameter )* ]
                      SET assignment ( ',' assignment )*
                      WHERE where_clause
                      [ IF ( EXISTS | condition ( AND condition )*) ]
update_parameter ::=  ( TIMESTAMP | TTL ) ( integer | bind_marker )
assignment       ::=  simple_selection '=' term
                     | column_name '=' column_name ( '+' | '-' ) term
                     | column_name '=' list_literal '+' column_name;;;;;;;;;;;;;;;;



simple_selection ::=  column_name
                     | column_name '[' term ']'
                     | column_name '.' `field_name ;;;;;;;;;;;;;;;;;;;
condition        ::=  simple_selection operator term
*********************************************************************************************/

/******************************************************************************/
/*_update_stmt() : _UPDATE _table_name() _update_using()*/
/******************************************************************************/
boolean _update_stmt(){
    currentStmt->type=update_stmt;
    currentStmt->node.update.keyspace=NULL;
    currentStmt->node.update.table=NULL;
    currentStmt->node.update.liRelation=creerListeRelation(NULL,NULL);
    currentStmt->node.update.liNameValue=creerListeNameValue(NULL,NULL);
    currentStmt->node.update.ifExist=false;
	boolean result;
	if(token==_UPDATE){
		token=_lire_token();
        tableName *tn=_create_tableName();
        if(_table_name(tn)){
            currentStmt->node.update.keyspace=trouverKeyspace(liKey,tn,currentKeyspace,yylineno);
            if(currentStmt->node.update.keyspace!=NULL){
                currentStmt->node.update.table=_chercher_table(currentStmt->node.update.keyspace,tn->nameTable,yylineno,false);
            }
			token=_lire_token();
			if(_update_using()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_update_using() : _USING _update_parameter() _update_parameters() _update_set() | _update_set()*/
/******************************************************************************/
boolean _update_using(){
	boolean result;
	if(token==_USING){
		token=_lire_token();
		if(_update_parameter()){
			token=_lire_token();
			if(_update_parameters()){
				token=_lire_token();
				if(_update_set()){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else if(_update_set()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_update_set() : _SET _assignment() _assignments() _WHERE _where_clause() _update_if()*/
/******************************************************************************/
boolean _update_set(){
	boolean result;
	if(token==_SET){
		token=_lire_token();
        objetNameValue *objetNameValue1=(objetNameValue *)malloc(sizeof(objetNameValue));
		if(_assignment(objetNameValue1)){
            insererEnFinDeListeNameValue(currentStmt->node.update.liNameValue,objetNameValue1);
			token=_lire_token();
			if(_assignments(currentStmt->node.update.liNameValue)){
                if(debug){
                    listerListeNameValue(currentStmt->node.update.liNameValue,printNameValue);
                }
                if(currentStmt->node.update.table!=NULL){
                    _valide_update_set(currentStmt->node.update.table,currentStmt->node.update.liNameValue,yylineno);
                }
				token=_lire_token();
				if(token==_WHERE){
					token=_lire_token();
                    if(_where_clause(currentStmt->node.update.liRelation)){
                        if(currentStmt->node.update.table!=NULL){
                            _valide_update_where(currentStmt->node.update.table,currentStmt->node.update.liRelation,yylineno);
                        }
                        if(debug){listerListeRelation(currentStmt->node.update.liRelation,printRelation);}
						token=_lire_token();
						if(_update_if()){
							result=true;
						}
						else{
							result=false;
						}
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_update_parameter() : _TIMESTAMP ( _INTEGER | _bind_marker() ) | _TTL ( _INTEGER | 
_bind_marker() )*/
/******************************************************************************/
boolean _update_parameter(){
	boolean result;
	if(token==_TIMESTAMP){
		token=_lire_token();
		if(token==_INTEGER){
			result=true;
		}
		else if(_bind_marker()){
			result=true;
		}
		else{
			result=false;
		}
	}
	else if(token==_TTL){
		token=_lire_token();
		if(token==_INTEGER){
			result=true;
		}
		else if(_bind_marker()){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_update_if() :  follow( _PVIRG ) | _IF ( _EXISTS | _condition() _conditions() )
*/
/******************************************************************************/
boolean _update_if(){
	boolean result;
	if(token==_PVIRG){
		follow_token = true;
		result=true;
	}
	else if(token==_IF){
		token=_lire_token();
		if(token==_EXISTS){
            currentStmt->node.update.ifExist=true;
			result=true;
		}
		else if(_condition()){
			token=_lire_token();
			if(_conditions()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_assignment() : _idf() ( _EQ ( _idf() ( _PLUS _term() |
_MINUS _term() ) | _list() _PLUS _idf() | _term() ) | 
_COPEN _simple_selection_aux() _EQ _term() )*/
/******************************************************************************/
boolean _assignment(objetNameValue *objetNameValue1){
	boolean result;
	if(_idf()){
        objetNameValue1->name=strdup(strlwr(yytext));
        objetNameValue1->t=(term *)malloc(sizeof(term));
		//objetNameValue1->t->node.list=(ListeList *)malloc(sizeof(ListeList));
		token=_lire_token();
		if(token==_EQ){
			token=_lire_token();
			if(_idf()){
				token=_lire_token();
				if(token==_PLUS){
					token=_lire_token();
					if(_term(NULL)){
						result=true;
					}
					else{
						result=false;
					}
				}
				else if(token==_MINUS){
					token=_lire_token();
					if(_term(NULL)){
						result=true;
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
            else if(_term(objetNameValue1->t)){
                result=true;
            }
			else if(_list(objetNameValue1->t->node.list)){
                objetNameValue1->t->type=list_term;
				token=_lire_token();
				if(token==_PLUS){
					token=_lire_token();
					if(_idf()){
						result=true;
					}
					else{
						result=false;
					}
				}else if(token==_WHERE){
                    follow_token=true;
                    result=true;
                }
				else{
					result=false;
				}
			}

			else{
				result=false;
			}
		}
		else if(token==_COPEN){
			token=_lire_token();
			if(_simple_selection_aux()){
				token=_lire_token();
				if(token==_EQ){
					token=_lire_token();
					if(_term(NULL)){
						result=true;
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_assignments() :  follow( _WHERE ) | _assignments_aux()*/
/******************************************************************************/
boolean _assignments(ListeNameValue *li){
	boolean result;
	if(token==_WHERE){
		follow_token = true;
		result=true;
	}
	else if(_assignments_aux(li)){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_assignments_aux() : _VIRG _assignment() _assignments()*/
/******************************************************************************/
boolean _assignments_aux(ListeNameValue *li){
	boolean result;
	if(token==_VIRG){
		token=_lire_token();
        objetNameValue *objetNameValue1=(objetNameValue *)malloc(sizeof(objetNameValue));
		if(_assignment(objetNameValue1)){
            insererEnFinDeListeNameValue(li,objetNameValue1);
			token=_lire_token();
			if(_assignments(li)){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_simple_selection() : _idf() (  follow( _FROM , _VIRG , _EQ , _LESS , _GREATER , _LESSEQ , 
_GREATEREQ , _DIFF , _IN , _CONTAINS ) | _COPEN _term() _CCLOSE )
*/
/******************************************************************************/
boolean _simple_selection(){
	boolean result;
	if(_idf()){
		token=_lire_token();
		if(token==_FROM || token==_VIRG || token==_EQ || token==_LESS || token==_GREATER || token==_LESSEQ  
		|| token==_GREATEREQ || token==_DIFF || token==_IN || token==_CONTAINS){
			follow_token = true;
			result=true;
		}
		else if(token==_COPEN){
			token=_lire_token();
			if(_term(NULL)){
				token=_lire_token();
				if(token==_CCLOSE){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_simple_selection_aux() : _term() _CCLOSE*/
/******************************************************************************/
boolean _simple_selection_aux(){
	boolean result;
	if(_term(NULL)){
		token=_lire_token();
		if(token==_CCLOSE){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_condition() : _simple_selection() _operator() _term()*/
/******************************************************************************/
boolean _condition(){
	boolean result;
	if(_simple_selection()){
		token=_lire_token();
		if(_operator(NULL)){
			token=_lire_token();
			if(_term(NULL)){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_conditions() :  follow( _PVIRG ) | _conditions_aux()*/
/******************************************************************************/
boolean _conditions(){
	boolean result;
	if(token==_PVIRG){
		follow_token = true;
		result=true;
	}
	else if(_conditions_aux()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_conditions_aux() : _AND _condition() _conditions()*/
/******************************************************************************/
boolean _conditions_aux(){
	boolean result;
	if(token==_AND){
		token=_lire_token();
		if(_condition()){
			token=_lire_token();
			if(_conditions()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/*************************************DELETE**************************************************
delete_statement ::=  DELETE [ simple_selection ( ',' simple_selection )* ]
                      FROM table_name
                      [ USING update_parameter ( AND update_parameter )* ]
                      WHERE where_clause
                      [ IF ( EXISTS | condition ( AND condition )*) ]

DELETE FROM NerdMovies USING TIMESTAMP 1240003134 WHERE movie = 'Serenity';
*********************************************************************************************/

/******************************************************************************/
/*_delete_stmt() : _DELETE ( _simple_selection() _simple_selections() _delete_from() | _delete_from() )*/
/******************************************************************************/
boolean _delete_stmt(){
	currentStmt->type=delete_stmt;
	currentStmt->node._delete.keyspace=NULL;
	currentStmt->node._delete.table=NULL;
	currentStmt->node._delete.liRelation=creerListeRelation(NULL,NULL);
	boolean result;
	if(token==_DELETE){
		token=_lire_token();
		if(_simple_selection()){
			token=_lire_token();
			if(_simple_selections()){
				token=_lire_token();
				if(_delete_from()){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else if(_delete_from()){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_delete_from() : _FROM _table_name() ( _USING _update_parameter() _update_parameters() _delete_where() | 
_delete_where() )*/
/******************************************************************************/
boolean _delete_from(){
	boolean result;
	if(token==_FROM){
		token=_lire_token();
		tableName *tn=_create_tableName();
		if(_table_name(tn)){
			currentStmt->node._delete.keyspace=trouverKeyspace(liKey,tn,currentKeyspace,yylineno);
			if(currentStmt->node._delete.keyspace!=NULL){
				currentStmt->node._delete.table=_chercher_table(currentStmt->node._delete.keyspace,tn->nameTable,yylineno,false);
			}
			token=_lire_token();
			if(token==_USING){
				token=_lire_token();
				if(_update_parameter()){
					token=_lire_token();
					if(_update_parameters()){
						token=_lire_token();
						if(_delete_where()){
							result=true;
						}
						else{
							result=false;
						}
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else if(_delete_where()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_delete_where() : _WHERE _where_clause() (  follow( _PVIRG ) | _IF ( _EXISTS | 
_condition() _conditions() ) )*/
/******************************************************************************/
boolean _delete_where(){
	boolean result;
	if(token==_WHERE){
		token=_lire_token();
		currentStmt->node._delete.liRelation=creerListeRelation(NULL,NULL);
		if(_where_clause(currentStmt->node._delete.liRelation)){
            if(currentStmt->node._delete.table!=NULL){
                _valide_delete_where(currentStmt->node._delete.table,currentStmt->node._delete.liRelation,yylineno);
            }
			token=_lire_token();
			if(token==_PVIRG){
				follow_token = true;
				result=true;
			}
			else if(token==_IF){
				token=_lire_token();
				if(token==_EXISTS){
					result=true;
				}
				else if(_condition()){
					token=_lire_token();
					if(_conditions()){
						result=true;
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_simple_selections() :  follow( _FROM ) | _simple_selections_aux()*/
/******************************************************************************/
boolean _simple_selections(){
	boolean result;
	if(token==_FROM){
		follow_token = true;
		result=true;
	}
	else if(_simple_selections_aux()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_simple_selections_aux() : _VIRG _simple_selection() _simple_selections()*/
/******************************************************************************/
boolean _simple_selections_aux(){
	boolean result;
	if(token==_VIRG){
		token=_lire_token();
		if(_simple_selection()){
			token=_lire_token();
			if(_simple_selections()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/*************************************BATCH**************************************************

batch_statement        ::=  BEGIN [ UNLOGGED | COUNTER ] BATCH
                            [ USING update_parameter ( AND update_parameter )* ]
                            modification_statement ( ';' modification_statement )*
                            APPLY BATCH
modification_statement ::=  insert_statement | update_statement | delete_statement
*********************************************************************************************/

/******************************************************************************/
/*_batch_stmt() : _BEGIN ( _UNLOGGED _batch_using() | _COUNTER _batch_using() | _batch_using() )
*/
/******************************************************************************/
boolean _batch_stmt(){
	boolean result;
	if(token==_BEGIN){
		token=_lire_token();
		if(token==_UNLOGGED){
			token=_lire_token();
			if(_batch_using()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else if(token==_COUNTER){
			token=_lire_token();
			if(_batch_using()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else if(_batch_using()){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_batch_using() : _BATCH ( _USING _update_parameter() _update_parameters() _batch_modification() | _batch_modification() )
*/
/******************************************************************************/
boolean _batch_using(){
	boolean result;
	if(token==_BATCH){
		token=_lire_token();
		if(token==_USING){
			token=_lire_token();
			if(_update_parameter()){
				token=_lire_token();
				if(_update_parameters()){
					token=_lire_token();
					if(_batch_modification()){
						result=true;
					}
					else{
						result=false;
					}
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else if(_batch_modification()){
			result=true;
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_batch_modification() : _modification_statement() _modification_statements() _APPLY _BATCH*/
/******************************************************************************/
boolean _batch_modification(){
	boolean result;
	if(_modification_statement()){
		token=_lire_token();
		if(_modification_statements()){
			token=_lire_token();
			if(token==_APPLY){
				token=_lire_token();
				if(token==_BATCH){
					result=true;
				}
				else{
					result=false;
				}
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_modification_statements() :  follow( _APPLY ) | _modification_statements_aux()*/
/******************************************************************************/
boolean _modification_statements(){
	boolean result;
	if(token==_APPLY){
		follow_token = true;
		result=true;
	}
	else if(_modification_statements_aux()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_modification_statements_aux() : _PVIRG _modification_statement() _modification_statements()*/
/******************************************************************************/
boolean _modification_statements_aux(){
	boolean result;
	if(token==_PVIRG){
		token=_lire_token();
		if(_modification_statement()){
			token=_lire_token();
			if(_modification_statements()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_modification_statement() : _insert_stmt() | _update_stmt() | _delete_stmt()*/
/******************************************************************************/
boolean _modification_statement(){
	boolean result;
	if(_insert_stmt()){
		result=true;
	}
	else if(_update_stmt()){
		result=true;
	}
	else if(_delete_stmt()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/*************************************STATEMENTS********************************************

*********************************************************************************************/

/******************************************************************************/
/*_statement() : _create_stmts() | _alter_stmts() | _use_stmt() | _drop_stmts() | _truncate_stmt() | _insert_stmt() | 
_update_stmt() | _delete_stmt() | _batch_stmt() | _select_stmt()*/
/******************************************************************************/
boolean _statement(){
	boolean result;
	if(_create_stmts()){
		result=true;
	}
	else if(_alter_stmts()){
		result=true;
	}
	else if(_use_stmt()){
		result=true;
	}
	else if(_drop_stmts()){
		result=true;
	}
	else if(_truncate_stmt()){
		result=true;
	}
	else if(_insert_stmt()){
		result=true;
	}
	else if(_update_stmt()){
		result=true;
	}
	else if(_delete_stmt()){
		result=true;
	}
	else if(_batch_stmt()){
		result=true;
	}
	else if(_select_stmt()){
		result=true;
	}
    else if(_descripe()){
        result=true;
    }
	else{
		result=false;
	}
	return result;
}














/******************************************************************************/
/*_create_stmts() : _CREATE ( _TABLE _create_table_stmt() | _KEYSPACE _create_keyspace_stmt() | _TYPE _create_type_stmt() )
*/
/******************************************************************************/
boolean _create_stmts(){
	boolean result;
	if(token==_CREATE){
		token=_lire_token();
		if(token==_TABLE){
			token=_lire_token();
			if(_create_table_stmt()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else if(token==_KEYSPACE){
			token=_lire_token();
			if(_create_keyspace_stmt()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else if(token==_TYPE){
			token=_lire_token();
			if(_create_type_stmt()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_alter_stmts() : _ALTER ( _TABLE _alter_table_stmt() | _KEYSPACE _alter_keyspace_stmt() | _TYPE _alter_type_stmt() )
*/
/******************************************************************************/
boolean _alter_stmts(){
	boolean result;
	if(token==_ALTER){
		token=_lire_token();
		if(token==_TABLE){
			token=_lire_token();
			if(_alter_table_stmt()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else if(token==_KEYSPACE){
			token=_lire_token();
			if(_alter_keyspace_stmt()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else if(token==_TYPE){
			token=_lire_token();
			if(_alter_type_stmt()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_drop_stmts() : _DROP ( _TABLE _drop_table_stmt() | _KEYSPACE _drop_keyspace_stmt() | _TYPE _drop_type_stmt() )
*/
/******************************************************************************/
boolean _drop_stmts(){
	boolean result;
	if(token==_DROP){
		token=_lire_token();
		if(token==_TABLE){
			token=_lire_token();
			if(_drop_table_stmt()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else if(token==_KEYSPACE){
			token=_lire_token();
			if(_drop_keyspace_stmt()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else if(token==_TYPE){
			token=_lire_token();
			if(_drop_type_stmt()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_statements_main() : _statement() _PVIRG _statements()*/
/******************************************************************************/
boolean _statements_main(){
	boolean result;
	if(_statement()){
		token=_lire_token();
		if(token==_PVIRG){
            if(!_existsError()){
				if (debug){printf("excute\n");}
                _excuteStmt(currentStmt);
            }else{
				if (debug){printf("non excute\n");}
                _afficherErreur();
            }
			reintialseErreur();
			if(!read_from_file){
                printCqlsh();
			}
			token=_lire_token();
			if(_statements()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_statements() :  follow( _EOF ) | _statements_aux()*/
/******************************************************************************/
boolean _statements(){
	boolean result;
	if(token==_EOF || token==_EXIT || token==_QUIT){
		follow_token = true;
		result=true;
	}
	else if(_statements_aux()){
		result=true;
	}
	else{
		result=false;
	}
	return result;
}

/******************************************************************************/
/*_statements_aux() : _statement() _PVIRG _statements()*/
/******************************************************************************/
boolean _statements_aux(){
	boolean result;
	if(_statement()){
		token=_lire_token();
		if(token==_PVIRG){
            if(!_existsError()){
               if(debug){ printf("excute\n");}
                _excuteStmt(currentStmt);
            }else{
               if(debug) {printf("non excute\n");}
                if(read_from_file) {
                    _afficherErreur();
                }
                else{
                    _afficherErreurNotFile();
                }

            }
            reintialseStmt();
            reintialseErreur();
			if(!read_from_file){
                printCqlsh();
			}
			token=_lire_token();
			if(_statements()){
				result=true;
			}
			else{
				result=false;
			}
		}
		else{
			result=false;
		}
	}
	else{
		result=false;
	}
	return result;
}
