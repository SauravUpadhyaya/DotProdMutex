#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct 
 {
   double      *a;
   double      *b;
   double     minimum; 
   int     veclen; 
 } DOTDATA;

#define NUMTHRDS 4
#define VECLEN 100000
   DOTDATA minimumVal; 
   pthread_t callThd[NUMTHRDS];
   pthread_mutex_t mutexminimum;

void *minimumNumberCalculation(void *arg)
{
   int i, start, end, len ;
   long offset;
   double minimum, *x, *y;
   offset = (long)arg;
     
   len = minimumVal.veclen;
   start = offset*len;
   end   = start + len;
   x = minimumVal.a;
   y = minimumVal.b;

   minimum = 0;
   for (i=start; i<end ; i++) 
    {
      if((x[i]) <  y[i]){
      minimum = x[i];
      
    }
   else
    {
     minimum = y[i];

    }
   }
   pthread_mutex_lock (&mutexminimum);
   minimumVal.minimum =minimum;
   printf("Thread %ld did %d to %d:  Minimum Value=%f global Minimum Value=%f\n",offset,start,end,minimum,minimumVal.minimum);
   pthread_mutex_unlock (&mutexminimum);

   pthread_exit((void*) 0);
}
int main (int argc, char *argv[])
{
long i;
double *a, *b;
void *status;
pthread_attr_t attr;


a = (double*) malloc (NUMTHRDS*VECLEN*sizeof(double));
b = (double*) malloc (NUMTHRDS*VECLEN*sizeof(double));
  
for (i=0; i<VECLEN*NUMTHRDS; i++) {
  a[i]=1;
  b[i]=a[i];
  }

minimumVal.veclen = VECLEN; 
minimumVal.a = a; 
minimumVal.b = b; 
minimumVal.minimum=0;

pthread_mutex_init(&mutexminimum, NULL);
         
pthread_attr_init(&attr);
pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

for(i=0;i<NUMTHRDS;i++)
  {
   pthread_create(&callThd[i], &attr, minimumNumberCalculation, (void *)i); 
   }

pthread_attr_destroy(&attr);


for(i=0;i<NUMTHRDS;i++) {
  pthread_join(callThd[i], &status);
  }
printf ("Minimum Value =  %f \n", minimumVal.minimum);
free (a);
free (b);
pthread_mutex_destroy(&mutexminimum);
pthread_exit(NULL);
}   

