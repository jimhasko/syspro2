

#include"child.h"





//sem_t mutex;





int main(int argv,char** args) {
    int i, j;
    int file_num = 0;
    char *work = NULL;
    char *buf = NULL;
    char *dir = NULL;
    if (argv != 7) {
        printf("need arguments");
        exit(1);

    }


    for (i = 0; i < argv; i++) {            //checks the arguments
        if (strcmp("-w", args[i]) == 0) {
            work = args[i + 1];
        }
        if (strcmp("-b", args[i]) == 0) {
            buf = args[i + 1];
        }
        if (strcmp("-i", args[i]) == 0) {
            dir = args[i + 1];
        }


    }



  //  sem_init(&mutex, 0, 1);
    signal(SIGUSR1, signalHandler);
    signal(SIGCHLD, signalkid);
    signal(SIGINT, exitsignal);
    signal(SIGQUIT, exitsignal);
    int N=atoi(work);

    int buffer=atoi(buf);
    char input[buffer];

    DIR *drr = opendir(dir);


    struct dirent *dee;

    if (drr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory\n");
            exit(1);
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



    // char* input;
    overlay* stats=NULL;
 //   fd_set rfds;
  //  struct timeval tv;

    childthings* children=read_files(dir,N);

   // int maxfd_reader=0;
   // int maxfd_writer=0;
   // int fd_matrix[2][N];



    for(i=0;i<N;i++){
        mkfifo( children[i].reader,0666);

        mkfifo( children[i].writer,0666);

    }

    pid_t pid;
    //////////////////////////////////////


//child_init_add("input_dir","GREECE",1,MAX_BUF);


















    //////////////////////////////////



    for(i=0;i<N;i++) {
        pid = fork();
        if(pid<0){
            printf("fork error\n");
        }else if(pid==0){
            // printf("got into child  \n");
            break;
        }else{
            children[i].pid=pid;
        }

    }



/////////////////////child
    if (pid == 0) {


//child_call(i,readfromchild[i]);
        child_big_call(i,children[i].reader,children[i].writer,&children[i], dir,buffer,1);


    } else {
    //////////////////////////parent

parent_big_call(N,dir,children,buffer);





    }

    for(i=0;i<N;i++){


        unlink( children[i].reader);
        //perror("mkfifo");
       unlink( children[i].writer);
        // perror("mkfifo");
    }

    for(i=0;i<N;i++){

        for(j=0;j<children[i].num_of_countries;j++) {
            free(children[i].countries[j]);

        }
        free(children[i].countries);
            free( children[i].reader);
         free( children[i].writer);
    }
       free(children);


    return 0;
}




