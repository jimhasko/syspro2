//
// Created by dimitris on 16/05/2020.
//

#ifndef SYSPRO2_FUNCTIONS_H
#define SYSPRO2_FUNCTIONS_H
#include "extras.h"




//void call_others(suma* ola);
void globalDiseaseStats(htable* table,char* date1,char* date2);
void numcurrentpatiens(htable* table,char* disease,char*date1,char* date2,char* country);
void insertpatienrecord(suma* ola,char** data_imputs);

void diseaseFrequency(suma* ola,char* disease,char* date1,char* date2, char* country);
void top_k(htable* table,char* N,char* tree_type,char* date1,char* date2,int what_is);
#endif //SYSPRO2_FUNCTIONS_H