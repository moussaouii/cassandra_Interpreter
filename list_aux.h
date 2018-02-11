//
// Created by ismail on 16/02/17.
//

#ifndef CASSANDRA1_LIST_AUX_H
#define CASSANDRA1_LIST_AUX_H

#include "structure.h"
int comparerKeyspace(objetKeyspace* objet1, objetKeyspace* objet2);

int comparerTable(objetTable* objet1, objetTable* objet2);

int comparerColumn(objetColumn* objet1, objetColumn* objet2);
int comparerNameValue(objetNameValue* objet1, objetNameValue* objet2);

int comparerRelation(objetRelation* objet1, objetRelation* objet2);

int comparerSelector(objetSelector* objet1, objetSelector* objet2);
#endif //CASSANDRA1_LIST_AUX_H
