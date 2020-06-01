
#include "tree.h"


int hash(char* str,int size){//hash function
    int i,j=0;
    for(i=0;i<strlen(str);i++){
        j=j+str[i];


    }
    return j%size;
}



void add_to_existing_branch(tree* temp,patient *pat){//an iparxi idi i imerominia bazi ton as8eni se lista
    temp->num_of_pat++;
    temp->brach_patients=realloc(temp->brach_patients,temp->num_of_pat*(sizeof(patient*)));
    temp->brach_patients[temp->num_of_pat-1]=pat;


}


int check_date(char* dat1,char* dat2) {//elenxi tin imerominies
    int day1, day2, month1, month2, year1, year2;
    char *ptr;
    char delim[2] = "-";

    char * date1;
    char *date2;

    if(strlen(dat2)<2){

        return 1;
    }


    date1=(char*)malloc(sizeof(char)*(strlen(dat1)+1));
    date2=(char*)malloc(sizeof(char)*(strlen(dat2)+1));
    strcpy(date1,dat1);
    strcpy(date2,dat2);




    ptr = strtok(date1, delim);
    day1 = atoi(ptr);
    ptr = strtok(NULL, delim);
    month1 = atoi(ptr);
    ptr = strtok(NULL, delim);
    year1 = atoi(ptr);




    ptr = strtok(date2, delim);
    day2 = atoi(ptr);
    ptr = strtok(NULL, delim);
    month2 = atoi(ptr);
    ptr = strtok(NULL, delim);
    year2 = atoi(ptr);

    free(date1);
    free(date2);

    if (year1 < year2) {
        return 1;
    } else if (year1 > year2) {
        return 2;


    } else if(month1<month2){
        return 1;
    }else if(month1>month2){
        return 2;


    }else if(day1<day2){
        return 1;
    }else if(day1>day2){
        return 2;
    }else{
        return 0;
    }
}

tree* init_tree(patient* pat ){
    tree* temp=(tree*)malloc(sizeof(tree));
    temp->num_of_pat=1;
    temp->brach_patients=(patient**)malloc(sizeof(patient*)*temp->num_of_pat);
    temp->branch_date=(char*)malloc(sizeof(char)*(strlen(pat->admited_date)+1));
    strcpy(temp->branch_date,pat->admited_date);
    temp->greater=NULL;
    temp->less_equal=NULL;
    temp->brach_patients[0]=pat;
    return temp;

}

void add_to_tree(patient* pat,root* rot){//pros8etei as8eni sto dentro
    tree* dentro=rot->bin_tree;
    tree* temp=dentro;
    tree* temp_next=NULL;
    //dentro->num_of_pat++;

    int cheker=check_date(pat->admited_date,dentro->branch_date);
    if(cheker==1){

        temp_next=dentro->less_equal;
    }else if(cheker==2){
        temp_next=dentro->greater;
    }


    while (temp_next!=NULL){
        cheker=   check_date(pat->admited_date,temp->branch_date);
        if(cheker==1){
            temp_next=temp->less_equal;
        }else if(cheker==2){
            temp_next=temp->greater;
        }else{
            break;


        }

        if(temp_next!=NULL){

            temp=temp_next;
        }

    }


    if(cheker==1){
        temp->less_equal=init_tree(pat);
        rot->tree_size++;
    }else if(cheker==2){
        temp->greater=init_tree(pat);
        rot->tree_size++;
    }else if(cheker==0){// an iprxi idi i imerominia balton se lista
        add_to_existing_branch(temp,pat);

    }else{
        printf(" date's negative \n");

    }
}

void free_tree(tree* dentro){
    if(dentro->greater!=NULL){
        free_tree(dentro->greater);
    }
    if(dentro->less_equal!=NULL){
        free_tree(dentro->less_equal);
    }
    free(dentro->brach_patients);
    free(dentro->branch_date);
    free(dentro);

}




int count_tree(tree* dentro,char* mindate,char* maxdate,int souma){//metraei tous as8eneis

    if(dentro->greater!=NULL){
        souma=  count_tree(dentro->greater,mindate,maxdate,souma);
    }
    if(dentro->less_equal!=NULL){
        souma=  count_tree(dentro->less_equal,mindate,maxdate,souma);
    }



    if(mindate!=NULL){
        if(check_date(mindate,dentro->branch_date)!=2&&check_date(maxdate,dentro->branch_date)==2){
            souma=souma+dentro->num_of_pat;
        }
    }else{
        souma=souma+dentro->num_of_pat;
    }


    return souma;

}

int search_tree_for_active(tree *dentro, int souma,char*date1,char* date2,char* disease){//metraei tous as8eneis xoris exit date
    int k;
    if(dentro->greater!=NULL){
        souma= search_tree_for_active(dentro->greater, souma,date1, date2, disease);
    }
    if(dentro->less_equal!=NULL){
        souma= search_tree_for_active(dentro->less_equal, souma,date1, date2, disease);
    }


    for(k=0;k<dentro->num_of_pat;k++) {
        if (strlen(dentro->brach_patients[k]->exit_date) > 1) {
            if(check_date(dentro->brach_patients[k]->exit_date,date2)!=2){
                if(check_date(dentro->brach_patients[k]->exit_date,date1)==2){
                    if(strcmp(disease,"nocountry")==0){
                         souma=souma+1;  } else{
                             if(strcmp(dentro->brach_patients[k]->disease,disease)==0){
                              souma=souma+1;
                             }


                }
            }}

        }}

    return souma;

}

int count_tree_with_country(tree* dentro,char* mindate,char* maxdate,char* country,int souma){//metrai tous as8eneis me dedomeni iparxi  imerominias
    int j;
    if(dentro->greater!=NULL){
        souma=   count_tree_with_country(dentro->greater,mindate,maxdate,country,souma);
    }
    if(dentro->less_equal!=NULL){
        souma=  count_tree_with_country(dentro->less_equal,mindate,maxdate,country,souma);
    }






    if(check_date(mindate,dentro->branch_date)!=2&&check_date(maxdate,dentro->branch_date)==2){
        if(country==NULL){
            souma=souma+dentro->num_of_pat;
        }else{
            for(j=0;j<dentro->num_of_pat;j++){
                if(strcmp(dentro->brach_patients[j]->country,country)==0){
                    souma=souma+1;
                }
            }
        }


    }
    return souma;
}

head* init_list(){
    head* temp;
    temp=(head*)malloc(sizeof(head));
    temp->first=NULL;
    temp->size=0;
    return temp;
}
void add_to_list(head* kef,patient* pat){
    listnode* temp;
    temp=(listnode*)malloc(sizeof(listnode));
    temp->pat=pat;
    temp->next=kef->first;
    kef->first=temp;
    kef->size++;



}
void delete_list(head* kef){
    listnode* temp;
    temp=kef->first;
    while(temp!=NULL){
        kef->first=kef->first->next;

        free( temp->pat->country);
        free( temp->pat->disease);
        free( temp->pat->admited_date);
        free( temp->pat->exit_date);
        free( temp->pat->last_name);
        free( temp->pat->first_name);
        free( temp->pat->id);
        free(temp->pat);
        free(temp);
        temp=kef->first;
    }
    free(kef);
}


void swap(in_node* A,in_node* B){
    in_node* temp;
    temp=A;
    A=B;
    B=temp;
}

void search_tree(tree* dentro,temp_hash** thash,int hash_size,int is_what,char* mindate,char* maxdate){//prosxedio gia to topk hash.den i8ela na to dioxo
    int i,j,loc,found=0;
    temp_hash* temp;
    if(dentro->less_equal!=NULL){

        search_tree(dentro->less_equal,thash,hash_size,is_what,mindate,maxdate);
    }

    if(dentro->greater!=NULL){

        search_tree(dentro->greater,thash,hash_size,is_what,mindate,maxdate);
    }
    if(check_date(mindate,dentro->branch_date)!=2&&check_date(maxdate,dentro->branch_date)==2){
        for(i=0;i<dentro->num_of_pat;i++) {
            found = 0;
            if (is_what == 0) {
                loc = hash(dentro->brach_patients[i]->disease, hash_size);
                if (thash[loc] == NULL) {

                    thash[loc] = (temp_hash *) malloc(sizeof(temp_hash));
                    thash[loc]->node = (in_node *) malloc(sizeof(in_node));
                    thash[loc]->node->count = 1;
                    thash[loc]->node->tree_type = dentro->brach_patients[i]->disease;
                    thash[loc]->next = NULL;


                } else {
                    temp = thash[loc];
                    while (temp != NULL) {

                        if (check_date(mindate, dentro->branch_date) != 2 &&
                            check_date(maxdate, dentro->branch_date) == 2) {
                            temp->node->count++;
                            found = 1;
                            break;
                        }

                        temp = temp->next;
                    }
                    if (found == 0) {
                        temp = thash[loc];
                        while (temp->next == NULL) {
                            temp = temp->next;
                        }
                        temp->next = (temp_hash *) malloc(sizeof(temp_hash));
                        temp->node = (in_node *) malloc(sizeof(in_node));
                        temp->node->count = 1;
                        temp->node->tree_type = dentro->brach_patients[i]->disease;
                        temp->next = NULL;


                    }

                }


            }
            ///////////////////////////////////////////////////////////

            if (is_what == 1) {
                loc = hash(dentro->brach_patients[i]->country, hash_size);
                if (thash[loc] == NULL) {
                    thash[loc] = (temp_hash *) malloc(sizeof(temp_hash));
                    thash[loc]->node = (in_node *) malloc(sizeof(in_node));
                    thash[loc]->node->count = 1;
                    thash[loc]->node->tree_type = dentro->brach_patients[i]->country;
                    thash[loc]->next = NULL;
                } else {
                    temp = thash[loc];
                    while (temp != NULL) {
                        if (strcmp(dentro->brach_patients[i]->country, temp->node->tree_type) == 0) {
                            temp->node->count++;
                            found = 1;
                            break;
                        }
                        temp = temp->next;
                    }
                    if (found == 0) {
                        temp = thash[loc];
                        while (temp->next == NULL) {
                            temp = temp->next;
                        }
                        temp->next = (temp_hash *) malloc(sizeof(temp_hash));
                        temp->node = (in_node *) malloc(sizeof(in_node));
                        temp->node->count = 1;
                        temp->node->tree_type = dentro->brach_patients[i]->country;
                        temp->next = NULL;


                    }

                }


            }


        }









    }



}