////////////////////////////////////////////////////////////////////////////////
// GOZDE DOGAN
//  131044019
//  
//  server.c
//  ./server <port id> <thpool size>
//
//  Description:
//
//
//
//  References:
//  1. semaphore
//      http://www.amparo.net/ce155/sem-ex.html
//  2. semaphore with thread
//      https://stackoverflow.com/questions/15182328/semaphore-implementation
//      https://www.daniweb.com/programming/software-development/threads/330806/semaphores-in-c
//  3. socket
//      http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
//  4. mutex
//      http://www.thegeekstuff.com/2012/05/c-mutex-examples/?refcom
//  5. shared memory
//      https://stackoverflow.com/questions/30199805/c-programming-shmat-permission-denied
//      https://stackoverflow.com/questions/7237540/how-to-use-shared-memory-to-communicate-between-two-processes
//      https://users.cs.cf.ac.uk/Dave.Marshall/C/node27.html
//      https://stackoverflow.com/questions/5656530/how-to-use-shared-memory-with-linux-in-c
//
//
////////////////////////////////////////////////////////////////////////////////


#include "server.h"


int main(int argc, char *argv[]){
   /* long timedif;
    struct timeval tpend;
    struct timeval tpstart;
    
    if (gettimeofday(&tpstart,NULL))
	perror("\nError 0x002 ");*/

    if(argc != 3){
        printf("Usage >>\n");
        printf("./server <port id> <thpool size>\n");
        exit(1);
    }
      
	iPortNumber = (u_port_t) atoi(argv[1]);
    printf("\n\niPortNumber : %d\n", (int)iPortNumber);
    printf("SERVER ID: %d\n", (int) getpid());
    
    //Create a socket for the soclet
    //If connfd<0 there was an error in the creation of the socket
    if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
        perror("Problem in creating the socket");
        exit(2);
    }
    
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(iPortNumber);
    
    //bind the socket
    bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    //listen to the socket by creating a connection queue, then wait for clients
    listen (listenfd, LISTENQ);

    //unlink(fifoName);
    fifo = mkfifo(fifoName, (S_IRUSR | S_IWUSR));
    if(fifo == -1){
        perror("mkfifo");
        exit(1);
    }
    
    for ( ; ; ) {
        fifo = open(fifoName, O_RDWR);
        if(fifo == -1){
            perror("open fifo for write");
        }
        
        signal(SIGINT, sigHandle);
        
        clilen = sizeof(cliaddr);
        //accept a connection
       // connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);
        connfd=accept(listenfd,(struct sockaddr*)&cliaddr,&clilen);	
        if(connfd==-1){
            perror("server accept");	
            continue;
        }
        
        printf("\n\nwaited request...\n\n");
        
        // client idsi ve pathi alindi
        r_read(connfd, &cArgs, sizeof(cArgs));
        
        //ClientID lerini kaydettim, CTRL-C komutu geldiginde clientlari yok etmek icin
        clientPidsArr[iPidsSize] = (int)(cArgs.id);
        strcpy(clientsPath[iPidsSize], cArgs.clientPath);
        (iPidsSize)++;
        
        //printf("iPidSize:%d\n", iPidsSize);
        
        // matrisler icin gerekli row, col ve client sayisi alindi
        r_read(connfd, &myArgs, sizeof(myArgs));
        printf("\ncol:%d\trow:%d\tnumOfClients:%d\n\n", myArgs.col, myArgs.row, myArgs.numOfClients);
    
    
        r_read(connfd, &threadID, sizeof(threadID));
        
        
        int indexP = 0;
        int index = 0;
        int i, j;
        pid_t childPid1, childPid2, childPid3;
        char log[SIZE];
        
        for(indexP = 0; indexP < myArgs.numOfClients; indexP++){
            printf("operation for thread\n\n");
        
            sprintf(log, "s_c_%d_%u.log", cArgs.id, threadID);
            
            childPid1 = fork();
            childPid2 = fork();
            childPid3 = fork();
            
            /////////////////////////////// P1 /////////////////////////////////
                
            if(childPid1 == -1 ){
                perror("Error:");
                exit(1);
            }
            else if(childPid1 == 0){
                //wait(&childPid2);
                printf("in p3\n");
                if ((shmid2_3 = shmget(KEY2_3, sizeof(ShMemData2_3), 0644 | IPC_CREAT)) == -1) {
                    perror("shmget");
                    exit(1);
                }
                
                data2_3 = (ShMemData2_3*)shmat(shmid2_3, NULL, 0);
                if(data2_3 == (ShMemData2_3*) -1){
                    perror("error shmat");
                    exit(1);
                }
                
                /////////////////////// data received //////////////////////////
                for(i=0; i<myArgs.row; i++)
                    for(j=0; j<myArgs.col; j++)
                        A[i][j] = data2_3->a_b.A[i][j];
                        
                for(i=0; i<myArgs.row; i++)
                    B[i] = data2_3->a_b.B[i];
                
                for(i=0; i<myArgs.col; i++)
                    x1[i] = data2_3->x1[i];
                    
                for(i=0; i<myArgs.col; i++)
                    x2[i] = data2_3->x2[i];
                    
                for(i=0; i<myArgs.col; i++)
                    x3[i] = data2_3->x3[i];
                ////////////////////////////////////////////////////////////////
                
                shmdt(data2_3);
                
                P3(log, threadID);
                
            }
            else{
                
            }
            ////////////////////////////////////////////////////////////////////
            
            //////////////////////////////// P2 ////////////////////////////////
            
                
            if(childPid2 == -1 ){
                perror("Error:");
                exit(1);
            }
            else if(childPid2 == 0){
                printf("in p2\n");
                if ((shmid1_2 = shmget(KEY1_2, sizeof(ShMemData1_2), 0644 | IPC_CREAT)) == -1) {
                    perror("shmget:");
                    exit(1);
                }
                
                data1_2 = ( ShMemData1_2 * ) shmat ( shmid1_2, NULL, 0 );

                if ( data1_2 == ( ShMemData1_2 * )-1 ) {
                    perror ( "shmat:" );
                    exit(1);
                }
                
                /////////////// data alma islemi -> received ///////////////////
                for(i=0; i<myArgs.row; i++)
                    for(j=0; j<myArgs.col; j++)
                        A[i][j] = data1_2->A[i][j];
                        
                for(i=0; i<myArgs.row; i++)
                    B[i] = data1_2->B[i];
                ////////////////////////////////////////////////////////////////
                
                shmdt(data1_2);
                
                P2(log, threadID);
            }
            else{
                
            }
            ////////////////////////////////////////////////////////////////////
            
            //////////////////////////////// P3 ////////////////////////////////
            
                
            if(childPid3 == -1 ){
                perror("Error:");
                exit(1);
            }
            else if(childPid3 == 0){
                printf("in p1\n");
                P1(log, threadID);
            }
            else{
            
            }
            ////////////////////////////////////////////////////////////////////
            
            wait(&childPid1);
            wait(&childPid2);
            wait(&childPid3);
                    
            writeToFileMatrices(log);
            write_socket();
              
        }
        //close socket of the server
        close(connfd);
        close(fifo);  
        
        printf("\n\nwaited request...\n");     
    }

    unlink(fifoName);
   /* if (gettimeofday(&tpend,NULL))
	perror("\nError 0x003 ");

    timedif = MILLION*(tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec;

    printf("Total run time, in miliseconds : %ld",timedif);*/

    return 0;
}


void P1(char fileLog[SIZE], unsigned int threadID){

    generateMatrix();
    
    if ((shmid1_2 = shmget(KEY1_2, sizeof(ShMemData1_2), 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }
    
    data1_2 = ( ShMemData1_2 * ) shmat ( shmid1_2, NULL, 0 );

    if ( data1_2 == ( ShMemData1_2 * )-1 ) {
        perror ( "Error in shmat\n" );
        exit(1);
    }
    //printf("fileLog: %s\n", fileLog);
    FILE *fPtrLog = fopen(fileLog, "w+");
    
    int i=0, j=0;
    
    /////////////////// write log file and data transmition ////////////////////
    /*fprintf(fPtrLog, "\n\nCLIENTID: %d\nTHREADID: %u>>>>>>>>>>>\n", cArgs.id, threadID);
    fprintf(fPtrLog, "Matrix A:\n");
    printf("Matrix A:\n");*/
    for(i=0; i<myArgs.row; i++){
        for(j=0; j<myArgs.col; j++){
            /*fprintf(fPtrLog, "%.4f ", A[i][j]);
            printf("%.4f ", A[i][j]);*/
            data1_2->A[i][j] = A[i][j];
        }
        /*fprintf(fPtrLog, "\n");
        printf("\n");*/
    }
    /*fprintf(fPtrLog, "\n\n");
    printf("\n\n");*/
    
    /*fprintf(fPtrLog, "Matrix B:\n");
    printf("Matrix B:\n");*/
    for(i=0; i<myArgs.row; i++){
        /*fprintf(fPtrLog, "%.4f\n", B[i]);
        printf("%.4f\n", B[i]);*/
        data1_2->B[i] = B[i];
    }
  /*  
    fprintf(fPtrLog, "\n\n");
    printf("\n\n");

    fclose(fPtrLog);  */
    ////////////////////////////////////////////////////////////////////////////
    
    shmdt(data1_2); // detached
    
    shmctl(shmid1_2, IPC_RMID, 0); // destroyed
}

void P2(char fileLog[SIZE], unsigned int threadID){
    pthread_mutex_init(&mutex, NULL);
    
    int i,j;
    
    if ((shmid2_3 = shmget(KEY2_3, sizeof(ShMemData2_3), 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }
    
    data2_3 = (ShMemData2_3*)shmat(shmid2_3, NULL, 0);
    if(data2_3 == (ShMemData2_3*) -1){
        perror("error shmat");
        exit(1);
    }
    
    for(i=0; i<myArgs.row; i++)
        for(j=0; j<myArgs.col; j++)
            data2_3->a_b.A[i][j] = A[i][j];
    
    for(i=0; i<myArgs.row; i++)
        data2_3->a_b.B[i] = B[i];
    
    FILE *fPtrLog = fopen(fileLog, "w+");
    
    for(i=0; i<3; i++){
        mArg[i].type = i+1;
        mArg[i].fPtr = fPtrLog;
        
        pthread_create(&p2Calculate[i], NULL, calculateX, (void*)&mArg[i]);       
    }  
    
    shmdt(data2_3);
    shmctl(shmid2_3, IPC_RMID, 0);
    
    fclose(fPtrLog);
    
    for(i=0; i<3; i++)
        pthread_join(p2Calculate[i], NULL);
    
    pthread_mutex_destroy(&mutex);
}

void* calculateX(void *args){ 
    pthread_mutex_lock(&mutex);

    MutexArg *current;
    current = (MutexArg*)args;
    
    int i,j;
    
    if(current->type == 1){
        t[0] = calculation1();
        ////////////////////////// data transmition ////////////////////////////
        ////////// write log file //////////
        /*fprintf(current->fPtr, "Matrix X1:\n");
        printf("Matrix X1:\n");*/
        for(i=0; i<myArgs.col; i++){
            /*fprintf(current->fPtr, "%.4f ", x1[i]);
            printf("%.4f ", x1[i]);*/
            data2_3->x1[i] = x1[i];
        }
        /*fprintf(current->fPtr, "\ntime:%.4f\n", t[0]);
        printf("\ntime:%.4f\n", t[0]);
        fprintf(current->fPtr, "\n\n");
        printf("\n\n");*/
        ////////////////////////////////////////////////////////////////////////
        
    }else if (current->type == 2){
        t[1] = calculation2();
        ////////////////////////// data transmition ////////////////////////////
        ////////// write log file //////////
        /*fprintf(current->fPtr, "Matrix X2:\n");
        printf("Matrix X2:\n");*/
        for(i=0; i<myArgs.col; i++){
            /*fprintf(current->fPtr, "%.4f ", x2[i]);
            printf("%.4f ", x2[i]);*/
            data2_3->x2[i] = x2[i];
        }
        /*fprintf(current->fPtr, "\ntime:%.4f\n", t[1]);
        printf("\ntime:%.4f\n", t[1]);
        fprintf(current->fPtr, "\n\n");
        printf("\n\n");*/
        ////////////////////////////////////////////////////////////////////////
    }else if(current->type == 3){  
        t[2] = calculation3();
        ////////////////////////// data transmition ////////////////////////////
        ////////// write log file //////////
        //fprintf(current->fPtr, "Matrix X3:\n");
        //printf("Matrix X3:\n");
        for(i=0; i<myArgs.col; i++){
          //  fprintf(current->fPtr, "%.4f ", x3[i]);
            //printf("%.4f ", x3[i]);
            data2_3->x3[i] = x3[i];
        }
        //fprintf(current->fPtr, "\ntime:%.4f\n", t[2]);
        //printf("\ntime:%.4f\n", t[2]); 
        //fprintf(current->fPtr, "\n\n");
        //printf("\n\n");
        ////////////////////////////////////////////////////////////////////////
    }
    
    /*tAverage = (t[0] + t[1] + t[2])/3.0;
    r_write(connfd, &tAverage, sizeof(tAverage));*/

    /*fprintf(current->fPtr, "tAverage: %.4f\n\n", tAverage);
    printf("tAverage: %.4f\n\n", tAverage);*/
    
    pthread_mutex_unlock(&mutex);
    
    pthread_exit(NULL); 
}

void P3(char fileLog[SIZE], unsigned int threadID){

    e[0] = calculationError1();
    e[1] = calculationError2();
    e[2] = calculationError3();
    
   /* FILE *fPtrLog = fopen(fileLog, "w+");
    
    //fprintf(fPtrLog, "CLIENTID: %d\nTHREADID: %u>>>>>>>>>>>\n", cArgs.id, threadID);
    /*fprintf(fPtrLog, "ERRORS:\n");
    fprintf(fPtrLog, "error1: %.4f\n\n", e1);
    fprintf(fPtrLog, "error2: %.4f\n\n", e2);
    fprintf(fPtrLog, "error3: %.4f\n\n", e3);
    
    printf("ERRORS:\n");
    printf("error1: %.4f\n\n", e1);
    printf("error2: %.4f\n\n", e2);
    printf("error3: %.4f\n\n", e3);

    fclose(fPtrLog); */
    
}


void generateMatrix(){
    int i=0, j=0;
    
    srand(time(NULL));
    
    for(i=0; i<myArgs.row; i++){
        for(j=0; j<myArgs.col; j++)
            A[i][j] = rand()%10;
    }
    
    for(i=0; i<myArgs.row; i++)
        B[i] = rand()%10;
      
    write(fifo, &A, sizeof(A));
    write(fifo, &B, sizeof(B));   
    /*r_write(connfd, &A, sizeof(A));
    r_write(connfd, &B, sizeof(B));*/
}

double calculation1(){
    
    double t;
    
    if( clock_gettime( CLOCK_REALTIME, &n_start) == -1 ) {
      perror( "clock gettime" );
      exit(1);
    }

    //calculation(A);
    int i, j;
    for(i=0; i<myArgs.col; i++)
        x1[i] = 7;
    
    
    if( clock_gettime( CLOCK_REALTIME, &n_stop) == -1 ) {
      perror( "clock gettime" );
      exit(1);
    }
    
    //islemin gerceklesme suresi hesaplandi
    t = ((n_stop.tv_sec - n_start.tv_sec) *1000 / CLOCKS_PER_SEC) + ( n_stop.tv_nsec - n_start.tv_nsec );
    
    write(fifo, &x1, sizeof(x1));
    write(fifo, &t, sizeof(t));
    
   /* r_write(connfd, &x1, sizeof(x1));
    r_write(connfd, &t, sizeof(t));*/
    return t;
}

double calculation2(){

    double t;
    
    if( clock_gettime( CLOCK_REALTIME, &n_start) == -1 ) {
      perror( "clock gettime" );
      exit(1);
    }
    
    //calculation(A);
    int i, j;
    for(i=0; i<myArgs.col; i++)
        x2[i] = 3;
    
    
    if( clock_gettime( CLOCK_REALTIME, &n_stop) == -1 ) {
      perror( "clock gettime" );
      exit(1);
    }
    
    //islemin gerceklesme suresi hesaplandi
    t = ((n_stop.tv_sec - n_start.tv_sec) *1000 / CLOCKS_PER_SEC) + ( n_stop.tv_nsec - n_start.tv_nsec );
    
    write(fifo, &x2, sizeof(x2));
    write(fifo, &t, sizeof(t));
    
    /*r_write(connfd, &x2, sizeof(x2));
    r_write(connfd, &t, sizeof(t));*/
    
    return t;
}

double calculation3(){

    double t;
    
    if( clock_gettime( CLOCK_REALTIME, &n_start) == -1 ) {
      perror( "clock gettime" );
      exit(1);
    }
    
    //calculation(A);
    int i, j;
    for(i=0; i<myArgs.col; i++)
        x3[i] = 5;
    
    if( clock_gettime( CLOCK_REALTIME, &n_stop) == -1 ) {
      perror( "clock gettime" );
      exit(1);
    }
    
    //islemin gerceklesme suresi hesaplandi
    t = ((n_stop.tv_sec - n_start.tv_sec) *1000 / CLOCKS_PER_SEC) + ( n_stop.tv_nsec - n_start.tv_nsec );
    
    write(fifo, &x3, sizeof(x3));
    write(fifo, &t, sizeof(t));
    
    /*r_write(connfd, &x3, sizeof(x3));
    r_write(connfd, &t, sizeof(t));*/
    
    return t;
}


double calculationError1(){
    double error = calculationError(A, B, x1);
    
    write(fifo, &error, sizeof(error));
    
    //r_write(connfd, &error, sizeof(error));
    
    return error;
}

double calculationError2(){
    double error = calculationError(A, B, x2);
    
    write(fifo, &error, sizeof(error));
    //r_write(connfd, &error, sizeof(error));
    
    return error;
}

double calculationError3(){
    double error = calculationError(A, B, x3);
    
    write(fifo, &error, sizeof(error));
    
    //r_write(connfd, &error, sizeof(error));
    
    return error;
}

double calculationError(double a[MAXSIZE][MAXSIZE], double b[MAXSIZE], double x[MAXSIZE]){
    double mult[MAXSIZE][1];
    double result[MAXSIZE][1];
    double transpose[1][MAXSIZE];
    int sum = 0;
    int i=0, j=0, k=0;

    for(i=0; i<myArgs.row; i++)
        mult[i][0] = 0;

    // a*x    
    for(i=0; i<myArgs.row; i++){ 
        sum = 0;
        k = 0;
            for(j=0; j<myArgs.col; j++){ 
                sum=sum+a[i][j]*x[k];
                if(k<myArgs.col)
                    k++;
                else
                    j = myArgs.col;
            }
        
        mult[i][0] = sum;
    }
        
    for(i=0; i<myArgs.row; i++)
        result[i][0] = mult[i][0] - b[i];
        
    for(i=0; i<myArgs.row; ++i)
        for(j=0; j<1; ++j)
            transpose[j][i] = result[i][j];
    
    sum = 0;
    for(i=0; i<myArgs.row; i++)
        sum = sum + result[i][0]*transpose[0][i];
                
    
    return sqrt(sum);  
}


void writeToFileMatrices(char fileLog[SIZE]){

    read(fifo, &A, sizeof(A));
    read(fifo, &B, sizeof(B));
    
    read(fifo, &x1, sizeof(x1));
    read(fifo, &t[0], sizeof(t[0]));
    read(fifo, &x2, sizeof(x2));
    read(fifo, &t[1], sizeof(t[1]));
    read(fifo, &x3, sizeof(x3));
    read(fifo, &t[2], sizeof(t[2]));
    
    read(fifo, &e[0], sizeof(e[0]));    
    read(fifo, &e[1], sizeof(e[1]));    
    read(fifo, &e[2], sizeof(e[2]));

    close(fifo);

    int i=0, j=0;
    
    FILE *fPtr = fopen(fileLog, "w+");
    
    fprintf(fPtr, "\n\nCLIENTID: %d\nTHREADID: %u>>>>>>>>>>>\n", cArgs.id, threadID);
    
    fprintf(fPtr, "Matrix A:\n");
    for(i=0; i<myArgs.row; i++){
        for(j=0; j<myArgs.col; j++){
            fprintf(fPtr, "%.4f ", A[i][j]);
        }
        fprintf(fPtr, "\n");
    }
    fprintf(fPtr, "\n\n");
    
    fprintf(fPtr, "Matrix B:\n");
    for(i=0; i<myArgs.row; i++)
        fprintf(fPtr, "%.4f\n", B[i]);
    fprintf(fPtr, "\n\n");
        
    fprintf(fPtr, "Matrix X1:\n");
    for(i=0; i<myArgs.col; i++)
        fprintf(fPtr, "%.4f ", x1[i]);
    fprintf(fPtr, "\n");
    fprintf(fPtr, "time for x1: %.4f\n\n", t[0]);
        
    fprintf(fPtr, "Matrix X2:\n");
    for(i=0; i<myArgs.col; i++)
        fprintf(fPtr, "%.4f ", x2[i]);
    fprintf(fPtr, "\n");
    fprintf(fPtr, "time for x2: %.4f\n\n", t[1]);
        
    fprintf(fPtr, "Matrix X3:\n");
    for(i=0; i<myArgs.col; i++)
        fprintf(fPtr, "%.4f ", x3[i]); 
    fprintf(fPtr, "\n");
    fprintf(fPtr, "time for x3: %.4f\n\n", t[2]);
    
    tAverage = (t[0] + t[1] + t[2])/3.0;
    fprintf(fPtr, "\n\ntimeAverage: %.4f\n\n", tAverage);
    
    fprintf(fPtr, "e1: %.4f\n", e[0]);   
    fprintf(fPtr, "e2: %.4f\n", e[1]);
    fprintf(fPtr, "e2: %.4f\n", e[2]);
    
    fclose(fPtr);
    
        printf(  "\n\nCLIENTID: %d\nTHREADID: %u>>>>>>>>>>>\n", cArgs.id, threadID);
    
    printf("Matrix A:\n");
    for(i=0; i<myArgs.row; i++){
        for(j=0; j<myArgs.col; j++){
            printf("%.4f ", A[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
    
    printf("Matrix B:\n");
    for(i=0; i<myArgs.row; i++)
        printf("%.4f\n", B[i]);
    printf("\n\n");
        
    printf("Matrix X1:\n");
    for(i=0; i<myArgs.col; i++)
        printf("%.4f ", x1[i]);
    printf("\n");
    printf("time for x1: %.4f\n\n", t[0]);
        
    printf("Matrix X2:\n");
    for(i=0; i<myArgs.col; i++)
        printf("%.4f ", x2[i]);
    printf("\n");
    printf("time for x2: %.4f\n\n", t[1]);
        
    printf("Matrix X3:\n");
    for(i=0; i<myArgs.col; i++)
        printf("%.4f ", x3[i]); 
    printf("\n");
    printf("time for x3: %.4f\n\n", t[2]);
    
    printf("\n\ntimeAverage: %.4f\n\n", tAverage);
    
    printf("e1: %.4f\n", e[0]);   
    printf("e2: %.4f\n", e[1]);
    printf("e2: %.4f\n", e[2]);
    
    
}

void write_socket(){
    r_write(connfd, &A, sizeof(A));
    r_write(connfd, &B, sizeof(B));
    
    r_write(connfd, &x1, sizeof(x1));
    r_write(connfd, &t[0], sizeof(t[0]));
    
    r_write(connfd, &x2, sizeof(x2));
    r_write(connfd, &t[1], sizeof(t[1]));
    
    r_write(connfd, &x3, sizeof(x3));
    r_write(connfd, &t[2], sizeof(t[2]));
    
    tAverage = (t[0] + t[1] + t[2])/3.0;
    r_write(connfd, &tAverage, sizeof(tAverage));
    
    r_write(connfd, &e[0], sizeof(e[0]));
    r_write(connfd, &e[1], sizeof(e[1]));
    r_write(connfd, &e[2], sizeof(e[2]));
    
    int OKEY = 1;
    
    r_write(connfd, &OKEY, sizeof(OKEY));
}

void sigHandle(int sig)
{
    int i=0;
	printf("\n*** Server killed ***\n");
	
	for (i = 0; i <iPidsSize; ++i)
    {
        if(clientPidsArr[i]>-1)
            kill(clientPidsArr[i], SIGINT);    
    }
    kill((long)getpid(), SIGINT);
    
    close(fifo);
    unlink(fifoName);
    
	printf("CTRL-C sinyali geldi\n2 saniye bekleyip kapanacak!\n");
	sleep(2);
	exit(sig);
}
