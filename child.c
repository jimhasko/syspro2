

#include "child.h"

//sem_t mutex;
int newdata;
int dead_pid;
int exit_sig;

void child_big_call(int i,char* reader,char* writer,childthings* data,char* dir,int buffer,int feed_back){
    newdata=0;
    exit_sig=0;
    int fdreader,fdwriter,done=1;
    char* command;
    suma* ola;
    ola=child_call(i, reader, writer, data, dir, buffer, feed_back);
    int test;
    fd_set rfds;
   // printf("writer %s",writer);
    fdreader=open(writer,O_RDONLY);
    fdwriter=open(reader,O_WRONLY);
   // fd=ola->readerfd;
    if(fdreader<0){
        printf("reader: %s , fd:%d \n",reader,fdreader);
        perror("fd");
        exit(1);
    }else{
       // printf("new reader good: %d \n",getpid());
    }


    if(fdwriter<0){
        printf("reader: %s  , fd:%d \n",reader,fdwriter);
        perror("fd");
        exit(1);
    }else{
       // printf("new writer good: %d\n",getpid());
    }
  //  printf("child ready %d\n",getpid());


    FD_ZERO(&rfds);
    FD_SET(fdreader, &rfds);

  ///////coammds
    while(done){
      //  printf("about to select\n");
        test = select(fdreader+1, &rfds, NULL, NULL, NULL);

     //   printf("child %d got signal\n",getpid());
       if(exit_sig==1){

           done=0;

       }else if(newdata==1){
           newdata=0;
           new_data(ola,fdwriter,dir,buffer);
       }

//printf("about to read\n");
       if(FD_ISSET(fdreader, &rfds))
       {

           command = read_fifo(fdreader, buffer);
         //   printf("gto command:%s\n",command);


           if (strcmp(command, "recordID") == 0) {
               free(command);
               command = read_fifo(fdreader, buffer);

               recordid_child(command, ola, fdwriter, buffer);
           }

           if (strcmp(command, "discharge") == 0) {
            //   printf("discharger\n");
               free(command);
              patient_discharges_child(ola,fdreader,buffer);
           }




       }

    }
close(fdreader);
close(fdwriter);


}


suma* child_call(int i,char* reader,char* writer,childthings* data,char* dir,int buffer,int feed_back){
    char input[buffer];
    int pid,readerfd,writerfd;
    int init_array=0;
    char** carray=NULL;
    int def_size=10;
    int countries_numb=0;
    suma* ola=NULL;
    pid = getpid();
    int l,j,done=0;
    char* inpput;
   // carray=(char**)malloc(sizeof(char*)*def_size);

    //////////////////////////////

   // sem_wait(&mutex);

    readerfd = open(writer, O_RDONLY);
    if (readerfd < 0) {
        printf("child:%d readerfd error\n",pid);
        perror("open");
        exit(1);
    }else{
     //   printf("child:%d readerfd good\n",pid);
    }
    dup2(readerfd,2*i+3);
   // sem_post(&mutex);

    //printf("got_in_child_writer\n");
    j=0;



    inpput=read_fifo(readerfd,buffer);
    if(strcmp(inpput,"starting_countries")==0){
        init_array=1;
        free(inpput);
       inpput= read_fifo(readerfd,buffer);
        countries_numb=atoi(inpput);
        free(inpput);
        //printf(" got countries: %d\n",countries_numb);
    }

    for(l=0;l<countries_numb;l++) {

        if (carray == NULL) {
            carray = (char **) malloc(sizeof(char *) * countries_numb);
        }
        inpput= read_fifo(readerfd,buffer);
        // read(readerfd, input, buffer);
    //    printf("     child pid: %d read: %s l %d\n", pid, inpput, l);
        carray[j] = malloc(strlen(inpput) + sizeof(char));
        strcpy(carray[j], inpput);
        j++;
        free(inpput);
        // countries_numb++;


    }





  //  printf("preclosed writerfd");
    close(readerfd);
 //return readerfd;
//printf("closed writerfd");
   // for(j=0;j<countries_numb;j++){
  //      printf("child:%d country: %s  \n",pid,carray[j]);

  //  }


/////////////////////////////////////////////////////

   // sem_wait(&mutex);
 //   printf(" got to writer");
    writerfd = open(reader, O_WRONLY);
    if(writerfd<0){
        printf("child writer writerfd error");
          perror("open");
         exit(1);}
    else{
    //    printf(" child:%d writerfd good\n",pid);

    }
    dup2(writerfd,2*i+3);
  //  sem_post(&mutex);

    if(feed_back==1) {
        write_fifo(writerfd, "file_inputs", buffer);
    }

    for(j=0;j<countries_numb;j++){
       // printf("child:%d country: %s",pid,carray[j]);
       ola= child_init_add(dir,carray[j],writerfd,buffer,ola,feed_back);
    }


 if(feed_back==1) {
     write_fifo(writerfd, "finish", buffer);
 }
//    printf("got_in_child_reader\n");



    close(writerfd);

  //  for(j=0;j<countries_numb;j++){
  //      printf("child:%d country: %s : %d \n",pid,carray[j],countries_numb);

 //   }

    int dayshere;
    char* filename;
    filename=malloc(sizeof(char)*50);
    char** dates_array=NULL;
    ola->over=malloc(sizeof(overlay));
    ola->over->num_of_countries=countries_numb;
    ola->over->stat=(main_struct**)malloc(sizeof(main_struct*)*countries_numb);
    for(i=0;i<countries_numb;i++) {
        ola->over->stat[i]=malloc(sizeof(main_struct));
        ola->over->stat[i]->country=malloc((strlen(carray[i])+1)*sizeof(char));
              strcpy(ola->over->stat[i]->country,carray[i]) ;

       sprintf(filename,"%s/%s",dir,carray[i]);
       dates_array= short_dates(filename,&dayshere);
        ola->over->stat[i]->dates=malloc(sizeof(date_cases*)*dayshere);
        for(j=0;j<dayshere;j++){
            ola->over->stat[i]->dates[j]=malloc(sizeof(date_cases));
            ola->over->stat[i]->dates[j]->date=dates_array[j];
            ola->over->stat[i]->dates[j]->day_disease=0;
            ola->over->stat[i]->dates[j]->in=NULL;
        }

    }
  //  free(filename);
  //  free(dates_array);




ola->readerfd=readerfd;
ola->writerfd=writerfd;
return ola;


}

overlay* parent_call(int N,char* dir,childthings* children,int buffer,int** fd_matrix){
    int tets;
    int i,j,fd,fd2;
    fd_set rfds;
    int maxfd_reader=0;
    int maxfd_writer=0;
   // int fd_matrix[2][N];
    char input[buffer];
    overlay* stats=NULL;


    //////////////
    stats=get_stats(dir,N,children);
    for(i=0;i<N;i++){
        fd2 = open(children[i].reader, O_RDONLY);

        if(fd2<0){
            printf ("errno = %d \n ", errno);
            perror("open");
            exit(1);
        }

        fd_matrix[0][i]=fd2;
        children[i].fdreader=fd2;
        if(fd2>maxfd_reader)
            maxfd_reader=fd2;

    }
    FD_ZERO(&rfds);
    for(i=0;i<N;i++){

        FD_SET(fd_matrix[0][i],&rfds);
    }


    tets=select(maxfd_reader + 1, &rfds, NULL, NULL, NULL);
    if(tets<0){
        perror("select parent small");
    }
    for(i=0;i<N;i++){

        FD_SET(fd_matrix[0][i],&rfds);
    }

    int size;

    char* input2;
int cdone=0;

   // while(cdone<N) {
        for (i = 0; i < N; i++) {

            if (FD_ISSET(fd_matrix[0][i], &rfds)) {


                input2 = read_fifo(fd_matrix[0][i], buffer);

                if (strcmp(input2, "file_inputs") == 0) {
                    read_some(fd_matrix[0][i], buffer, stats);
                } else if (strcmp(input2, "DONE CALLING") == 0) {
                    cdone++;
                }
                free(input2);
            }




        }




/////////////
    for(i=0;i<N;i++){

       close(fd_matrix[0][i]);
    }
    //////////////////
    return  stats;
}

childthings* read_files(char* dir,int N){
    char* s1="/tmp/fifeoao";
    char* s2="/tmp/readerfifeoao";

    int i,j,file_num=0;

    DIR *drr = opendir(dir);


    struct dirent *dee;

    if (drr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory");

    }

    char *date;

    drr = opendir(dir);
    while ((dee = readdir(drr)) != NULL) {
        if (dee->d_name[0] != '.') {
            file_num++;

        }
    }
    closedir(drr);


    if(N>file_num){
        N=file_num;
    }

char** countries_list=malloc(sizeof(char*)*file_num);
i=0;
    drr = opendir(dir);
    while ((dee = readdir(drr)) != NULL) {
        if (dee->d_name[0] != '.') {
            countries_list[i]=malloc(strlen(dee->d_name)+sizeof(char));
        strcpy(countries_list[i],dee->d_name);
        i++;
        }
    }
    closedir(drr);
   int  child_countries=file_num/N;


    childthings* children;
    children=(childthings*)malloc(sizeof(childthings)*N);
    for(i=0;i<N;i++) {
        if(i==0) {
            children[i].num_of_countries = child_countries+file_num%N;

            children[i].countries = (char **) malloc(sizeof(char *) * (child_countries+file_num%N));
        }else{
            children[i].num_of_countries = child_countries;
            children[i].countries = (char  **) malloc(sizeof(char *) * (child_countries));
        }

    }
int k=0;
for(i=0;i<N;i++){
for(j=0;j<children[i].num_of_countries;j++){
    children[i].countries[j]=malloc(strlen(countries_list[k])+1);
    strcpy(children[i].countries[j],countries_list[k]);
        k++;

}



}


    char h[11];


    for(i=0;i<N;i++){
        sprintf(h,"%d",i);
        children[i].writer=malloc(strlen(s1) + strlen(h) + 1);
        strcpy(children[i].writer, s1);
        strcat(children[i].writer, h);



        children[i].reader=malloc(strlen(s2) + strlen(h) + 1);
        strcpy(children[i].reader, s2);
        strcat(children[i].reader, h);

    }



for(i=0;i<file_num;i++){
    free(countries_list[i]);
}
free(countries_list);


    return children;

}


suma* child_init_add(char* dir,char* gr,int fd,int buffer,suma* ola,int feed_back){
    int j,i,datenum=0;
    char*  ptr=(char*)malloc(sizeof(char)*200);

    char add='/';


    char* newdir=NULL;
    char* filename=NULL;
    char**  datelist;

    newdir = (char *) malloc(strlen(gr) + strlen(dir) + (2 * sizeof(char)));

    strcpy(newdir, dir);
    strncat(newdir, &add, 1);
    strcat(newdir, gr);
    datelist=short_dates(newdir,&datenum);

    DIR *drr = opendir(newdir);


    struct dirent *dee;

    if (drr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory");

    }
    i = 0;
    char *date;
   // suma *ola = NULL;
int size;
    for(i=0;i<datenum;i++) {


      //  printf("%s\n", datelist[i]);
        date = malloc(strlen(datelist[i]) + (sizeof(char)));
        strcpy(date, datelist[i]);
        filename = (char *) malloc(strlen(newdir) + strlen(datelist[i]) + (2 * sizeof(char)));
        sprintf(filename, "%s/%s", newdir, datelist[i]);
        ola = get_from_file(ola, filename, 30, 30, datelist[i], gr);

      /////////////



     //   write(fd,&ola->day_disease,sizeof(int));
       char* sum=malloc(100*sizeof(char));
        for(j=0;j<ola->day_disease;j++){
            sprintf(sum,"%s||%s||%d||%s||%d||%d||%d||%d",gr,datelist[i],ola->day_disease,ola->starter[j]->virus,ola->starter[j]->catA,ola->starter[j]->catB,ola->starter[j]->catC,ola->starter[j]->catD);

            if(feed_back==1) {
              write_fifo(fd, sum, buffer);
            }



        }
        kill_state(ola);
       ///////////////////



    }
    closedir(drr);
    free(newdir);
return ola;

}


char** short_dates(char* dir,int* files){

    char** darray;
    int j,file_num=0;


    DIR *drr = opendir(dir);


    struct dirent *dee;

    if (drr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory");
        perror("opendir");
    }

    char *date;


    while ((dee = readdir(drr)) != NULL) {
        if (dee->d_name[0] != '.') {
            file_num++;



        }
    }
    closedir(drr);
    drr = opendir(dir);
    (*files)=file_num;
    darray=(char**)malloc(sizeof(char*)*file_num);
    j=0;
    while ((dee = readdir(drr)) != NULL) {
        if (dee->d_name[0] != '.') {
            //  printf("%s\n", dee->d_name);
            darray[j] = malloc(strlen(dee->d_name) + (sizeof(char)));
            strcpy(darray[j], dee->d_name);
            j++;
        }
    }
    closedir(drr);

    quicksort(darray,0,file_num-1);

    return darray;
}

void quicksort(char**dates, int from, int to) {

    int i, j, pivot;
    char * temp;

//uint64_t **dates=table->TableB;
//columns=table->columns;

    if (from < to) {
        pivot = from;
        i = from;
        j = to;

        while (i < j) {

            while (check_date(dates[i] , dates[pivot])==1 && i < to) {

                i++;
            }
            while (check_date(dates[j] , dates[pivot])==2) {

                j--;
            }
            if (i < j) {



                temp = dates[i];
                dates[i] = dates[j];
                dates[j] = temp;



            }
        }




        temp = dates[i];
        dates[i] = dates[j];
        dates[j] = temp;


        quicksort(dates, from, j - 1);
        quicksort(dates, j + 1, to);
    }
}


overlay* get_stats(char* dir,int N,childthings* child){

int i,j,l,k,num_of_countries=0;
int days=0;
char** darray;
for(i=0;i<N;i++){
    num_of_countries+= child[i].num_of_countries;
}
j=0;
main_struct** stat=malloc(sizeof(main_struct*)*num_of_countries);
for(i=0;i<num_of_countries;i++){
    stat[i]=malloc(sizeof(main_struct));
    stat[i]->country=NULL;
    stat[i]->num_of_dates=0;
    stat[i]->dates=NULL;

}
for(i=0;i<N;i++){
    for(k=0;k<child[i].num_of_countries;k++){
        stat[j]->country=malloc(strlen(child[i].countries[k])+sizeof(char));
        strcpy(stat[j]->country,child[i].countries[k]);


        stat[j]->dates=file_arr(dir,stat[j]->country);
        stat[j]->num_of_dates=file_num(dir,stat[j]->country);
        j++;
    }
}


    overlay* over=malloc(sizeof(overlay));
    over->stat=stat;
    over->num_of_countries=num_of_countries;

return over;

    }



int file_num(char* dir,char* gr){
    int file_num=0;
    char* filename;
    filename = (char *) malloc(strlen(dir) + strlen(gr) + (2 * sizeof(char)));
    sprintf(filename, "%s/%s", dir, gr);


    DIR *drr = opendir(filename);


    struct dirent *dee;

    if (drr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory");

    }

    char *date;

   // drr = opendir(dir);
    while ((dee = readdir(drr)) != NULL) {
        if (dee->d_name[0] != '.') {
            file_num++;

        }
    }
    closedir(drr);
    free(filename);
    return file_num;
}

date_cases** file_arr(char* dir,char* gr){
    date_cases** date_array;

    int i,files=0;
    char* filename;
    filename = (char *) malloc(strlen(dir) + strlen(gr) + (2 * sizeof(char)));
    sprintf(filename, "%s/%s", dir, gr);

    files=file_num(dir,gr);
    date_array=(date_cases**)malloc(sizeof(date_cases*)*files);
    DIR *drr = opendir(filename);


    struct dirent *dee;

    if (drr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory");

    }

    char *date;
    i=0;
  //  drr = opendir(dir);
    while ((dee = readdir(drr)) != NULL) {
        if (dee->d_name[0] != '.') {
            date_array[i]=(date_cases*)malloc(sizeof(date_cases));
            date_array[i]->date=malloc((strlen(dee->d_name)+1)*sizeof(char));
            strcpy(date_array[i]->date,dee->d_name);
            date_array[i]->in=NULL;
            date_array[i]->day_disease=0;
            i++;
        }
    }
    closedir(drr);
free(filename);
    return date_array;

}

void exit_main(childthings* children,int N,overlay* stats){
    int i,j,k;


    for(i=0;i<stats->num_of_countries;i++){
        for(j=0;j<stats->stat[i]->num_of_dates;j++){
            for(k=0;k<stats->stat[i]->dates[j]->day_disease;k++){
                free(stats->stat[i]->dates[j]->in[k]->virus);
                free(stats->stat[i]->dates[j]->in[k]);
            }

            free(stats->stat[i]->dates[j]->date);
            free(stats->stat[i]->dates[j]->in);
            free(stats->stat[i]->dates[j]);
        }
        free(stats->stat[i]->dates);
        free(stats->stat[i]->country);
        free(stats->stat[i]);
    }
    free(stats->stat);

    free(stats);

}

void kill_state(suma* ola){
    int i,j;
    for(i=0;i<ola->day_disease;i++){
        free(ola->starter[i]->virus);
    }
    free(ola->starter);

}

void read_some(int fd,int buffer,overlay* stats){

    char* total;


    int isdone=0;



    while(isdone==0) {


total=read_fifo(fd,buffer);
        if (strcmp(total,"finish")==0) {
                isdone=1;
            free(total);
        }
        if(isdone==0){
        char_to_str(total,stats);}


    }





}



void char_to_str(char* ptr,overlay* stats){
    int i,j,k,l;
    initial_transfer* in;
    in=malloc(sizeof(initial_transfer));
    char* country=NULL;
    char* date=NULL;
    char delim[3]="||";
    char* test=NULL;
    int day_diseases=0;
    test = strtok(ptr, delim);
    if(test!=NULL) {
        country = malloc((strlen(test) + 1)*sizeof(char));
        strcpy(country, test);
    }
    test = strtok(NULL, delim);
    if(test!=NULL) {
        date = malloc((strlen(test) + 1)*sizeof(char));
        strcpy(date, test);
    }

    test = strtok(NULL, delim);
    if(test!=NULL) {
        day_diseases=atoi(test);
    }

    test = strtok(NULL, delim);
    if(test!=NULL) {
        in->virus = malloc((strlen(test) + 1)*sizeof(char));
        strcpy(in->virus, test);
    }

    test = strtok(NULL, delim);
    if(test!=NULL) {
        in->catA=atoi(test);
    }
    test = strtok(NULL, delim);
    if(test!=NULL) {
        in->catB=atoi(test);
    }
    test = strtok(NULL, delim);
    if(test!=NULL) {
        in->catC=atoi(test);
    }
    test = strtok(NULL, delim);
    if(test!=NULL) {
        in->catD=atoi(test);
    }

int brk=0;
if(strcmp(country,"CROATIA")==0) {
    if(strcmp(date,"29-2-2013")==0){
        int ll = 0;
    }

}
    for(i=0;i<stats->num_of_countries;i++){
        if(brk==1){
            break;
        }
        k=0;
        if(strcmp(stats->stat[i]->country,country)==0){
            for(j=0;j<stats->stat[i]->num_of_dates;j++){
               if(strcmp(stats->stat[i]->dates[j]->date,date)==0){
                   if(stats->stat[i]->dates[j]->in==NULL){
                       stats->stat[i]->dates[j]->in=(initial_transfer**)malloc(sizeof(initial_transfer*)*day_diseases);
                       for(l=0;l<day_diseases;l++){
                           stats->stat[i]->dates[j]->day_disease=day_diseases;
                           stats->stat[i]->dates[j]->in[l]=NULL;
                       }
                       stats->stat[i]->dates[j]->in[0]=in;
                   }else{
                       for(k=0;k<day_diseases;k++){
                           if(stats->stat[i]->dates[j]->in[k]==NULL){
                               stats->stat[i]->dates[j]->in[k]=in;
                               break;
                           }

                       }


                   }

                   brk=1;
                   break ;
               }
            }
        }
    }


if(country!=NULL)
    free(country);
if(date!=NULL)
    free(date);

free(ptr);

}



void list_countries(childthings* child,int N){

    int i,j;
    for(i=0;i<N;i++){
        for(j=0;j<child[i].num_of_countries;j++){
            printf("%s %d\n",child[i].countries[j],child[i].pid);
        }
    }
}


void disease_frequency(char* virus,char* date1,char* date2,char*country,overlay* stats){
    int i,j,l;
    char* date;
    int suma=0;
    if(country==NULL) {
        for (i = 0; i < stats->num_of_countries; i++) {
            for (j = 0; j < stats->stat[i]->num_of_dates; j++) {
                date = stats->stat[i]->dates[j]->date;
                if ((check_date(date1, date) == 1) && (check_date(date, date2) == 1))
                    for (l = 0; l < stats->stat[i]->dates[j]->day_disease; l++) {
                        if (strcmp(stats->stat[i]->dates[j]->in[l]->virus, virus) == 0) {
                            suma = suma + stats->stat[i]->dates[j]->in[l]->catA +
                                   stats->stat[i]->dates[j]->in[l]->catB + stats->stat[i]->dates[j]->in[l]->catC +
                                   stats->stat[i]->dates[j]->in[l]->catD;
                        }
                    }

            }

        }
    }else{

        for (i = 0; i < stats->num_of_countries; i++) {
            if (strcmp(country, stats->stat[i]->country) == 0){
                for (j = 0; j < stats->stat[i]->num_of_dates; j++) {
                    date = stats->stat[i]->dates[j]->date;
                    if ((check_date(date1, date) == 1) && (check_date(date, date2) == 1))
                        for (l = 0; l < stats->stat[i]->dates[j]->day_disease; l++) {
                            if (strcmp(stats->stat[i]->dates[j]->in[l]->virus, virus) == 0) {
                                suma = suma + stats->stat[i]->dates[j]->in[l]->catA +
                                       stats->stat[i]->dates[j]->in[l]->catB + stats->stat[i]->dates[j]->in[l]->catC +
                                       stats->stat[i]->dates[j]->in[l]->catD;
                            }
                        }

                }
        }
        }









    }

    printf("%d \n",suma);



}


void num_patient_admissions(char* virus,char* date1,char* date2,char*country,overlay* stats){
    int i,j,l;
    char* date;
    int suma=0;
    if(country==NULL) {
        for (i = 0; i < stats->num_of_countries; i++) {
            suma=0;
            printf("%s: ",stats->stat[i]->country);
            for (j = 0; j < stats->stat[i]->num_of_dates; j++) {
                date = stats->stat[i]->dates[j]->date;
                if ((check_date(date1, date) == 1) && (check_date(date, date2) == 1))
                    for (l = 0; l < stats->stat[i]->dates[j]->day_disease; l++) {
                        if (strcmp(stats->stat[i]->dates[j]->in[l]->virus, virus) == 0) {
                            suma = suma + stats->stat[i]->dates[j]->in[l]->catA +
                                   stats->stat[i]->dates[j]->in[l]->catB + stats->stat[i]->dates[j]->in[l]->catC +
                                   stats->stat[i]->dates[j]->in[l]->catD;
                        }
                    }

            }
            printf("%d \n",suma);

        }



    }else{

        for (i = 0; i < stats->num_of_countries; i++) {
            if (strcmp(country, stats->stat[i]->country) == 0){
                for (j = 0; j < stats->stat[i]->num_of_dates; j++) {
                    date = stats->stat[i]->dates[j]->date;
                    if ((check_date(date1, date) == 1) && (check_date(date, date2) == 1))
                        for (l = 0; l < stats->stat[i]->dates[j]->day_disease; l++) {
                            if (strcmp(stats->stat[i]->dates[j]->in[l]->virus, virus) == 0) {
                                suma = suma + stats->stat[i]->dates[j]->in[l]->catA +
                                       stats->stat[i]->dates[j]->in[l]->catB + stats->stat[i]->dates[j]->in[l]->catC +
                                       stats->stat[i]->dates[j]->in[l]->catD;
                            }
                        }

                }
            }
        }




        printf("%d \n",suma);




    }





}

void topk(char* num,char*country,char* virus,char* date1,char* date2,overlay* stats){
    int i,j,l;
    int K;
    K=atoi(num);
    char* date;
    int catA,catB,catC,catD;
    double suma=0.0;
    catA=0;
    catB=0;
    catC=0;
    catD=0;


        for (i = 0; i < stats->num_of_countries; i++) {
            if (strcmp(country, stats->stat[i]->country) == 0){
                for (j = 0; j < stats->stat[i]->num_of_dates; j++) {
                    date = stats->stat[i]->dates[j]->date;
                    if ((check_date(date1, date) == 1) && (check_date(date, date2) == 1))
                        for (l = 0; l < stats->stat[i]->dates[j]->day_disease; l++) {
                            if (strcmp(stats->stat[i]->dates[j]->in[l]->virus, virus) == 0) {
                                catA=catA+ stats->stat[i]->dates[j]->in[l]->catA ;
                                catB=catB+stats->stat[i]->dates[j]->in[l]->catB;
                                catC=catC+stats->stat[i]->dates[j]->in[l]->catC ;
                                catD=catD+stats->stat[i]->dates[j]->in[l]->catD;
                            }
                        }

                }
            }
        }

    helper* hlp=malloc(sizeof(helper)*4);
    suma=catA+catB+catC+catD;
    double temp;
    char* tt=malloc(5*sizeof(char)+1);
    hlp[0].cat=(catA/suma)*100;
    hlp[1].cat=(catB/suma)*100;
    hlp[2].cat=(catC/suma)*100;
    hlp[3].cat=(catD/suma)*100;
    hlp[0].name=malloc(5*sizeof(char)+1);
    strcpy(hlp[0].name,"0-20");
    hlp[1].name=malloc(5*sizeof(char)+1);
    strcpy(hlp[1].name,"20-40");
    hlp[2].name=malloc(5*sizeof(char)+1);
    strcpy(hlp[2].name,"40-60");
    hlp[3].name=malloc(5*sizeof(char)+1);
    strcpy(hlp[3].name,"60+");


for(i=0;i<3;i++){
    for(j=0;j<3-i;j++){
        if(hlp[j].cat<hlp[j+1].cat){
           temp=hlp[j].cat;
           strcpy(tt,hlp[j].name);
           hlp[j].cat=hlp[j+1].cat;
            strcpy(hlp[j].name,hlp[j+1].name);
            hlp[j+1].cat=temp;
            strcpy(hlp[j+1].name,tt);
        }


    }


}

for(j=0;j<K;j++){
    printf("%s :%f %s\n",hlp[j].name,hlp[j].cat,"%");

}
free(tt);
free(hlp[0].name);
    free(hlp[1].name);
    free(hlp[2].name);
    free(hlp[3].name);
free(hlp);




}



char* read_fifo(int fd,int buffer){

    char *input;
    input=malloc(buffer*sizeof(char));
    char* total;
    char* total2;

    int fois;
    size_t size=0;
    int i;
    int nbytes;
    int isdone=0;
//printf("got in reader\n");
    total=malloc(sizeof(size_t));
    total2=malloc(sizeof(size_t));

    nbytes=read(fd, input, 1);
    if(nbytes<0){
        perror("read");
    }
    memcpy(total,input,1);
    total[1]='\0';
  //  printf("total:%s \n",total);
    while(isdone<2) {
     //   printf("total:%s \n",total);
        nbytes=read(fd, input, 1);
        if(nbytes<0){
            perror("read");
        }
        memcpy(total2,input,1);
        total2[1]='\0';



        if(strcmp(total2,"|")==0){
            isdone++;
        }else{
            strcat(total,total2);
        }

    }


free(total2);

    size=atoi(total);
    free(total);
  //   printf("got size %ld \n",size);
    if(size<=buffer){
        nbytes=read(fd,input,size);
        if(nbytes<0){
            perror("read");
        }
        total=malloc(size+1);
        memcpy(total,input,size);
        total[size]='\0';

    }else{
        fois=size/buffer;
        total=malloc(size+1);
        total2=malloc(buffer+1);
        nbytes= read(fd,input,buffer);
        if(nbytes<0){
            perror("read");
        }
        memcpy(total,input,buffer);
        total[buffer]='\0';

        for(i=1;i<fois;i++){


            nbytes= read(fd,input,buffer);
            if(nbytes<0){
                perror("read");
            }
            memcpy(total2,input,buffer);
            total2[buffer]='\0';

            strcat(total,total2);
        }

        nbytes=read(fd,input,size%buffer);
        if(nbytes<0){
            perror("read");
        }
        memcpy(total2,input,size%buffer);
        total2[size%buffer]='\0';

        strcat(total,total2);


    free(total2);

    }
    free(input);
//printf("command:%s ",total);
    return total;


}


void write_fifo(int fd,char* command,int buffer){
    size_t size;
    char* total;
    char* togo;
    int size2;
    int fois;
    int nbytes,i;
    togo=malloc(strlen(command)+1);
    strcpy(togo,command);
    char* number=malloc(21);
    sprintf(number,"%ld",strlen(togo));

    if(buffer<sizeof(size_t)){
        while(strlen(number)>buffer){
            nbytes= write(fd,number,buffer);
            if(nbytes<0){
                perror("write");
            }
            number=number+buffer;
        }
        if(buffer>1){
            nbytes= write(fd,number,strlen(number)%buffer);
            if(nbytes<0){
                perror("write");
            }}
        else{
            nbytes= write(fd,number,1);
            if(nbytes<0){
                perror("write");
            }
        }


    }else{


        nbytes=write(fd,number,strlen(number));
        if(nbytes<0){
            perror("write");
        }
    }

    nbytes=write(fd,"||",strlen("||"));
    if(nbytes<0){
        perror("write");
    }

    if(buffer<strlen(togo)){
        while(strlen(togo)>buffer){
            nbytes= write(fd,togo,buffer);
            if(nbytes<0){
                perror("write");
            }
            togo=togo+buffer;
        }
        if(buffer>1){
            nbytes= write(fd,togo,strlen(togo)%buffer);
            if(nbytes<0){
                perror("write");
            }}
        else{
            nbytes= write(fd,togo,1);
            if(nbytes<0){
                perror("write");
            }
        }


    }else{


        nbytes= write(fd,togo,strlen(togo));
        if(nbytes<0){
            perror("write");
        }

    }

  //  free(togo);
    //free(number);
}


void parent_big_call(int N,char* dir,childthings* children,int buffer){
    int i,j,fd,fd2;
    exit_sig=0;
    int newpid=-1;
    fd_set rfds;
    struct timeval timer;
    char* command;
    int maxfd_reader=0;
    int maxfd_writer=0;
    int** fd_matrix;
    int exited=0;
    char * sender=NULL;
  int pass,fail;
  pass=0;
  fail=0;
    overlay* stats=NULL;
    fd_matrix=malloc(sizeof(int*)*2);
    fd_matrix[0]=malloc(sizeof(int)*N);
    fd_matrix[1]=malloc(sizeof(int)*N);
    for(i=0;i<N;i++) {
        j=0;
        fd = open(children[i].writer, O_WRONLY);
        if(fd<0){
            printf("parrent fd:%d writer error",fd);
            perror("open");
        }
        fd_matrix[1][i]=fd;
        children[i].fdwriter=fd;
        if(fd>maxfd_writer)
            maxfd_writer=fd;


        write_fifo(fd_matrix[1][i],"starting_countries",buffer);
        sender=malloc(sizeof(char)*10);
        sprintf(sender,"%d",children[i].num_of_countries);
        write_fifo(fd_matrix[1][i],sender,buffer);
        free(sender);

        while(children[i].num_of_countries>j){
            write_fifo(fd_matrix[1][i],children[i].countries[j],buffer);

            j++;
            if(j==children[i].num_of_countries){
               // write_fifo(fd_matrix[1][i],"done",buffer);

            }
        }
        close(fd_matrix[1][i]);


    }



    stats=parent_call(N,dir,children,buffer,fd_matrix);

    int child_done=0;




///////////////////////////////////comands

   int is;
    for(i=0;i<N;i++) {
        j=0;
        fd = open(children[i].writer, O_WRONLY);

        if(fd<0){
            printf("parrent fd:%d writer error",fd);
            perror("open");
            exit(1);
        }


        fd_matrix[1][i]=fd;
        children[i].fdwriter=fd;
        if(fd>maxfd_writer)
            maxfd_writer=fd;
        //////////////////
        fd2 = open(children[i].reader, O_RDWR);
        if(fd2<0){
            printf("parrent fd2:%d reader error",fd);
            perror("open");
            exit(1);
        }
        //////////
        fd_matrix[0][i]=fd2;
        children[i].fdreader=fd2;
        if(fd2>maxfd_reader)
            maxfd_reader=fd2;}




                timer.tv_sec=0;
                timer.tv_usec=0;
                int sell,fdmaxreader=-1;


                    int got_one=0;

                while(exited==0){
                    got_one=0;



                    FD_ZERO(&rfds);
                    for(i=0;i<N;i++) {
                        FD_SET(fd_matrix[0][i], &rfds);
                        if(fd_matrix[0][i]>fdmaxreader){
                            fdmaxreader=fd_matrix[0][i];
                        }
                    }
                    FD_SET(0,&rfds);
                    printf("im at select \n");
                    sell = select(fdmaxreader + 1, &rfds, NULL, NULL, NULL);
                    if(sell<0){
                        perror("select father big");
                    }






                    ////////////////////////////FORKforkforkfork
                    if(dead_pid>0){
                        for(i=0;i<N;i++){
                            if(children[i].pid==dead_pid){
                                close(fd_matrix[0][i]);
                                close(fd_matrix[1][i]);
                                break;
                            }
                        }
                        newpid=fork();
                        if(newpid==0){

                            break;
                        }else{
                            children[i].pid=newpid;
                            j=0;
                            ///////
                            fd = open(children[i].writer, O_WRONLY);
                            if(fd<0){
                                printf("parrent fd:%d writer error",fd);
                                perror("open");
                                exit(1);
                            }
                            fd_matrix[1][i]=fd;
                            children[i].fdwriter=fd;
                            if(fd>maxfd_writer)
                                maxfd_writer=fd;


                            write_fifo(fd_matrix[1][i],"starting_countries",buffer);
                            sender=malloc(sizeof(char)*10);
                            sprintf(sender,"%d",children[i].num_of_countries);
                            write_fifo(fd_matrix[1][i],sender,buffer);
                            free(sender);

                            while(children[i].num_of_countries>j){
                                write_fifo(fd_matrix[1][i],children[i].countries[j],buffer);

                                j++;
                                if(j==children[i].num_of_countries){
                              //      write_fifo(fd_matrix[1][i],"done",buffer);

                                }
                            }
                            close(fd_matrix[1][i]);
                            ///////
                            j=0;
                            fd = open(children[i].writer, O_WRONLY);

                            if(fd<0){
                                printf("parrent fd:%d writer error\n",fd);
                                perror("open");
                                exit(1);
                            }


                            fd_matrix[1][i]=fd;
                            children[i].fdwriter=fd;
                            if(fd>maxfd_writer)
                                maxfd_writer=fd;
                            //////////////////
                            fd2 = open(children[i].reader, O_RDWR);
                            if(fd2<0){
                                printf("parrent fd2:%d reader error\n",fd);
                                perror("open");
                                exit(1);
                            }
                            //////////
                            fd_matrix[0][i]=fd2;
                            children[i].fdreader=fd2;
                            if(fd2>maxfd_reader)
                                maxfd_reader=fd2;


                        }
                        pass++;
                        dead_pid=0;
                    }



              //////////////////////////





                    if(FD_ISSET(0,&rfds)){
                        if(got_one<(N)) {
                            got_one=N;
                            call_others(stats, N, fd_matrix, buffer, children, &exited,&pass,&fail);

                        }

                    }




                    for(i=0;i<N;i++) {
                        if (FD_ISSET(fd_matrix[0][i], &rfds)) {

                            if(got_one<(N)) {
                                got_one++;
                                command = read_fifo(fd_matrix[0][i], buffer);
                                if (strcmp(command, "newdata") == 0) {
                                    new_data_parent(fd_matrix[0][i], stats, buffer);
                                }
                            }
                        }
                    }





            }

if(newpid==0){
    child_big_call(i,children[i].reader,children[i].writer,&children[i], dir,buffer,0);

}






for(i=0;i<N;i++){
    close(fd_matrix[1][i]);
}




}


void recordid_parent(char* id,childthings* children,int N,int ** fd_matrix,int buffer,int* pass,int* fail){
    int i,fd;
    size_t test;
    char* result;
    int found=0;
    int done=0;


    for(i=0;i<N;i++){

       // kill(children[i].pid,SIGUSR2);
      fd=fd_matrix[1][i];
        if(fd<0){
            printf("parrent fd:%d writer error",fd);
            perror("open");
            exit(1);
        }
        write_fifo(fd,"recordID",buffer);
        write_fifo(fd,id,buffer);
        //kill(children[i].pid,32);
    }

    fd_set readers;
    struct timeval timer;
    timer.tv_sec=2;
    timer.tv_usec=0;
    int sell,fdmax=-1;
    while(done<N){
        FD_ZERO(&readers);
        for(i=0;i<N;i++) {
              FD_SET(fd_matrix[0][i], &readers);
              if(fd_matrix[0][i]>fdmax){
                  fdmax=fd_matrix[0][i];
              }
        }
        sell = select(fdmax + 1, &readers, NULL, NULL, &timer);
        if(sell<0){
            perror("select");
        }
        for(i=0;i<N;i++) {
            if (FD_ISSET(fd_matrix[0][i], &readers)) {
                done++;
            //    printf("got one\n");
                result=read_fifo(fd_matrix[0][i],buffer);
                if(strcmp(result,"nope")!=0){
                    printf("%s \n",result);
                    found=1;
                }
                free(result);

            }
        }

    }


if(found==1){
    (*pass)++;}
    else{
        printf(" patient not found\n");
    (*fail)++;

}

}

void signalHandler(int signalNum) {

    printf("Caught SIGUSR1 signal.%d\n",signalNum);
    newdata=1;

}

void exitsignal(int signalNum) {

    printf("Caught sigint signal.%d\n",getpid());
    exit_sig=1;

}

void signalkid(int signum) {
    pid_t pid;
    int   status;
    fputs("child",stdin);
    if ((pid = waitpid(-1, NULL, WNOHANG)) != -1)
    {
        dead_pid=pid;
    }
}

void recordid_child(char* id,suma* ola,int fd,int buffer){
    int i,size;
    char* result;
    listnode *temp = ola->pat_list->first;
   int  found=0;
   // printf("id %s :: %d \n",id,getpid());
    while (temp != NULL) {      //psaxni ti lisat as8enon gia ton as8eni me to sosto id

        if (strcmp(id, temp->pat->id) == 0) {
result=malloc(150*sizeof(char));
sprintf(result,"%s %s  %s  %s  %s  %s  %s",temp->pat->id,temp->pat->first_name,temp->pat->last_name,temp->pat->disease,temp->pat->age,temp->pat->admited_date,temp->pat->exit_date);
        found=1;
            break;
        }
        temp = temp->next;
    }
if(found==0){
    write_fifo(fd,"nope",buffer);
}else{
    write_fifo(fd,result,buffer);


}

}

void exit_parent(childthings* children,int N,int ** fd_matrix,overlay* stats,int* succses,int* fail){
int i,fd;

    int status = 0;
    for(i=0;i<N;i++){

        kill(children[i].pid,SIGKILL);


      }

    while ((wait(&status)) > 0);

        exit_parent_file(stats,succses,fail);
      exit_main(children,N,stats);
   // free(fd_matrix[0]);
   // free(fd_matrix[1]);
    //free(fd_matrix);

}


void new_data(suma* ola,int fd,char* dir,int buffer){
   // printf("got into new data\n");
    int pr;
    int i,j,k,l,add;
    char** countrydir;
    char** newdates;
    int newnum=0;
    char* filename;
    char** addeddates=NULL;
    int num=  ola->over->num_of_countries;
    char* sum=malloc(100*sizeof(char));
    countrydir=malloc(sizeof(char*)*num);

    for(i=0;i<num;i++){
        countrydir[i]=malloc(strlen(dir)+strlen(ola->over->stat[i]->country)+sizeof (char));
        sprintf(countrydir[i], "%s/%s", dir, ola->over->stat[i]->country);
      //  printf("countries: %s \n",countrydir[i]);

    }

    for(i=0;i<num;i++) {//for each country
        newdates = short_dates(countrydir[i],&newnum);
       num= 0;
       // printf("got country %s old:%d , new %d \n",countrydir[i],num,newnum);
        if(newnum>num){
            //addeddates=malloc(sizeof(char*)*(newnum-num));
            add=0;
            k=0;
            for(j=0;j<newnum;j++){
               // printf("date comp: %s :: %s\n",newdates[j],ola->over->stat[i]->dates[k]->date);
                if(strcmp(newdates[j],ola->over->stat[i]->dates[k]->date)!=0){
                    //addeddates[add]=malloc(strlen(newdates[j])+sizeof(char));
                //    printf("got 1\n");
                    add++;
                }else{
                    k++;
                }
            }
            if(add>0){
                addeddates=malloc(sizeof(char*)*add);
                add=0;
                k=0;
                for(j=0;j<newnum;j++){
                   // printf("date comp: %s :: %s\n",newdates[j],ola->over->stat[i]->dates[k]->date);
                    if(strcmp(newdates[j],ola->over->stat[i]->dates[k]->date)!=0){
                        addeddates[add]=malloc(strlen(newdates[j])+sizeof(char));
                        strcpy(addeddates[add],newdates[j]);

                        add++;
                    }else{
                        k++;
                    }
                }



            }


            write_fifo(fd, "newdata", buffer);
            for(k=0;k<(add);k++) {

                filename = (char *) malloc(strlen(countrydir[i]) + strlen(addeddates[k]) + (2 * sizeof(char)));
                sprintf(filename, "%s/%s", countrydir[i], addeddates[k]);
              //  printf("filename: %s \n", filename);
                ola = get_from_file(ola, filename, 30, 30, addeddates[k], ola->over->stat[i]->country);

                j = 0;
               // printf("child got extra: %s||%s||%d||%s||%d||%d||%d||%d\n", ola->over->stat[i]->country, addeddates[k],
                  //     ola->day_disease, ola->starter[j]->virus, ola->starter[j]->catA, ola->starter[j]->catB,
                  //     ola->starter[j]->catC, ola->starter[j]->catD);
                printf("got_from_file %d\n", ola->day_disease);

                for (j = 0; j < ola->day_disease; j++) {
                    pr = sprintf(sum, "%s||%s||%d||%s||%d||%d||%d||%d", ola->over->stat[i]->country, addeddates[k], ola->day_disease, ola->starter[j]->virus, ola->starter[j]->catA, ola->starter[j]->catB, ola->starter[j]->catC, ola->starter[j]->catD);
                    if (pr < 0) {
                        perror("sprintf");
                    }
                    printf("copied: %d\n", pr);
                    printf("child got extra: %s||%s||%d||%s||%d||%d||%d||%d\n", ola->over->stat[i]->country, addeddates[k], ola->day_disease, ola->starter[j]->virus, ola->starter[j]->catA, ola->starter[j]->catB, ola->starter[j]->catC, ola->starter[j]->catD);

                    write_fifo(fd, sum, buffer);

                }
               // printf("donedate\n");
                write_fifo(fd,"donedate",buffer);
            }
            kill_state(ola);






        }
    }


write_fifo(fd,"done",buffer);

//printf("DONE GETTING EXTRA\n");



}



void call_others(overlay* stats,int N,int **fd_matrix,int buffer,childthings* children,int* exit,int*pass,int*fail){
    int j,i,got_space=0;
    char * ptr,*k,*test;
    int is;


    ptr=(char*)malloc(sizeof(char)*100);
    k=(char*)malloc(sizeof(char)*100);
    char delim[]=" ";
    char** data_inputs;
    data_inputs=(char**)malloc(sizeof(char*)*8);
    for(j=0;j<8;j++){
        data_inputs[j]=NULL;
    }

//diabazei ta imputs kai ta xexorizei

//    printf("Insert Commands: ");
        is=  scanf("%[^\n]%*c", ptr);

        if(is!=1){
            printf("error with comand \n");
        }else {


            strcpy(k, ptr);
            for (j = 0; j < strlen(ptr); j++) {
                if (ptr[j] == 32) {
                    got_space = 1;
                    break;
                }


            }

            j = 0;
            if (got_space == 0) {
                data_inputs[0] = (char *) malloc(sizeof(char) * (strlen(ptr) + 1));
                strcpy(data_inputs[0], ptr);
            } else {
                test = strtok(ptr, delim);
                data_inputs[0] = (char *) malloc(sizeof(char) * (strlen(test) + 1));
                strcpy(data_inputs[0], test);
                test = strtok(NULL, delim);
                while (test != NULL) {
                    j++;


                    data_inputs[j] = (char *) malloc(sizeof(char) * (strlen(test) + 1));
                    strcpy(data_inputs[j], test);
                    test = strtok(NULL, delim);
                }
            }


//kalei ola ta alla
//printf("%s\n",ptr);
            switch (ptr[1]) {
                case 101:
                    if (strcmp(data_inputs[0], "/exit") == 0) {

                        (*exit) = 1;
                        exit_parent(children, N, fd_matrix, stats,pass,fail);

                        break;
                    }
                case 108:
                    if (strcmp(data_inputs[0], "/listCountries") == 0) {
                        list_countries(children, N);
                        (*pass)++;
                        break;
                    }

                    if (strcmp(data_inputs[0], "/lnewstuff") == 0) {

                        kill(children[0].pid, SIGUSR1);
                        break;
                    }

                    if (strcmp(data_inputs[0], "/lkill") == 0) {

                        kill(children[0].pid, SIGKILL);
                        break;
                    }


                case 100:
                    if (strcmp(data_inputs[0], "/diseaseFrequency") == 0) {
                        disease_frequency(data_inputs[1], data_inputs[2], data_inputs[3], data_inputs[4], stats);
                        (*pass)++;
                        break;
                    }
                case 116:
                    if (strcmp(data_inputs[0], "/topk-AgeRanges") == 0) {
                        topk(data_inputs[1], data_inputs[2], data_inputs[3], data_inputs[4], data_inputs[5], stats);
                        (*pass)++;
                        break;
                    }
                case 115:
                    if (strcmp(data_inputs[0], "/searchPatientRecord") == 0) {
                        recordid_parent(data_inputs[1], children, N, fd_matrix, buffer,pass,fail);
                       // (*pass)++;
                        break;
                    }
                case 110:
                    if (strcmp(data_inputs[0], "/numPatientAdmissions") == 0) {
                        num_patient_admissions(data_inputs[1], data_inputs[2], data_inputs[3], data_inputs[4], stats);
                      //  (*pass)++;
                        break;
                    }

                    if (strcmp(data_inputs[0], "/numPatientDischarges") == 0) {
                        patient_discharges_parent(data_inputs[1], data_inputs[2], data_inputs[3], data_inputs[4],
                                                  children, buffer, N, fd_matrix);
                        (*pass)++;

                        break;
                    }


                default:
                    printf("wrong command \n");
                    (*fail)++;
            }

            for (i = 0; i <= j; i++) {
                free(data_inputs[i]);
                data_inputs[i] = NULL;
            }


            free(data_inputs);
            free(ptr);
            free(k);
        }

}

void patient_discharges_parent(char* disease,char* date1,char* date2,char* country,childthings* children,int buffer,int N,int** fd_matrix) {
    int i, j, c_num, fd;
    int writerfd, readerfd;
    int brk = 0;
    int retur;
    printf("got in discharge\n");
    if (disease == NULL || date1 == NULL || date2 == NULL) {
        printf("wrong arguments for discharge\n");
    } else {
    if (country != NULL) {
        for (i = 0; i < N; i++) {
            c_num = children[i].num_of_countries;
            for (j = 0; j < c_num; j++) {
                if (strcmp(children[i].countries[j], country) == 0) {
                    writerfd = children[i].fdwriter;
                    readerfd = children[i].fdreader;
                   // kill(children[i].pid, 32);
                   write_fifo(writerfd, "discharge", buffer);
                    write_fifo(writerfd, disease, buffer);
                    write_fifo(writerfd, date1, buffer);
                    write_fifo(writerfd, date2, buffer);
                    write_fifo(writerfd, country, buffer);
                    brk = 1;
                    break;
                }

            }
            if (brk == 1)
                break;
        }


    } else {
        printf("signaling children\n");
        for (i = 0; i < N; i++) {

          //  kill(children[i].pid, 32);
            fd = fd_matrix[1][i];
            if (fd < 0) {
                printf("parrent fd:%d writer error", fd);
                perror("open");
            }
            write_fifo(fd, "discharge", buffer);
            write_fifo(fd, disease, buffer);
            write_fifo(fd, date1, buffer);
            write_fifo(fd, date2, buffer);
            write_fifo(fd, "nocountry", buffer);
        }

    }


        }
    }



void patient_discharges_child(suma* ola,int fdreaderfrom,int buffer){
    int fd=fdreaderfrom;
    int i,size;
    char* country;
    char* disease;
    char* date1;
    char* date2;
    disease=read_fifo(fd,buffer);
    date1=read_fifo(fd,buffer);
    date2=read_fifo(fd,buffer);
    country=read_fifo(fd,buffer);
numcurrentpatiens(ola->hashcountry,disease,date1,date2,country);
}







void new_data_parent(int fdreader,overlay* stats,int buffer){
char* input;
char* input2;
    date_cases* newdates=NULL;

input=read_fifo(fdreader,buffer);
while(strcmp(input,"done")!=0) {
    while(strcmp(input,"donedate")!=0) {
        input2=malloc(strlen(input)+1);
        strcpy(input2,input);
        newdates=char_to_str2(input, stats, newdates);

        input = read_fifo(fdreader, buffer);

    }

    char_to_str3( input2, stats,newdates);
    input = read_fifo(fdreader, buffer);
}


}


date_cases* char_to_str2(char* ptr,overlay* stats,date_cases* newdates){
    int i,j,k,l;
    int found=0;
    initial_transfer* in;
    in=malloc(sizeof(initial_transfer));
    char* country;
    char* date;
    char delim[3]="||";
    char* test;
    int day_diseases=0;

    date_cases* last_date=malloc(sizeof(date_cases));
    test = strtok(ptr, delim);
    if(test!=NULL) {
        country = malloc((strlen(test) + 1)*sizeof(char));
        strcpy(country, test);
    }
    test = strtok(NULL, delim);
    if(test!=NULL) {
        date = malloc((strlen(test) + 1)*sizeof(char));
        strcpy(date, test);
    }

    test = strtok(NULL, delim);
    if(test!=NULL) {
        day_diseases=atoi(test);
    }

    test = strtok(NULL, delim);
    if(test!=NULL) {
        in->virus = malloc((strlen(test) + 1)*sizeof(char));
        strcpy(in->virus, test);
    }

    test = strtok(NULL, delim);
    if(test!=NULL) {
        in->catA=atoi(test);
    }
    test = strtok(NULL, delim);
    if(test!=NULL) {
        in->catB=atoi(test);
    }
    test = strtok(NULL, delim);
    if(test!=NULL) {
        in->catC=atoi(test);
    }
    test = strtok(NULL, delim);
    if(test!=NULL) {
        in->catD=atoi(test);
    }
if(newdates==NULL) {
    newdates = malloc(sizeof(date_cases));
    newdates->day_disease = day_diseases;
    newdates->date = date;
    newdates->in = malloc(sizeof(initial_transfer) * day_diseases);
    for (i = 0; i < day_diseases; i++) {
        newdates->in[i] = NULL;
    }
}
    for(i=0;i<day_diseases;i++){
        if(newdates->in[i]==NULL){
            newdates->in[i]=in;
            break;
        }

    }









    free(ptr);
return newdates;
}


void char_to_str3(char* ptr,overlay* stats,date_cases* newdates){
    int i,j,k,l;
    initial_transfer* in;
    in=malloc(sizeof(initial_transfer));
    char* country;
    char* date;
    char delim[3]="||";
    char* test;
    int day_diseases=0;
    test = strtok(ptr, delim);
    if(test!=NULL) {
        country = malloc((strlen(test) + 1)*sizeof(char));
        strcpy(country, test);
    }
    test = strtok(NULL, delim);
    if(test!=NULL) {
        date = malloc((strlen(test) + 1)*sizeof(char));
        strcpy(date, test);
    }

    test = strtok(NULL, delim);
    if(test!=NULL) {
        day_diseases=atoi(test);
    }

    test = strtok(NULL, delim);
    if(test!=NULL) {
        in->virus = malloc((strlen(test) + 1)*sizeof(char));
        strcpy(in->virus, test);
    }

    test = strtok(NULL, delim);
    if(test!=NULL) {
        in->catA=atoi(test);
    }
    test = strtok(NULL, delim);
    if(test!=NULL) {
        in->catB=atoi(test);
    }
    test = strtok(NULL, delim);
    if(test!=NULL) {
        in->catC=atoi(test);
    }
    test = strtok(NULL, delim);
    if(test!=NULL) {
        in->catD=atoi(test);
    }

    int brk=0;
    if(strcmp(country,"CROATIA")==0) {
        if(strcmp(date,"29-2-2013")==0){
            int ll = 0;
        }

    }
    date_cases** newarray;
    for(i=0;i<stats->num_of_countries;i++){
        if(brk==1){
            break;
        }
        k=0;
        if(strcmp(stats->stat[i]->country,country)==0){
            stats->stat[i]->num_of_dates++;
            newarray=malloc(sizeof(date_cases*)*stats->stat[i]->num_of_dates);
            l=0;
            for(j=0;j<stats->stat[i]->num_of_dates;j++) {
                if(k==0){
                   if( check_date(stats->stat[i]->dates[l]->date,date)==1){
                       newarray[j]=stats->stat[i]->dates[l];
                       l++;
                    }else{
                       newarray[j]=newdates;
                       k=1;
                   }

                }else{
                newarray[j]=stats->stat[i]->dates[l];
                l++;}

            }
            free(stats->stat[i]->dates);
            stats->stat[i]->dates=newarray;





        }
    }




    free(ptr);

}


void exit_child(char** carray,int succses,int fail,int num_of_countries){
    int total=succses+fail;
    char* tot=malloc(9);
    char* succ=malloc(9);
    char* fai=malloc(9);
    sprintf(tot,"%d",total);
    sprintf(succ,"%d",succses);
    sprintf(fai,"%d",fail);
    FILE * logfile=NULL;
    char* id=malloc(9);
    sprintf(id,"%d",getpid());
    char* filename=malloc(strlen("logfile.")+strlen(id)+1);
    sprintf(filename,"logfile.%s",id);
    logfile=fopen(filename,"w");
    int i;

    for(i=0;i<num_of_countries;i++){
        fputs(carray[i],logfile);
        fputs("\n", logfile);

    }
    fputs("TOTAL ",logfile);
    fputs(tot,logfile);
    fputs("SUCCESS ",logfile);
    fputs(succ,logfile);
    fputs("FAIL ",logfile);
    fputs(fai,logfile);


}


void exit_parent_file(overlay* stats,int* succses,int* fail){


    int total=(*succses)+(*fail);
    char* tot=malloc(9);
    char* succ=malloc(9);
    char* fai=malloc(9);
    sprintf(tot,"%d",total);
    sprintf(succ,"%d",(*succses));
    sprintf(fai,"%d",(*fail));
    FILE * logfile=NULL;
    char* id=malloc(9);
    sprintf(id,"%d",getpid());
    char* filename=malloc(strlen("logfile.")+strlen(id)+1);
    sprintf(filename,"logfile.%s",id);
    logfile=fopen(filename,"w");
    int i;

    for(i=0;i<stats->num_of_countries;i++){
        fputs(stats->stat[i]->country,logfile);
        fputs("\n", logfile);

    }
    fputs("TOTAL ",logfile);
    fputs(tot,logfile);
    fputs("\n", logfile);
    fputs("SUCCESS ",logfile);
    fputs(succ,logfile);
    fputs("\n", logfile);
    fputs("FAIL ",logfile);
    fputs(fai,logfile);
    fputs("\n", logfile);

    free(tot);
    free(fai);
    free(succ);
    free(id);
    free(filename);

}


