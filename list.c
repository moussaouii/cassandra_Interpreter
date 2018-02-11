#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "structure.h"
#define debug 0










/*=======================================Keyspace=======================================*/

// initialiser la liste pointée par li (cas général)
void initListeKey (ListeKey* li, char* (*toString) (objetKeyspace*),int (*comparer) (objetKeyspace*, objetKeyspace*)) {
	li->premier = NULL;
	li->dernier = NULL;
	li->courant = NULL;
	li->nbElt= 0;
	li->toString = toString;
	li->comparer = comparerKeyspace;
}

ListeKey* creerListeKey (char* (*toString) (objetKeyspace*),int (*comparer) (objetKeyspace*, objetKeyspace*)) {
	ListeKey* li =(ListeKey *)malloc(sizeof(ListeKey));
	initListeKey (li, toString, comparer);
	return li;
}

// la liste est-elle vide ?
int listeVideKey (ListeKey* li) {
	return li->nbElt == 0;
}

// fournir le nombre d'éléments dans la liste
int nbElementKey (ListeKey* li) {
	return li->nbElt;
}

ElementKey* creerElementKey () {
	return (ElementKey *)malloc(sizeof(ElementKey));
}


// insérer objetKeyspace en tête de la liste li
// l'objetKeyspace est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeKey (ListeKey* li, objetKeyspace* ch) {
	ElementKey* nouveau= creerElementKey();
	nouveau->reference = ch;
	nouveau->suivant=li->premier;
	li->premier= nouveau;
	if (li->dernier == NULL) li->dernier = nouveau;
	li->nbElt++;
}


// insérer un objetKeyspace en fin de la liste li
/*void insererEnFinDeListe (ListeKey* li, objetKeyspace* ch) {
	ElementKey* nouveaux= creerElementKey();
	nouveaux->reference = ch;
	nouveaux->suivant=NULL;
	if(li->premier==NULL){
		li->premier=nouveaux;
	}
	else{
		li->dernier->suivant=nouveaux;	
	}
	li->dernier=nouveaux;
}*/

// se positionner sur le premier élément de la liste li
void ouvrirListeKey (ListeKey* li) {
	li->courant = li->premier;
}


int finListeKey (ListeKey* li) {
	return li->courant==NULL;
}


int finListeSuivantKey (ListeKey* li) {
	return li->courant->suivant==NULL;
}

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementKey* elementCourantKey (ListeKey* li) {
	ElementKey* ptc = li->courant;
	if (li->courant != NULL) {
		li->courant = li->courant->suivant;
	}
	return ptc;
}

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeKey (ListeKey* li, void (*f) (objetKeyspace*)) {
	ouvrirListeKey (li);
	while (!finListeKey (li)) {
		objetKeyspace* ch = objetCourantKey (li);
		f (ch); // appliquer la fonction f() à objetKeyspace
	}
}

// fournir un pointeur sur l'objetKeyspace "objetCherche" de la liste li;
// NULL si l'objetKeyspace n'existe pas
objetKeyspace* chercherUnobjetKeyspace (ListeKey* li, objetKeyspace* objetCherche) {
	int trouve=0;
	void* ch;
	// pointeur courant
	ouvrirListeKey (li);
	while (!finListeKey (li) && !trouve) {
		ch = objetCourantKey (li);
		trouve = li->comparer (objetCherche, ch) == 0;
	}
	return trouve ? ch : NULL;
}

// fournir un pointeur sur l'objet courant de la liste li,
// et se positionner sur le suivant qui devient le courant
objetKeyspace* objetCourantKey (ListeKey* li) {
	ElementKey* ptc = elementCourantKey (li);
	return ptc==NULL ? NULL : ptc->reference;
}

// insérer dans la liste li, objet après precedent
// si precedent est NULL, insérer en tête de liste
void insererApresKey (ListeKey* li, ElementKey* precedent, objetKeyspace* objet) {
	if (precedent == NULL) {
		insererEnTeteDeListeKey (li, objet);
	} else {
		ElementKey* nouveau= creerElementKey();
		nouveau->reference = objet;
		nouveau->suivant= precedent->suivant;
		precedent->suivant = nouveau;
		if (precedent == li->dernier) 
			li->dernier = nouveau;
		li->nbElt++;
	}
}


// insérer un objet en fin de la liste li
void insererEnFinDeListeKey (ListeKey* li, objetKeyspace* objet) {
	insererApresKey (li, li->dernier, objet);
}

// extraire l'objet en tête de la liste li
objetKeyspace* extraireEnTeteDeListeKey (ListeKey* li) {
	ElementKey* extrait = li->premier;
	if (!listeVideKey(li)) {
		li->premier = li->premier->suivant;
		if (li->premier==NULL) 
			li->dernier=NULL; // Liste devenue vide
		li->nbElt--;
	}
	return extrait != NULL ? extrait->reference : NULL;
}


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetKeyspace* extraireApresKey (ListeKey* li, ElementKey* precedent) {
	if (precedent == NULL) {
		return extraireEnTeteDeListeKey (li);
	} else {
		ElementKey* extrait = precedent->suivant;
		if (extrait != NULL) {
			precedent->suivant = extrait->suivant;
		if (extrait == li->dernier) li->dernier = precedent;
			li->nbElt--;
		}
		return extrait != NULL ? extrait->reference : NULL;
	}
}


// extraire l'objet en fin de la liste li
objetKeyspace* extraireEnFinDeListeKey (ListeKey* li) {
	objetKeyspace* extrait;
	if (listeVideKey(li)) {
		extrait = NULL;
	} else if (li->premier == li->dernier) { // un seul élément
		extrait = extraireEnTeteDeListeKey (li);
	} else {
		ElementKey* ptc = li->premier;
	while (ptc->suivant != li->dernier) ptc = ptc->suivant;
		extrait = extraireApresKey (li, ptc);
	}
	return extrait;
}


// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetKeyspace (ListeKey* li, objetKeyspace* objet) {
	ElementKey* precedent = NULL;
	ElementKey* ptc= NULL;
	// repère l'élément précédent
	booleen trouve = faux;
	ouvrirListeKey (li);
	while (!finListeKey (li) && !trouve) {
		precedent = ptc;
		ptc= elementCourantKey (li);
		trouve = (ptc->reference == objet) ? vrai : faux;
	}
	if (!trouve)
		 return faux;
	objetKeyspace* extrait = extraireApresKey (li, precedent);
	return vrai;
}


// parcours de liste avec destruction de chaque élément
void detruireListeKey (ListeKey* li) {
	ouvrirListeKey (li);
	while (!finListeKey (li)) {
		ElementKey* ptc = elementCourantKey (li);
		free (ptc->reference); //si on veut détruire les objets de la liste
		free (ptc);
	}
	initListeKey (li,NULL,NULL);
}

// recopie l2 dans l1 et initialise l2
void recopierListeKey (ListeKey* l1, ListeKey* l2) {
	detruireListeKey (l1);
	*l1 = *l2;
	// on recopie les têtes de listes
}


/*==========================================Table===========================================*/


// initialiser la liste pointée par li (cas général)
void initListeTable (ListeTable* li, char* (*toString) (objetTable*),int (*comparer) (objetTable*, objetTable*)) {
	li->premier = NULL;
	li->dernier = NULL;
	li->courant = NULL;
	li->nbElt= 0;
	li->toString = toString;
	li->comparer = comparerTable;
}

ListeTable* creerListeTable (char* (*toString) (objetTable*),int (*comparer) (objetTable*, objetTable*)) {
	ListeTable* li =(ListeTable *)malloc(sizeof(ListeTable));
	initListeTable (li, toString, comparer);
	return li;
}

// la liste est-elle vide ?
int listeVideTable (ListeTable* li) {
	return li->nbElt == 0;
}

// fournir le nombre d'éléments dans la liste
int nbElementTable (ListeTable* li) {
	return li->nbElt;
}

ElementTable* creerElementTable () {
	return (ElementTable *)malloc(sizeof(ElementTable));
}


// insérer objetTable en tête de la liste li
// l'objetTable est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeTable (ListeTable* li, objetTable* ch) {
	ElementTable* nouveau= creerElementTable();
	nouveau->reference = ch;
	nouveau->suivant= li->premier;
	li->premier= nouveau;
	if (li->dernier == NULL) li->dernier = nouveau;
	li->nbElt++;
}


// insérer un objetTable en fin de la liste li
/*void insererEnFinDeListe (ListeTable* li, objetTable* ch) {
	ElementTable* nouveaux= creerElementTable();
	nouveaux->reference = ch;
	nouveaux->suivant=NULL;
	if(li->premier==NULL){
		li->premier=nouveaux;
	}
	else{
		li->dernier->suivant=nouveaux;	
	}
	li->dernier=nouveaux;
}*/

// se positionner sur le premier élément de la liste li
void ouvrirListeTable (ListeTable* li) {
	li->courant = li->premier;
}


int finListeTable (ListeTable* li) {
	return li->courant==NULL;
}


int finListeSuivantTable (ListeTable* li) {
	return li->courant->suivant==NULL;
}

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementTable* elementCourantTable (ListeTable* li) {
	ElementTable* ptc = li->courant;
	if (li->courant != NULL) {
		li->courant = li->courant->suivant;
	}
	return ptc;
}

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeTable (ListeTable* li, void (*f) (objetTable*)) {
	ouvrirListeTable (li);
	while (!finListeTable (li)) {
		objetTable* ch = objetCourantTable (li);
		f (ch); // appliquer la fonction f() à objetTable
	}
}

// fournir un pointeur sur l'objetTable "objetCherche" de la liste li;
// NULL si l'objetTable n'existe pas
objetTable* chercherUnobjetTable (ListeTable* li, objetTable* objetCherche) {
	int trouve=0;
	void* ch;
	// pointeur courant
	ouvrirListeTable (li);
	while (!finListeTable (li) && !trouve) {
		ch = objetCourantTable (li);
		trouve = li->comparer (objetCherche, ch) == 0;
	}
	return trouve ? ch : NULL;
}

// fournir un pointeur sur l'objet courant de la liste li,
// et se positionner sur le suivant qui devient le courant
objetTable* objetCourantTable (ListeTable* li) {
	ElementTable* ptc = elementCourantTable (li);
	return ptc==NULL ? NULL : ptc->reference;
}

// insérer dans la liste li, objet après precedent
// si precedent est NULL, insérer en tête de liste
void insererApresTable (ListeTable* li, ElementTable* precedent, objetTable* objet) {
	if (precedent == NULL) {
		insererEnTeteDeListeTable (li, objet);
	} else {
		ElementTable* nouveau= creerElementTable();
		nouveau->reference = objet;
		nouveau->suivant= precedent->suivant;
		precedent->suivant = nouveau;
		if (precedent == li->dernier) 
			li->dernier = nouveau;
		li->nbElt++;
	}
}


// insérer un objet en fin de la liste li
void insererEnFinDeListeTable (ListeTable* li, objetTable* objet) {
	insererApresTable (li, li->dernier, objet);
}

// extraire l'objet en tête de la liste li
objetTable* extraireEnTeteDeListeTable (ListeTable* li) {
	ElementTable* extrait = li->premier;
	if (!listeVideTable(li)) {
		li->premier = li->premier->suivant;
		if (li->premier==NULL) 
			li->dernier=NULL; // Liste devenue vide
		li->nbElt--;
	}
	return extrait != NULL ? extrait->reference : NULL;
}


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetTable* extraireApresTable (ListeTable* li, ElementTable* precedent) {
	if (precedent == NULL) {
		return extraireEnTeteDeListeTable (li);
	} else {
		ElementTable* extrait = precedent->suivant;
		if (extrait != NULL) {
			precedent->suivant = extrait->suivant;
		if (extrait == li->dernier) li->dernier = precedent;
			li->nbElt--;
		}
		return extrait != NULL ? extrait->reference : NULL;
	}
}


// extraire l'objet en fin de la liste li
objetTable* extraireEnFinDeListeTable (ListeTable* li) {
	objetTable* extrait;
	if (listeVideTable(li)) {
		extrait = NULL;
	} else if (li->premier == li->dernier) { // un seul élément
		extrait = extraireEnTeteDeListeTable (li);
	} else {
		ElementTable* ptc = li->premier;
	while (ptc->suivant != li->dernier) ptc = ptc->suivant;
		extrait = extraireApresTable (li, ptc);
	}
	return extrait;
}


// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetTable (ListeTable* li, objetTable* objet) {
	ElementTable* precedent = NULL;
	ElementTable* ptc= NULL;
	// repère l'élément précédent
	booleen trouve = faux;
	ouvrirListeTable (li);
	while (!finListeTable (li) && !trouve) {
		precedent = ptc;
		ptc= elementCourantTable (li);
		trouve = (ptc->reference == objet) ? vrai : faux;
	}
	if (!trouve)
		 return faux;
	objetTable* extrait = extraireApresTable (li, precedent);
	return vrai;
}


// parcours de liste avec destruction de chaque élément
void detruireListeTable (ListeTable* li) {
	ouvrirListeTable (li);
	while (!finListeTable (li)) {
		ElementTable* ptc = elementCourantTable (li);
		free (ptc->reference); //si on veut détruire les objets de la liste
		free (ptc);
	}
	initListeTable (li,NULL,NULL);
}

// recopie l2 dans l1 et initialise l2
void recopierListeTable (ListeTable* l1, ListeTable* l2) {
	detruireListeTable (l1);
	*l1 = *l2;
	// on recopie les têtes de listes
}

/*=================================Keyspace=Options======================================*/

// initialiser la liste pointée par li (cas général)
void initListeOptionKeyspace (ListeOptionKeyspace* li, char* (*toString) (optionKeyspace*),int (*comparer) (optionKeyspace*, optionKeyspace*)) {
	li->premier = NULL;
	li->dernier = NULL;
	li->courant = NULL;
	li->nbElt= 0;
	li->toString = toString;
	li->comparer = comparer;
}

ListeOptionKeyspace* creerListeOptionKeyspace (char* (*toString) (optionKeyspace*),int (*comparer) (optionKeyspace*, optionKeyspace*)) {
	ListeOptionKeyspace* li =(ListeOptionKeyspace *)malloc(sizeof(ListeKey));
	initListeOptionKeyspace (li, toString, comparer);
	return li;
}

// la liste est-elle vide ?
int listeVideOptionKeyspace (ListeOptionKeyspace* li) {
	return li->nbElt == 0;
}

// fournir le nombre d'éléments dans la liste
int nbElementOptionKeyspace (ListeOptionKeyspace* li) {
	return li->nbElt;
}

ElementOptionKeyspace* creerElementOptionKeyspace () {
	return (ElementOptionKeyspace *)malloc(sizeof(ElementKey));
}


// insérer optionKeyspace en tête de la liste li
// l'optionKeyspace est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeOptionKeyspace (ListeOptionKeyspace* li, optionKeyspace* ch) {
	ElementOptionKeyspace* nouveau= creerElementOptionKeyspace();
	nouveau->reference = ch;
	nouveau->suivant=li->premier;
	li->premier= nouveau;
	if (li->dernier == NULL) li->dernier = nouveau;
	li->nbElt++;
}


// insérer un optionKeyspace en fin de la liste li
/*void insererEnFinDeListe (ListeOptionKeyspace* li, optionKeyspace* ch) {
	ElementOptionKeyspace* nouveaux= creerElementKey();
	nouveaux->reference = ch;
	nouveaux->suivant=NULL;
	if(li->premier==NULL){
		li->premier=nouveaux;
	}
	else{
		li->dernier->suivant=nouveaux;	
	}
	li->dernier=nouveaux;
}*/

// se positionner sur le premier élément de la liste li
void ouvrirListeOptionKeyspace (ListeOptionKeyspace* li) {
	li->courant = li->premier;
}


int finListeOptionKeyspace (ListeOptionKeyspace* li) {
	return li->courant==NULL;
}


int finListeSuivantOptionKeyspace (ListeOptionKeyspace* li) {
	return li->courant->suivant==NULL;
}

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementOptionKeyspace* elementCourantOptionKeyspace (ListeOptionKeyspace* li) {
	ElementOptionKeyspace* ptc = li->courant;
	if (li->courant != NULL) {
		li->courant = li->courant->suivant;
	}
	return ptc;
}

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeOptionKeyspace (ListeOptionKeyspace* li, void (*f) (optionKeyspace*)) {
	ouvrirListeOptionKeyspace (li);
	while (!finListeOptionKeyspace (li)) {
		optionKeyspace* ch = objetCourantOptionKeyspace (li);
		f (ch); // appliquer la fonction f() à optionKeyspace
	}
}

// fournir un pointeur sur l'optionKeyspace "objetCherche" de la liste li;
// NULL si l'optionKeyspace n'existe pas
optionKeyspace* chercherUnoptionKeyspace (ListeOptionKeyspace* li, optionKeyspace* objetCherche) {
	int trouve=0;
	void* ch;
	// pointeur courant
	ouvrirListeOptionKeyspace (li);
	while (!finListeOptionKeyspace (li) && !trouve) {
		ch = objetCourantOptionKeyspace (li);
		trouve = li->comparer (objetCherche, ch) == 0;
	}
	return trouve ? ch : NULL;
}

// fournir un pointeur sur l'objet courant de la liste li,
// et se positionner sur le suivant qui devient le courant
optionKeyspace* objetCourantOptionKeyspace (ListeOptionKeyspace* li) {
	ElementOptionKeyspace* ptc = elementCourantOptionKeyspace (li);
	return ptc==NULL ? NULL : ptc->reference;
}

// insérer dans la liste li, objet après precedent
// si precedent est NULL, insérer en tête de liste
void insererApresOptionKeyspace (ListeOptionKeyspace* li, ElementOptionKeyspace* precedent, optionKeyspace* objet) {
	if (precedent == NULL) {
		insererEnTeteDeListeOptionKeyspace (li, objet);
	} else {
		ElementOptionKeyspace* nouveau= creerElementOptionKeyspace();
		nouveau->reference = objet;
		nouveau->suivant= precedent->suivant;
		precedent->suivant = nouveau;
		if (precedent == li->dernier) 
			li->dernier = nouveau;
		li->nbElt++;
	}
}


// insérer un objet en fin de la liste li
void insererEnFinDeListeOptionKeyspace (ListeOptionKeyspace* li, optionKeyspace* objet) {
	insererApresOptionKeyspace (li, li->dernier, objet);
}

// extraire l'objet en tête de la liste li
optionKeyspace* extraireEnTeteDeListeOptionKeyspace (ListeOptionKeyspace* li) {
	ElementOptionKeyspace* extrait = li->premier;
	if (!listeVideOptionKeyspace(li)) {
		li->premier = li->premier->suivant;
		if (li->premier==NULL) 
			li->dernier=NULL; // Liste devenue vide
		li->nbElt--;
	}
	return extrait != NULL ? extrait->reference : NULL;
}


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
optionKeyspace* extraireApresOptionKeyspace (ListeOptionKeyspace* li, ElementOptionKeyspace* precedent) {
	if (precedent == NULL) {
		return extraireEnTeteDeListeOptionKeyspace (li);
	} else {
		ElementOptionKeyspace* extrait = precedent->suivant;
		if (extrait != NULL) {
			precedent->suivant = extrait->suivant;
		if (extrait == li->dernier) li->dernier = precedent;
			li->nbElt--;
		}
		return extrait != NULL ? extrait->reference : NULL;
	}
}


// extraire l'objet en fin de la liste li
optionKeyspace* extraireEnFinDeListeOptionKeyspace (ListeOptionKeyspace* li) {
	optionKeyspace* extrait;
	if (listeVideOptionKeyspace(li)) {
		extrait = NULL;
	} else if (li->premier == li->dernier) { // un seul élément
		extrait = extraireEnTeteDeListeOptionKeyspace (li);
	} else {
		ElementOptionKeyspace* ptc = li->premier;
	while (ptc->suivant != li->dernier) ptc = ptc->suivant;
		extrait = extraireApresOptionKeyspace (li, ptc);
	}
	return extrait;
}


// extraire de la liste li, l'objet pointé par objet
booleen extraireUnoptionKeyspace (ListeOptionKeyspace* li, optionKeyspace* objet) {
	ElementOptionKeyspace* precedent = NULL;
	ElementOptionKeyspace* ptc= NULL;
	// repère l'élément précédent
	booleen trouve = faux;
	ouvrirListeOptionKeyspace (li);
	while (!finListeOptionKeyspace (li) && !trouve) {
		precedent = ptc;
		ptc= elementCourantOptionKeyspace (li);
		trouve = (ptc->reference == objet) ? vrai : faux;
	}
	if (!trouve)
		 return faux;
	optionKeyspace* extrait = extraireApresOptionKeyspace (li, precedent);
	return vrai;
}


// parcours de liste avec destruction de chaque élément
void detruireListeOptionKeyspace (ListeOptionKeyspace* li) {
	ouvrirListeOptionKeyspace (li);
	while (!finListeOptionKeyspace (li)) {
		ElementOptionKeyspace* ptc = elementCourantOptionKeyspace (li);
		free (ptc->reference); //si on veut détruire les objets de la liste
		free (ptc);
	}
	initListeOptionKeyspace (li,NULL,NULL);
}

// recopie l2 dans l1 et initialise l2
void recopierListeOptionKeyspace (ListeOptionKeyspace* l1, ListeOptionKeyspace* l2) {
	detruireListeOptionKeyspace (l1);
	*l1 = *l2;
	// on recopie les têtes de listes
}

/*=======================================Table==Options==============================*/

// initialiser la liste pointée par li (cas général)
void initListeOptionTable (ListeOptionTable* li, char* (*toString) (optionTable *),int (*comparer) (optionTable *, optionTable *)) {
	li->premier = NULL;
	li->dernier = NULL;
	li->courant = NULL;
	li->nbElt= 0;
	li->toString = toString;
	li->comparer = comparer;
}

ListeOptionTable* creerListeOptionTable (char* (*toString) (optionTable *),int (*comparer) (optionTable *, optionTable *)) {
	ListeOptionTable* li =(ListeOptionTable *)malloc(sizeof(ListeKey));
	initListeOptionTable (li, toString, comparer);
	return li;
}

// la liste est-elle vide ?
int listeVideOptionTable (ListeOptionTable* li) {
	return li->nbElt == 0;
}

// fournir le nombre d'éléments dans la liste
int nbElementOptionTable (ListeOptionTable* li) {
	return li->nbElt;
}

ElementOptionTable* creerElementOptionTable () {
	return (ElementOptionTable *)malloc(sizeof(ElementKey));
}


// insérer optionTable  en tête de la liste li
// l'optionTable  est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeOptionTable (ListeOptionTable* li, optionTable * ch) {
	ElementOptionTable* nouveau= creerElementOptionTable();
	nouveau->reference = ch;
	nouveau->suivant=li->premier;
	li->premier= nouveau;
	if (li->dernier == NULL) li->dernier = nouveau;
	li->nbElt++;
}


// insérer un optionTable  en fin de la liste li
/*void insererEnFinDeListe (ListeOptionTable* li, optionTable * ch) {
	ElementOptionTable* nouveaux= creerElementOptionKeyspace();
	nouveaux->reference = ch;
	nouveaux->suivant=NULL;
	if(li->premier==NULL){
		li->premier=nouveaux;
	}
	else{
		li->dernier->suivant=nouveaux;	
	}
	li->dernier=nouveaux;
}*/

// se positionner sur le premier élément de la liste li
void ouvrirListeOptionTable (ListeOptionTable* li) {
	li->courant = li->premier;
}


int finListeOptionTable (ListeOptionTable* li) {
	return li->courant==NULL;
}


int finListeSuivantOptionTable (ListeOptionTable* li) {
	return li->courant->suivant==NULL;
}

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementOptionTable* elementCourantOptionTable (ListeOptionTable* li) {
	ElementOptionTable* ptc = li->courant;
	if (li->courant != NULL) {
		li->courant = li->courant->suivant;
	}
	return ptc;
}

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeOptionTable (ListeOptionTable* li, void (*f) (optionTable *)) {
	ouvrirListeOptionTable (li);
	while (!finListeOptionTable (li)) {
		optionTable * ch = objetCourantOptionTable (li);
		f (ch); // appliquer la fonction f() à optionTable 
	}
}

// fournir un pointeur sur l'optionTable  "objetCherche" de la liste li;
// NULL si l'optionTable  n'existe pas
optionTable * chercherUnoptionTable  (ListeOptionTable* li, optionTable * objetCherche) {
	int trouve=0;
	void* ch;
	// pointeur courant
	ouvrirListeOptionTable (li);
	while (!finListeOptionTable (li) && !trouve) {
		ch = objetCourantOptionTable (li);
		trouve = li->comparer (objetCherche, ch) == 0;
	}
	return trouve ? ch : NULL;
}

// fournir un pointeur sur l'objet courant de la liste li,
// et se positionner sur le suivant qui devient le courant
optionTable * objetCourantOptionTable (ListeOptionTable* li) {
	ElementOptionTable* ptc = elementCourantOptionTable (li);
	return ptc==NULL ? NULL : ptc->reference;
}

// insérer dans la liste li, objet après precedent
// si precedent est NULL, insérer en tête de liste
void insererApresOptionTable (ListeOptionTable* li, ElementOptionTable* precedent, optionTable * objet) {
	if (precedent == NULL) {
		insererEnTeteDeListeOptionTable (li, objet);
	} else {
		ElementOptionTable* nouveau= creerElementOptionTable();
		nouveau->reference = objet;
		nouveau->suivant= precedent->suivant;
		precedent->suivant = nouveau;
		if (precedent == li->dernier) 
			li->dernier = nouveau;
		li->nbElt++;
	}
}


// insérer un objet en fin de la liste li
void insererEnFinDeListeOptionTable (ListeOptionTable* li, optionTable * objet) {
	insererApresOptionTable (li, li->dernier, objet);
}

// extraire l'objet en tête de la liste li
optionTable * extraireEnTeteDeListeOptionTable (ListeOptionTable* li) {
	ElementOptionTable* extrait = li->premier;
	if (!listeVideOptionTable(li)) {
		li->premier = li->premier->suivant;
		if (li->premier==NULL) 
			li->dernier=NULL; // Liste devenue vide
		li->nbElt--;
	}
	return extrait != NULL ? extrait->reference : NULL;
}


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
optionTable * extraireApresOptionTable (ListeOptionTable* li, ElementOptionTable* precedent) {
	if (precedent == NULL) {
		return extraireEnTeteDeListeOptionTable (li);
	} else {
		ElementOptionTable* extrait = precedent->suivant;
		if (extrait != NULL) {
			precedent->suivant = extrait->suivant;
		if (extrait == li->dernier) li->dernier = precedent;
			li->nbElt--;
		}
		return extrait != NULL ? extrait->reference : NULL;
	}
}


// extraire l'objet en fin de la liste li
optionTable * extraireEnFinDeListeOptionTable (ListeOptionTable* li) {
	optionTable * extrait;
	if (listeVideOptionTable(li)) {
		extrait = NULL;
	} else if (li->premier == li->dernier) { // un seul élément
		extrait = extraireEnTeteDeListeOptionTable (li);
	} else {
		ElementOptionTable* ptc = li->premier;
	while (ptc->suivant != li->dernier) ptc = ptc->suivant;
		extrait = extraireApresOptionTable (li, ptc);
	}
	return extrait;
}


// extraire de la liste li, l'objet pointé par objet
booleen extraireUnoptionTable  (ListeOptionTable* li, optionTable * objet) {
	ElementOptionTable* precedent = NULL;
	ElementOptionTable* ptc= NULL;
	// repère l'élément précédent
	booleen trouve = faux;
	ouvrirListeOptionTable (li);
	while (!finListeOptionTable (li) && !trouve) {
		precedent = ptc;
		ptc= elementCourantOptionTable (li);
		trouve = (ptc->reference == objet) ? vrai : faux;
	}
	if (!trouve)
		 return faux;
	optionTable * extrait = extraireApresOptionTable (li, precedent);
	return vrai;
}


// parcours de liste avec destruction de chaque élément
void detruireListeOptionTable (ListeOptionTable* li) {
	ouvrirListeOptionTable (li);
	while (!finListeOptionTable (li)) {
		ElementOptionTable* ptc = elementCourantOptionTable (li);
		free (ptc->reference); //si on veut détruire les objets de la liste
		free (ptc);
	}
	initListeOptionTable (li,NULL,NULL);
}

// recopie l2 dans l1 et initialise l2
void recopierListeOptionTable (ListeOptionTable* l1, ListeOptionTable* l2) {
	detruireListeOptionTable (l1);
	*l1 = *l2;
	// on recopie les têtes de listes
}

/*========================================Map=======================================*/



// initialiser la liste pointée par li (cas général)
void initListeMap (ListeMap* li, char* (*toString) (objetMap*),int (*comparer) (objetMap*, objetMap*)) {
    li->premier = NULL;
    li->dernier = NULL;
    li->courant = NULL;
    li->nbElt= 0;
    li->toString = toString;
    li->comparer = comparer;
}

ListeMap* creerListeMap (char* (*toString) (objetMap*),int (*comparer) (objetMap*, objetMap*)) {
    ListeMap* li =(ListeMap *)malloc(sizeof(ListeMap));
    initListeMap (li, toString, comparer);
    return li;
}

// la liste est-elle vide ?
int listeVideMap (ListeMap* li) {
    return li->nbElt == 0;
}

// fournir le nombre d'éléments dans la liste
int nbElementMap (ListeMap* li) {
    return li->nbElt;
}

ElementMap* creerElementMap () {
    return (ElementMap *)malloc(sizeof(ElementMap));
}


// insérer objetMap en tête de la liste li
// l'objetMap est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeMap (ListeMap* li, objetMap* ch) {
    ElementMap* nouveau= creerElementMap();
    nouveau->reference = ch;
    nouveau->suivant= li->premier;
    li->premier= nouveau;
    if (li->dernier == NULL) li->dernier = nouveau;
    li->nbElt++;
}


// insérer un objetMap en fin de la liste li
/*void insererEnFinDeListe (ListeMap* li, objetMap* ch) {
	ElementMap* nouveaux= creerElementMap();
	nouveaux->reference = ch;
	nouveaux->suivant=NULL;
	if(li->premier==NULL){
		li->premier=nouveaux;
	}
	else{
		li->dernier->suivant=nouveaux;
	}
	li->dernier=nouveaux;
}*/

// se positionner sur le premier élément de la liste li
void ouvrirListeMap (ListeMap* li) {
    li->courant = li->premier;
}


int finListeMap (ListeMap* li) {
    return li->courant==NULL;
}


int finListeSuivantMap (ListeMap* li) {
    return li->courant->suivant==NULL;
}

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementMap* elementCourantMap (ListeMap* li) {
    ElementMap* ptc = li->courant;
    if (li->courant != NULL) {
        li->courant = li->courant->suivant;
    }
    return ptc;
}

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeMap (ListeMap* li, void (*f) (objetMap*)) {
    ouvrirListeMap (li);
    while (!finListeMap (li)) {
        objetMap* ch = objetCourantMap (li);
        f (ch); // appliquer la fonction f() à objetMap
    }
}

// fournir un pointeur sur l'objetMap "objetCherche" de la liste li;
// NULL si l'objetMap n'existe pas
objetMap* chercherUnobjetMap (ListeMap* li, objetMap* objetCherche) {
    int trouve=0;
    void* ch;
    // pointeur courant
    ouvrirListeMap (li);
    while (!finListeMap (li) && !trouve) {
        ch = objetCourantMap (li);
        trouve = li->comparer (objetCherche, ch) == 0;
    }
    return trouve ? ch : NULL;
}

// fournir un pointeur sur l'objet courant de la liste li,
// et se positionner sur le suivant qui devient le courant
objetMap* objetCourantMap (ListeMap* li) {
    ElementMap* ptc = elementCourantMap (li);
    return ptc==NULL ? NULL : ptc->reference;
}

// insérer dans la liste li, objet après precedent
// si precedent est NULL, insérer en tête de liste
void insererApresMap (ListeMap* li, ElementMap* precedent, objetMap* objet) {
    if (precedent == NULL) {
        insererEnTeteDeListeMap (li, objet);
    } else {
        ElementMap* nouveau= creerElementMap();
        nouveau->reference = objet;
        nouveau->suivant= precedent->suivant;
        precedent->suivant = nouveau;
        if (precedent == li->dernier)
            li->dernier = nouveau;
        li->nbElt++;
    }
}


// insérer un objet en fin de la liste li
void insererEnFinDeListeMap (ListeMap* li, objetMap* objet) {
    insererApresMap (li, li->dernier, objet);
}

// extraire l'objet en tête de la liste li
objetMap* extraireEnTeteDeListeMap (ListeMap* li) {
    ElementMap* extrait = li->premier;
    if (!listeVideMap(li)) {
        li->premier = li->premier->suivant;
        if (li->premier==NULL)
            li->dernier=NULL; // Liste devenue vide
        li->nbElt--;
    }
    return extrait != NULL ? extrait->reference : NULL;
}


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetMap* extraireApresMap (ListeMap* li, ElementMap* precedent) {
    if (precedent == NULL) {
        return extraireEnTeteDeListeMap (li);
    } else {
        ElementMap* extrait = precedent->suivant;
        if (extrait != NULL) {
            precedent->suivant = extrait->suivant;
            if (extrait == li->dernier) li->dernier = precedent;
            li->nbElt--;
        }
        return extrait != NULL ? extrait->reference : NULL;
    }
}


// extraire l'objet en fin de la liste li
objetMap* extraireEnFinDeListeMap (ListeMap* li) {
    objetMap* extrait;
    if (listeVideMap(li)) {
        extrait = NULL;
    } else if (li->premier == li->dernier) { // un seul élément
        extrait = extraireEnTeteDeListeMap (li);
    } else {
        ElementMap* ptc = li->premier;
        while (ptc->suivant != li->dernier) ptc = ptc->suivant;
        extrait = extraireApresMap (li, ptc);
    }
    return extrait;
}


// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetMap (ListeMap* li, objetMap* objet) {
    ElementMap* precedent = NULL;
    ElementMap* ptc= NULL;
    // repère l'élément précédent
    booleen trouve = faux;
    ouvrirListeMap (li);
    while (!finListeMap (li) && !trouve) {
        precedent = ptc;
        ptc= elementCourantMap (li);
        trouve = (ptc->reference == objet) ? vrai : faux;
    }
    if (!trouve)
        return faux;
    objetMap* extrait = extraireApresMap (li, precedent);
    return vrai;
}


// parcours de liste avec destruction de chaque élément
void detruireListeMap (ListeMap* li) {
    ouvrirListeMap (li);
    while (!finListeMap (li)) {
        ElementMap* ptc = elementCourantMap (li);
        free (ptc->reference); //si on veut détruire les objets de la liste
        free (ptc);
    }
    initListeMap (li,NULL,NULL);
}

// recopie l2 dans l1 et initialise l2
void recopierListeMap (ListeMap* l1, ListeMap* l2) {
    detruireListeMap (l1);
    *l1 = *l2;
    // on recopie les têtes de listes
}


/*========================================List=======================================*/



// initialiser la liste pointée par li (cas général)
void initListeList (ListeList* li, char* (*toString) (term*),int (*comparer) (term*, term*)) {
	li->premier = NULL;
	li->dernier = NULL;
	li->courant = NULL;
	li->nbElt= 0;
	li->toString = toString;
	li->comparer = comparer;
}

ListeList* creerListeList (char* (*toString) (term*),int (*comparer) (term*, term*)) {
	ListeList* li =(ListeList *)malloc(sizeof(ListeList));
	initListeList (li, toString, comparer);
	return li;
}

// la liste est-elle vide ?
int listeVideList (ListeList* li) {
	return li->nbElt == 0;
}

// fournir le nombre d'éléments dans la liste
int nbElementList (ListeList* li) {
	return li->nbElt;
}

ElementList* creerElementList () {
	return (ElementList *)malloc(sizeof(ElementList));
}


// insérer objetList en tête de la liste li
// l'objetList est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeList (ListeList* li, term* ch) {
	ElementList* nouveau= creerElementList();
	nouveau->reference = ch;
	nouveau->suivant= li->premier;
	li->premier= nouveau;
	if (li->dernier == NULL) li->dernier = nouveau;
	li->nbElt++;
}


// insérer un objetList en fin de la liste li
/*void insererEnFinDeListe (ListeList* li, term* ch) {
	ElementList* nouveaux= creerElementList();
	nouveaux->reference = ch;
	nouveaux->suivant=NULL;
	if(li->premier==NULL){
		li->premier=nouveaux;
	}
	else{
		li->dernier->suivant=nouveaux;	
	}
	li->dernier=nouveaux;
}*/

// se positionner sur le premier élément de la liste li
void ouvrirListeList (ListeList* li) {
	li->courant = li->premier;
}


int finListeList (ListeList* li) {
	return li->courant==NULL;
}


int finListeSuivantList (ListeList* li) {
	return li->courant->suivant==NULL;
}

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementList* elementCourantList (ListeList* li) {
	ElementList* ptc = li->courant;
	if (li->courant != NULL) {
		li->courant = li->courant->suivant;
	}
	return ptc;
}

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeList (ListeList* li, void (*f) (term*)) {
	ouvrirListeList (li);
	while (!finListeList (li)) {
		term* ch = objetCourantList (li);
		f (ch); // appliquer la fonction f() à objetList
	}
}

// fournir un pointeur sur l'objetList "objetCherche" de la liste li;
// NULL si l'objetList n'existe pas
term* chercherUnobjetList (ListeList* li, term* objetCherche) {
	int trouve=0;
	void* ch;
	// pointeur courant
	ouvrirListeList (li);
	while (!finListeList (li) && !trouve) {
		ch = objetCourantList (li);
		trouve = li->comparer (objetCherche, ch) == 0;
	}
	return trouve ? ch : NULL;
}

// fournir un pointeur sur l'objet courant de la liste li,
// et se positionner sur le suivant qui devient le courant
term* objetCourantList (ListeList* li) {
	ElementList* ptc = elementCourantList (li);
	return ptc==NULL ? NULL : ptc->reference;
}

// insérer dans la liste li, objet après precedent
// si precedent est NULL, insérer en tête de liste
void insererApresList (ListeList* li, ElementList* precedent, term* objet) {
	if (precedent == NULL) {
		insererEnTeteDeListeList (li, objet);
	} else {
		ElementList* nouveau= creerElementList();
		nouveau->reference = objet;
		nouveau->suivant= precedent->suivant;
		precedent->suivant = nouveau;
		if (precedent == li->dernier) 
			li->dernier = nouveau;
		li->nbElt++;
	}
}


// insérer un objet en fin de la liste li
void insererEnFinDeListeList (ListeList* li, term* objet) {
	insererApresList (li, li->dernier, objet);
}

// extraire l'objet en tête de la liste li
term* extraireEnTeteDeListeList (ListeList* li) {
	ElementList* extrait = li->premier;
	if (!listeVideList(li)) {
		li->premier = li->premier->suivant;
		if (li->premier==NULL) 
			li->dernier=NULL; // Liste devenue vide
		li->nbElt--;
	}
	return extrait != NULL ? extrait->reference : NULL;
}


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
term* extraireApresList (ListeList* li, ElementList* precedent) {
	if (precedent == NULL) {
		return extraireEnTeteDeListeList (li);
	} else {
		ElementList* extrait = precedent->suivant;
		if (extrait != NULL) {
			precedent->suivant = extrait->suivant;
		if (extrait == li->dernier) li->dernier = precedent;
			li->nbElt--;
		}
		return extrait != NULL ? extrait->reference : NULL;
	}
}


// extraire l'objet en fin de la liste li
term* extraireEnFinDeListeList (ListeList* li) {
	term* extrait;
	if (listeVideList(li)) {
		extrait = NULL;
	} else if (li->premier == li->dernier) { // un seul élément
		extrait = extraireEnTeteDeListeList (li);
	} else {
		ElementList* ptc = li->premier;
	while (ptc->suivant != li->dernier) ptc = ptc->suivant;
		extrait = extraireApresList (li, ptc);
	}
	return extrait;
}


// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetList (ListeList* li, term* objet) {
	ElementList* precedent = NULL;
	ElementList* ptc= NULL;
	// repère l'élément précédent
	booleen trouve = faux;
	ouvrirListeList (li);
	while (!finListeList (li) && !trouve) {
		precedent = ptc;
		ptc= elementCourantList (li);
		trouve = (ptc->reference == objet) ? vrai : faux;
	}
	if (!trouve)
		 return faux;
	term* extrait = extraireApresList (li, precedent);
	return vrai;
}


// parcours de liste avec destruction de chaque élément
void detruireListeList (ListeList* li) {
	ouvrirListeList (li);
	while (!finListeList (li)) {
		ElementList* ptc = elementCourantList (li);
		free (ptc->reference); //si on veut détruire les objets de la liste
		free (ptc);
	}
	initListeList (li,NULL,NULL);
}

// recopie l2 dans l1 et initialise l2
void recopierListeList (ListeList* l1, ListeList* l2) {
	detruireListeList (l1);
	*l1 = *l2;
	// on recopie les têtes de listes
}


/*========================================Set=======================================*/



// initialiser la liste pointée par li (cas général)
void initListeSet (ListeSet* li, char* (*toString) (objetSet*),int (*comparer) (objetSet*, objetSet*)) {
	li->premier = NULL;
	li->dernier = NULL;
	li->courant = NULL;
	li->nbElt= 0;
	li->toString = toString;
	li->comparer = comparer;
}

ListeSet* creerListeSet (char* (*toString) (objetSet*),int (*comparer) (objetSet*, objetSet*)) {
	ListeSet* li =(ListeSet *)malloc(sizeof(ListeSet));
	initListeSet (li, toString, comparer);
	return li;
}

// la liste est-elle vide ?
int listeVideSet (ListeSet* li) {
	return li->nbElt == 0;
}

// fournir le nombre d'éléments dans la liste
int nbElementSet (ListeSet* li) {
	return li->nbElt;
}

ElementSet* creerElementSet () {
	return (ElementSet *)malloc(sizeof(ElementSet));
}


// insérer objetSet en tête de la liste li
// l'objetSet est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeSet (ListeSet* li, objetSet* ch) {
	ElementSet* nouveau= creerElementSet();
	nouveau->reference = ch;
	nouveau->suivant= li->premier;
	li->premier= nouveau;
	if (li->dernier == NULL) li->dernier = nouveau;
	li->nbElt++;
}


// insérer un objetSet en fin de la liste li
/*void insererEnFinDeListe (ListeSet* li, objetSet* ch) {
	ElementSet* nouveaux= creerElementSet();
	nouveaux->reference = ch;
	nouveaux->suivant=NULL;
	if(li->premier==NULL){
		li->premier=nouveaux;
	}
	else{
		li->dernier->suivant=nouveaux;	
	}
	li->dernier=nouveaux;
}*/

// se positionner sur le premier élément de la liste li
void ouvrirListeSet (ListeSet* li) {
	li->courant = li->premier;
}


int finListeSet (ListeSet* li) {
	return li->courant==NULL;
}


int finListeSuivantSet (ListeSet* li) {
	return li->courant->suivant==NULL;
}

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementSet* elementCourantSet (ListeSet* li) {
	ElementSet* ptc = li->courant;
	if (li->courant != NULL) {
		li->courant = li->courant->suivant;
	}
	return ptc;
}

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeSet (ListeSet* li, void (*f) (objetSet*)) {
	ouvrirListeSet (li);
	while (!finListeSet (li)) {
		objetSet* ch = objetCourantSet (li);
		f (ch); // appliquer la fonction f() à objetSet
	}
}

// fournir un pointeur sur l'objetSet "objetCherche" de la liste li;
// NULL si l'objetSet n'existe pas
objetSet* chercherUnobjetSet (ListeSet* li, objetSet* objetCherche) {
	int trouve=0;
	void* ch;
	// pointeur courant
	ouvrirListeSet (li);
	while (!finListeSet (li) && !trouve) {
		ch = objetCourantSet (li);
		trouve = li->comparer (objetCherche, ch) == 0;
	}
	return trouve ? ch : NULL;
}

// fournir un pointeur sur l'objet courant de la liste li,
// et se positionner sur le suivant qui devient le courant
objetSet* objetCourantSet (ListeSet* li) {
	ElementSet* ptc = elementCourantSet (li);
	return ptc==NULL ? NULL : ptc->reference;
}

// insérer dans la liste li, objet après precedent
// si precedent est NULL, insérer en tête de liste
void insererApresSet (ListeSet* li, ElementSet* precedent, objetSet* objet) {
	if (precedent == NULL) {
		insererEnTeteDeListeSet (li, objet);
	} else {
		ElementSet* nouveau= creerElementSet();
		nouveau->reference = objet;
		nouveau->suivant= precedent->suivant;
		precedent->suivant = nouveau;
		if (precedent == li->dernier) 
			li->dernier = nouveau;
		li->nbElt++;
	}
}


// insérer un objet en fin de la liste li
void insererEnFinDeListeSet (ListeSet* li, objetSet* objet) {
	insererApresSet (li, li->dernier, objet);
}

// extraire l'objet en tête de la liste li
objetSet* extraireEnTeteDeListeSet (ListeSet* li) {
	ElementSet* extrait = li->premier;
	if (!listeVideSet(li)) {
		li->premier = li->premier->suivant;
		if (li->premier==NULL) 
			li->dernier=NULL; // Liste devenue vide
		li->nbElt--;
	}
	return extrait != NULL ? extrait->reference : NULL;
}


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetSet* extraireApresSet (ListeSet* li, ElementSet* precedent) {
	if (precedent == NULL) {
		return extraireEnTeteDeListeSet (li);
	} else {
		ElementSet* extrait = precedent->suivant;
		if (extrait != NULL) {
			precedent->suivant = extrait->suivant;
		if (extrait == li->dernier) li->dernier = precedent;
			li->nbElt--;
		}
		return extrait != NULL ? extrait->reference : NULL;
	}
}


// extraire l'objet en fin de la liste li
objetSet* extraireEnFinDeListeSet (ListeSet* li) {
	objetSet* extrait;
	if (listeVideSet(li)) {
		extrait = NULL;
	} else if (li->premier == li->dernier) { // un seul élément
		extrait = extraireEnTeteDeListeSet (li);
	} else {
		ElementSet* ptc = li->premier;
	while (ptc->suivant != li->dernier) ptc = ptc->suivant;
		extrait = extraireApresSet (li, ptc);
	}
	return extrait;
}


// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetSet (ListeSet* li, objetSet* objet) {
	ElementSet* precedent = NULL;
	ElementSet* ptc= NULL;
	// repère l'élément précédent
	booleen trouve = faux;
	ouvrirListeSet (li);
	while (!finListeSet (li) && !trouve) {
		precedent = ptc;
		ptc= elementCourantSet (li);
		trouve = (ptc->reference == objet) ? vrai : faux;
	}
	if (!trouve)
		 return faux;
	objetSet* extrait = extraireApresSet (li, precedent);
	return vrai;
}


// parcours de liste avec destruction de chaque élément
void detruireListeSet (ListeSet* li) {
	ouvrirListeSet (li);
	while (!finListeSet (li)) {
		ElementSet* ptc = elementCourantSet (li);
		free (ptc->reference); //si on veut détruire les objets de la liste
		free (ptc);
	}
	initListeSet (li,NULL,NULL);
}

// recopie l2 dans l1 et initialise l2
void recopierListeSet (ListeSet* l1, ListeSet* l2) {
	detruireListeSet (l1);
	*l1 = *l2;
	// on recopie les têtes de listes
}

/*=======================================Column=======================================*/



// initialiser la liste pointée par li (cas général)
void initListeColumn (ListeColumn* li, char* (*toString) (objetColumn*),int (*comparer) (objetColumn*, objetColumn*)) {
	li->premier = NULL;
	li->dernier = NULL;
	li->courant = NULL;
	li->nbElt= 0;
	li->toString = toString;
	li->comparer = comparerColumn;
}

ListeColumn* creerListeColumn (char* (*toString) (objetColumn*),int (*comparer) (objetColumn*, objetColumn*)) {
	ListeColumn* li =(ListeColumn *)malloc(sizeof(ListeColumn));
	initListeColumn (li, toString, comparer);
	return li;
}

// la liste est-elle vide ?
int listeVideColumn (ListeColumn* li) {
	return li->nbElt == 0;
}

// fournir le nombre d'éléments dans la liste
int nbElementColumn (ListeColumn* li) {
	return li->nbElt;
}

ElementColumn* creerElementColumn () {
	return (ElementColumn *)malloc(sizeof(ElementColumn));
}


// insérer objetColumn en tête de la liste li
// l'objetColumn est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeColumn (ListeColumn* li, objetColumn* ch) {
	ElementColumn* nouveau= creerElementColumn();
	nouveau->reference = ch;
	nouveau->suivant= li->premier;
	li->premier= nouveau;
	if (li->dernier == NULL) li->dernier = nouveau;
	li->nbElt++;
}


// insérer un objetColumn en fin de la liste li
/*void insererEnFinDeListe (ListeColumn* li, objetColumn* ch) {
	ElementColumn* nouveaux= creerElementColumn();
	nouveaux->reference = ch;
	nouveaux->suivant=NULL;
	if(li->premier==NULL){
		li->premier=nouveaux;
	}
	else{
		li->dernier->suivant=nouveaux;	
	}
	li->dernier=nouveaux;
}*/

// se positionner sur le premier élément de la liste li
void ouvrirListeColumn (ListeColumn* li) {
	li->courant = li->premier;
}


int finListeColumn (ListeColumn* li) {
	return li->courant==NULL;
}


int finListeSuivantColumn (ListeColumn* li) {
	return li->courant->suivant==NULL;
}

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementColumn* elementCourantColumn (ListeColumn* li) {
	ElementColumn* ptc = li->courant;
	if (li->courant != NULL) {
		li->courant = li->courant->suivant;
	}
	return ptc;
}

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeColumn (ListeColumn* li, void (*f) (objetColumn*)) {
	ouvrirListeColumn (li);
	while (!finListeColumn (li)) {
		objetColumn* ch = objetCourantColumn (li);
		f (ch); // appliquer la fonction f() à objetColumn
	}
}

// fournir un pointeur sur l'objetColumn "objetCherche" de la liste li;
// NULL si l'objetColumn n'existe pas
objetColumn* chercherUnobjetColumn (ListeColumn* li, objetColumn* objetCherche) {
	int trouve=0;
	void* ch;
	// pointeur courant
	ouvrirListeColumn (li);
	while (!finListeColumn (li) && !trouve) {
		ch = objetCourantColumn (li);
		trouve = li->comparer (objetCherche, ch) == 0;
	}
	return trouve ? ch : NULL;
}

// fournir un pointeur sur l'objet courant de la liste li,
// et se positionner sur le suivant qui devient le courant
objetColumn* objetCourantColumn (ListeColumn* li) {
	ElementColumn* ptc = elementCourantColumn (li);
	return ptc==NULL ? NULL : ptc->reference;
}

// insérer dans la liste li, objet après precedent
// si precedent est NULL, insérer en tête de liste
void insererApresColumn (ListeColumn* li, ElementColumn* precedent, objetColumn* objet) {
	if (precedent == NULL) {
		insererEnTeteDeListeColumn (li, objet);
	} else {
		ElementColumn* nouveau= creerElementColumn();
		nouveau->reference = objet;
		nouveau->suivant= precedent->suivant;
		precedent->suivant = nouveau;
		if (precedent == li->dernier) 
			li->dernier = nouveau;
		li->nbElt++;
	}
}


// insérer un objet en fin de la liste li
void insererEnFinDeListeColumn (ListeColumn* li, objetColumn* objet) {
	insererApresColumn (li, li->dernier, objet);
}

// extraire l'objet en tête de la liste li
objetColumn* extraireEnTeteDeListeColumn (ListeColumn* li) {
	ElementColumn* extrait = li->premier;
	if (!listeVideColumn(li)) {
		li->premier = li->premier->suivant;
		if (li->premier==NULL) 
			li->dernier=NULL; // Liste devenue vide
		li->nbElt--;
	}
	return extrait != NULL ? extrait->reference : NULL;
}


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetColumn* extraireApresColumn (ListeColumn* li, ElementColumn* precedent) {
	if (precedent == NULL) {
		return extraireEnTeteDeListeColumn (li);
	} else {
		ElementColumn* extrait = precedent->suivant;
		if (extrait != NULL) {
			precedent->suivant = extrait->suivant;
		if (extrait == li->dernier) li->dernier = precedent;
			li->nbElt--;
		}
		return extrait != NULL ? extrait->reference : NULL;
	}
}


// extraire l'objet en fin de la liste li
objetColumn* extraireEnFinDeListeColumn (ListeColumn* li) {
	objetColumn* extrait;
	if (listeVideColumn(li)) {
		extrait = NULL;
	} else if (li->premier == li->dernier) { // un seul élément
		extrait = extraireEnTeteDeListeColumn (li);
	} else {
		ElementColumn* ptc = li->premier;
	while (ptc->suivant != li->dernier) ptc = ptc->suivant;
		extrait = extraireApresColumn (li, ptc);
	}
	return extrait;
}


// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetColumn (ListeColumn* li, objetColumn* objet) {
	ElementColumn* precedent = NULL;
	ElementColumn* ptc= NULL;
	// repère l'élément précédent
	booleen trouve = faux;
	ouvrirListeColumn (li);
	while (!finListeColumn (li) && !trouve) {
		precedent = ptc;
		ptc= elementCourantColumn (li);
		trouve = (ptc->reference == objet) ? vrai : faux;
	}
	if (!trouve)
		 return faux;
	objetColumn* extrait = extraireApresColumn (li, precedent);
	return vrai;
}


// parcours de liste avec destruction de chaque élément
void detruireListeColumn (ListeColumn* li) {
	ouvrirListeColumn (li);
	while (!finListeColumn (li)) {
		ElementColumn* ptc = elementCourantColumn (li);
		//free (ptc->reference); //si on veut détruire les objets de la liste
		free (ptc);
	}
	initListeColumn (li,NULL,NULL);
}

// recopie l2 dans l1 et initialise l2
void recopierListeColumn (ListeColumn* l1, ListeColumn* l2) {
	detruireListeColumn (l1);
	*l1 = *l2;
	// on recopie les têtes de listes
}

/*=======================================TupleT=======================================*/



// initialiser la liste pointée par li (cas général)
void initListeTupleT (ListeTupleT* li, char* (*toString) (objetTupleT*),int (*comparer) (objetTupleT*, objetTupleT*)) {
	li->premier = NULL;
	li->dernier = NULL;
	li->courant = NULL;
	li->nbElt= 0;
	li->toString = toString;
	li->comparer = comparer;
}

ListeTupleT* creerListeTupleT (char* (*toString) (objetTupleT*),int (*comparer) (objetTupleT*, objetTupleT*)) {
	ListeTupleT* li =(ListeTupleT *)malloc(sizeof(ListeTupleT));
	initListeTupleT (li, toString, comparer);
	return li;
}

// la liste est-elle vide ?
int listeVideTupleT (ListeTupleT* li) {
	return li->nbElt == 0;
}

// fournir le nombre d'éléments dans la liste
int nbElementTupleT (ListeTupleT* li) {
	return li->nbElt;
}

ElementTupleT* creerElementTupleT () {
	return (ElementTupleT *)malloc(sizeof(ElementTupleT));
}


// insérer objetTupleT en tête de la liste li
// l'objetTupleT est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeTupleT (ListeTupleT* li, objetTupleT* ch) {
	ElementTupleT* nouveau= creerElementTupleT();
	nouveau->reference = ch;
	nouveau->suivant= li->premier;
	li->premier= nouveau;
	if (li->dernier == NULL) li->dernier = nouveau;
	li->nbElt++;
}


// insérer un objetTupleT en fin de la liste li
/*void insererEnFinDeListe (ListeTupleT* li, objetTupleT* ch) {
	ElementTupleT* nouveaux= creerElementTupleT();
	nouveaux->reference = ch;
	nouveaux->suivant=NULL;
	if(li->premier==NULL){
		li->premier=nouveaux;
	}
	else{
		li->dernier->suivant=nouveaux;	
	}
	li->dernier=nouveaux;
}*/

// se positionner sur le premier élément de la liste li
void ouvrirListeTupleT (ListeTupleT* li) {
	li->courant = li->premier;
}


int finListeTupleT (ListeTupleT* li) {
	return li->courant==NULL;
}


int finListeSuivantTupleT (ListeTupleT* li) {
	return li->courant->suivant==NULL;
}

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementTupleT* elementCourantTupleT (ListeTupleT* li) {
	ElementTupleT* ptc = li->courant;
	if (li->courant != NULL) {
		li->courant = li->courant->suivant;
	}
	return ptc;
}

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeTupleT (ListeTupleT* li, void (*f) (objetTupleT*)) {
	ouvrirListeTupleT (li);
	while (!finListeTupleT (li)) {
		objetTupleT* ch = objetCourantTupleT (li);
		f (ch); // appliquer la fonction f() à objetTupleT
	}
}

// fournir un pointeur sur l'objetTupleT "objetCherche" de la liste li;
// NULL si l'objetTupleT n'existe pas
objetTupleT* chercherUnobjetTupleT (ListeTupleT* li, objetTupleT* objetCherche) {
	int trouve=0;
	void* ch;
	// pointeur courant
	ouvrirListeTupleT (li);
	while (!finListeTupleT (li) && !trouve) {
		ch = objetCourantTupleT (li);
		trouve = li->comparer (objetCherche, ch) == 0;
	}
	return trouve ? ch : NULL;
}

// fournir un pointeur sur l'objet courant de la liste li,
// et se positionner sur le suivant qui devient le courant
objetTupleT* objetCourantTupleT (ListeTupleT* li) {
	ElementTupleT* ptc = elementCourantTupleT (li);
	return ptc==NULL ? NULL : ptc->reference;
}

// insérer dans la liste li, objet après precedent
// si precedent est NULL, insérer en tête de liste
void insererApresTupleT (ListeTupleT* li, ElementTupleT* precedent, objetTupleT* objet) {
	if (precedent == NULL) {
		insererEnTeteDeListeTupleT (li, objet);
	} else {
		ElementTupleT* nouveau= creerElementTupleT();
		nouveau->reference = objet;
		nouveau->suivant= precedent->suivant;
		precedent->suivant = nouveau;
		if (precedent == li->dernier) 
			li->dernier = nouveau;
		li->nbElt++;
	}
}


// insérer un objet en fin de la liste li
void insererEnFinDeListeTupleT (ListeTupleT* li, objetTupleT* objet) {
	insererApresTupleT (li, li->dernier, objet);
}

// extraire l'objet en tête de la liste li
objetTupleT* extraireEnTeteDeListeTupleT (ListeTupleT* li) {
	ElementTupleT* extrait = li->premier;
	if (!listeVideTupleT(li)) {
		li->premier = li->premier->suivant;
		if (li->premier==NULL) 
			li->dernier=NULL; // Liste devenue vide
		li->nbElt--;
	}
	return extrait != NULL ? extrait->reference : NULL;
}


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetTupleT* extraireApresTupleT (ListeTupleT* li, ElementTupleT* precedent) {
	if (precedent == NULL) {
		return extraireEnTeteDeListeTupleT (li);
	} else {
		ElementTupleT* extrait = precedent->suivant;
		if (extrait != NULL) {
			precedent->suivant = extrait->suivant;
		if (extrait == li->dernier) li->dernier = precedent;
			li->nbElt--;
		}
		return extrait != NULL ? extrait->reference : NULL;
	}
}


// extraire l'objet en fin de la liste li
objetTupleT* extraireEnFinDeListeTupleT (ListeTupleT* li) {
	objetTupleT* extrait;
	if (listeVideTupleT(li)) {
		extrait = NULL;
	} else if (li->premier == li->dernier) { // un seul élément
		extrait = extraireEnTeteDeListeTupleT (li);
	} else {
		ElementTupleT* ptc = li->premier;
	while (ptc->suivant != li->dernier) ptc = ptc->suivant;
		extrait = extraireApresTupleT (li, ptc);
	}
	return extrait;
}


// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetTupleT (ListeTupleT* li, objetTupleT* objet) {
	ElementTupleT* precedent = NULL;
	ElementTupleT* ptc= NULL;
	// repère l'élément précédent
	booleen trouve = faux;
	ouvrirListeTupleT (li);
	while (!finListeTupleT (li) && !trouve) {
		precedent = ptc;
		ptc= elementCourantTupleT (li);
		trouve = (ptc->reference == objet) ? vrai : faux;
	}
	if (!trouve)
		 return faux;
	objetTupleT* extrait = extraireApresTupleT (li, precedent);
	return vrai;
}


// parcours de liste avec destruction de chaque élément
void detruireListeTupleT (ListeTupleT* li) {
	ouvrirListeTupleT (li);
	while (!finListeTupleT (li)) {
		ElementTupleT* ptc = elementCourantTupleT (li);
		free (ptc->reference); //si on veut détruire les objets de la liste
		free (ptc);
	}
	initListeTupleT (li,NULL,NULL);
}

// recopie l2 dans l1 et initialise l2
void recopierListeTupleT (ListeTupleT* l1, ListeTupleT* l2) {
	detruireListeTupleT (l1);
	*l1 = *l2;
	// on recopie les têtes de listes
}

/*=======================================Udt=======================================*/



// initialiser la liste pointée par li (cas général)
void initListeUdt (ListeUdt* li, char* (*toString) (objetUdt*),int (*comparer) (objetUdt*, objetUdt*)) {
	li->premier = NULL;
	li->dernier = NULL;
	li->courant = NULL;
	li->nbElt= 0;
	li->toString = toString;
	li->comparer = comparer;
}

ListeUdt* creerListeUdt (char* (*toString) (objetUdt*),int (*comparer) (objetUdt*, objetUdt*)) {
	ListeUdt* li =(ListeUdt *)malloc(sizeof(ListeUdt));
	initListeUdt (li, toString, comparer);
	return li;
}

// la liste est-elle vide ?
int listeVideUdt (ListeUdt* li) {
	return li->nbElt == 0;
}

// fournir le nombre d'éléments dans la liste
int nbElementUdt (ListeUdt* li) {
	return li->nbElt;
}

ElementUdt* creerElementUdt () {
	return (ElementUdt *)malloc(sizeof(ElementUdt));
}


// insérer objetUdt en tête de la liste li
// l'objetUdt est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeUdt (ListeUdt* li, objetUdt* ch) {
	ElementUdt* nouveau= creerElementUdt();
	nouveau->reference = ch;
	nouveau->suivant= li->premier;
	li->premier= nouveau;
	if (li->dernier == NULL) li->dernier = nouveau;
	li->nbElt++;
}


// insérer un objetUdt en fin de la liste li
/*void insererEnFinDeListe (ListeUdt* li, objetUdt* ch) {
	ElementUdt* nouveaux= creerElementUdt();
	nouveaux->reference = ch;
	nouveaux->suivant=NULL;
	if(li->premier==NULL){
		li->premier=nouveaux;
	}
	else{
		li->dernier->suivant=nouveaux;	
	}
	li->dernier=nouveaux;
}*/

// se positionner sur le premier élément de la liste li
void ouvrirListeUdt (ListeUdt* li) {
	li->courant = li->premier;
}


int finListeUdt (ListeUdt* li) {
	return li->courant==NULL;
}


int finListeSuivantUdt (ListeUdt* li) {
	return li->courant->suivant==NULL;
}

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementUdt* elementCourantUdt (ListeUdt* li) {
	ElementUdt* ptc = li->courant;
	if (li->courant != NULL) {
		li->courant = li->courant->suivant;
	}
	return ptc;
}

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeUdt (ListeUdt* li, void (*f) (objetUdt*)) {
	ouvrirListeUdt (li);
	while (!finListeUdt (li)) {
		objetUdt* ch = objetCourantUdt (li);
		f (ch); // appliquer la fonction f() à objetUdt
	}
}

// fournir un pointeur sur l'objetUdt "objetCherche" de la liste li;
// NULL si l'objetUdt n'existe pas
objetUdt* chercherUnobjetUdt (ListeUdt* li, objetUdt* objetCherche) {
	int trouve=0;
	void* ch;
	// pointeur courant
	ouvrirListeUdt (li);
	while (!finListeUdt (li) && !trouve) {
		ch = objetCourantUdt (li);
		trouve = li->comparer (objetCherche, ch) == 0;
	}
	return trouve ? ch : NULL;
}

// fournir un pointeur sur l'objet courant de la liste li,
// et se positionner sur le suivant qui devient le courant
objetUdt* objetCourantUdt (ListeUdt* li) {
	ElementUdt* ptc = elementCourantUdt (li);
	return ptc==NULL ? NULL : ptc->reference;
}

// insérer dans la liste li, objet après precedent
// si precedent est NULL, insérer en tête de liste
void insererApresUdt (ListeUdt* li, ElementUdt* precedent, objetUdt* objet) {
	if (precedent == NULL) {
		insererEnTeteDeListeUdt (li, objet);
	} else {
		ElementUdt* nouveau= creerElementUdt();
		nouveau->reference = objet;
		nouveau->suivant= precedent->suivant;
		precedent->suivant = nouveau;
		if (precedent == li->dernier) 
			li->dernier = nouveau;
		li->nbElt++;
	}
}


// insérer un objet en fin de la liste li
void insererEnFinDeListeUdt (ListeUdt* li, objetUdt* objet) {
	insererApresUdt (li, li->dernier, objet);
}

// extraire l'objet en tête de la liste li
objetUdt* extraireEnTeteDeListeUdt (ListeUdt* li) {
	ElementUdt* extrait = li->premier;
	if (!listeVideUdt(li)) {
		li->premier = li->premier->suivant;
		if (li->premier==NULL) 
			li->dernier=NULL; // Liste devenue vide
		li->nbElt--;
	}
	return extrait != NULL ? extrait->reference : NULL;
}


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetUdt* extraireApresUdt (ListeUdt* li, ElementUdt* precedent) {
	if (precedent == NULL) {
		return extraireEnTeteDeListeUdt (li);
	} else {
		ElementUdt* extrait = precedent->suivant;
		if (extrait != NULL) {
			precedent->suivant = extrait->suivant;
		if (extrait == li->dernier) li->dernier = precedent;
			li->nbElt--;
		}
		return extrait != NULL ? extrait->reference : NULL;
	}
}


// extraire l'objet en fin de la liste li
objetUdt* extraireEnFinDeListeUdt (ListeUdt* li) {
	objetUdt* extrait;
	if (listeVideUdt(li)) {
		extrait = NULL;
	} else if (li->premier == li->dernier) { // un seul élément
		extrait = extraireEnTeteDeListeUdt (li);
	} else {
		ElementUdt* ptc = li->premier;
	while (ptc->suivant != li->dernier) ptc = ptc->suivant;
		extrait = extraireApresUdt (li, ptc);
	}
	return extrait;
}


// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetUdt (ListeUdt* li, objetUdt* objet) {
	ElementUdt* precedent = NULL;
	ElementUdt* ptc= NULL;
	// repère l'élément précédent
	booleen trouve = faux;
	ouvrirListeUdt (li);
	while (!finListeUdt (li) && !trouve) {
		precedent = ptc;
		ptc= elementCourantUdt (li);
		trouve = (ptc->reference == objet) ? vrai : faux;
	}
	if (!trouve)
		 return faux;
	objetUdt* extrait = extraireApresUdt (li, precedent);
	return vrai;
}


// parcours de liste avec destruction de chaque élément
void detruireListeUdt (ListeUdt* li) {
	ouvrirListeUdt (li);
	while (!finListeUdt (li)) {
		ElementUdt* ptc = elementCourantUdt (li);
		free (ptc->reference); //si on veut détruire les objets de la liste
		free (ptc);
	}
	initListeUdt (li,NULL,NULL);
}

// recopie l2 dans l1 et initialise l2
void recopierListeUdt (ListeUdt* l1, ListeUdt* l2) {
	detruireListeUdt (l1);
	*l1 = *l2;
	// on recopie les têtes de listes
}

/*=======================================TypesUdt=======================================*/

// initialiser la liste pointée par li (cas général)
void initListeTypesUdt (ListeTypesUdt* li, char* (*toString) (objetTypesUdt*),int (*comparer) (objetTypesUdt*, objetTypesUdt*)) {
	li->premier = NULL;
	li->dernier = NULL;
	li->courant = NULL;
	li->nbElt= 0;
	li->toString = toString;
	li->comparer = comparer;
}

ListeTypesUdt* creerListeTypesUdt (char* (*toString) (objetTypesUdt*),int (*comparer) (objetTypesUdt*, objetTypesUdt*)) {
	ListeTypesUdt* li =(ListeTypesUdt *)malloc(sizeof(ListeTypesUdt));
	initListeTypesUdt (li, toString, comparer);
	return li;
}

// la liste est-elle vide ?
int listeVideTypesUdt (ListeTypesUdt* li) {
	return li->nbElt == 0;
}

// fournir le nombre d'éléments dans la liste
int nbElementTypesUdt (ListeTypesUdt* li) {
	return li->nbElt;
}

ElementTypesUdt* creerElementTypesUdt () {
	return (ElementTypesUdt *)malloc(sizeof(ElementTypesUdt));
}


// insérer objetTypesUdt en tête de la liste li
// l'objetTypesUdt est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeTypesUdt (ListeTypesUdt* li, objetTypesUdt* ch) {
	ElementTypesUdt* nouveau= creerElementTypesUdt();
	nouveau->reference = ch;
	nouveau->suivant= li->premier;
	li->premier= nouveau;
	if (li->dernier == NULL) li->dernier = nouveau;
	li->nbElt++;
}


// insérer un objetTypesUdt en fin de la liste li
/*void insererEnFinDeListe (ListeTypesUdt* li, objetTypesUdt* ch) {
	ElementTypesUdt* nouveaux= creerElementTypesUdt();
	nouveaux->reference = ch;
	nouveaux->suivant=NULL;
	if(li->premier==NULL){
		li->premier=nouveaux;
	}
	else{
		li->dernier->suivant=nouveaux;	
	}
	li->dernier=nouveaux;
}*/

// se positionner sur le premier élément de la liste li
void ouvrirListeTypesUdt (ListeTypesUdt* li) {
	li->courant = li->premier;
}


int finListeTypesUdt (ListeTypesUdt* li) {
	return li->courant==NULL;
}


int finListeSuivantTypesUdt (ListeTypesUdt* li) {
	return li->courant->suivant==NULL;
}

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementTypesUdt* elementCourantTypesUdt (ListeTypesUdt* li) {
	ElementTypesUdt* ptc = li->courant;
	if (li->courant != NULL) {
		li->courant = li->courant->suivant;
	}
	return ptc;
}

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeTypesUdt (ListeTypesUdt* li, void (*f) (objetTypesUdt*)) {
	ouvrirListeTypesUdt (li);
	while (!finListeTypesUdt (li)) {
		objetTypesUdt* ch = objetCourantTypesUdt (li);
		f (ch); // appliquer la fonction f() à objetTypesUdt
	}
}

// fournir un pointeur sur l'objetTypesUdt "objetCherche" de la liste li;
// NULL si l'objetTypesUdt n'existe pas
objetTypesUdt* chercherUnobjetTypesUdt (ListeTypesUdt* li, objetTypesUdt* objetCherche) {
	int trouve=0;
	void* ch;
	// pointeur courant
	ouvrirListeTypesUdt (li);
	while (!finListeTypesUdt (li) && !trouve) {
		ch = objetCourantTypesUdt (li);
		trouve = li->comparer (objetCherche, ch) == 0;
	}
	return trouve ? ch : NULL;
}

// fournir un pointeur sur l'objet courant de la liste li,
// et se positionner sur le suivant qui devient le courant
objetTypesUdt* objetCourantTypesUdt (ListeTypesUdt* li) {
	ElementTypesUdt* ptc = elementCourantTypesUdt (li);
	return ptc==NULL ? NULL : ptc->reference;
}

// insérer dans la liste li, objet après precedent
// si precedent est NULL, insérer en tête de liste
void insererApresTypesUdt (ListeTypesUdt* li, ElementTypesUdt* precedent, objetTypesUdt* objet) {
	if (precedent == NULL) {
		insererEnTeteDeListeTypesUdt (li, objet);
	} else {
		ElementTypesUdt* nouveau= creerElementTypesUdt();
		nouveau->reference = objet;
		nouveau->suivant= precedent->suivant;
		precedent->suivant = nouveau;
		if (precedent == li->dernier) 
			li->dernier = nouveau;
		li->nbElt++;
	}
}


// insérer un objet en fin de la liste li
void insererEnFinDeListeTypesUdt (ListeTypesUdt* li, objetTypesUdt* objet) {
	insererApresTypesUdt (li, li->dernier, objet);
}

// extraire l'objet en tête de la liste li
objetTypesUdt* extraireEnTeteDeListeTypesUdt (ListeTypesUdt* li) {
	ElementTypesUdt* extrait = li->premier;
	if (!listeVideTypesUdt(li)) {
		li->premier = li->premier->suivant;
		if (li->premier==NULL) 
			li->dernier=NULL; // Liste devenue vide
		li->nbElt--;
	}
	return extrait != NULL ? extrait->reference : NULL;
}


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetTypesUdt* extraireApresTypesUdt (ListeTypesUdt* li, ElementTypesUdt* precedent) {
	if (precedent == NULL) {
		return extraireEnTeteDeListeTypesUdt (li);
	} else {
		ElementTypesUdt* extrait = precedent->suivant;
		if (extrait != NULL) {
			precedent->suivant = extrait->suivant;
		if (extrait == li->dernier) li->dernier = precedent;
			li->nbElt--;
		}
		return extrait != NULL ? extrait->reference : NULL;
	}
}


// extraire l'objet en fin de la liste li
objetTypesUdt* extraireEnFinDeListeTypesUdt (ListeTypesUdt* li) {
	objetTypesUdt* extrait;
	if (listeVideTypesUdt(li)) {
		extrait = NULL;
	} else if (li->premier == li->dernier) { // un seul élément
		extrait = extraireEnTeteDeListeTypesUdt (li);
	} else {
		ElementTypesUdt* ptc = li->premier;
	while (ptc->suivant != li->dernier) ptc = ptc->suivant;
		extrait = extraireApresTypesUdt (li, ptc);
	}
	return extrait;
}


// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetTypesUdt (ListeTypesUdt* li, objetTypesUdt* objet) {
	ElementTypesUdt* precedent = NULL;
	ElementTypesUdt* ptc= NULL;
	// repère l'élément précédent
	booleen trouve = faux;
	ouvrirListeTypesUdt (li);
	while (!finListeTypesUdt (li) && !trouve) {
		precedent = ptc;
		ptc= elementCourantTypesUdt (li);
		trouve = (ptc->reference == objet) ? vrai : faux;
	}
	if (!trouve)
		 return faux;
	objetTypesUdt* extrait = extraireApresTypesUdt (li, precedent);
	return vrai;
}


// parcours de liste avec destruction de chaque élément
void detruireListeTypesUdt (ListeTypesUdt* li) {
	ouvrirListeTypesUdt (li);
	while (!finListeTypesUdt (li)) {
		ElementTypesUdt* ptc = elementCourantTypesUdt (li);
		free (ptc->reference); //si on veut détruire les objets de la liste
		free (ptc);
	}
	initListeTypesUdt (li,NULL,NULL);
}

// recopie l2 dans l1 et initialise l2
void recopierListeTypesUdt (ListeTypesUdt* l1, ListeTypesUdt* l2) {
	detruireListeTypesUdt (l1);
	*l1 = *l2;
	// on recopie les têtes de listes
}


/*=======================================Tuple=======================================*/



// initialiser la liste pointée par li (cas général)
void initListeTuple (ListeTuple* li, char* (*toString) (objetTuple*),int (*comparer) (objetTuple*, objetTuple*)) {
	li->premier = NULL;
	li->dernier = NULL;
	li->courant = NULL;
	li->nbElt= 0;
	li->toString = toString;
	li->comparer = comparer;
}

ListeTuple* creerListeTuple (char* (*toString) (objetTuple*),int (*comparer) (objetTuple*, objetTuple*)) {
	ListeTuple* li =(ListeTuple *)malloc(sizeof(ListeTuple));
	initListeTuple (li, toString, comparer);
	return li;
}

// la liste est-elle vide ?
int listeVideTuple (ListeTuple* li) {
	return li->nbElt == 0;
}

// fournir le nombre d'éléments dans la liste
int nbElementTuple (ListeTuple* li) {
	return li->nbElt;
}

ElementTuple* creerElementTuple () {
	return (ElementTuple *)malloc(sizeof(ElementTuple));
}


// insérer objetTuple en tête de la liste li
// l'objetTuple est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeTuple (ListeTuple* li, objetTuple* ch) {
	ElementTuple* nouveau= creerElementTuple();
	nouveau->reference = ch;
	nouveau->suivant= li->premier;
	li->premier= nouveau;
	if (li->dernier == NULL) li->dernier = nouveau;
	li->nbElt++;
}


// insérer un objetTuple en fin de la liste li
/*void insererEnFinDeListe (ListeTuple* li, objetTuple* ch) {
	ElementTuple* nouveaux= creerElementTuple();
	nouveaux->reference = ch;
	nouveaux->suivant=NULL;
	if(li->premier==NULL){
		li->premier=nouveaux;
	}
	else{
		li->dernier->suivant=nouveaux;	
	}
	li->dernier=nouveaux;
}*/

// se positionner sur le premier élément de la liste li
void ouvrirListeTuple (ListeTuple* li) {
	li->courant = li->premier;
}


int finListeTuple (ListeTuple* li) {
	return li->courant==NULL;
}


int finListeSuivantTuple (ListeTuple* li) {
	return li->courant->suivant==NULL;
}

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementTuple* elementCourantTuple (ListeTuple* li) {
	ElementTuple* ptc = li->courant;
	if (li->courant != NULL) {
		li->courant = li->courant->suivant;
	}
	return ptc;
}

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeTuple (ListeTuple* li, void (*f) (objetTuple*)) {
	ouvrirListeTuple (li);
	while (!finListeTuple (li)) {
		objetTuple* ch = objetCourantTuple (li);
		f (ch); // appliquer la fonction f() à objetTuple
	}
}

// fournir un pointeur sur l'objetTuple "objetCherche" de la liste li;
// NULL si l'objetTuple n'existe pas
objetTuple* chercherUnobjetTuple (ListeTuple* li, objetTuple* objetCherche) {
	int trouve=0;
	void* ch;
	// pointeur courant
	ouvrirListeTuple (li);
	while (!finListeTuple (li) && !trouve) {
		ch = objetCourantTuple (li);
		trouve = li->comparer (objetCherche, ch) == 0;
	}
	return trouve ? ch : NULL;
}

// fournir un pointeur sur l'objet courant de la liste li,
// et se positionner sur le suivant qui devient le courant
objetTuple* objetCourantTuple (ListeTuple* li) {
	ElementTuple* ptc = elementCourantTuple (li);
	return ptc==NULL ? NULL : ptc->reference;
}

// insérer dans la liste li, objet après precedent
// si precedent est NULL, insérer en tête de liste
void insererApresTuple (ListeTuple* li, ElementTuple* precedent, objetTuple* objet) {
	if (precedent == NULL) {
		insererEnTeteDeListeTuple (li, objet);
	} else {
		ElementTuple* nouveau= creerElementTuple();
		nouveau->reference = objet;
		nouveau->suivant= precedent->suivant;
		precedent->suivant = nouveau;
		if (precedent == li->dernier) 
			li->dernier = nouveau;
		li->nbElt++;
	}
}


// insérer un objet en fin de la liste li
void insererEnFinDeListeTuple (ListeTuple* li, objetTuple* objet) {
	insererApresTuple (li, li->dernier, objet);
}

// extraire l'objet en tête de la liste li
objetTuple* extraireEnTeteDeListeTuple (ListeTuple* li) {
	ElementTuple* extrait = li->premier;
	if (!listeVideTuple(li)) {
		li->premier = li->premier->suivant;
		if (li->premier==NULL) 
			li->dernier=NULL; // Liste devenue vide
		li->nbElt--;
	}
	return extrait != NULL ? extrait->reference : NULL;
}


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetTuple* extraireApresTuple (ListeTuple* li, ElementTuple* precedent) {
	if (precedent == NULL) {
		return extraireEnTeteDeListeTuple (li);
	} else {
		ElementTuple* extrait = precedent->suivant;
		if (extrait != NULL) {
			precedent->suivant = extrait->suivant;
		if (extrait == li->dernier) li->dernier = precedent;
			li->nbElt--;
		}
		return extrait != NULL ? extrait->reference : NULL;
	}
}


// extraire l'objet en fin de la liste li
objetTuple* extraireEnFinDeListeTuple (ListeTuple* li) {
	objetTuple* extrait;
	if (listeVideTuple(li)) {
		extrait = NULL;
	} else if (li->premier == li->dernier) { // un seul élément
		extrait = extraireEnTeteDeListeTuple (li);
	} else {
		ElementTuple* ptc = li->premier;
	while (ptc->suivant != li->dernier) ptc = ptc->suivant;
		extrait = extraireApresTuple (li, ptc);
	}
	return extrait;
}


// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetTuple (ListeTuple* li, objetTuple* objet) {
	ElementTuple* precedent = NULL;
	ElementTuple* ptc= NULL;
	// repère l'élément précédent
	booleen trouve = faux;
	ouvrirListeTuple (li);
	while (!finListeTuple (li) && !trouve) {
		precedent = ptc;
		ptc= elementCourantTuple (li);
		trouve = (ptc->reference == objet) ? vrai : faux;
	}
	if (!trouve)
		 return faux;
	objetTuple* extrait = extraireApresTuple (li, precedent);
	return vrai;
}


// parcours de liste avec destruction de chaque élément
void detruireListeTuple (ListeTuple* li) {
	ouvrirListeTuple (li);
	while (!finListeTuple (li)) {
		ElementTuple* ptc = elementCourantTuple (li);
		free (ptc->reference); //si on veut détruire les objets de la liste
		free (ptc);
	}
	initListeTuple (li,NULL,NULL);
}

// recopie l2 dans l1 et initialise l2
void recopierListeTuple (ListeTuple* l1, ListeTuple* l2) {
	detruireListeTuple (l1);
	*l1 = *l2;
	// on recopie les têtes de listes
}



/*=======================================Name=======================================*/



// initialiser la liste pointée par li (cas général)
void initListeName (ListeName* li, char* (*toString) (char*),int (*comparer) (char*, char*)) {
    li->premier = NULL;
    li->dernier = NULL;
    li->courant = NULL;
    li->nbElt= 0;
    li->toString = toString;
    li->comparer = strcmp;
}

ListeName* creerListeName (char* (*toString) (char*),int (*comparer) (char*, char*)) {
    ListeName* li =(ListeName *)malloc(sizeof(ListeName));
    initListeName (li, toString, comparer);
    return li;
}

// la liste est-elle vide ?
int listeVideName (ListeName* li) {
    return li->nbElt == 0;
}

// fournir le nombre d'éléments dans la liste
int nbElementName (ListeName* li) {
    return li->nbElt;
}

ElementName* creerElementName () {
    return (ElementName *)malloc(sizeof(ElementName));
}


// insérer char en tête de la liste li
// l'char est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeName (ListeName* li, char* ch) {
    ElementName* nouveau= creerElementName();
    nouveau->reference = ch;
    nouveau->suivant= li->premier;
    li->premier= nouveau;
    if (li->dernier == NULL) li->dernier = nouveau;
    li->nbElt++;
}


// insérer un char en fin de la liste li
/*void insererEnFinDeListe (ListeName* li, char* ch) {
  ElementName* nouveaux= creerElementName();
  nouveaux->reference = ch;
  nouveaux->suivant=NULL;
  if(li->premier==NULL){
    li->premier=nouveaux;
  }
  else{
    li->dernier->suivant=nouveaux;
  }
  li->dernier=nouveaux;
}*/

// se positionner sur le premier élément de la liste li
void ouvrirListeName (ListeName* li) {
    li->courant = li->premier;
}


int finListeName (ListeName* li) {
    return li->courant==NULL;
}


int finListeSuivantName (ListeName* li) {
    return li->courant->suivant==NULL;
}

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementName* elementCourantName (ListeName* li) {
    ElementName* ptc = li->courant;
    if (li->courant != NULL) {
        li->courant = li->courant->suivant;
    }
    return ptc;
}

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeName (ListeName* li, void (*f) (char*)) {
    ouvrirListeName (li);
    while (!finListeName (li)) {
        char* ch = objetCourantName (li);
        f (ch); // appliquer la fonction f() à char
    }
}

// fournir un pointeur sur l'char "objetCherche" de la liste li;
// NULL si l'char n'existe pas
char* chercherUnchar (ListeName* li, char* objetCherche) {
    int trouve=0;
    void* ch;
    // pointeur courant
    ouvrirListeName (li);
    while (!finListeName (li) && !trouve) {
        ch = objetCourantName (li);
        trouve = li->comparer (objetCherche, ch) == 0;
    }
    return trouve ? ch : NULL;
}

// fournir un pointeur sur l'objet courant de la liste li,
// et se positionner sur le suivant qui devient le courant
char* objetCourantName (ListeName* li) {
    ElementName* ptc = elementCourantName (li);
    return ptc==NULL ? NULL : ptc->reference;
}

// insérer dans la liste li, objet après precedent
// si precedent est NULL, insérer en tête de liste
void insererApresName (ListeName* li, ElementName* precedent, char* objet) {
    if (precedent == NULL) {
        insererEnTeteDeListeName (li, objet);
    } else {
        ElementName* nouveau= creerElementName();
        nouveau->reference = objet;
        nouveau->suivant= precedent->suivant;
        precedent->suivant = nouveau;
        if (precedent == li->dernier)
            li->dernier = nouveau;
        li->nbElt++;
    }
}


// insérer un objet en fin de la liste li
void insererEnFinDeListeName (ListeName* li, char* objet) {
    insererApresName (li, li->dernier, objet);
}

// extraire l'objet en tête de la liste li
char* extraireEnTeteDeListeName (ListeName* li) {
    ElementName* extrait = li->premier;
    if (!listeVideName(li)) {
        li->premier = li->premier->suivant;
        if (li->premier==NULL)
            li->dernier=NULL; // Liste devenue vide
        li->nbElt--;
    }
    return extrait != NULL ? extrait->reference : NULL;
}


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
char* extraireApresName (ListeName* li, ElementName* precedent) {
    if (precedent == NULL) {
        return extraireEnTeteDeListeName (li);
    } else {
        ElementName* extrait = precedent->suivant;
        if (extrait != NULL) {
            precedent->suivant = extrait->suivant;
            if (extrait == li->dernier) li->dernier = precedent;
            li->nbElt--;
        }
        return extrait != NULL ? extrait->reference : NULL;
    }
}


// extraire l'objet en fin de la liste li
char* extraireEnFinDeListeName (ListeName* li) {
    char* extrait;
    if (listeVideName(li)) {
        extrait = NULL;
    } else if (li->premier == li->dernier) { // un seul élément
        extrait = extraireEnTeteDeListeName (li);
    } else {
        ElementName* ptc = li->premier;
        while (ptc->suivant != li->dernier) ptc = ptc->suivant;
        extrait = extraireApresName (li, ptc);
    }
    return extrait;
}


// extraire de la liste li, l'objet pointé par objet
booleen extraireUnchar (ListeName* li, char* objet) {
    ElementName* precedent = NULL;
    ElementName* ptc= NULL;
    // repère l'élément précédent
    booleen trouve = faux;
    ouvrirListeName (li);
    while (!finListeName (li) && !trouve) {
        precedent = ptc;
        ptc= elementCourantName (li);
        trouve = (ptc->reference == objet) ? vrai : faux;
    }
    if (!trouve)
        return faux;
    char* extrait = extraireApresName (li, precedent);
    return vrai;
}


// parcours de liste avec destruction de chaque élément
void detruireListeName (ListeName* li) {
    ouvrirListeName (li);
    while (!finListeName (li)) {
        ElementName* ptc = elementCourantName (li);
        //free (ptc->reference); //si on veut détruire les objets de la liste
        free (ptc);
    }
    initListeName (li,NULL,NULL);
}

// recopie l2 dans l1 et initialise l2
void recopierListeName (ListeName* l1, ListeName* l2) {
    detruireListeName (l1);
    *l1 = *l2;
    // on recopie les têtes de listes
}


/*=======================================NameValue=======================================*/



// initialiser la liste pointée par li (cas général)
void initListeNameValue (ListeNameValue* li, char* (*toString) (objetNameValue*),int (*comparer) (objetNameValue*, objetNameValue*)) {
	li->premier = NULL;
	li->dernier = NULL;
	li->courant = NULL;
	li->nbElt= 0;
	li->toString = toString;
	li->comparer = comparerNameValue;
}

ListeNameValue* creerListeNameValue (char* (*toString) (objetNameValue*),int (*comparer) (objetNameValue*, objetNameValue*)) {
	ListeNameValue* li =(ListeNameValue *)malloc(sizeof(ListeNameValue));
	initListeNameValue (li, toString, comparer);
	return li;
}

// la liste est-elle vide ?
int listeVideNameValue (ListeNameValue* li) {
	return li->nbElt == 0;
}

// fournir le nombre d'éléments dans la liste
int nbElementNameValue (ListeNameValue* li) {
	return li->nbElt;
}

ElementNameValue* creerElementNameValue () {
	return (ElementNameValue *)malloc(sizeof(ElementNameValue));
}


// insérer objetNameValue en tête de la liste li
// l'objetNameValue est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeNameValue (ListeNameValue* li, objetNameValue* ch) {
	ElementNameValue* nouveau= creerElementNameValue();
	nouveau->reference = ch;
	nouveau->suivant= li->premier;
	li->premier= nouveau;
	if (li->dernier == NULL) li->dernier = nouveau;
	li->nbElt++;
}


// insérer un objetNameValue en fin de la liste li
/*void insererEnFinDeListe (ListeNameValue* li, objetNameValue* ch) {
	ElementNameValue* nouveaux= creerElementNameValue();
	nouveaux->reference = ch;
	nouveaux->suivant=NULL;
	if(li->premier==NULL){
		li->premier=nouveaux;
	}
	else{
		li->dernier->suivant=nouveaux;
	}
	li->dernier=nouveaux;
}*/

// se positionner sur le premier élément de la liste li
void ouvrirListeNameValue (ListeNameValue* li) {
	li->courant = li->premier;
}


int finListeNameValue (ListeNameValue* li) {
	return li->courant==NULL;
}


int finListeSuivantNameValue (ListeNameValue* li) {
	return li->courant->suivant==NULL;
}

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementNameValue* elementCourantNameValue (ListeNameValue* li) {
	ElementNameValue* ptc = li->courant;
	if (li->courant != NULL) {
		li->courant = li->courant->suivant;
	}
	return ptc;
}

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeNameValue (ListeNameValue* li, void (*f) (objetNameValue*)) {
	ouvrirListeNameValue (li);
	while (!finListeNameValue (li)) {
		objetNameValue* ch = objetCourantNameValue (li);
		f (ch); // appliquer la fonction f() à objetNameValue
	}
}

// fournir un pointeur sur l'objetNameValue "objetCherche" de la liste li;
// NULL si l'objetNameValue n'existe pas
objetNameValue* chercherUnobjetNameValue (ListeNameValue* li, objetNameValue* objetCherche) {
	int trouve=0;
	void* ch;
	// pointeur courant
	ouvrirListeNameValue (li);
	while (!finListeNameValue (li) && !trouve) {
		ch = objetCourantNameValue (li);
		trouve = li->comparer (objetCherche, ch) == 0;
	}
	return trouve ? ch : NULL;
}

// fournir un pointeur sur l'objet courant de la liste li,
// et se positionner sur le suivant qui devient le courant
objetNameValue* objetCourantNameValue (ListeNameValue* li) {
	ElementNameValue* ptc = elementCourantNameValue (li);
	return ptc==NULL ? NULL : ptc->reference;
}

// insérer dans la liste li, objet après precedent
// si precedent est NULL, insérer en tête de liste
void insererApresNameValue (ListeNameValue* li, ElementNameValue* precedent, objetNameValue* objet) {
	if (precedent == NULL) {
		insererEnTeteDeListeNameValue (li, objet);
	} else {
		ElementNameValue* nouveau= creerElementNameValue();
		nouveau->reference = objet;
		nouveau->suivant= precedent->suivant;
		precedent->suivant = nouveau;
		if (precedent == li->dernier)
			li->dernier = nouveau;
		li->nbElt++;
	}
}


// insérer un objet en fin de la liste li
void insererEnFinDeListeNameValue (ListeNameValue* li, objetNameValue* objet) {
	insererApresNameValue (li, li->dernier, objet);
}

// extraire l'objet en tête de la liste li
objetNameValue* extraireEnTeteDeListeNameValue (ListeNameValue* li) {
	ElementNameValue* extrait = li->premier;
	if (!listeVideNameValue(li)) {
		li->premier = li->premier->suivant;
		if (li->premier==NULL)
			li->dernier=NULL; // Liste devenue vide
		li->nbElt--;
	}
	return extrait != NULL ? extrait->reference : NULL;
}


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetNameValue* extraireApresNameValue (ListeNameValue* li, ElementNameValue* precedent) {
	if (precedent == NULL) {
		return extraireEnTeteDeListeNameValue (li);
	} else {
		ElementNameValue* extrait = precedent->suivant;
		if (extrait != NULL) {
			precedent->suivant = extrait->suivant;
			if (extrait == li->dernier) li->dernier = precedent;
			li->nbElt--;
		}
		return extrait != NULL ? extrait->reference : NULL;
	}
}


// extraire l'objet en fin de la liste li
objetNameValue* extraireEnFinDeListeNameValue (ListeNameValue* li) {
	objetNameValue* extrait;
	if (listeVideNameValue(li)) {
		extrait = NULL;
	} else if (li->premier == li->dernier) { // un seul élément
		extrait = extraireEnTeteDeListeNameValue (li);
	} else {
		ElementNameValue* ptc = li->premier;
		while (ptc->suivant != li->dernier) ptc = ptc->suivant;
		extrait = extraireApresNameValue (li, ptc);
	}
	return extrait;
}


// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetNameValue (ListeNameValue* li, objetNameValue* objet) {
	ElementNameValue* precedent = NULL;
	ElementNameValue* ptc= NULL;
	// repère l'élément précédent
	booleen trouve = faux;
	ouvrirListeNameValue (li);
	while (!finListeNameValue (li) && !trouve) {
		precedent = ptc;
		ptc= elementCourantNameValue (li);
		trouve = (ptc->reference == objet) ? vrai : faux;
	}
	if (!trouve)
		return faux;
	objetNameValue* extrait = extraireApresNameValue (li, precedent);
	return vrai;
}


// parcours de liste avec destruction de chaque élément
void detruireListeNameValue (ListeNameValue* li) {
	ouvrirListeNameValue (li);
	while (!finListeNameValue (li)) {
		ElementNameValue* ptc = elementCourantNameValue (li);
		//free (ptc->reference); //si on veut détruire les objets de la liste
		free (ptc);
	}
	initListeNameValue (li,NULL,NULL);
}

// recopie l2 dans l1 et initialise l2
void recopierListeNameValue (ListeNameValue* l1, ListeNameValue* l2) {
	detruireListeNameValue (l1);
	*l1 = *l2;
	// on recopie les têtes de listes
}
/*=======================================Relation=======================================*/



// initialiser la liste pointée par li (cas général)
void initListeRelation (ListeRelation* li, char* (*toString) (objetRelation*),int (*comparer) (objetRelation*, objetRelation*)) {
	li->premier = NULL;
	li->dernier = NULL;
	li->courant = NULL;
	li->nbElt= 0;
	li->toString = toString;
	li->comparer = comparerRelation;
}

ListeRelation* creerListeRelation (char* (*toString) (objetRelation*),int (*comparer) (objetRelation*, objetRelation*)) {
	ListeRelation* li =(ListeRelation *)malloc(sizeof(ListeRelation));
	initListeRelation (li, toString, comparer);
	return li;
}

// la liste est-elle vide ?
int listeVideRelation (ListeRelation* li) {
	return li->nbElt == 0;
}

// fournir le nombre d'éléments dans la liste
int nbElementRelation (ListeRelation* li) {
	return li->nbElt;
}

ElementRelation* creerElementRelation () {
	return (ElementRelation *)malloc(sizeof(ElementRelation));
}


// insérer objetRelation en tête de la liste li
// l'objetRelation est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeRelation (ListeRelation* li, objetRelation* ch) {
	ElementRelation* nouveau= creerElementRelation();
	nouveau->reference = ch;
	nouveau->suivant= li->premier;
	li->premier= nouveau;
	if (li->dernier == NULL) li->dernier = nouveau;
	li->nbElt++;
}


// insérer un objetRelation en fin de la liste li
/*void insererEnFinDeListe (ListeRelation* li, objetRelation* ch) {
	ElementRelation* nouveaux= creerElementRelation();
	nouveaux->reference = ch;
	nouveaux->suivant=NULL;
	if(li->premier==NULL){
		li->premier=nouveaux;
	}
	else{
		li->dernier->suivant=nouveaux;
	}
	li->dernier=nouveaux;
}*/

// se positionner sur le premier élément de la liste li
void ouvrirListeRelation (ListeRelation* li) {
	li->courant = li->premier;
}


int finListeRelation (ListeRelation* li) {
	return li->courant==NULL;
}


int finListeSuivantRelation (ListeRelation* li) {
	return li->courant->suivant==NULL;
}

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementRelation* elementCourantRelation (ListeRelation* li) {
	ElementRelation* ptc = li->courant;
	if (li->courant != NULL) {
		li->courant = li->courant->suivant;
	}
	return ptc;
}

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeRelation (ListeRelation* li, void (*f) (objetRelation*)) {
	ouvrirListeRelation (li);
	while (!finListeRelation (li)) {
		objetRelation* ch = objetCourantRelation (li);
		f (ch); // appliquer la fonction f() à objetRelation
	}
}

// fournir un pointeur sur l'objetRelation "objetCherche" de la liste li;
// NULL si l'objetRelation n'existe pas
objetRelation* chercherUnobjetRelation (ListeRelation* li, objetRelation* objetCherche) {
	int trouve=0;
	void* ch;
	// pointeur courant
	ouvrirListeRelation (li);
	while (!finListeRelation (li) && !trouve) {
		ch = objetCourantRelation (li);
		trouve = li->comparer (objetCherche, ch) == 0;
	}
	return trouve ? ch : NULL;
}

// fournir un pointeur sur l'objet courant de la liste li,
// et se positionner sur le suivant qui devient le courant
objetRelation* objetCourantRelation (ListeRelation* li) {
	ElementRelation* ptc = elementCourantRelation (li);
	return ptc==NULL ? NULL : ptc->reference;
}

// insérer dans la liste li, objet après precedent
// si precedent est NULL, insérer en tête de liste
void insererApresRelation (ListeRelation* li, ElementRelation* precedent, objetRelation* objet) {
	if (precedent == NULL) {
		insererEnTeteDeListeRelation (li, objet);
	} else {
		ElementRelation* nouveau= creerElementRelation();
		nouveau->reference = objet;
		nouveau->suivant= precedent->suivant;
		precedent->suivant = nouveau;
		if (precedent == li->dernier)
			li->dernier = nouveau;
		li->nbElt++;
	}
}


// insérer un objet en fin de la liste li
void insererEnFinDeListeRelation (ListeRelation* li, objetRelation* objet) {
	insererApresRelation (li, li->dernier, objet);
}

// extraire l'objet en tête de la liste li
objetRelation* extraireEnTeteDeListeRelation (ListeRelation* li) {
	ElementRelation* extrait = li->premier;
	if (!listeVideRelation(li)) {
		li->premier = li->premier->suivant;
		if (li->premier==NULL)
			li->dernier=NULL; // Liste devenue vide
		li->nbElt--;
	}
	return extrait != NULL ? extrait->reference : NULL;
}


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetRelation* extraireApresRelation (ListeRelation* li, ElementRelation* precedent) {
	if (precedent == NULL) {
		return extraireEnTeteDeListeRelation (li);
	} else {
		ElementRelation* extrait = precedent->suivant;
		if (extrait != NULL) {
			precedent->suivant = extrait->suivant;
			if (extrait == li->dernier) li->dernier = precedent;
			li->nbElt--;
		}
		return extrait != NULL ? extrait->reference : NULL;
	}
}


// extraire l'objet en fin de la liste li
objetRelation* extraireEnFinDeListeRelation (ListeRelation* li) {
	objetRelation* extrait;
	if (listeVideRelation(li)) {
		extrait = NULL;
	} else if (li->premier == li->dernier) { // un seul élément
		extrait = extraireEnTeteDeListeRelation (li);
	} else {
		ElementRelation* ptc = li->premier;
		while (ptc->suivant != li->dernier) ptc = ptc->suivant;
		extrait = extraireApresRelation (li, ptc);
	}
	return extrait;
}


// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetRelation (ListeRelation* li, objetRelation* objet) {
	ElementRelation* precedent = NULL;
	ElementRelation* ptc= NULL;
	// repère l'élément précédent
	booleen trouve = faux;
	ouvrirListeRelation (li);
	while (!finListeRelation (li) && !trouve) {
		precedent = ptc;
		ptc= elementCourantRelation (li);
		trouve = (ptc->reference == objet) ? vrai : faux;
	}
	if (!trouve)
		return faux;
	objetRelation* extrait = extraireApresRelation (li, precedent);
	return vrai;
}


// parcours de liste avec destruction de chaque élément
void detruireListeRelation (ListeRelation* li) {
	ouvrirListeRelation (li);
	while (!finListeRelation (li)) {
		ElementRelation* ptc = elementCourantRelation (li);
		//free (ptc->reference); //si on veut détruire les objets de la liste
		free (ptc);
	}
	initListeRelation (li,NULL,NULL);
}

// recopie l2 dans l1 et initialise l2
void recopierListeRelation (ListeRelation* l1, ListeRelation* l2) {
	detruireListeRelation (l1);
	*l1 = *l2;
	// on recopie les têtes de listes
}


/*=======================================Selector=======================================*/



// initialiser la liste pointée par li (cas général)
void initListeSelector (ListeSelector* li, char* (*toString) (objetSelector*),int (*comparer) (objetSelector*, objetSelector*)) {
	li->premier = NULL;
	li->dernier = NULL;
	li->courant = NULL;
	li->nbElt= 0;
	li->toString = toString;
	li->comparer = comparerSelector;
}

ListeSelector* creerListeSelector (char* (*toString) (objetSelector*),int (*comparer) (objetSelector*, objetSelector*)) {
	ListeSelector* li =(ListeSelector *)malloc(sizeof(ListeSelector));
	initListeSelector (li, toString, comparer);
	return li;
}

// la liste est-elle vide ?
int listeVideSelector (ListeSelector* li) {
	return li->nbElt == 0;
}

// fournir le nombre d'éléments dans la liste
int nbElementSelector (ListeSelector* li) {
	return li->nbElt;
}

ElementSelector* creerElementSelector () {
	return (ElementSelector *)malloc(sizeof(ElementSelector));
}


// insérer objetSelector en tête de la liste li
// l'objetSelector est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeSelector (ListeSelector* li, objetSelector* ch) {
	ElementSelector* nouveau= creerElementSelector();
	nouveau->reference = ch;
	nouveau->suivant= li->premier;
	li->premier= nouveau;
	if (li->dernier == NULL) li->dernier = nouveau;
	li->nbElt++;
}


// insérer un objetSelector en fin de la liste li
/*void insererEnFinDeListe (ListeSelector* li, objetSelector* ch) {
	ElementSelector* nouveaux= creerElementSelector();
	nouveaux->reference = ch;
	nouveaux->suivant=NULL;
	if(li->premier==NULL){
		li->premier=nouveaux;
	}
	else{
		li->dernier->suivant=nouveaux;
	}
	li->dernier=nouveaux;
}*/

// se positionner sur le premier élément de la liste li
void ouvrirListeSelector (ListeSelector* li) {
	li->courant = li->premier;
}


int finListeSelector (ListeSelector* li) {
	return li->courant==NULL;
}


int finListeSuivantSelector (ListeSelector* li) {
	return li->courant->suivant==NULL;
}

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementSelector* elementCourantSelector (ListeSelector* li) {
	ElementSelector* ptc = li->courant;
	if (li->courant != NULL) {
		li->courant = li->courant->suivant;
	}
	return ptc;
}

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeSelector (ListeSelector* li, void (*f) (objetSelector*)) {
	ouvrirListeSelector (li);
	while (!finListeSelector (li)) {
		objetSelector* ch = objetCourantSelector (li);
		f (ch); // appliquer la fonction f() à objetSelector
	}
}

// fournir un pointeur sur l'objetSelector "objetCherche" de la liste li;
// NULL si l'objetSelector n'existe pas
objetSelector* chercherUnobjetSelector (ListeSelector* li, objetSelector* objetCherche) {
	int trouve=0;
	void* ch;
	// pointeur courant
	ouvrirListeSelector (li);
	while (!finListeSelector (li) && !trouve) {
		ch = objetCourantSelector (li);
		trouve = li->comparer (objetCherche, ch) == 0;
	}
	return trouve ? ch : NULL;
}

// fournir un pointeur sur l'objet courant de la liste li,
// et se positionner sur le suivant qui devient le courant
objetSelector* objetCourantSelector (ListeSelector* li) {
	ElementSelector* ptc = elementCourantSelector (li);
	return ptc==NULL ? NULL : ptc->reference;
}

// insérer dans la liste li, objet après precedent
// si precedent est NULL, insérer en tête de liste
void insererApresSelector (ListeSelector* li, ElementSelector* precedent, objetSelector* objet) {
	if (precedent == NULL) {
		insererEnTeteDeListeSelector (li, objet);
	} else {
		ElementSelector* nouveau= creerElementSelector();
		nouveau->reference = objet;
		nouveau->suivant= precedent->suivant;
		precedent->suivant = nouveau;
		if (precedent == li->dernier)
			li->dernier = nouveau;
		li->nbElt++;
	}
}


// insérer un objet en fin de la liste li
void insererEnFinDeListeSelector (ListeSelector* li, objetSelector* objet) {
	insererApresSelector (li, li->dernier, objet);
}

// extraire l'objet en tête de la liste li
objetSelector* extraireEnTeteDeListeSelector (ListeSelector* li) {
	ElementSelector* extrait = li->premier;
	if (!listeVideSelector(li)) {
		li->premier = li->premier->suivant;
		if (li->premier==NULL)
			li->dernier=NULL; // Liste devenue vide
		li->nbElt--;
	}
	return extrait != NULL ? extrait->reference : NULL;
}


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetSelector* extraireApresSelector (ListeSelector* li, ElementSelector* precedent) {
	if (precedent == NULL) {
		return extraireEnTeteDeListeSelector (li);
	} else {
		ElementSelector* extrait = precedent->suivant;
		if (extrait != NULL) {
			precedent->suivant = extrait->suivant;
			if (extrait == li->dernier) li->dernier = precedent;
			li->nbElt--;
		}
		return extrait != NULL ? extrait->reference : NULL;
	}
}


// extraire l'objet en fin de la liste li
objetSelector* extraireEnFinDeListeSelector (ListeSelector* li) {
	objetSelector* extrait;
	if (listeVideSelector(li)) {
		extrait = NULL;
	} else if (li->premier == li->dernier) { // un seul élément
		extrait = extraireEnTeteDeListeSelector (li);
	} else {
		ElementSelector* ptc = li->premier;
		while (ptc->suivant != li->dernier) ptc = ptc->suivant;
		extrait = extraireApresSelector (li, ptc);
	}
	return extrait;
}


// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetSelector (ListeSelector* li, objetSelector* objet) {
	ElementSelector* precedent = NULL;
	ElementSelector* ptc= NULL;
	// repère l'élément précédent
	booleen trouve = faux;
	ouvrirListeSelector (li);
	while (!finListeSelector (li) && !trouve) {
		precedent = ptc;
		ptc= elementCourantSelector (li);
		trouve = (ptc->reference == objet) ? vrai : faux;
	}
	if (!trouve)
		return faux;
	objetSelector* extrait = extraireApresSelector (li, precedent);
	return vrai;
}


// parcours de liste avec destruction de chaque élément
void detruireListeSelector (ListeSelector* li) {
	ouvrirListeSelector (li);
	while (!finListeSelector (li)) {
		ElementSelector* ptc = elementCourantSelector (li);
		//free (ptc->reference); //si on veut détruire les objets de la liste
		free (ptc);
	}
	initListeSelector (li,NULL,NULL);
}

// recopie l2 dans l1 et initialise l2
void recopierListeSelector (ListeSelector* l1, ListeSelector* l2) {
	detruireListeSelector (l1);
	*l1 = *l2;
	// on recopie les têtes de listes
}

/*=======================================Data=======================================*/



// initialiser la liste pointée par li (cas général)
void initListeData (ListeData* li, char* (*toString) (objetData*),int (*comparer) (objetData*, objetData*)) {
	li->premier = NULL;
	li->dernier = NULL;
	li->courant = NULL;
	li->nbElt= 0;
	li->toString = toString;
	li->comparer = comparer;
}

ListeData* creerListeData (char* (*toString) (objetData*),int (*comparer) (objetData*, objetData*)) {
	ListeData* li =(ListeData *)malloc(sizeof(ListeData));
	initListeData (li, toString, comparer);
	return li;
}

// la liste est-elle vide ?
int listeVideData (ListeData* li) {
	return li->nbElt == 0;
}

// fournir le nombre d'éléments dans la liste
int nbElementData (ListeData* li) {
	return li->nbElt;
}

ElementData* creerElementData () {
	return (ElementData *)malloc(sizeof(ElementData));
}


// insérer objetData en tête de la liste li
// l'objetData est repéré par le champ reference de l'élément de la liste
void insererEnTeteDeListeData (ListeData* li, objetData* ch) {
	ElementData* nouveau= creerElementData();
	nouveau->reference = ch;
	nouveau->suivant= li->premier;
	li->premier= nouveau;
	if (li->dernier == NULL) li->dernier = nouveau;
	li->nbElt++;
}


// insérer un objetData en fin de la liste li
/*void insererEnFinDeListe (ListeData* li, objetData* ch) {
	ElementData* nouveaux= creerElementData();
	nouveaux->reference = ch;
	nouveaux->suivant=NULL;
	if(li->premier==NULL){
		li->premier=nouveaux;
	}
	else{
		li->dernier->suivant=nouveaux;
	}
	li->dernier=nouveaux;
}*/

// se positionner sur le premier élément de la liste li
void ouvrirListeData (ListeData* li) {
	li->courant = li->premier;
}


int finListeData (ListeData* li) {
	return li->courant==NULL;
}


int finListeSuivantData (ListeData* li) {
	return li->courant->suivant==NULL;
}

// fournir un pointeur sur l'élément courant de la liste li,
// et se positionner sur le suivant qui devient le courant
ElementData* elementCourantData (ListeData* li) {
	ElementData* ptc = li->courant;
	if (li->courant != NULL) {
		li->courant = li->courant->suivant;
	}
	return ptc;
}

// lister la liste li;
// f est une fonction passée en paramètre
// et ayant un pointeur de type quelconque.
// Ceci s'apparente aux méthodes virtuelles en PO.
void listerListeData (ListeData* li, void (*f) (objetData*)) {
	ouvrirListeData (li);
	while (!finListeData (li)) {
		objetData* ch = objetCourantData (li);
		f (ch); // appliquer la fonction f() à objetData
	}
}

// fournir un pointeur sur l'objetData "objetCherche" de la liste li;
// NULL si l'objetData n'existe pas
objetData* chercherUnobjetData (ListeData* li, objetData* objetCherche) {
	int trouve=0;
	void* ch;
	// pointeur courant
	ouvrirListeData (li);
	while (!finListeData (li) && !trouve) {
		ch = objetCourantData (li);
		trouve = li->comparer (objetCherche, ch) == 0;
	}
	return trouve ? ch : NULL;
}

// fournir un pointeur sur l'objet courant de la liste li,
// et se positionner sur le suivant qui devient le courant
objetData* objetCourantData (ListeData* li) {
	ElementData* ptc = elementCourantData (li);
	return ptc==NULL ? NULL : ptc->reference;
}

// insérer dans la liste li, objet après precedent
// si precedent est NULL, insérer en tête de liste
void insererApresData (ListeData* li, ElementData* precedent, objetData* objet) {
	if (precedent == NULL) {
		insererEnTeteDeListeData (li, objet);
	} else {
		ElementData* nouveau= creerElementData();
		nouveau->reference = objet;
		nouveau->suivant= precedent->suivant;
		precedent->suivant = nouveau;
		if (precedent == li->dernier)
			li->dernier = nouveau;
		li->nbElt++;
	}
}


// insérer un objet en fin de la liste li
void insererEnFinDeListeData (ListeData* li, objetData* objet) {
	insererApresData (li, li->dernier, objet);
}

// extraire l'objet en tête de la liste li
objetData* extraireEnTeteDeListeData (ListeData* li) {
	ElementData* extrait = li->premier;
	if (!listeVideData(li)) {
		li->premier = li->premier->suivant;
		if (li->premier==NULL)
			li->dernier=NULL; // Liste devenue vide
		li->nbElt--;
	}
	return extrait != NULL ? extrait->reference : NULL;
}


// Extraire l'objet de li se trouvant après l'élément precedent;
// si precedent vaut NULL, on extrait le premier de la liste;
// retourne NULL si l'objet à extraire n'existe pas
objetData* extraireApresData (ListeData* li, ElementData* precedent) {
	if (precedent == NULL) {
		return extraireEnTeteDeListeData (li);
	} else {
		ElementData* extrait = precedent->suivant;
		if (extrait != NULL) {
			precedent->suivant = extrait->suivant;
			if (extrait == li->dernier) li->dernier = precedent;
			li->nbElt--;
		}
		return extrait != NULL ? extrait->reference : NULL;
	}
}


// extraire l'objet en fin de la liste li
objetData* extraireEnFinDeListeData (ListeData* li) {
	objetData* extrait;
	if (listeVideData(li)) {
		extrait = NULL;
	} else if (li->premier == li->dernier) { // un seul élément
		extrait = extraireEnTeteDeListeData (li);
	} else {
		ElementData* ptc = li->premier;
		while (ptc->suivant != li->dernier) ptc = ptc->suivant;
		extrait = extraireApresData (li, ptc);
	}
	return extrait;
}


// extraire de la liste li, l'objet pointé par objet
booleen extraireUnobjetData (ListeData* li, objetData* objet) {
	ElementData* precedent = NULL;
	ElementData* ptc= NULL;
	// repère l'élément précédent
	booleen trouve = faux;
	ouvrirListeData (li);
	while (!finListeData (li) && !trouve) {
		precedent = ptc;
		ptc= elementCourantData (li);
		trouve = (ptc->reference == objet) ? vrai : faux;
	}
	if (!trouve)
		return faux;
	objetData* extrait = extraireApresData (li, precedent);
	return vrai;
}


// parcours de liste avec destruction de chaque élément
void detruireListeData (ListeData* li) {
	ouvrirListeData (li);
	while (!finListeData (li)) {
		ElementData* ptc = elementCourantData (li);
		//free (ptc->reference); //si on veut détruire les objets de la liste
		free (ptc);
	}
	initListeData (li,NULL,NULL);
}

// recopie l2 dans l1 et initialise l2
void recopierListeData (ListeData* l1, ListeData* l2) {
	detruireListeData (l1);
	*l1 = *l2;
	// on recopie les têtes de listes
}