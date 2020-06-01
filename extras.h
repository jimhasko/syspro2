
#ifndef SYSPRO2_EXTRAS_H
#define SYSPRO2_EXTRAS_H

#include <stdlib.h>
#include <stdio.h>


#include "tree.h"

typedef struct tree_array{
    bin_node* nodes_array;
    int size;
    int current;
    int max_lvl;
}tree_array;

typedef struct transfer{
    temp_hash** htable;;
    int added;
}transfer;

typedef struct bucket{
    root** dentro;
    int size,extra;
    struct bucket* next;
}bucket;

typedef struct table{
    bucket* hashtable;
    int hashnum;
    int bucket_size,entries;

}htable;


typedef struct  initial_transfer{
    char* virus;
    int catA;
    int catB;
    int catC;
    int catD;
    int diseases;
}initial_transfer;



typedef struct date_cases{
    char* date;
    initial_transfer ** in;
    int day_disease;
}date_cases;


typedef struct main_struct{

    char* country;
    date_cases** dates;
    int num_of_dates;
}main_struct;

typedef struct overlay{

    main_struct** stat;
    int num_of_countries;

}overlay;


typedef struct suma{
    htable * hashdisease;
    htable* hashcountry;
    head* pat_list;
    int patient_num;
    initial_transfer** starter;
    int readerfd;
    int writerfd;
    int day_disease;
    overlay* over;
}suma;

typedef struct day_hash{
    struct day_hash *next;
    initial_transfer* in;
}day_hash;


initial_transfer* init_init();
void add_to_day_hash(day_hash* dhash,patient* pat,int* daydiseases);
suma* get_from_file(suma* ola,char* filename,int diseasehash,int countryhash,char* date,char* country);
void convert_all(htable* table);
void convert(root* dentro);
int  tree_to_array(tree** array,int added,tree *dentro);
int get_lvl(int num);
void add_to_temp(temp_hash** thash,bin_node* head,int hash_size);
void finish(suma* ola);
void add_to_bin(int max_lvl,tree_array *tarray,in_node* new);
void print(patient* cas);
htable* init_hash(int diseasenum,int bucket_size);
void add_to_hash(htable* table,patient * pat,char* compair);
bin_node* bin_init();
void recordpatientexit(suma* ola,char* id,char* exit_date);
void free_tarray(tree_array* tarray);
void pop_down(bin_node* head);
void bin_check(bin_node* node);
temp_hash* init_temp(char* tree_type);
void get_data_temp(transfer* trans,tree* dentro,int what_is,char* mindate,char* maxdate,int hash_size);
tree_array * get_thash(tree* dentro,int what_is,char* mindate,char* maxdate);
#endif //UNTITLED7_EXTRAS_H