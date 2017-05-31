////////////////////////////////////////////////////////////////////////////////
// GOZDE DOGAN
//  131044019
//  
//  server.h
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
#include <math.h>
#include "restart.h"
#include "uici.h"
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////// MACROS //////////////////////////////////////
#define LISTENQ 20
#define SIZE 1024
#define MAXSIZE 20
#define SHMSIZE 1024
#define KEY1_2 456321
#define KEY2_3 789516
#define ERROR -1
#define fifoName "wr_socket_file.txt"
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
	int m;
	int n;
	double ** v;
} mat_t, *mat;

typedef struct {
    int type;
    FILE *fPtr;
} MutexArg;

typedef struct{
    double A[MAXSIZE][MAXSIZE];
    double B[MAXSIZE];
}ShMemData1_2;

typedef struct{
    ShMemData1_2 a_b;
    double x1[MAXSIZE];
    double x2[MAXSIZE];
    double x3[MAXSIZE];
}ShMemData2_3;

////////////////////////////////////////////////////////////////////////////////

/////////////////////////// GLOBAL VARIABLES ///////////////////////////////////

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
double myX[MAXSIZE];

double t[3];
double tAverage;

double e[3];

struct timespec n_start, n_stop; //zaman hesabi icin kullanilan degiskenler

pthread_t p2Calculate[3];
pthread_mutex_t mutex;
MutexArg mArg[3];

int shmid1_2;
ShMemData1_2 *data1_2;

int shmid2_3;
ShMemData2_3 *data2_3;

unsigned int threadID;

int fifo;
//char *fifoName = "wr_socket_file";

////////////////////////////////////////////////////////////////////////////////

/////////////////////////// FUNCTION PROTOTYPES ////////////////////////////////

void P1(char fileLog[SIZE], unsigned int threadID);

void P2(char fileLog[SIZE], unsigned int threadID);

void P3(char fileLog[SIZE], unsigned int threadID);

void generateMatrix();

void* calculateX(void *args);

double calculation1();

double calculation2();

double calculation3();

double calculationError1();
double calculationError2();
double calculationError3();

double calculationError(double a[MAXSIZE][MAXSIZE], double b[MAXSIZE], double x[MAXSIZE]);

void writeToFileMatrices(char fileLog[SIZE]);

void write_socket();

void sigHandle(int sig);

////////////////////////////////////////////////////////////////////////////////
