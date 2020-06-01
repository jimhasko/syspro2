

#ifndef SYSPRO2_TREE_H
#define SYSPRO2_TREE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct patient{
    char * first_name;
    char* last_name;
    char* age;
    char* disease;
    char* id;
    char* admited_date;
    char* exit_date;
    char* country;
}patient;


typedef struct tree{
    char* branch_date;
    struct tree * less_equal;
    struct tree * greater;
    int num_of_pat;
    patient** brach_patients;
}tree;

typedef struct listnode{
    struct listnode* next;
    patient* pat;
}listnode;

typedef struct head{
    listnode* first;
    int size;

}head;


typedef struct bin_in_node{
    int count;
    char* tree_type;

}in_node;

typedef struct bin_node{
    in_node* inside;
    struct bin_node * first;
    struct bin_node* second;
    struct bin_node* parent;
}bin_node;

typedef struct temp_hash{
    struct temp_hash * next;
    in_node* node;
}temp_hash;

typedef struct root{
    char* tree_type;
    tree* bin_tree;
    int tree_size;
}root;



head* init_list();
void add_to_list(head* kef,patient* pat);
void delete_list(head* kef);
int hash(char* str,int size);
void add_to_tree(patient* pat,root* rot);
int count_tree_with_country(tree* dentro,char* mindate,char* maxdate,char* country,int souma);
tree* init_tree(patient* pat );
void swap(in_node* A,in_node* B);
int check_date(char* date1,char* date2);
void add_to_existing_branch(tree* temp,patient *pat);
void free_tree(tree* dentro);
int search_tree_for_active(tree *dentro, int souma,char*date1,char* date2,char* disease);
int count_tree(tree* dentro,char* mindate,char* maxdate,int souma);
void swap(in_node* A,in_node* B);
void search_tree(tree* dentro,temp_hash** thash,int hash_size,int is_what,char* mindate,char* maxdate);

#endif //UNTITLED7_TREE_H