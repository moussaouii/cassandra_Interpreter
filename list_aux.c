//
// Created by ismail on 16/02/17.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list_aux.h"



int comparerKeyspace(objetKeyspace* objet1, objetKeyspace* objet2)
{
    return strcmp(objet1->name,objet2->name);
}


int comparerTable(objetTable* objet1, objetTable* objet2)
{
    return strcmp(objet1->name,objet2->name);
}

int comparerColumn(objetColumn* objet1, objetColumn* objet2)
{
    return strcmp(objet1->name,objet2->name);
}

int comparerNameValue(objetNameValue* objet1, objetNameValue* objet2)
{
    return strcmp(objet1->name,objet2->name);
}

int comparerRelation(objetRelation* objet1, objetRelation* objet2)
{
    return strcmp(objet1->name,objet2->name);
}

int comparerSelector(objetSelector* objet1, objetSelector* objet2)
{
    return strcmp(objet1->name,objet2->name);
}