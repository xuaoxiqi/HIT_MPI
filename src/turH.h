#include <mpi.h>

#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#include <cublas.h>
#include <cufft.h>

#include <hdf5.h>
#include <hdf5_hl.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>



#define NSS 128

typedef struct { float2* x;float2* y;float2* z;} vectorField;

//ONLY FOR NY=NX

static const int N =NSS;
static const int NX=NSS;
static const int NY=NSS;
static const int NZ=NSS/2+1;

static const int THREADSPERBLOCK_IN=16;

static const float REYNOLDS=NSS;

//Global variables

extern cudaError_t RET;

extern int RANK;
extern int SIZE;

extern int NXSIZE;
extern int NYSIZE;
extern int IGLOBAL;


/////////////////////////// C FUNCTIONS /////////////////////////////////////////

//Set up

void setUp(void);
void starSimulation(void);

//RK2

void RK2setup(void);
int RK2step(vectorField u,float* time);

//Random

int randomNumberGen(int T);
void seedGen(void);
void genDelta(float* Delta);


//Fft

void fftSetup(void);
void fftDestroy(void);

void fftForward(float2* buffer_1);
void fftBackward(float2* buffer_1);

void calcUmax(vectorField t,float* ux,float* uy,float* uz);
float sumElements(float2* buffer);

//F

void copyVectorField(vectorField u1,vectorField u2);
void F( vectorField u, vectorField r,float* Delta);

void mpiCheck(int error, const char* function );

//Check

void cudaCheck( cudaError_t error, const char* function);
void mpiCheck( int error, const char* function);

//hit_mpi

void reduceMAX(float* u1,float* u2,float* u3);
void reduceSUM(float* sum,float* sum_all);

int chyzx2xyz(double *y, double *x, int Nx, int Ny, int Nz,
	      int rank, int size);
int chxyz2yzx(double *x, double *y, int Nx, int Ny, int Nz,
	      int rank, int size);
int read_parallel_float(char *filename, float *x, int NX, int NY, int NZ,
			 int rank, int size);
int wrte_parallel_float(char *filename, float *x, int NX, int NY, int NZ,
			 int rank, int size);

//Statistics
void calc_E( vectorField u, float2* t,float* E);
void calc_E( vectorField u, float2* t,float* D);


///////////CUDA FUNCTIONS////////////////////////////////////////////

//RK_kernels
extern void RK_step_1(vectorField uw,vectorField u,vectorField r,float Re,float dt,float Cf,int kf);
extern void RK_step_05(vectorField u,vectorField uw,float Re,float dt,float Cf,int kf);
extern void RK_step_2(vectorField uw,vectorField r,float Re,float dt,float Cf,int kf);

//Dealias

extern void dealias(vectorField t);
extern void projectFourier(vectorField u);
extern void set2zero(float2* u);

//check
void kernelCheck(  cudaError_t error,const char* function , int a);

//Memory
extern void memoryInfo(void);

//Rotor convolution

extern void calc_conv_rotor(vectorField r, vectorField s);


//Shift

extern void shift(vectorField t,float* Delta);

//UW

extern void calc_U_W( vectorField U,vectorField W);


//Statistics

extern void  calc_D( vectorField u, float2* t, float* D);
extern void calc_E( vectorField u, float2* t, float* E);

//Routine check
extern void routineCheck(vectorField t);

//Statistics Kernels
extern void calc_E_kernel( vectorField u, float2* t);
extern void calc_D_kernel( vectorField u, float2* t);
