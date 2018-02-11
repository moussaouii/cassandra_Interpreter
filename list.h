#ifndef LIST_H
#define LIST_H
#define faux 0
#define vrai 1

#include "structure.h"
#include "list_aux.h"
typedef int booleen;

optionKeyspace *createOptionKeyspace(type_option to,char *idf,nodeOptionKeyspace node);

objetKeyspace *createKeyspace(char* name);
void printTerm(term *t);


void printMap(objetMap *oM);








/*============================================prototype=keyspace================================*/
// initialiser la liste pointée par li (cas général)
void initListeKey (ListeKey* li, char* (*toString) (objetKeyspace*),int (*comparer) (objetKeyspace*, objetKeyspace*));

ListeKey* creerListeKey (char* (*toString) (objetKeyspace*),int (*comparer) (objetKeyspace*, objetKeyspace*));
// la liste est-elle vide ?
int listeVideKey (ListeKey* li);

// fournir le nombre d'éléments dans la liste
int nbElementKey (ListeKey* li) ;

ElementKey* creerElementKey ();


// insérer objetKeyspace en tête de la liste li
// l'objetKeyspace est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeKey (ListeKey* li, objetKeyspace* ch);

// insérer un objetKeyspace en fin de la liste li
//void insererEnFinDeListeKey (ListeKey* li, objetKeyspace* ch);

// se positionner sur le premier élément de la liste li
void ouvrirListeKey (ListeKey* li);

// a-t-on atteint la fin de la liste li ?
int finListeKey (ListeKey* li);

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementKey* elementCourantKey (ListeKey* li);

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeKey (ListeKey* li, void (*f) (objetKeyspace*));

// fournir un pointeur sur l'objetKeyspace "objetCherche" de la liste li;
// NULL si l'objetKeyspace n'existe pas
objetKeyspace* chercherUnobjetKeyspace (ListeKey* li, objetKeyspace* objetCherche);

// et se positionner sur le suivant qui devient le courant
objetKeyspace* objetCourantKey (ListeKey* li);


void insererApresKey (ListeKey* li, ElementKey* precedent, objetKeyspace* objet) ;


// insérer un objet en fin de la liste li
void insererEnFinDeListeKey (ListeKey* li, objetKeyspace* objet) ;

// extraire l'objet en tête de la liste li
objetKeyspace* extraireEnTeteDeListeKey (ListeKey* li);


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetKeyspace* extraireApresKey (ListeKey* li, ElementKey* precedent) ;


// extraire l'objet en fin de la liste li
objetKeyspace* extraireEnFinDeListeKey (ListeKey* li);
// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetKeyspace (ListeKey* li, objetKeyspace* objet) ;


// parcours de liste avec destruction de chaque élément
void detruireListeKey (ListeKey* li) ;

// recopie l2 dans l1 et initialise l2
void recopierListeKey (ListeKey* l1, ListeKey* l2);




/*===========================prototype=option=keyspace===============================*/
// initialiser la liste pointée par li (cas général)
void initListeOptionKeyspace(ListeOptionKeyspace* li, char* (*toString) (optionKeyspace*),int (*comparer) (optionKeyspace*, optionKeyspace*));

ListeOptionKeyspace* creerListeOptionKeyspace(char* (*toString) (optionKeyspace*),int (*comparer) (optionKeyspace*, optionKeyspace*));
// la liste est-elle vide ?
int listeVideOptionKeyspace(ListeOptionKeyspace* li);

// fournir le nombre d'éléments dans la liste
int nbElementOptionKeyspace(ListeOptionKeyspace* li) ;

ElementOptionKeyspace* creerElementOptionKeyspace();


// insérer OptionKeyspace en tête de la liste li
// l'OptionKeyspace est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeOptionKeyspace(ListeOptionKeyspace* li, optionKeyspace* ch);

// insérer un OptionKeyspace en fin de la liste li
//void insererEnFinDeListeOptionKeyspace(ListeOptionKeyspace* li, OptionKeyspace* ch);

// se positionner sur le premier élément de la liste li
void ouvrirListeOptionKeyspace(ListeOptionKeyspace* li);

// a-t-on atteint la fin de la liste li ?
int finListeOptionKeyspace(ListeOptionKeyspace* li);

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementOptionKeyspace* elementCourantOptionKeyspace(ListeOptionKeyspace* li);

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeOptionKeyspace(ListeOptionKeyspace* li, void (*f) (optionKeyspace*));

// fournir un pointeur sur l'OptionKeyspace "objetCherche" de la liste li;
// NULL si l'OptionKeyspace n'existe pas
optionKeyspace* chercherUnOptionKeyspace (ListeOptionKeyspace* li, optionKeyspace* objetCherche);

// et se positionner sur le suivant qui devient le courant
optionKeyspace* objetCourantOptionKeyspace(ListeOptionKeyspace* li);


void insererApresOptionKeyspace(ListeOptionKeyspace* li, ElementOptionKeyspace* precedent, optionKeyspace* objet) ;


// insérer un objet en fin de la liste li
void insererEnFinDeListeOptionKeyspace(ListeOptionKeyspace* li, optionKeyspace* objet) ;

// extraire l'objet en tête de la liste li
optionKeyspace* extraireEnTeteDeListeOptionKeyspace(ListeOptionKeyspace* li);


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
optionKeyspace* extraireApresOptionKeyspace(ListeOptionKeyspace* li, ElementOptionKeyspace* precedent) ;


// extraire l'objet en fin de la liste li
optionKeyspace* extraireEnFinDeListeOptionKeyspace(ListeOptionKeyspace* li);
// extraire de la liste li, l'objet pointé par objet
booleen extraireUnOptionKeyspace (ListeOptionKeyspace* li, optionKeyspace* objet) ;


// parcours de liste avec destruction de chaque élément
void detruireListeOptionKeyspace(ListeOptionKeyspace* li) ;

// recopie l2 dans l1 et initialise l2
void recopierListeOptionKeyspace(ListeOptionKeyspace* l1, ListeOptionKeyspace* l2);

/*===============================Prototype=Option=Table==============================*/
void initListeOptionTable (ListeOptionTable* li, char* (*toString) (optionTable *),int (*comparer) (optionTable *, optionTable *));

ListeOptionTable* creerListeOptionTable (char* (*toString) (optionTable *),int (*comparer) (optionTable *, optionTable *));
// la liste est-elle vide ?
int listeVideOptionTable (ListeOptionTable* li);

// fournir le nombre d'éléments dans la liste
int nbElementOptionTable (ListeOptionTable* li) ;

ElementOptionTable* creerElementOptionTable ();


// insérer optionTable  en tête de la liste li
// l'optionTable  est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeOptionTable (ListeOptionTable* li, optionTable * ch);

// insérer un optionTable  en fin de la liste li
//void insererEnFinDeListeOptionTable (ListeOptionTable* li, optionTable * ch);

// se positionner sur le premier élément de la liste li
void ouvrirListeOptionTable (ListeOptionTable* li);

// a-t-on atteint la fin de la liste li ?
int finListeOptionTable (ListeOptionTable* li);

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementOptionTable* elementCourantOptionTable (ListeOptionTable* li);

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeOptionTable (ListeOptionTable* li, void (*f) (optionTable *));

// fournir un pointeur sur l'optionTable  "objetCherche" de la liste li;
// NULL si l'optionTable  n'existe pas
optionTable * chercherUnoptionTable  (ListeOptionTable* li, optionTable * objetCherche);

// et se positionner sur le suivant qui devient le courant
optionTable * objetCourantOptionTable (ListeOptionTable* li);


void insererApresOptionTable (ListeOptionTable* li, ElementOptionTable* precedent, optionTable * objet) ;


// insérer un objet en fin de la liste li
void insererEnFinDeListeOptionTable (ListeOptionTable* li, optionTable * objet) ;

// extraire l'objet en tête de la liste li
optionTable * extraireEnTeteDeListeOptionTable (ListeOptionTable* li);


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
optionTable * extraireApresOptionTable (ListeOptionTable* li, ElementOptionTable* precedent) ;


// extraire l'objet en fin de la liste li
optionTable * extraireEnFinDeListeOptionTable (ListeOptionTable* li);
// extraire de la liste li, l'objet pointé par objet
booleen extraireUnoptionTable  (ListeOptionTable* li, optionTable * objet) ;


// parcours de liste avec destruction de chaque élément
void detruireListeOptionTable (ListeOptionTable* li) ;

// recopie l2 dans l1 et initialise l2
void recopierListeOptionTable (ListeOptionTable* l1, ListeOptionTable* l2);

/*===============================prototype=Map====================================*/
// initialiser la liste pointée par li (cas général)
void initListeMap (ListeMap* li, char* (*toString) (objetMap*),int (*comparer) (objetMap*, objetMap*));

ListeMap* creerListeMap (char* (*toString) (objetMap*),int (*comparer) (objetMap*, objetMap*));
// la liste est-elle vide ?
int listeVideMap (ListeMap* li);

// fournir le nombre d'éléments dans la liste
int nbElementMap (ListeMap* li) ;

ElementMap* creerElementMap ();


// insérer objetMap en tête de la liste li
// l'objetMap est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeMap (ListeMap* li, objetMap* ch);

// insérer un objetMap en fin de la liste li
//void insererEnFinDeListeMap (ListeMap* li, objetMap* ch);

// se positionner sur le premier élément de la liste li
void ouvrirListeMap (ListeMap* li);

// a-t-on atteint la fin de la liste li ?
int finListeMap (ListeMap* li);

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementMap* elementCourantMap (ListeMap* li);

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeMap (ListeMap* li, void (*f) (objetMap*));

// fournir un pointeur sur l'objetMap "objetCherche" de la liste li;
// NULL si l'objetMap n'existe pas
objetMap* chercherUnobjetMap (ListeMap* li, objetMap* objetCherche);

// et se positionner sur le suivant qui devient le courant
objetMap* objetCourantMap (ListeMap* li);


void insererApresMap (ListeMap* li, ElementMap* precedent, objetMap* objet) ;


// insérer un objet en fin de la liste li
void insererEnFinDeListeMap (ListeMap* li, objetMap* objet) ;

// extraire l'objet en tête de la liste li
objetMap* extraireEnTeteDeListeMap (ListeMap* li);


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetMap* extraireApresMap (ListeMap* li, ElementMap* precedent) ;


// extraire l'objet en fin de la liste li
objetMap* extraireEnFinDeListeMap (ListeMap* li);
// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetMap (ListeMap* li, objetMap* objet) ;


// parcours de liste avec destruction de chaque élément
void detruireListeMap (ListeMap* li) ;

// recopie l2 dans l1 et initialise l2
void recopierListeMap (ListeMap* l1, ListeMap* l2);

/*================================prototype=table================================*/
// initialiser la liste pointée par li (cas général)
void initListeTable (ListeTable* li, char* (*toString) (objetTable*),int (*comparer) (objetTable*, objetTable*));

ListeTable* creerListeTable (char* (*toString) (objetTable*),int (*comparer) (objetTable*, objetTable*));
// la liste est-elle vide ?
int listeVideTable (ListeTable* li);

// fournir le nombre d'éléments dans la liste
int nbElementTable (ListeTable* li) ;

ElementTable* creerElementTable ();


// insérer objetTable en tête de la liste li
// l'objetTable est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeTable (ListeTable* li, objetTable* ch);

// insérer un objetTable en fin de la liste li
//void insererEnFinDeListeTable (ListeTable* li, objetTable* ch);

// se positionner sur le premier élément de la liste li
void ouvrirListeTable (ListeTable* li);

// a-t-on atteint la fin de la liste li ?
int finListeTable (ListeTable* li);

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementTable* elementCourantTable (ListeTable* li);

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeTable (ListeTable* li, void (*f) (objetTable*));

// fournir un pointeur sur l'objetTable "objetCherche" de la liste li;
// NULL si l'objetTable n'existe pas
objetTable* chercherUnobjetTable (ListeTable* li, objetTable* objetCherche);

// et se positionner sur le suivant qui devient le courant
objetTable* objetCourantTable (ListeTable* li);


void insererApresTable (ListeTable* li, ElementTable* precedent, objetTable* objet) ;


// insérer un objet en fin de la liste li
void insererEnFinDeListeTable (ListeTable* li, objetTable* objet) ;

// extraire l'objet en tête de la liste li
objetTable* extraireEnTeteDeListeTable (ListeTable* li);


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetTable* extraireApresTable (ListeTable* li, ElementTable* precedent) ;


// extraire l'objet en fin de la liste li
objetTable* extraireEnFinDeListeTable (ListeTable* li);
// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetTable (ListeTable* li, objetTable* objet) ;


// parcours de liste avec destruction de chaque élément
void detruireListeTable (ListeTable* li) ;

// recopie l2 dans l1 et initialise l2
void recopierListeTable (ListeTable* l1, ListeTable* l2);

/*===============================prototype=column====================================*/
// initialiser la liste pointée par li (cas général)
void initListeColumn (ListeColumn* li, char* (*toString) (objetColumn*),int (*comparer) (objetColumn*, objetColumn*));

ListeColumn* creerListeColumn (char* (*toString) (objetColumn*),int (*comparer) (objetColumn*, objetColumn*));
// la liste est-elle vide ?
int listeVideColumn (ListeColumn* li);

// fournir le nombre d'éléments dans la liste
int nbElementColumn (ListeColumn* li) ;

ElementColumn* creerElementColumn ();


// insérer objetColumn en tête de la liste li
// l'objetColumn est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeColumn (ListeColumn* li, objetColumn* ch);

// insérer un objetColumn en fin de la liste li
//void insererEnFinDeListeColumn (ListeColumn* li, objetColumn* ch);

// se positionner sur le premier élément de la liste li
void ouvrirListeColumn (ListeColumn* li);

// a-t-on atteint la fin de la liste li ?
int finListeColumn (ListeColumn* li);

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementColumn* elementCourantColumn (ListeColumn* li);

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeColumn (ListeColumn* li, void (*f) (objetColumn*));

// fournir un pointeur sur l'objetColumn "objetCherche" de la liste li;
// NULL si l'objetColumn n'existe pas
objetColumn* chercherUnobjetColumn (ListeColumn* li, objetColumn* objetCherche);

// et se positionner sur le suivant qui devient le courant
objetColumn* objetCourantColumn (ListeColumn* li);


void insererApresColumn (ListeColumn* li, ElementColumn* precedent, objetColumn* objet) ;


// insérer un objet en fin de la liste li
void insererEnFinDeListeColumn (ListeColumn* li, objetColumn* objet) ;

// extraire l'objet en tête de la liste li
objetColumn* extraireEnTeteDeListeColumn (ListeColumn* li);


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetColumn* extraireApresColumn (ListeColumn* li, ElementColumn* precedent) ;


// extraire l'objet en fin de la liste li
objetColumn* extraireEnFinDeListeColumn (ListeColumn* li);
// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetColumn (ListeColumn* li, objetColumn* objet) ;


// parcours de liste avec destruction de chaque élément
void detruireListeColumn (ListeColumn* li) ;

// recopie l2 dans l1 et initialise l2
void recopierListeColumn (ListeColumn* l1, ListeColumn* l2);

/*===============================prototype=Set====================================*/
// initialiser la liste pointée par li (cas général)
void initListeSet (ListeSet* li, char* (*toString) (objetSet*),int (*comparer) (objetSet*, objetSet*));

ListeSet* creerListeSet (char* (*toString) (objetSet*),int (*comparer) (objetSet*, objetSet*));
// la liste est-elle vide ?
int listeVideSet (ListeSet* li);

// fournir le nombre d'éléments dans la liste
int nbElementSet (ListeSet* li) ;

ElementSet* creerElementSet ();


// insérer objetSet en tête de la liste li
// l'objetSet est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeSet (ListeSet* li, objetSet* ch);

// insérer un objetSet en fin de la liste li
//void insererEnFinDeListeSet (ListeSet* li, objetSet* ch);

// se positionner sur le premier élément de la liste li
void ouvrirListeSet (ListeSet* li);

// a-t-on atteint la fin de la liste li ?
int finListeSet (ListeSet* li);

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementSet* elementCourantSet (ListeSet* li);

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeSet (ListeSet* li, void (*f) (objetSet*));

// fournir un pointeur sur l'objetSet "objetCherche" de la liste li;
// NULL si l'objetSet n'existe pas
objetSet* chercherUnobjetSet (ListeSet* li, objetSet* objetCherche);

// et se positionner sur le suivant qui devient le courant
objetSet* objetCourantSet (ListeSet* li);


void insererApresSet (ListeSet* li, ElementSet* precedent, objetSet* objet) ;


// insérer un objet en fin de la liste li
void insererEnFinDeListeSet (ListeSet* li, objetSet* objet) ;

// extraire l'objet en tête de la liste li
objetSet* extraireEnTeteDeListeSet (ListeSet* li);


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetSet* extraireApresSet (ListeSet* li, ElementSet* precedent) ;


// extraire l'objet en fin de la liste li
objetSet* extraireEnFinDeListeSet (ListeSet* li);
// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetSet (ListeSet* li, objetSet* objet) ;


// parcours de liste avec destruction de chaque élément
void detruireListeSet (ListeSet* li) ;

// recopie l2 dans l1 et initialise l2
void recopierListeSet (ListeSet* l1, ListeSet* l2);



/*===============================prototype=List====================================*/
// initialiser la liste pointée par li (cas général)
void initListeList (ListeList* li, char* (*toString) (term*),int (*comparer) (term*, term*));

ListeList* creerListeList (char* (*toString) (term*),int (*comparer) (term*, term*));
// la liste est-elle vide ?
int listeVideList (ListeList* li);

// fournir le nombre d'éléments dans la liste
int nbElementList (ListeList* li) ;

ElementList* creerElementList ();


// insérer objetList en tête de la liste li
// l'objetList est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeList (ListeList* li, term* ch);

// insérer un objetList en fin de la liste li
//void insererEnFinDeListeList (ListeList* li, term* ch);

// se positionner sur le premier élément de la liste li
void ouvrirListeList (ListeList* li);

// a-t-on atteint la fin de la liste li ?
int finListeList (ListeList* li);

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementList* elementCourantList (ListeList* li);

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeList (ListeList* li, void (*f) (term*));

// fournir un pointeur sur l'objetList "objetCherche" de la liste li;
// NULL si l'objetList n'existe pas
term* chercherUnobjetList (ListeList* li, term* objetCherche);

// et se positionner sur le suivant qui devient le courant
term* objetCourantList (ListeList* li);


void insererApresList (ListeList* li, ElementList* precedent, term* objet) ;


// insérer un objet en fin de la liste li
void insererEnFinDeListeList (ListeList* li, term* objet) ;

// extraire l'objet en tête de la liste li
term* extraireEnTeteDeListeList (ListeList* li);


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
term* extraireApresList (ListeList* li, ElementList* precedent) ;


// extraire l'objet en fin de la liste li
term* extraireEnFinDeListeList (ListeList* li);
// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetList (ListeList* li, term* objet) ;


// parcours de liste avec destruction de chaque élément
void detruireListeList (ListeList* li) ;

// recopie l2 dans l1 et initialise l2
void recopierListeList (ListeList* l1, ListeList* l2);

/*===============================prototype=TupleT====================================*/
// initialiser la liste pointée par li (cas général)
void initListeTupleT (ListeTupleT* li, char* (*toString) (objetTupleT*),int (*comparer) (objetTupleT*, objetTupleT*));

ListeTupleT* creerListeTupleT (char* (*toString) (objetTupleT*),int (*comparer) (objetTupleT*, objetTupleT*));
// la liste est-elle vide ?
int listeVideTupleT (ListeTupleT* li);

// fournir le nombre d'éléments dans la liste
int nbElementTupleT (ListeTupleT* li) ;

ElementTupleT* creerElementTupleT ();


// insérer objetTupleT en tête de la liste li
// l'objetTupleT est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeTupleT (ListeTupleT* li, objetTupleT* ch);

// insérer un objetTupleT en fin de la liste li
//void insererEnFinDeListeTupleT (ListeTupleT* li, objetTupleT* ch);

// se positionner sur le premier élément de la liste li
void ouvrirListeTupleT (ListeTupleT* li);

// a-t-on atteint la fin de la liste li ?
int finListeTupleT (ListeTupleT* li);

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementTupleT* elementCourantTupleT (ListeTupleT* li);

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeTupleT (ListeTupleT* li, void (*f) (objetTupleT*));

// fournir un pointeur sur l'objetTupleT "objetCherche" de la liste li;
// NULL si l'objetTupleT n'existe pas
objetTupleT* chercherUnobjetTupleT (ListeTupleT* li, objetTupleT* objetCherche);

// et se positionner sur le suivant qui devient le courant
objetTupleT* objetCourantTupleT (ListeTupleT* li);


void insererApresTupleT (ListeTupleT* li, ElementTupleT* precedent, objetTupleT* objet) ;


// insérer un objet en fin de la liste li
void insererEnFinDeListeTupleT (ListeTupleT* li, objetTupleT* objet) ;

// extraire l'objet en tête de la liste li
objetTupleT* extraireEnTeteDeListeTupleT (ListeTupleT* li);


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetTupleT* extraireApresTupleT (ListeTupleT* li, ElementTupleT* precedent) ;


// extraire l'objet en fin de la liste li
objetTupleT* extraireEnFinDeListeTupleT (ListeTupleT* li);
// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetTupleT (ListeTupleT* li, objetTupleT* objet) ;


// parcours de liste avec destruction de chaque élément
void detruireListeTupleT (ListeTupleT* li) ;

// recopie l2 dans l1 et initialise l2
void recopierListeTupleT (ListeTupleT* l1, ListeTupleT* l2);

/*===============================prototype=Udt====================================*/
// initialiser la liste pointée par li (cas général)
void initListeUdt (ListeUdt* li, char* (*toString) (objetUdt*),int (*comparer) (objetUdt*, objetUdt*));

ListeUdt* creerListeUdt (char* (*toString) (objetUdt*),int (*comparer) (objetUdt*, objetUdt*));
// la liste est-elle vide ?
int listeVideUdt (ListeUdt* li);

// fournir le nombre d'éléments dans la liste
int nbElementUdt (ListeUdt* li) ;

ElementUdt* creerElementUdt ();


// insérer objetUdt en tête de la liste li
// l'objetUdt est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeUdt (ListeUdt* li, objetUdt* ch);

// insérer un objetUdt en fin de la liste li
//void insererEnFinDeListeUdt (ListeUdt* li, objetUdt* ch);

// se positionner sur le premier élément de la liste li
void ouvrirListeUdt (ListeUdt* li);

// a-t-on atteint la fin de la liste li ?
int finListeUdt (ListeUdt* li);

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementUdt* elementCourantUdt (ListeUdt* li);

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeUdt (ListeUdt* li, void (*f) (objetUdt*));

// fournir un pointeur sur l'objetUdt "objetCherche" de la liste li;
// NULL si l'objetUdt n'existe pas
objetUdt* chercherUnobjetUdt (ListeUdt* li, objetUdt* objetCherche);

// et se positionner sur le suivant qui devient le courant
objetUdt* objetCourantUdt (ListeUdt* li);


void insererApresUdt (ListeUdt* li, ElementUdt* precedent, objetUdt* objet) ;


// insérer un objet en fin de la liste li
void insererEnFinDeListeUdt (ListeUdt* li, objetUdt* objet) ;

// extraire l'objet en tête de la liste li
objetUdt* extraireEnTeteDeListeUdt (ListeUdt* li);


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetUdt* extraireApresUdt (ListeUdt* li, ElementUdt* precedent) ;


// extraire l'objet en fin de la liste li
objetUdt* extraireEnFinDeListeUdt (ListeUdt* li);
// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetUdt (ListeUdt* li, objetUdt* objet) ;


// parcours de liste avec destruction de chaque élément
void detruireListeUdt (ListeUdt* li) ;

// recopie l2 dans l1 et initialise l2
void recopierListeUdt (ListeUdt* l1, ListeUdt* l2);

/*===============================prototype=column====================================*/
// initialiser la liste pointée par li (cas général)
void initListeTypesUdt (ListeTypesUdt* li, char* (*toString) (objetTypesUdt*),int (*comparer) (objetTypesUdt*, objetTypesUdt*));

ListeTypesUdt* creerListeTypesUdt (char* (*toString) (objetTypesUdt*),int (*comparer) (objetTypesUdt*, objetTypesUdt*));
// la liste est-elle vide ?
int listeVideTypesUdt (ListeTypesUdt* li);

// fournir le nombre d'éléments dans la liste
int nbElementTypesUdt (ListeTypesUdt* li) ;

ElementTypesUdt* creerElementTypesUdt ();


// insérer objetTypesUdt en tête de la liste li
// l'objetTypesUdt est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeTypesUdt (ListeTypesUdt* li, objetTypesUdt* ch);

// insérer un objetTypesUdt en fin de la liste li
//void insererEnFinDeListeTypesUdt (ListeTypesUdt* li, objetTypesUdt* ch);

// se positionner sur le premier élément de la liste li
void ouvrirListeTypesUdt (ListeTypesUdt* li);

// a-t-on atteint la fin de la liste li ?
int finListeTypesUdt (ListeTypesUdt* li);

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementTypesUdt* elementCourantTypesUdt (ListeTypesUdt* li);

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeTypesUdt (ListeTypesUdt* li, void (*f) (objetTypesUdt*));

// fournir un pointeur sur l'objetTypesUdt "objetCherche" de la liste li;
// NULL si l'objetTypesUdt n'existe pas
objetTypesUdt* chercherUnobjetTypesUdt (ListeTypesUdt* li, objetTypesUdt* objetCherche);

// et se positionner sur le suivant qui devient le courant
objetTypesUdt* objetCourantTypesUdt (ListeTypesUdt* li);


void insererApresTypesUdt (ListeTypesUdt* li, ElementTypesUdt* precedent, objetTypesUdt* objet) ;


// insérer un objet en fin de la liste li
void insererEnFinDeListeTypesUdt (ListeTypesUdt* li, objetTypesUdt* objet) ;

// extraire l'objet en tête de la liste li
objetTypesUdt* extraireEnTeteDeListeTypesUdt (ListeTypesUdt* li);


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetTypesUdt* extraireApresTypesUdt (ListeTypesUdt* li, ElementTypesUdt* precedent) ;


// extraire l'objet en fin de la liste li
objetTypesUdt* extraireEnFinDeListeTypesUdt (ListeTypesUdt* li);
// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetTypesUdt (ListeTypesUdt* li, objetTypesUdt* objet) ;


// parcours de liste avec destruction de chaque élément
void detruireListeTypesUdt (ListeTypesUdt* li) ;

// recopie l2 dans l1 et initialise l2
void recopierListeTypesUdt (ListeTypesUdt* l1, ListeTypesUdt* l2);
/*===============================prototype=NameVAlue====================================*/
// initialiser la liste pointée par li (cas général)
void initListeNameValue (ListeNameValue* li, char* (*toString) (objetNameValue*),int (*comparer) (objetNameValue*, objetNameValue*));

ListeNameValue* creerListeNameValue (char* (*toString) (objetNameValue*),int (*comparer) (objetNameValue*, objetNameValue*));
// la liste est-elle vide ?
int listeVideNameValue (ListeNameValue* li);

// fournir le nombre d'éléments dans la liste
int nbElementNameValue (ListeNameValue* li) ;

ElementNameValue* creerElementNameValue ();


// insérer objetNameValue en tête de la liste li
// l'objetNameValue est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeNameValue (ListeNameValue* li, objetNameValue* ch);

// insérer un objetNameValue en fin de la liste li
//void insererEnFinDeListeNameValue (ListeNameValue* li, objetNameValue* ch);

// se positionner sur le premier élément de la liste li
void ouvrirListeNameValue (ListeNameValue* li);

// a-t-on atteint la fin de la liste li ?
int finListeNameValue (ListeNameValue* li);

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementNameValue* elementCourantNameValue (ListeNameValue* li);

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeNameValue (ListeNameValue* li, void (*f) (objetNameValue*));

// fournir un pointeur sur l'objetNameValue "objetCherche" de la liste li;
// NULL si l'objetNameValue n'existe pas
objetNameValue* chercherUnobjetNameValue (ListeNameValue* li, objetNameValue* objetCherche);

// et se positionner sur le suivant qui devient le courant
objetNameValue* objetCourantNameValue (ListeNameValue* li);


void insererApresNameValue (ListeNameValue* li, ElementNameValue* precedent, objetNameValue* objet) ;


// insérer un objet en fin de la liste li
void insererEnFinDeListeNameValue (ListeNameValue* li, objetNameValue* objet) ;

// extraire l'objet en tête de la liste li
objetNameValue* extraireEnTeteDeListeNameValue (ListeNameValue* li);


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetNameValue* extraireApresNameValue (ListeNameValue* li, ElementNameValue* precedent) ;


// extraire l'objet en fin de la liste li
objetNameValue* extraireEnFinDeListeNameValue (ListeNameValue* li);
// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetNameValue (ListeNameValue* li, objetNameValue* objet) ;


// parcours de liste avec destruction de chaque élément
void detruireListeNameValue (ListeNameValue* li) ;

// recopie l2 dans l1 et initialise l2
void recopierListeNameValue (ListeNameValue* l1, ListeNameValue* l2);

/*===============================prototype=Tuple====================================*/
// initialiser la liste pointée par li (cas général)
void initListeTuple (ListeTuple* li, char* (*toString) (objetTuple*),int (*comparer) (objetTuple*, objetTuple*));

ListeTuple* creerListeTuple (char* (*toString) (objetTuple*),int (*comparer) (objetTuple*, objetTuple*));
// la liste est-elle vide ?
int listeVideTuple (ListeTuple* li);

// fournir le nombre d'éléments dans la liste
int nbElementTuple (ListeTuple* li) ;

ElementTuple* creerElementTuple ();


// insérer objetTuple en tête de la liste li
// l'objetTuple est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeTuple (ListeTuple* li, objetTuple* ch);

// insérer un objetTuple en fin de la liste li
//void insererEnFinDeListeTuple (ListeTuple* li, objetTuple* ch);

// se positionner sur le premier élément de la liste li
void ouvrirListeTuple (ListeTuple* li);

// a-t-on atteint la fin de la liste li ?
int finListeTuple (ListeTuple* li);

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementTuple* elementCourantTuple (ListeTuple* li);

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeTuple (ListeTuple* li, void (*f) (objetTuple*));

// fournir un pointeur sur l'objetTuple "objetCherche" de la liste li;
// NULL si l'objetTuple n'existe pas
objetTuple* chercherUnobjetTuple (ListeTuple* li, objetTuple* objetCherche);

// et se positionner sur le suivant qui devient le courant
objetTuple* objetCourantTuple (ListeTuple* li);


void insererApresTuple (ListeTuple* li, ElementTuple* precedent, objetTuple* objet) ;


// insérer un objet en fin de la liste li
void insererEnFinDeListeTuple (ListeTuple* li, objetTuple* objet) ;

// extraire l'objet en tête de la liste li
objetTuple* extraireEnTeteDeListeTuple (ListeTuple* li);


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetTuple* extraireApresTuple (ListeTuple* li, ElementTuple* precedent) ;


// extraire l'objet en fin de la liste li
objetTuple* extraireEnFinDeListeTuple (ListeTuple* li);
// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetTuple (ListeTuple* li, objetTuple* objet) ;


// parcours de liste avec destruction de chaque élément
void detruireListeTuple (ListeTuple* li) ;

// recopie l2 dans l1 et initialise l2
void recopierListeTuple (ListeTuple* l1, ListeTuple* l2);


/*===============================prototype=column====================================*/
// initialiser la liste pointée par li (cas général)
void initListeName (ListeName* li, char* (*toString) (char*),int (*comparer) (char*, char*));

ListeName* creerListeName (char* (*toString) (char*),int (*comparer) (char*, char*));
// la liste est-elle vide ?
int listeVideName (ListeName* li);

// fournir le nombre d'éléments dans la liste
int nbElementName (ListeName* li) ;

ElementName* creerElementName ();


// insérer char en tête de la liste li
// l'char est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeName (ListeName* li, char* ch);

// insérer un char en fin de la liste li
//void insererEnFinDeListeName (ListeName* li, char* ch);

// se positionner sur le premier élément de la liste li
void ouvrirListeName (ListeName* li);

// a-t-on atteint la fin de la liste li ?
int finListeName (ListeName* li);

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementName* elementCourantName (ListeName* li);

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeName (ListeName* li, void (*f) (char*));

// fournir un pointeur sur l'char "objetCherche" de la liste li;
// NULL si l'char n'existe pas
char* chercherUnchar (ListeName* li, char* objetCherche);

// et se positionner sur le suivant qui devient le courant
char* objetCourantName (ListeName* li);


void insererApresName (ListeName* li, ElementName* precedent, char* objet) ;


// insérer un objet en fin de la liste li
void insererEnFinDeListeName (ListeName* li, char* objet) ;

// extraire l'objet en tête de la liste li
char* extraireEnTeteDeListeName (ListeName* li);


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
char* extraireApresName (ListeName* li, ElementName* precedent) ;


// extraire l'objet en fin de la liste li
char* extraireEnFinDeListeName (ListeName* li);
// extraire de la liste li, l'objet pointé par objet
booleen extraireUnchar (ListeName* li, char* objet) ;


// parcours de liste avec destruction de chaque élément
void detruireListeName (ListeName* li) ;

// recopie l2 dans l1 et initialise l2
void recopierListeName (ListeName* l1, ListeName* l2);


/*===============================prototype=relation====================================*/
// initialiser la liste pointée par li (cas général)
        void initListeRelation (ListeRelation* li, char* (*toString) (objetRelation*),int (*comparer) (objetRelation*, objetRelation*));

        ListeRelation* creerListeRelation (char* (*toString) (objetRelation*),int (*comparer) (objetRelation*, objetRelation*));
// la liste est-elle vide ?
        int listeVideRelation (ListeRelation* li);

// fournir le nombre d'éléments dans la liste
        int nbElementRelation (ListeRelation* li) ;

        ElementRelation* creerElementRelation ();


// insérer objetRelation en tête de la liste li
// l'objetRelation est repéré par le champ reference de l'élément de la liste
        void insererEnTeteDeListeRelation (ListeRelation* li, objetRelation* ch);

// insérer un objetRelation en fin de la liste li
//void insererEnFinDeListeRelation (ListeRelation* li, objetRelation* ch);

// se positionner sur le premier élément de la liste li
        void ouvrirListeRelation (ListeRelation* li);

// a-t-on atteint la fin de la liste li ?
        int finListeRelation (ListeRelation* li);

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
        ElementRelation* elementCourantRelation (ListeRelation* li);

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
        void listerListeRelation (ListeRelation* li, void (*f) (objetRelation*));

// fournir un pointeur sur l'objetRelation "objetCherche" de la liste li;
// NULL si l'objetRelation n'existe pas
        objetRelation* chercherUnobjetRelation (ListeRelation* li, objetRelation* objetCherche);

// et se positionner sur le suivant qui devient le courant
        objetRelation* objetCourantRelation (ListeRelation* li);


        void insererApresRelation (ListeRelation* li, ElementRelation* precedent, objetRelation* objet) ;


// insérer un objet en fin de la liste li
        void insererEnFinDeListeRelation (ListeRelation* li, objetRelation* objet) ;

// extraire l'objet en tête de la liste li
        objetRelation* extraireEnTeteDeListeRelation (ListeRelation* li);


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
        objetRelation* extraireApresRelation (ListeRelation* li, ElementRelation* precedent) ;


// extraire l'objet en fin de la liste li
        objetRelation* extraireEnFinDeListeRelation (ListeRelation* li);
// extraire de la liste li, l'objet pointé par objet
        booleen extraireUnobjetRelation (ListeRelation* li, objetRelation* objet) ;


// parcours de liste avec destruction de chaque élément
        void detruireListeRelation (ListeRelation* li) ;

// recopie l2 dans l1 et initialise l2
        void recopierListeRelation (ListeRelation* l1, ListeRelation* l2);

/*===============================prototype=selector====================================*/
// initialiser la liste pointée par li (cas général)
void initListeSelector (ListeSelector* li, char* (*toString) (objetSelector*),int (*comparer) (objetSelector*, objetSelector*));

ListeSelector* creerListeSelector (char* (*toString) (objetSelector*),int (*comparer) (objetSelector*, objetSelector*));
// la liste est-elle vide ?
int listeVideSelector (ListeSelector* li);

// fournir le nombre d'éléments dans la liste
int nbElementSelector (ListeSelector* li) ;

ElementSelector* creerElementSelector ();


// insérer objetSelector en tête de la liste li
// l'objetSelector est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeSelector (ListeSelector* li, objetSelector* ch);

// insérer un objetSelector en fin de la liste li
//void insererEnFinDeListeSelector (ListeSelector* li, objetSelector* ch);

// se positionner sur le premier élément de la liste li
void ouvrirListeSelector (ListeSelector* li);

// a-t-on atteint la fin de la liste li ?
int finListeSelector (ListeSelector* li);

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementSelector* elementCourantSelector (ListeSelector* li);

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeSelector (ListeSelector* li, void (*f) (objetSelector*));

// fournir un pointeur sur l'objetSelector "objetCherche" de la liste li;
// NULL si l'objetSelector n'existe pas
objetSelector* chercherUnobjetSelector (ListeSelector* li, objetSelector* objetCherche);

// et se positionner sur le suivant qui devient le courant
objetSelector* objetCourantSelector (ListeSelector* li);


void insererApresSelector (ListeSelector* li, ElementSelector* precedent, objetSelector* objet) ;


// insérer un objet en fin de la liste li
void insererEnFinDeListeSelector (ListeSelector* li, objetSelector* objet) ;

// extraire l'objet en tête de la liste li
objetSelector* extraireEnTeteDeListeSelector (ListeSelector* li);


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetSelector* extraireApresSelector (ListeSelector* li, ElementSelector* precedent) ;


// extraire l'objet en fin de la liste li
objetSelector* extraireEnFinDeListeSelector (ListeSelector* li);
// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetSelector (ListeSelector* li, objetSelector* objet) ;


// parcours de liste avec destruction de chaque élément
void detruireListeSelector (ListeSelector* li) ;

// recopie l2 dans l1 et initialise l2
void recopierListeSelector (ListeSelector* l1, ListeSelector* l2);

/*===============================prototype=data====================================*/
// initialiser la liste pointée par li (cas général)
void initListeData (ListeData* li, char* (*toString) (objetData*),int (*comparer) (objetData*, objetData*));

ListeData* creerListeData (char* (*toString) (objetData*),int (*comparer) (objetData*, objetData*));
// la liste est-elle vide ?
int listeVideData (ListeData* li);

// fournir le nombre d'éléments dans la liste
int nbElementData (ListeData* li) ;

ElementData* creerElementData ();


// insérer objetData en tête de la liste li
// l'objetData est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeData (ListeData* li, objetData* ch);

// insérer un objetData en fin de la liste li
//void insererEnFinDeListeData (ListeData* li, objetData* ch);

// se positionner sur le premier élément de la liste li
void ouvrirListeData (ListeData* li);

// a-t-on atteint la fin de la liste li ?
int finListeData (ListeData* li);

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementData* elementCourantData (ListeData* li);

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeData (ListeData* li, void (*f) (objetData*));

// fournir un pointeur sur l'objetData "objetCherche" de la liste li;
// NULL si l'objetData n'existe pas
objetData* chercherUnobjetData (ListeData* li, objetData* objetCherche);

// et se positionner sur le suivant qui devient le courant
objetData* objetCourantData (ListeData* li);


void insererApresData (ListeData* li, ElementData* precedent, objetData* objet) ;


// insérer un objet en fin de la liste li
void insererEnFinDeListeData (ListeData* li, objetData* objet) ;

// extraire l'objet en tête de la liste li
objetData* extraireEnTeteDeListeData (ListeData* li);


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetData* extraireApresData (ListeData* li, ElementData* precedent) ;


// extraire l'objet en fin de la liste li
objetData* extraireEnFinDeListeData (ListeData* li);
// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetData (ListeData* li, objetData* objet) ;


// parcours de liste avec destruction de chaque élément
void detruireListeData (ListeData* li) ;

// recopie l2 dans l1 et initialise l2
void recopierListeData (ListeData* l1, ListeData* l2);


#endif




















//void afficherKeyspace(objetKeyspace *oks);
//
//
//
//
//
//
//
//
//void _createKeyspaceStmt(ListeKey *li,createKeyspaceStmt *cKS);
//
//void printKeyspace(objetKeyspace *oks);
//int comparerKeyspace(objetKeyspace* objet1, objetKeyspace* objet2);
//char* toStringKeyspace(objetKeyspace *oks);
//
//
//
//optionKeyspace *createOptionKeyspace(type_option to,char *idf,nodeOptionKeyspace node);
//
//objetKeyspace *createKeyspace(char* name);