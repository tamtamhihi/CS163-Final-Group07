#ifndef _SEARCHOPERATOR_H_
#define _SEARCHOPERATOR_H_

#include "gui.h"
#include "Indexing.h"
#include <conio.h>

void search();
void getQuery(string& query);
void operatorAnd(Node*& root, string& query);



#endif