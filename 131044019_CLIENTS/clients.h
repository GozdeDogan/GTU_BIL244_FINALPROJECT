////////////////////////////////////////////////////////////////////////////////
// GOZDE DOGAN
//  131044019
//  
//  clients.h
//
//  Description:
//
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////// LIBRARIES ///////////////////////////////////////
#include <stdio.h>
#include <stdlib.h> 
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "restart.h"
#include "uici.h"
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////// MACROS //////////////////////////////////////
#define LISTENQ 20
#define SIZE 1024
#define MAXSIZE 20
#define MAXTHREADSIZE 50
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////// STRUCTS ///////////////////////////////////////

typedef struct {
    int row;
    int col;
    int numOfClients;
} Arguments;

typedef struct {
    char clientPath[SIZE];
    pid_t id;
} ClientsArgs;

typedef struct {
    char logFile[SIZE];
} ThreadArgs;

////////////////////////////////////////////////////////////////////////////////

/////////////////////////// GLOBAL VARIABLES ///////////////////////////////////
int sockfd;

u_port_t iPortNumber;
size_t thpoolSize;
    
int listenfd;
int connfd;

socklen_t clilen;
struct sockaddr_in cliaddr, servaddr;

pid_t clientPidsArr[SIZE]; // her client icin id tutuyor
char clientsPath[SIZE][SIZE]; //her client'in path ini tutuyor
int iPidsSize;

Arguments myArgs;
ClientsArgs cArgs;

double A[MAXSIZE][MAXSIZE];
double B[MAXSIZE];

double x1[MAXSIZE];
double x2[MAXSIZE];
double x3[MAXSIZE];

double t1;
double t2;
double t3;
double tAverage;

double e1;
double e2;
double e3;

int indexTh = 0;
pthread_t threads[MAXTHREADSIZE];
sem_t semp;

ThreadArgs thArgs[MAXTHREADSIZE];

unsigned int threadID;

////////////////////////////////////////////////////////////////////////////////

/////////////////////////// FUNCTION PROTOTYPES ////////////////////////////////

void* threadOperations(void* args);

void writeToFile(FILE *fPtr, unsigned int threadID);

void sigHandle(int sig);
////////////////////////////////////////////////////////////////////////////////
