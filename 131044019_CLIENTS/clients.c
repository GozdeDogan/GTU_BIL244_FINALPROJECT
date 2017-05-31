////////////////////////////////////////////////////////////////////////////////
// GOZDE DOGAN
//  131044019
//  
//  clients.c
//  ./clients <port number> <m(col)> <p(row)> <q(num of clients)>
//
//  Description:
//
//
////////////////////////////////////////////////////////////////////////////////


#include "clients.h"


int main(int argc, char *argv[]){

    if(argc != 5){
        printf("Usage >>\n");
        printf("./clients <port number> <m(col)> <p(row)> <q(num of clients)>\n");
        exit(1);
    }
    
    iPortNumber = (u_port_t)atoi(argv[1]);
    printf("iPortNumber : %d\n", (int)iPortNumber);
    
    myArgs.col = atoi(argv[2]);
    myArgs.row = atoi(argv[3]);
    myArgs.numOfClients = atoi(argv[4]);
    
    //Create a socket for the client
    //If sockfd<0 there was an error in the creation of the socket
    if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
        perror("Problem in creating the socket");
        exit(2);
    }

    //Creation of the socket
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(iPortNumber);
    //inet_pton(AF_INET, iPAdress ,&servaddr.sin_addr); //iPAdress alinmadi
    
   
   /* inet_pton(AF_INET, Ip, &ipv4addr);
    server = gethostbyaddr(&ipv4addr, sizeof(ipv4addr), AF_INET);
*/
    //Connection of the client to the socket
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))); 

    fprintf(stderr, "[%ld]:connected %s\n", (long)getpid(), argv[1]);
    
    signal(SIGINT, sigHandle);
    
    cArgs.id = getpid();
    if (getcwd(cArgs.clientPath, sizeof(cArgs.clientPath)) == NULL){
       perror("getcwd() error");
       exit(1);
    }

    r_write(sockfd, &cArgs, sizeof(cArgs));
    
    r_write(sockfd, &myArgs, sizeof(myArgs));
    
    indexTh = 0;    
    sem_init(&semp, 0, 1);
    
    while(indexTh < myArgs.numOfClients){
    
        pthread_create(&threads[indexTh], NULL, threadOperations, (void *)(NULL));
        
        indexTh++;
    }

    indexTh = 0;
    while(indexTh < myArgs.numOfClients){
    
        pthread_join(threads[indexTh], NULL);
        indexTh++;
    }

    sem_destroy(&semp);
    return 0;
}


void* threadOperations(void* args){
    sem_wait(&semp);
    
    char log[SIZE];
    unsigned int threadID = pthread_self();
    sprintf(log, "c_%d_%u.log", cArgs.id, threadID);
    
    printf("\n\nCLIENTID: %d\nTHREADID: %u>>>>>>>>>>>\n",cArgs.id, threadID);
    
    r_write(sockfd, &threadID, sizeof(threadID));
    
    int i, j;
    
    r_read(sockfd, &A, sizeof(A)); 
    r_read(sockfd, &B, sizeof(B));
    
    r_read(sockfd, &x1, sizeof(x1));
    r_read(sockfd, &t1, sizeof(t1));
    
    r_read(sockfd, &x2, sizeof(x2));
    r_read(sockfd, &t2, sizeof(t2));
    
    r_read(sockfd, &x3, sizeof(x3));
    r_read(sockfd, &t3, sizeof(t3));
    
    r_read(sockfd, &tAverage, sizeof(tAverage));
    
    r_read(sockfd, &e1, sizeof(e1));
    r_read(sockfd, &e2, sizeof(e2));
    r_read(sockfd, &e3, sizeof(e3));
    
    int OKEY = 1;
    r_read(sockfd, &OKEY, sizeof(OKEY));
    
    //printf("OKEY:%d\n", OKEY);
    
    if(OKEY == 1){
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
          
        printf("time for x1: %.4f\n\n", t1);
            
        printf("Matrix X2:\n");
        for(i=0; i<myArgs.col; i++)
            printf("%.4f ", x2[i]);
        printf("\n");
        
        printf("time for x2: %.4f\n\n", t2);
        
        printf("Matrix X3:\n");
        for(i=0; i<myArgs.col; i++)
            printf("%.4f ", x3[i]); 
        printf("\n");    
        
        printf("time for x3: %.4f\n", t3);
        
        printf("\n\ntimeAverage: %.4f\n\n", tAverage);
        
        printf("e1: %.4f\n", e1);
        printf("e2: %.4f\n", e2);
        printf("e2: %.4f\n", e2);   
        
        FILE *fPtrLog = fopen(log, "w");
        
        writeToFile(fPtrLog, pthread_self());
        
        fclose(fPtrLog);
    }
    else{
        //FILE *fPtrLog = fopen(log, "w");
        
        //fprintf(fPtrLog, "DATA DIDN'T COME FROM SERVER\n");
        //printf("DATA DIDN'T COME FROM SERVER\n");
        
        //fclose(fPtrLog);
    }
        
    sem_post(&semp);
    
    // thread'in islemi bittigi icin thread sonlandirilir.
    pthread_exit(NULL);
}


void writeToFile(FILE *fPtr, unsigned int threadID){
    int i=0, j=0;
    
    fprintf(fPtr, "CLIENTID: %d\nTHREADID: %u>>>>>>>>>>>\n",cArgs.id, threadID);
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
    fprintf(fPtr, "\n\ntime: %.4f\n", t1);
    fprintf(fPtr, "\n\n");
        
    fprintf(fPtr, "Matrix X2:\n");
    for(i=0; i<myArgs.col; i++)
        fprintf(fPtr, "%.4f ", x2[i]);
    fprintf(fPtr, "\n\ntime: %.4f\n", t2);
    fprintf(fPtr, "\n\n");
        
    fprintf(fPtr, "Matrix X3:\n");
    for(i=0; i<myArgs.col; i++)
        fprintf(fPtr, "%.4f ", x3[i]);
    fprintf(fPtr, "\n\ntime: %.4f\n", t3); 
    fprintf(fPtr, "\n\n");    
    
    fprintf(fPtr, "timeAverage: %.4f\n\n", tAverage);
    
    fprintf(fPtr, "ERRORS:\n");
    fprintf(fPtr, "e1: %.4f\n", e1);
    fprintf(fPtr, "e2: %.4f\n", e2);
    fprintf(fPtr, "e2: %.4f\n", e2);
}

void sigHandle(int sig)
{ 
	printf("\n*** Client killed ***\n");
	
	//fprintf(fPtrLogFile,"*** Client killed *** PID:[%ld]\n", (long)getpid());
	//fprintf(fPtrLogFile, "client_pid:%ld\n", (long)getpid());
	/* client e sinyal gelince server da bagli oldugu childi oldurur. */
	
	
	kill((long)getpid(),SIGINT);
	
	printf("CTRL-C sinyali geldi\n2 saniye bekleyip kapanacak!\n");
	sleep(2);
	exit(sig);
}
