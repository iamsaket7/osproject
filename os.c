

	

	#include <stdio.h>

	#include <stdlib.h>

	#include <pthread.h>

	#include <time.h>
    #include <unistd.h>

	 

	#define noc 5             

	#define nor 3             

	int pc[5][3];   int temp[5][3];       

	int available[nor];        int max[noc][nor]; 

	int allocation[noc][nor] = { {1,2,3},{3,2,1},{1,1,1},{1,1,1},{1,1,1} }; 

	int need[noc][nor];       

	int ci = 0;             //count of i

	int cj = 0;             //count of j  

	int ti = 5;  int tj = 3;

	 

	void *inc_count(void *r);

	void *watch_c(void *r);

	 

	pthread_mutex_t mutex; 

	pthread_cond_t thresholdcv;

	 

	int main(){

 

	  long r1 = 1,r2 = 2,r3 = 3;

	srand(time(NULL));

	int x, y;

	    for(x=0; x<noc; x++){                           /*noc= Number of customers*/

	      for(y=0; y<nor; y++){                         /*nor= Number of resources*/

	        if(y==0){

	            max[x][y] = rand() % r1 + 1;

	          }

	        else if(y==1){

	            max[x][y] = rand() % r2 + 1;

	          }

	        else{

	            max[x][y] = rand() % r3 + 1;

	          }

	 

	        }

	      }

	   

	 

	  pthread_t pc[5][3]; 

	  pthread_attr_t attr;

	  int  i, j;

	 

	 

	    printf("Enter Resource 1: ");     

	    scanf("%ld", &r1);

	 

	    printf("Enter Resource 2: ");     

	    scanf("%ld", &r2);

	 

	    printf("Enter Resource 3: ");     

	    scanf("%ld", &r3);

 

	 

	 

	  if(pthread_mutex_init(&mutex, NULL) < 0){

	     perror("Pthread_mutex_init error.");

	     exit(1);   

	     }

	  else

	     

	    

	  pthread_cond_init(&thresholdcv, NULL);

	 

	  pthread_attr_init(&attr); 

	 

	  pthread_create(&pc[0][0], &attr, watch_c, (void *)r1);

	  pthread_create(&pc[1][0], &attr, inc_count, (void *)r2);

	  pthread_create(&pc[2][0], &attr, inc_count, (void *)r3);

	   

	   

	  for(i=0; i<=ti; i++){

	      for(j=0; j<=tj; j++){

	          pthread_join(pc[i][j],NULL); 

	          }

	      }

	  printf("Main: waited on %d, %d threads. Done.\n", ti, tj);

	   

	  pthread_attr_destroy(&attr);

	  pthread_mutex_destroy(&mutex);

	  pthread_cond_destroy(&thresholdcv);

	  pthread_exit(NULL);

	 

	}

	 

	void *inc_count(void *r)

	{  

	  int i, j, n, m;

	  long my_id = (long)r;

	 

	  for(i=0; i<10; i++){

	      for(j=0; j<10; j++){

	    need[n][m] = max[n][m] - allocation[i][j];

	    printf("allocation = %d, need = %d\n", allocation[i][j], need[n][m]);

	    }

	    pthread_mutex_lock(&mutex);

	    if(ci == noc && cj == nor){

	       pthread_cond_signal(&thresholdcv);

	       printf("inc_count: thread %ld, need = %d. Threshold reached.\n",my_id, need[n][m]);

	       }

	    printf("inc_count: thread %ld, need = %d. Unlocking mutex.\n", my_id, need[n][m]);

	    pthread_mutex_unlock(&mutex);

	    sleep(1);

	    watch_c(r);

   }

	  pthread_exit(NULL);

	  watch_c(r);

	 

	}

	 

	void *watch_c(void *r)

	{

	  long my_id = (long)r;

	  int n, m;

	 

	  printf("Start watch_c: thread %ld\n", my_id);

	 

	   while(ci < noc && cj <nor)

	  { pthread_mutex_lock(&mutex);

	   available[n] = max[n][m] - allocation[ci++][cj++];

	   printf("available = %d\n", available[n]);

	   pthread_cond_wait(&thresholdcv, &mutex);

	   printf("watch_c: thread %ld, available = %d. Conditional Signal Received.\n", my_id, available[m]);

	   cj++;

	   printf("watch_c: thread %ld, need now = %d.\n", my_id, need[ci][cj]);

	  }

	  pthread_mutex_unlock(&mutex);

	  pthread_exit(NULL);

	}