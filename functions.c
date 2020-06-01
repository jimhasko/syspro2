
#include "functions.h"





void globalDiseaseStats(htable* table,char* date1,char* date2) {
    int j, i, its_ok = 0;
    htable* temp_t=table;
    bucket* temp;
    int count;
    if (date1 != NULL) {
        if(date2==NULL){
            its_ok=1;
            printf("need both dates\n");
        }else {
            if (check_date(date1, date2) == 2) {
                char *tempp = date1;
                date1 = date2;
                date2 = tempp;
            }
        }
    }


    int done;
    if(its_ok==0){
        for (j = 0; j < temp_t->hashnum; j++) {
            done=0;
            temp=&temp_t->hashtable[j];
            while (done==0) {
                for (i = 0; i < temp->size; i++) {   //gia oles tis egrafes metrise tous as8eneis
                    count=count_tree(temp->dentro[i]->bin_tree, date1, date2, 0);
                    //  if(count>0){
                    printf("%s %d \n", temp->dentro[i]->tree_type, count  );}
                // }
                if(temp->next==NULL){
                    done=1;
                }else {
                    temp=temp->next;
                }

            }

        }}

}

void numcurrentpatiens(htable* table,char* disease,char*date1,char* date2,char* country){
    int j,i,k;
    htable* temp_t=table;
    bucket* temp;
    int done;
    int count;
    for (j = 0; j < temp_t->hashnum; j++) {
        done=0;
        temp=&temp_t->hashtable[j];
        while (done==0) {
            for (i = 0; i < temp->size; i++) {
                //   for(k=0;k<temp->dentro[i]->bin_tree->num_of_pat;k++){
                if(strcmp(country,"nocountry")==0) {          //metrise olous tous energous as8eneis
                    count=search_tree_for_active(temp->dentro[i]->bin_tree,0,date1, date2,disease);
                    //    if(count>0) {
                    printf("%s %d\n", temp->dentro[i]->tree_type, count);
                    // }
                }else{
                    if(strcmp(country,temp->dentro[i]->tree_type)==0){
                        printf("%s %d\n",temp->dentro[i]->tree_type,  search_tree_for_active(temp->dentro[i]->bin_tree,0,date1, date2, disease));

                    }


                }

                //  }
            }
            if(temp->next==NULL){
                done=1;
            }else {
                temp=temp->next;
            }

        }

    }






}


void insertpatienrecord(suma* ola,char** data_imputs){
    int i,j,done,is_good=0;
    ola->patient_num=ola->patient_num+1;
    patient* temp;
    temp=(patient*)malloc(sizeof(patient));


    i=ola->patient_num-1;
    temp->first_name=(char*)malloc(sizeof(char)*20);
    temp->last_name=(char*)malloc(sizeof(char)*20);
    temp->id=(char*)malloc(sizeof(char)*20);
    temp->disease=(char*)malloc(sizeof(char)*20);
    temp->country= (char*)malloc(sizeof(char)*20);
    temp->admited_date=(char*)malloc(sizeof(char)*20);
    temp->exit_date=(char*)malloc(sizeof(char)*20);

    for(i=0;i<7;i++){
        if(data_imputs[i]==NULL){
            printf(" nesessary info: id,first name ,last name,disease,country admited date \n");
            is_good=1;
            free(temp);
            break;
        }



    }



    if(is_good==0) {
        strcpy(temp->first_name, data_imputs[2]);
        strcpy(temp->last_name, data_imputs[3]);
        strcpy(temp->country, data_imputs[5]);
        strcpy(temp->disease, data_imputs[4]);
        strcpy(temp->id, data_imputs[1]);
        strcpy(temp->admited_date, data_imputs[6]);
        if (data_imputs[7] == NULL) {
            strcpy(temp->exit_date, " ");
        } else {
            strcpy(temp->exit_date, data_imputs[7]);
        }

        add_to_hash(ola->hashcountry, temp, temp->country);
        add_to_hash(ola->hashdisease, temp, temp->disease);
        add_to_list(ola->pat_list,temp);
        printf("record added\n");
        //////////////////////////////////////////////////
        htable* temp_t;
        bucket* tempp;
        temp_t=ola->hashdisease;
        for (j = 0; j < temp_t->hashnum; j++) {
            done=0;
            tempp=&temp_t->hashtable[j];
            while (done==0) {
                for (i = 0; i < tempp->size; i++) {

                    if(strcmp(tempp->dentro[i]->tree_type,temp->disease)==0){

                        convert(tempp->dentro[i]);
                        break;
                    }




                }
                if(tempp->next==NULL){
                    done=1;
                }else {
                    tempp=tempp->next;
                }

            }

        }


//////////////////////////////////////////

        temp_t=ola->hashcountry;
        for (j = 0; j < temp_t->hashnum; j++) {
            done=0;
            tempp=&temp_t->hashtable[j];
            while (done==0) {
                for (i = 0; i < tempp->size; i++) {

                    if(strcmp(tempp->dentro[i]->tree_type,temp->country)==0){

                        convert(tempp->dentro[i]);

                        break;
                    }




                }

                if(tempp->next==NULL){
                    done=1;
                }else {
                    tempp=tempp->next;
                }

            }

        }
////////////////////////

    }



}


void diseaseFrequency(suma* ola,char* disease,char* date1,char* date2, char* country) {

    int j, i, its_ok = 0;
    if (disease == NULL || date1 == NULL || date2 == NULL) {
        its_ok = 1;

    }
    if(its_ok==0){
        if (check_date(date1, date2) == 2) {
            char *temp = date1;
            date1 = date2;
            date2 = temp;
        }

        bucket* temp;
        htable *temp_t = ola->hashdisease;
        int done;
        int result = 0;

        for (j = 0; j < temp_t->hashnum; j++) {
            done = 0;
            temp=&temp_t->hashtable[j];
            while (done == 0) {
                for (i = 0; i < temp->size; i++) {//gia oles tis ergafes psaxni to dentro
                    if(strcmp(temp->dentro[i]->tree_type,disease)==0){
                        result = count_tree_with_country(temp->dentro[i]->bin_tree, date1, date2, country, 0);
                        // if (result > 0) {
                        // if (country != NULL)
                        //  printf("country: %s ", country);
                        printf("%s %d\n", temp->dentro[i]->tree_type, result);
                        // }
                    }
                }
                if (temp->next == NULL) {
                    done = 1;
                } else {
                    temp = temp->next;
                }

            }

        }
    }else{ printf("diseaseFrequency virusName  date1 date2 [country] \n");}


}


void top_k(htable* table,char* N,char* tree_type,char* date1,char* date2,int what_is){
    tree* dentro=NULL;
    bucket* temp;
    bin_node* head;
    tree_array * tarray;
    int j, i,loc, size,done,k,tops,its_ok = 0;
    tops=atoi(N);
    htable* temp_t=table;
    if (date1 != NULL) {
        if(date2==NULL){
            its_ok=1;
            printf("need both dates\n");
        }else {
            if (check_date(date1, date2) == 2) {
                char *tempp = date1;
                date1 = date2;
                date2 = tempp;
            }
        }
    }


    if(N==NULL||tree_type==NULL){
        its_ok=1;
        printf("needs number and category \n");
    }

    if(its_ok==0){
        for (j = 0; j < temp_t->hashnum; j++) {
            done=0;
            temp=&temp_t->hashtable[j];
            while (done==0) {
                for (i = 0; i < temp->size; i++) {
                    if(strcmp(temp->dentro[i]->tree_type,tree_type)==0){
                        tarray= get_thash(temp->dentro[i]->bin_tree,what_is,date1,date2);//basei se enahash tableta stoixeia
                        if(tarray!=NULL) {
                            head = &tarray[0].nodes_array[0];
                            for (k = 0; k < tops; k++) {
                                if(head->inside->count==-1){ //diatrexei to dentro k fores kai bgazei to head
                                    break;
                                }
                                printf("%s %d\n", head->inside->tree_type, head->inside->count);
                                head->inside->count = -1;
                                pop_down(head);

                            }
                            free_tarray(tarray);
                        }
                        break;

                    }
                }
                if(temp->next==NULL){
                    done=1;
                }else {
                    temp=temp->next;
                }

            }

        }





    }

}