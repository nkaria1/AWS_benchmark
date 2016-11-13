#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>

#define BYTE (1)
#define KILOBYTE (1024)
#define MEGABYTE (1024*1024)

int timer=0,fetch=0;



//~~~~~~~~~~~~~~~~~~~~~~~~_MEM_RAN_~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void *mem_ran_ops(void *add_sz)
{int *size=add_sz,i,x;
int sz=*size;
char *scr, *dest;
double k=0;
scr =(char *)malloc(sz+1);
dest =(char *)malloc((sz+1)*2);
memset(scr, '0',sz+1);
//bzero(dest,sz);
do{	x=rand()*rand();
	x%=(sz-2);
	memcpy(dest, scr, sz);	
	k++;}while(timer==1);
//printf ("sz= %d , k = %f \n", sz,k);
printf ("The random memory throughput is = %f bytes/sec \n", ((sz*k*2)/5)); 
//printf ("The random memory latency is = %f msec \n", 5000/(k*2*sz));
free(scr);
free(dest);

pthread_exit;
}

void thread_mem_ran_ops(int sz, int n)
{int i,elapsed,*add_sz;
time_t start, end;
pthread_t tid[n];
timer=1;

add_sz=&sz;

time(&start);
for( i=0; i<n; i++ ) 
    pthread_create( &tid[i], NULL,mem_ran_ops, add_sz);
printf("When the number of threads is %d and chunk size %d\n", n, sz);
do{time(&end);
   elapsed = difftime(end, start);
}while(elapsed < 5);
timer=0;

for( i=0; i<n; i++ ) 
    pthread_join( tid[i], NULL );
printf("\n");
}


//~~~~~~~~~~~~~~~~~~~~~~~~_MEM_SEQ_~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void *mem_seq_ops(void *add_sz)
{int *size=add_sz;
int sz=*size;
char *scr, *dest;
double k=0;
scr =(char *)malloc(sz+1);
dest =(char *)malloc(sz+1);
memset(scr, '4',sz+1);
while(timer==1){

memcpy(dest, scr, sz);
k++;}
//for(i=0;i<sz;i++)
//		printf ("%c ", dest[i]);
printf ("The sequential memory throughput is = %f bytes/sec \n", ((sz*k*2)/5)); 
printf ("The sequential latency is = %f msec \n", 5000/(k*2*sz));
free(scr);
free(dest);
}
void thread_mem_seq_ops(int sz, int n)
{int i,elapsed,*add_sz;
time_t start, end;
pthread_t tid[n];
timer=1;

add_sz=&sz;

time(&start);
for( i=0; i<n; i++ ) 
    pthread_create( &tid[i], NULL,mem_seq_ops, add_sz);
printf("When the number of threads is %d and chunk size %d\n", n, sz);

do{time(&end);
   elapsed = difftime(end, start);
}while(elapsed < 5);
timer=0;

for( i=0; i<n; i++ ) 
    pthread_join( tid[i], NULL );
printf("\n");
}

//~~~~~~~~~~~~~~~~~~~~~~~~_DISK_RAN_READ_~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void *disk_ran_read(void *add_sz)
{int *size=add_sz;
int sz=*size;
double k=0;
char s[sz];
FILE *f;
f=fopen("fox.txt", "rb");
do
{  int x=rand()*rand();
   x%=sz;
   fseek(f,-x,SEEK_SET); 
   fread(&s,sz,1,f);
   k++;
}while(timer==1);
fclose(f);
printf ("The disk random read throughput is = %f bytes/sec \n", ((sz*k)/5)); 
printf ("The disk random read latency is = %f msec \n", 5000/(k*sz));
pthread_exit;
}


void thread_disk_ran_read(int sz, int n)
{int i,elapsed,*add_sz;
time_t start, end;

pthread_t tid[n];
timer=1;
add_sz=&sz;


time(&start);
for( i=0; i<n; i++ ) 
    pthread_create( &tid[i], NULL,disk_ran_read, add_sz);
printf("When the number of threads is %d and chunk size %d\n", n, sz);
do{time(&end);
   elapsed = difftime(end, start);
}while(elapsed < 5);
timer=0;

for( i=0; i<n; i++ ) 
    pthread_join( tid[i], NULL);
printf("\n");
}

//~~~~~~~~~~~~~~~~~~~~~~~~_DISK_RAN_WRITE_~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void *disk_ran_write(void *add_sz)
{
int *size=add_sz; double k=0;
int sz=*size;
int i,j;
   char s[40]="A quick brown fox jumps over a lazy dog!";
   char bf[sz];
   if (40>sz)
	{for (j=0;j<sz;j++)
		bf[j]=s[j];
 	}  
   else
	{for (j=0;j<(sz/40);j++)
		{for (i=0;i<40;i++)
			bf[i+(j*40)]=s[i];
		}
	for (i=0; i<(sz%40);i++)
		bf[i+(j*40)]=s[i];
	}

FILE *f;
   f=fopen("fox.txt","rb+");
do
{ int x=rand()*rand();
   x%=sz;
  fseek(f,x,SEEK_SET);
  fwrite(&s,sz,1,f);
   k++;
}while(timer==1);
fclose(f);

printf ("The disk random write throughput is = %f bytes/sec \n", (sz*1)*k/5); 
printf ("The disk random write latency is = %f msec \n", (5000)/(k*(sz*1)));
pthread_exit;

}
void thread_disk_ran_write(int sz,int n)
{
int i,elapsed,*add_sz;
time_t start, end;

pthread_t tid[n];
timer=1;
add_sz=&sz;


time(&start);
for( i=0; i<n; i++ ) 
    pthread_create( &tid[i], NULL,disk_ran_write, add_sz);
printf("When the number of threads is %d and chunk size %d\n", n, sz);
do{time(&end);
   elapsed = difftime(end, start);
}while(elapsed < 5);
timer=0;

for( i=0; i<n; i++ ) 
    pthread_join( tid[i], NULL );
printf("\n");
}

//~~~~~~~~~~~~~~~~~~~~~~~~_DISK_SEQ_READ_~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void *disk_seq_read(void *add_sz)
{int *size=add_sz; double k=0;
int sz=*size;
char s[sz];
FILE *f;
f=fopen("fox.txt", "rb");
do
{  fread(&s,sz,1,f);
   k++;
}while(timer==1);
fclose(f);
printf ("The disk seq read throughput is = %f bytes/sec \n", sz*k/5); 
printf ("The disk seq read latency is = %f msec \n", 5000/(k*sz));
pthread_exit;
}


void thread_disk_seq_read(int sz, int n)
{int i,elapsed,*add_sz;
time_t start, end;

pthread_t tid[n];
timer=1;
add_sz=&sz;


time(&start);
for( i=0; i<n; i++ ) 
    pthread_create( &tid[i], NULL,disk_seq_read, add_sz);
printf("When the number of threads is %d and chunk size %d\n", n, sz);
do{time(&end);
   elapsed = difftime(end, start);
}while(elapsed < 5);
timer=0;

for( i=0; i<n; i++ ) 
    pthread_join( tid[i], NULL);
printf("\n");
}

//~~~~~~~~~~~~~~~~~~~~~~~~_DISK_SEQ_WRITE_~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void *disk_seq_write(void *add_sz)
{int *size=add_sz; double k=0;
int sz=*size;
int i,j;
   char s[40]="A quick brown fox jumps over a lazy dog!";
   char bf[sz];
   if (40>sz)
	{for (j=0;j<sz;j++)
		bf[j]=s[j];
 	}  
   else
	{for (j=0;j<(sz/40);j++)
		{for (i=0;i<40;i++)
			bf[i+(j*40)]=s[i];
		}
	for (i=0; i<(sz%40);i++)
		bf[i+(j*40)]=s[i];
	}

FILE *f;
   f=fopen("fox.txt","wb");
do
{  fwrite(&s,sz,1,f);
   k++;
}while(timer==1);
fclose(f);

printf ("The disk seq write throughput is = %f bytes/sec \n", (sz*1)*k/10); 
printf ("The disk seq write latency is = %f msec \n",(double) (10000)/(k*(sz*1)));
pthread_exit;
}


void thread_disk_seq_write(int sz, int n)
{int i,elapsed,*add_sz;
time_t start, end;

pthread_t tid[n];
timer=1;
add_sz=&sz;


time(&start);
for( i=0; i<n; i++ ) 
    pthread_create( &tid[i], NULL,disk_seq_write, add_sz);
printf("When the number of threads is %d and chunk size %d\n", n, sz);
do{time(&end);
   elapsed = difftime(end, start);
}while(elapsed < 10);
timer=0;

for( i=0; i<n; i++ ) 
    pthread_join( tid[i], NULL );
printf("\n");
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~FLOPS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void *get_flops(void *val) 
{

int x=rand(); double k=0; float j;
do
{
	j+=10.10*x+333.3/4.4+x*66.6/7.7+x-9.9-10.10+x*12.12-x/14.14+15.15+x-17.17*18.18*19.19/20.20;
	j+=10.10*x+333.3/4.4+x*66.6/7.7+x-9.9-10.10+x*12.12-x/14.14+15.15+x-17.17*18.18*19.19/20.20;
	j+=10.10*x+333.3/4.4+x*66.6/7.7+x-9.9-10.10+x*12.12-x/14.14+15.15+x-17.17*18.18*19.19/20.20;
	j+=10.10*x+333.3/4.4+x*66.6/7.7+x-9.9-10.10+x*12.12-x/14.14+15.15+x-17.17*18.18*19.19/20.20;
	j+=10.10*x+333.3/4.4+x*66.6/7.7+x-9.9-10.10+x*12.12-x/14.14+15.15+x-17.17*18.18*19.19/20.20;
	k++;
}while(timer==1);
printf ("The CPU speed is = %f FLOPS \n", k*101);
pthread_exit;
}

void thread_flops(int n)
{
int i,elapsed;
time_t start, end;

pthread_t tid[n];
timer=1;

time(&start);
for( i=0; i<n; i++ ) 
    pthread_create( &tid[i], NULL,get_flops, NULL );
printf("When the number of threads is %d \n", n);

do{time(&end);
   elapsed = difftime(end, start);
}while(elapsed < 5);
timer=0;

for( i=0; i<n; i++ ) 
    pthread_join( tid[i], NULL );
printf("\n");
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~_PLOT_FLOPS_~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void *plot_flops(void *val)  
{

int x=rand(); double k=0; float j;
do
{    fetch=0; 
do{
	j+=10.10*x+333.3/4.4+x*66.6/7.7+x-9.9-10.10+x*12.12-x/14.14+15.15+x-17.17*18.18*19.19/20.20;
	j+=10.10*x+333.3/4.4+x*66.6/7.7+x-9.9-10.10+x*12.12-x/14.14+15.15+x-17.17*18.18*19.19/20.20;
	j+=10.10*x+333.3/4.4+x*66.6/7.7+x-9.9-10.10+x*12.12-x/14.14+15.15+x-17.17*18.18*19.19/20.20;
	j+=10.10*x+333.3/4.4+x*66.6/7.7+x-9.9-10.10+x*12.12-x/14.14+15.15+x-17.17*18.18*19.19/20.20;
	j+=10.10*x+333.3/4.4+x*66.6/7.7+x-9.9-10.10+x*12.12-x/14.14+15.15+x-17.17*18.18*19.19/20.20;
    	k++;
}while(fetch==0);
printf (" %f FOPS \n" , k*101);
k=0;
}while(timer==1);
pthread_exit;
}

void thread_plot_flops(int n)
{
int i,elapsed,sec=1;
time_t start, end;

pthread_t tid[n];
timer=1;

time(&start);
for( i=0; i<n; i++ ) 
    pthread_create( &tid[i], NULL,plot_flops, NULL );
printf("When the number of threads is %d \n", n);


do{fetch =0;
   time(&end);
   elapsed = difftime(end, start);
   //printf("%d", elapsed);
   sleep(1);
   fetch=1;
   printf("\n");
}while(elapsed < 599);

timer=0;

for( i=0; i<n; i++ ) 
    pthread_join( tid[i], NULL );
printf("\n");
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~_PLOT_IOPS_~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void *plot_iops(void *val)  
{

int j,x=rand(); double k=0;
do
{    fetch=0; 
do{
	j+=10*x+333/4+x*66/7+88-x-10+11*x-13/14+15+x-17*18*x/20;
	j+=10*x+333/4+x*66/7+88-x-10+11*x-13/14+15+x-17*18*x/20;
	j+=10*x+333/4+x*66/7+88-x-10+11*x-13/14+15+x-17*18*x/20;
	j+=10*x+333/4+x*66/7+88-x-10+11*x-13/14+15+x-17*18*x/20;
	j+=10*x+333/4+x*66/7+88-x-10+11*x-13/14+15+x-17*18*x/20;
    	k++;
}while(fetch==0);
printf (" %f IOPS \n", k*101);
k=0;
}while(timer==1);
pthread_exit;
}

void thread_plot_iops(int n)
{
int i,elapsed,sec=1;
time_t start, end;

pthread_t tid[n];
timer=1;

time(&start);
for( i=0; i<n; i++ ) 
    pthread_create( &tid[i], NULL,plot_iops, NULL );
printf("When the number of threads is %d \n", n);


do{fetch =0;
   time(&end);
   elapsed = difftime(end, start);
   //printf("%d", elapsed);
   sleep(1);
   fetch=1;
   printf(" \n");
}while(elapsed < 599);

timer=0;

for( i=0; i<n; i++ ) 
    pthread_join( tid[i], NULL );
printf("\n");
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~IOPS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void *get_iops(void *val)  
{

int j,x=rand(); double k=0;
do
{       
	j+=10*x+333/4+x*66/7+88-x-10+11*x-13/14+15+x-17*18*x/20;
	j+=10*x+333/4+x*66/7+88-x-10+11*x-13/14+15+x-17*18*x/20;
	j+=10*x+333/4+x*66/7+88-x-10+11*x-13/14+15+x-17*18*x/20;
	j+=10*x+333/4+x*66/7+88-x-10+11*x-13/14+15+x-17*18*x/20;
	j+=10*x+333/4+x*66/7+88-x-10+11*x-13/14+15+x-17*18*x/20;
    	k++;
}while(timer==1);
printf ("The CPU speed is = %f IOPS \n", k*101/5);
pthread_exit;
}

void thread_iops(int n)
{
int i,elapsed;
time_t start, end;

pthread_t tid[n];
timer=1;

time(&start);
for( i=0; i<n; i++ ) 
    pthread_create( &tid[i], NULL,get_iops, NULL );
printf("When the number of threads is %d \n", n);

do{time(&end);
   elapsed = difftime(end, start);
}while(elapsed < 5);
timer=0;

for( i=0; i<n; i++ ) 
    pthread_join( tid[i], NULL );
printf("\n");
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

int main()
{
/*thread_iops(1);
thread_iops(2);
thread_iops(4);
thread_flops(1);
thread_flops(2);
thread_flops(4);

thread_disk_seq_write(BYTE,1);
thread_disk_seq_write(BYTE,2);
thread_disk_seq_write(KILOBYTE,1);
thread_disk_seq_write(KILOBYTE,2);
thread_disk_seq_write(MEGABYTE,1);
thread_disk_seq_write(MEGABYTE,2);


thread_disk_seq_read(BYTE,1);
thread_disk_seq_read(BYTE,2);
thread_disk_seq_read(KILOBYTE,1);
thread_disk_seq_read(KILOBYTE,2);
thread_disk_seq_read(MEGABYTE,1);
thread_disk_seq_read(MEGABYTE,2);


thread_disk_ran_write(BYTE,1);
thread_disk_ran_write(BYTE,2);
thread_disk_ran_write(KILOBYTE,1);
thread_disk_ran_write(KILOBYTE,2);
thread_disk_ran_write(MEGABYTE,1);
thread_disk_ran_write(MEGABYTE,2);


thread_disk_ran_read(BYTE,1);
thread_disk_ran_read(BYTE,2);
thread_disk_ran_read(KILOBYTE,1);
thread_disk_ran_read(KILOBYTE,2);
thread_disk_ran_read(MEGABYTE,1);
thread_disk_ran_read(MEGABYTE,2);


thread_mem_seq_ops(BYTE,1);
thread_mem_seq_ops(BYTE,2);
thread_mem_seq_ops(KILOBYTE,1);
thread_mem_seq_ops(KILOBYTE,2);
thread_mem_seq_ops(MEGABYTE,1);
thread_mem_seq_ops(MEGABYTE,2);


thread_mem_ran_ops(BYTE,1);
thread_mem_ran_ops(BYTE,2);
thread_mem_ran_ops(KILOBYTE,1);
thread_mem_ran_ops(KILOBYTE,2);
thread_mem_ran_ops(MEGABYTE,1);
thread_mem_ran_ops(MEGABYTE,2);
*/
thread_plot_iops(1);
thread_plot_flops(1);
return 0;}
