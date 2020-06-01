//
// Created by dimitris on 15/05/2020.
//
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include<string.h>
#include <dirent.h>
#include<errno.h>
#include<sys/wait.h>
#include "functions.h"
#ifndef SYSPRO2_CHILD_H
#define SYSPRO2_CHILD_H

typedef struct gg{
    int test;
    int done;
    char* countrytest;
}gg;



typedef struct childthings{
    pid_t pid;
    int fdreader;
    int fdwriter;
    char** countries;
    char* reader;
    char* writer;
    int num_of_countries;
}childthings;

typedef struct helper{
    char* name;
    double cat;

}helper;


void exitsignal(int signalNum);
void signalHandler(int signalNum);
void signalkid(int signum);
suma* child_call(int i,char* reader,char* writer,childthings* data,char* dir,int buffer,int feed_back);
childthings* read_files(char* dir,int N);
suma* child_init_add(char* dir,char* gr,int fd,int buffer,suma* ola,int feed_back);
char** short_dates(char* dir,int* files);
void quicksort(char**dates, int from, int to);
main_struct** init_main_struct(char** countries_array,int countries_num);
int file_num(char* dir,char* gr);
date_cases** file_arr(char* dir,char* gr);
overlay* get_stats(char* dir,int N,childthings* child);
initial_transfer* reader(int fd,int buffer);
void kill_state(suma* ola);
void read_some(int fd,int buffer,overlay* stats);
void char_to_str(char* ptr,overlay* stats);
void list_countries(childthings* child,int N);
void disease_frequency(char* virus,char* date1,char* date2,char*country,overlay* stats);
void num_patient_admissions(char* virus,char* date1,char* date2,char*country,overlay* stats);
void topk(char* num,char*country,char* virus,char* date1,char* date2,overlay* stats);
overlay* parent_call(int N,char* dir,childthings* children,int buffer,int** fd_matrix);
void parent_big_call(int N,char* dir,childthings* children,int buffer);
char* read_fifo(int fd,int buffer);
//void write_fifo(int fd,char* command);
void write_fifo(int fd,char* command,int buffer);
void child_big_call(int i,char* reader,char* writer,childthings* data,char* dir,int buffer,int feed_back);
void recordid_parent(char* id,childthings* children,int N,int ** fd_matrix,int buffer,int* pass,int* fail);
void recordid_child(char* id,suma* ola,int fd,int buffer);
void exit_parent(childthings* children,int N,int ** fd_matrix,overlay* stats,int* succses,int* fail);
void new_data(suma* ola,int fd,char* dir,int buffer);
void call_others(overlay* stats,int N,int **fd_matrix,int buffer,childthings* children,int* exit,int*pass,int*fail);
void patient_discharges_parent(char* disease,char* date1,char* date2,char* country,childthings* children,int buffer,int N,int** fd_matrix);
void patient_discharges_child(suma* ola,int fdreaderfrom,int buffer);

void new_data_parent(int fdreader,overlay* stats,int buffer);
date_cases* char_to_str2(char* ptr,overlay* stats,date_cases* newdates);
void char_to_str3(char* ptr,overlay* stats,date_cases* newdates);
void exit_parent_file(overlay* stats,int* succses,int* fail);
void exit_main(childthings* children,int N,overlay* stats);
#endif //SYSPRO2_CHILD_H