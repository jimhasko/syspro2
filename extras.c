
#include "extras.h"

bin_node* bin_init(){
    bin_node* temp;
    temp=(bin_node*)malloc(sizeof(bin_node));
    temp->inside=NULL;
    temp->first=NULL;
    temp->second=NULL;
    return temp;



}

int get_lvl(int num){//briskei posa epipeda dentrou xreiazontai gia tis ergafes pou brike
    num=num;
    int lvl=0;
    while(num!=0){
        lvl++;
        num=num/2;
    }

    return lvl;
}

int  tree_to_array(tree** array,int added,tree *dentro){// perni to non binary tree kai to bazei se taxinomimeni array

    if(dentro->less_equal!=NULL){
        added=  tree_to_array(array,added,dentro->less_equal);
    }

    array[added]=dentro;

    added++;


    if(dentro->greater!=NULL){
        added=     tree_to_array(array,added,dentro->greater);
    }
    return added;
}

tree* array_to_tree(tree** array,int min,int max){//perni tin array kai tin kanei binary tree
    tree* temp;

    if(min<=max) {

        int j=(max + min) / 2;

        temp = array[j];


        temp->less_equal = array_to_tree(array, min, j - 1);



        temp->greater = array_to_tree(array, j + 1, max);


        return temp;
    }else{
        return NULL;
    }
}

void convert(root* dentro){//kanei konvert ena dentro apo aplo se binary
    int i;
    tree** array;
    array=(tree**)malloc(sizeof(tree*)*dentro->tree_size);
    for(i=0;i<dentro->tree_size;i++){
        array[i]=NULL;
    }
    tree_to_array(array,0,dentro->bin_tree);
    for(i=0;i<dentro->tree_size;i++){
        array[i]->less_equal=NULL;
        array[i]->greater=NULL;
    }
    dentro->bin_tree=array_to_tree(array,0,dentro->tree_size-1);


    free(array);
}

void convert_all(htable* table){ //kanei convert ola at dentra
    htable* temp_t=table;
    bucket* temp;
    int i,j,done;

    for (j = 0; j < temp_t->hashnum; j++) {
        done = 0;
        if(temp_t->hashtable[j].dentro!=NULL){
            temp = &temp_t->hashtable[j];
            while (done == 0) {
                for (i = 0; i < temp->size; i++) {
                    if (temp->dentro[i] != NULL) {
                        convert(temp->dentro[i]);
                    }
                }
                if (temp->next == NULL) {
                    done = 1;
                } else {
                    temp = temp->next;
                }

            }
        }
    }



}

void add_to_hash(htable* table,patient * pat,char* compair){

    int i,j,found,size,loc,is_what,fits;
    found=0;
    fits=(table->bucket_size-sizeof(bucket*))/sizeof(root);


    bucket* temp;
    loc=hash(compair,table->hashnum);
    temp=&table->hashtable[loc];//get the hash bucket

    size=temp->size;

    if(strcmp(compair,pat->disease)==0) {

        is_what=0;
    }else{

        is_what=1;
    }



    if(size==0){//an eiani to proto stixeio sto hash location
        size++;
        temp->size++;
        temp->dentro=(root**)malloc(sizeof(root*)*size);
        temp->dentro[0]=(root*)malloc(sizeof(root));
        temp->dentro[0]->tree_size=1;
        table->entries++;

        if(is_what==0) {
            temp->dentro[0]->tree_type=(char*)malloc(sizeof(char)*(strlen(pat->disease)+1));
            strcpy(temp->dentro[0]->tree_type , pat->disease);

        }else{
            temp->dentro[0]->tree_type=(char*)malloc(sizeof(char)*(strlen(compair)+1));
            strcpy(temp->dentro[0]->tree_type , compair);

        }

        temp->dentro[0]->bin_tree=init_tree(pat);
        temp->extra=0;


    }else{          //allios
        while(temp!=NULL) {//psaxnei na brei an uparxei idi mesa

            j = 0;
            for (i = 0; i < temp->size; i++) {
                // j=i-(i/fits)*fits;
                j = i;
                if (is_what == 0) {
                    if (strcmp(pat->disease, temp->dentro[j]->tree_type) == 0) {
                        add_to_tree(pat, temp->dentro[j]);
                        // temp->dentro[j]->tree_size++;
                        found = 1;
                        break;
                    }

                } else {

                    if (strcmp(compair, temp->dentro[j]->tree_type) == 0) {
                        add_to_tree(pat, temp->dentro[j]);
                        //   temp->dentro[j]->tree_size++;
                        found = 1;
                        break;
                    }

                }

            }
            temp=temp->next;
        }
        if(found==0){        // to pros8etei san kainourio
            temp=&table->hashtable[loc];
            // bucket* place_to_add;
            table->entries++;

            while(temp->next!=NULL){
                temp=temp->next;
            }

            j=temp->size-(temp->size/fits)*fits;
            if(j==0) {       //an xreiazete ftiaxnei neo bucket
                temp->next = (bucket *) malloc(sizeof(bucket));
                temp = temp->next;
                temp->next = NULL;
                temp->size = 0;
                temp->dentro = (root **) malloc(sizeof(root *) * 1);
                temp->dentro[0]=NULL;
                temp->extra=1;
            }
            temp->size++;
            if(temp->size>1) {


                temp->dentro = realloc(temp->dentro, (temp->size) * sizeof(root *));
            }
            temp->dentro[j]=(root*)malloc(sizeof(root));
            temp->dentro[j]->tree_size=0;
            if(strcmp(compair,pat->disease)==0) {
                temp->dentro[j]->tree_type=(char*)malloc(sizeof(char)*(strlen(pat->disease)+1));
                strcpy(temp->dentro[j]->tree_type , pat->disease);
                is_what=0;
            }else{
                temp->dentro[j]->tree_type=(char*)malloc(sizeof(char)*(strlen(compair)+1));
                strcpy(temp->dentro[j]->tree_type , compair);
                is_what=1;
            }
            temp->dentro[j]->tree_size++;
            temp->dentro[j]->bin_tree=init_tree(pat);
        }
    }
}



initial_transfer* init_init(){
    initial_transfer* init;
    init=(initial_transfer*)malloc(sizeof(initial_transfer));
    init->diseases=0;
    init->virus=NULL;
    init->catA=0;
    init->catB=0;
    init->catC=0;
    init->catD=0;
    return init;
}



htable* init_hash(int diseasenum,int bucket_size){
    htable * hashdisease;
    int i;
    hashdisease=(htable*)malloc(sizeof(htable));
    hashdisease->hashnum=diseasenum;
    hashdisease->bucket_size=bucket_size;
    hashdisease->hashtable=(bucket*)malloc(sizeof(bucket)*diseasenum);
    hashdisease->entries=0;

    for(i=0;i<diseasenum;i++){
        hashdisease->hashtable[i].size=0;
        hashdisease->hashtable[i].dentro=NULL;
        hashdisease->hashtable[i].next=0;
    }


    return hashdisease;

}

temp_hash* init_temp(char* tree_type){
    temp_hash* temp;
    temp=(temp_hash*)malloc(sizeof(temp_hash));
    temp->next=NULL;
    temp->node=(in_node*)malloc(sizeof(in_node));
    temp->node->tree_type=tree_type;
    temp->node->count=1;

    return temp;

}

suma* get_from_file(suma* ola,char* filename,int diseasehash,int countryhash,char* date,char* country){
    FILE * fp;
    int i,j,count=0;

    int day_diseases=0;
    int diseasenum=diseasehash;
    int countrynum=countryhash;
    int bucket_size=4*sizeof(root)+sizeof(bucket*);
    fp=fopen(filename,"r");
    if(fp==NULL){
        printf("invalid file");
        exit(1);


    }



    while(!feof(fp)) {
        if(fgetc(fp) == '\n') {
            count++;
        }
    }

    fseek(fp, 0, SEEK_SET);

    if (count <= 0 ) {

        printf("\n\nInit_file was empty, check file again!\n");
        exit(1);
    }



    rewind(fp);

    htable * hashdisease=NULL;
    htable* hashcountry=NULL;
    head* kef=NULL;
    char*  ptr=(char*)malloc(sizeof(char)*200);
    char* test;
    char delim[]=" ";
    if(ola==NULL){
        ola=(suma*)malloc(sizeof(suma));
        hashdisease=init_hash(diseasenum,bucket_size);
        hashcountry=init_hash(countrynum,bucket_size);
        kef=init_list();
        ola->patient_num=0;
        //   ola->starter=(initial_transfer**)malloc(sizeof(initial_transfer*)*initdiseases);
    }else{
        hashdisease=ola->hashdisease;
        hashcountry=ola->hashcountry;
        kef=ola->pat_list;
        //   ola->starter=(initial_transfer**)malloc(sizeof(initial_transfer*)*initdiseases);

    }


    patient* pat;
    day_hash* dhash;
    dhash=(day_hash*)malloc(sizeof(day_hash)*10);

    for(i=0;i<10;i++){
        dhash[i].next=NULL;
        dhash[i].in=NULL;


    }


    i=0;
    char* is_what;

    while( fscanf(fp,"%[^\n]%*c",ptr)==1 ) {


        is_what=(char*)malloc(sizeof(char)*20);
        pat=(patient*)malloc(sizeof(patient));
        pat->first_name=(char*)malloc(sizeof(char)*20);
        pat->last_name=(char*)malloc(sizeof(char)*20);
        pat->id=(char*)malloc(sizeof(char)*20);
        pat->disease=(char*)malloc(sizeof(char)*20);
        pat->age= (char*)malloc(sizeof(char)*20);
        pat->admited_date=(char*)malloc(sizeof(char)*20);
        pat->exit_date=(char*)malloc(sizeof(char)*20);
        pat->country=(char*)malloc(sizeof(char)*20);

        test = strtok(ptr, delim);

        strcpy(  pat->id, test);
        test = strtok(NULL, delim);

        if(test!=NULL){
            strcpy(is_what, test);

        }

        if(strcmp(is_what,"ENTER")==0) {
            strcpy(pat->country,country);
            strcpy(pat->admited_date,date);
            strcpy(pat->exit_date," ");
            test = strtok(NULL, delim);
            if (test != NULL) {
                strcpy(pat->first_name, test);

            }

            test = strtok(NULL, delim);
            if (test != NULL) {
                strcpy(pat->last_name, test);

            }
            test = strtok(NULL, delim);
            if (test != NULL) {
                strcpy(pat->disease, test);

            }
            test = strtok(NULL, delim);
            if (test != NULL) {
                strcpy(pat->age, test);

            }





            add_to_list(kef,pat);       // pros8eti sti lista me tous as8eneis
            add_to_hash(hashcountry,pat,country);// pros8etei sta hash
            add_to_hash(hashdisease,pat,pat->disease);
            i++;

            add_to_day_hash(dhash,pat,&day_diseases);



            pat=NULL;}
        else{
            listnode *temp = kef->first;
            int found=0;

            while (temp != NULL) {      //psaxni ti lisat as8enon gia ton as8eni me to sosto id
                if (strcmp(pat->id, temp->pat->id) == 0) {

                    if (check_date(temp->pat->admited_date, date) < 2) {
                        strcpy(temp->pat->exit_date, date);
                        //   printf("record updated \n");
                    } else {
                        printf("patiet exit date error\n");
                    }
                    break;
                }
                temp = temp->next;
            }
        }




    }
    //  convert_all(hashcountry);
    //  convert_all(hashdisease);



    /* suma* sum;
     sum=(suma*)malloc(sizeof(suma));
     sum->hashcountry=hashcountry;
     sum->hashdisease=hashdisease;
     sum->pat_list=kef;
     sum->patient_num=count;*/
    ola->patient_num=ola->patient_num+count;
    ola->pat_list=kef;
    ola->hashdisease=hashdisease;
    ola->hashcountry=hashcountry;
    free(ptr);

    ola->starter=(initial_transfer**)malloc(sizeof(initial_transfer*)*day_diseases);
    j=0;
    day_hash* temp;
    for(i=0;i<10;i++){
        temp=&dhash[i];
        if(temp->in!=NULL){
            ola->starter[j]=temp->in;
            j++;
            temp=temp->next;
            while(temp!=NULL){
                if(temp->in!=NULL){
                    ola->starter[j]=temp->in;
                    j++;
                }

                temp=temp->next;
            }



        }
    }


ola->day_disease=day_diseases;

    return ola;


}

void free_bucket(bucket* buc){
    int j;
    bucket* temp,*del=NULL;
    temp=buc;
    while(temp!=NULL){
        for(j=0;j<temp->size;j++){
            free(temp->dentro[j]->tree_type);
            free_tree(temp->dentro[j]->bin_tree);
            free(temp->dentro[j]);}
        free(temp->dentro);
        if(temp!=buc){
            del=temp;
        }
        temp=temp->next;
        if(del!=NULL){
            free(del);
        }

    }


}

void finish(suma* ola){ //elef8eronei oli ti desmeumeni mnimi pou eftiaxe sto get from file
    int j;
    delete_list(ola->pat_list);
    bucket* temp;
    for(j=0;j<ola->hashdisease->hashnum;j++){
        temp=&ola->hashdisease->hashtable[j];
        free_bucket(temp);
    }
    free(ola->hashdisease->hashtable);
    free(ola->hashdisease);
    for(j=0;j<ola->hashcountry->hashnum;j++){
        temp=&ola->hashcountry->hashtable[j];
        free_bucket(temp);
    }
    free(ola->hashcountry->hashtable);
    free(ola->hashcountry);
    free(ola);
}


void free_tarray(tree_array* tarray){
    int i,j,max_lvl;
    max_lvl=tarray[0].max_lvl;
    for(i=0;i<max_lvl;i++){
        for(j=0;j<tarray[i].size;j++) {
            free(tarray[i].nodes_array[j].inside);
        }
        free(tarray[i].nodes_array);
    }

    free(tarray);
}

tree_array* get_thash(tree* dentro,int what_is,char* mindate,char* maxdate){        //friaxnei to prosorino hash table gia to tok-k opou apo8ikeuei ola ta a8rismata gia tous as8eneis
    int i,j,loc,its_ok=0;
    tree* temp;
    tree_array *tarray;
    int hash_size=50;
    temp_hash** htable;
    bin_node* ret=NULL;
    int done;
    htable=(temp_hash**)malloc(sizeof(temp_hash*)*hash_size);
    for(i=0;i<hash_size;i++){
        htable[i]=NULL;
    }
    temp_hash* tt;
    transfer* trans;
    trans=(transfer*)malloc(sizeof(transfer));
    trans->htable=htable;
    trans->added=0;
    if((mindate!=NULL&& maxdate==NULL)||(mindate==NULL&& maxdate!=NULL)){
        its_ok=1;

    }
    if(its_ok==0) {
        get_data_temp(trans, dentro, what_is, mindate, maxdate, hash_size);

        if (trans->added > 0) {

            int max_lvl = get_lvl(trans->added);


            int k = 0;


            tarray = (tree_array *) malloc(sizeof(tree_array) * max_lvl);
            tarray[0].size = 1;
            tarray[0].current=1;
            tarray[0].nodes_array=(bin_node*)malloc(sizeof(bin_node)*tarray[0].size);
            tarray[0].max_lvl=max_lvl;
            for(i=1;i<max_lvl;i++){
                tarray[i].max_lvl=max_lvl;
                tarray[i].current=0;
                tarray[i].size=tarray[i-1].size*2;
                tarray[i].nodes_array=(bin_node*)malloc(sizeof(bin_node)*tarray[i].size);
                for(j=0;j<tarray[i].size;j++){
                    tarray[i].nodes_array[j].inside=NULL;
                    tarray[i].nodes_array[j].parent=NULL;
                    tarray[i].nodes_array[j].first=NULL;
                    tarray[i].nodes_array[j].second=NULL;
                }

            }

            i=0;
            for(loc=0;loc<hash_size;loc++){
                done=0;
                while(trans->htable[loc]!=NULL){
                    if(i==0){
                        tarray[i].nodes_array[0].inside=trans->htable[loc]->node;
                        tarray[i].nodes_array[0].parent=NULL;
                        tarray[i].nodes_array[0].first=NULL;
                        tarray[i].nodes_array[0].second=NULL;
                        i++;
                    }
                    else{

                        add_to_bin(max_lvl,tarray,trans->htable[loc]->node);



                    }

                    tt=trans->htable[loc];
                    trans->htable[loc]=trans->htable[loc]->next;
                    free(tt);
                }




            }

            //ret=&tarray[0].nodes_array[0];
            // free(tarray);
            temp_hash* deleter;
            for(i=0;i<hash_size;i++){    //elef8eronei to hash table kai epistre8ei to dentro gia to bin search
                done=0;
                while(trans->htable[i]!=NULL){
                    deleter=trans->htable[i];
                    trans->htable[i]=trans->htable[i]->next;
                    free(deleter);
                }

            }
            free(trans->htable);
            free(trans);
            return tarray;
        }


    }else{

        printf("provide both dates or none \n");

    }
    return NULL;
};

void add_to_bin(int max_lvl,tree_array *tarray,in_node* new){ //pros8etei ena node apo to hash sto top-k dentro
    int i,j;
    for(i=1;i<max_lvl;i++){
        if(tarray[i].current<tarray[i].size){
            break; }
    }

    tarray[i].nodes_array[tarray[i].current].inside=new;
    for(j=0;j<tarray[i-1].size;j++){
        if(tarray[i-1].nodes_array[j].first==NULL){
            tarray[i-1].nodes_array[j].first=&tarray[i].nodes_array[tarray[i].current];
            tarray[i].nodes_array[tarray[i].current].parent=&tarray[i-1].nodes_array[j];

            bin_check(&tarray[i].nodes_array[tarray[i].current]);
            tarray[i].current++;
            break;
        }
        if(tarray[i-1].nodes_array[j].second==NULL){
            tarray[i-1].nodes_array[j].second=&tarray[i].nodes_array[tarray[i].current];
            tarray[i].nodes_array[tarray[i].current].parent=&tarray[i-1].nodes_array[j];

            bin_check(&tarray[i].nodes_array[tarray[i].current]);
            tarray[i].current++;
            break;
        }


    }


}

void pop_down(bin_node* head){
    in_node* temp;
    int first;
    int second;
    if(head->first!=NULL) {
        first = head->first->inside->count;
        if(head->second!=NULL) {
            second = head->second->inside->count;
        }else{
            second=-2;
        }
        if (first > second) {
            temp = head->inside;
            head->inside = head->first->inside;
            head->first->inside = temp;
            pop_down(head->first);
        } else {
            temp = head->inside;
            head->inside = head->second->inside;
            head->second->inside = temp;
            pop_down(head->second);
        }

    }

}

void bin_check(bin_node* node){ //elenxei to topk dentro an tirei tin idiotita kai kanei swap
    bin_node* father;
    in_node* temp;
    if(node->parent!=NULL) {
        father = node->parent;
        if (father->inside->count < node->inside->count) {
            temp=father->inside;
            father->inside=node->inside;
            node->inside=temp;
            bin_check(father);
        }
    }
}

void get_data_temp(transfer* trans,tree* dentro,int what_is,char* mindate,char* maxdate,int hash_size){//ftiaxnei to temp hash table kai afxani ta counts
    int i,j,loc,found=0;
    int pass=1;
    char* comp;

    temp_hash* temp=NULL;
    temp_hash* temp_prev=NULL;
    if(dentro->less_equal!=NULL){
        get_data_temp(trans,dentro->less_equal,what_is,mindate,maxdate,hash_size);
    }
    if(dentro->greater!=NULL){
        get_data_temp(trans,dentro->greater,what_is,mindate,maxdate,hash_size);
    }

    if(mindate==NULL){
        pass=0;
    }else {
        if (check_date(mindate, dentro->branch_date) != 2 && check_date(maxdate, dentro->branch_date) == 2) {
            pass = 0;
        }

    }

    if(pass==0){
        for(i=0;i<dentro->num_of_pat;i++) {//gia ola ta stoixeia tou dentrou elenxei an tiroun tis proipo8eseis kai ta bazei sto hepm array/home/dimitris/Desktop/validation_v2/small.txt
            found=0;
            if (what_is == 0) {
                loc= hash(dentro->brach_patients[i]->country,hash_size);
                comp=dentro->brach_patients[i]->country;
            }else{
                loc= hash(dentro->brach_patients[i]->disease,hash_size);
                comp=dentro->brach_patients[i]->disease;
            }
            temp=trans->htable[loc];

            if(temp==NULL){
                trans->htable[loc]=init_temp(comp);
                trans->added++;
            } else {

                while (temp != NULL) {
                    if (strcmp(temp->node->tree_type, comp) == 0) {
                        temp->node->count++;
                        found = 1;
                        break;
                    }
                    temp_prev = temp;
                    temp = temp->next;
                }
                if (found == 0) {
                    temp_prev->next = init_temp(comp);
                    trans->added++;
                }

            }

        }



    }

}

void recordpatientexit(suma* ola,char* id,char* exit_date) {
    int i;
    listnode *temp = ola->pat_list->first;
    if (exit_date != NULL&& id!=NULL) {

        while (temp != NULL) {      //psaxni ti lisat as8enon gia ton as8eni me to sosto id
            if (strcmp(id, temp->pat->id) == 0) {
                if (check_date(temp->pat->admited_date, exit_date) < 2) {
                    strcpy(temp->pat->exit_date, exit_date);
                    printf("record updated \n");
                } else {
                    printf("exit date smaller that admited date \n");
                }
                break;
            }
            temp = temp->next;
        }
    }else{

        printf("please provide id and exit date\n");
    }
}


void add_to_day_hash(day_hash* dhash,patient* pat,int* daydiseases){
    day_hash* temp=NULL;
    day_hash* temp_prev=NULL;
    int loc=hash(pat->disease,10);
    int found=0;


    temp=&dhash[loc];

    if(temp->in==NULL){
        temp->in=init_init();
        temp->in->virus=malloc(strlen(pat->disease)+sizeof(char));
        (*daydiseases)++;
        strcpy(temp->in->virus,pat->disease);
        if(atoi(pat->age)<21){
            temp->in->catA++;
        }else if(atoi(pat->age)<41){
            temp->in->catB++;
        }else if(atoi(pat->age)<61){
            temp->in->catC++;
        }else{
            temp->in->catD++;
        }
    } else {

        while (temp != NULL) {
            if (strcmp(temp->in->virus, pat->disease) == 0) {
                if(atoi(pat->age)<21){
                    temp->in->catA++;
                }else if(atoi(pat->age)<41){
                    temp->in->catB++;
                }else if(atoi(pat->age)<61){
                    temp->in->catC++;
                }else{
                    temp->in->catD++;
                }
                found = 1;
                break;
            }
            temp_prev = temp;
            temp = temp->next;
        }
        if (found == 0) {
            temp_prev->next = (day_hash*)malloc(sizeof(day_hash));
            temp=temp_prev->next;
            temp->next=NULL;
            temp->in=init_init();
            temp->in->virus=malloc(strlen(pat->disease)+sizeof(char));
            (*daydiseases)++;
            strcpy(temp->in->virus,pat->disease);
            if(atoi(pat->age)<21){
                temp->in->catA++;
            }else if(atoi(pat->age)<41){
                temp->in->catB++;
            }else if(atoi(pat->age)<61){
                temp->in->catC++;
            }else{
                temp->in->catD++;
            }

        }

    }





}