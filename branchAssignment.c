#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
//#include "City.h"
#define LINE_LEN 10
#define ITER_TIMES 10


// READ NORMAL FORMAT CITIES (integer, 51 & 105 cities)
int** readNorm(FILE *f, int dim)
{
  int i;
  int **city;
  char string[LINE_LEN];

  city = (int**) malloc(sizeof(int*)*dim);
  for (i = 0; i < dim; ++i)
  {
    city[i] = (int*) malloc(sizeof(int)*2);
  }

  fgets(string, LINE_LEN, f); // to skip "NODE_COORD_SECTION"
  for (i = 0; i < dim; ++i)
  {
    fgets(string, LINE_LEN, f);
    strtok(string, " "); // to remove first number

    city[i][0] = atoi(strtok(NULL, " \n"));
    city[i][1] = atoi(strtok(NULL, " \n"));

    
  }
  
  return city;
}

// READ EXPONENTIAL FORMAT CITIES (442 cities)
int** readExp(FILE *f, int dim)
{
  int i;
  int **city;
  char string[LINE_LEN];
  char tmp[2][50];

  city = (int**) malloc(sizeof(int*)*dim);
  for (i = 0; i < dim; ++i)
  {
    city[i] = (int*) malloc(sizeof(int)*2);
  }

  fgets(string, LINE_LEN, f);     // to skip "NODE_COORD_SECTION"
  for (i = 0; i < dim; ++i)
  {
    fgets(string, LINE_LEN, f);
    strtok(string, " ");          // to remove first number

    strcpy(tmp[0], strtok(NULL, " \n"));
    strcpy(tmp[1], strtok(NULL, " \n"));

    city[i][0] = convertExptoInt(tmp[0]);
    city[i][1] = convertExptoInt(tmp[1]);
  }
  
  return city;
}

float** getDistMatrix(int **city, int dim)
{
  int i, j;
  float **dist;

  dist = (float**) malloc(sizeof(float*)*dim);
  for (i = 0; i < dim; ++i)
  {
    dist[i] = (float*) malloc(sizeof(float)*dim);
  }

  for (i = 0; i < dim; ++i)
  {
    for (j = 0; j < dim; ++j)
    {
      dist[i][j] = (float)sqrt((double)pow(city[i][0] - city[j][0], 2)
                              +(double)pow(city[i][1] - city[j][1], 2));
    }
  }
 
  return dist;

}

float getDist(int *seq_city, float **dist, int dim)
{
  int i;
  float total_dist = 0;

  for (i = 0; i < dim-1; ++i)
  {
    total_dist += dist[seq_city[i]][seq_city[i+1]];
  }
  total_dist += dist[seq_city[i]][seq_city[0]];

  return total_dist;
}

int* randRoute(int dim)
{
  int i,j;
  int *seq_city;
int a[dim];
   //int a[48]={1,8,38,31,44,18,7,28,6,37,19,27,17,43,30,36,46,33,20,47,21,32,39,48,5,42,24,10,45,35,4,26,2,29,34,41,16,22,3,23,14,25,13,11,12,15,40,9};
  seq_city = (int*) malloc(sizeof(int)*dim);
 // seq_city= {1,8,38,31,44,18,7,28,6,37,19,27,17,43,30,36,46,33,20,47,21,32,39,48,5,42,24,10,45,35,4,26,2,29,34,41,16,22,3,23,14,25,13,11,12,15,40,9};
 for (i = 0; i < dim; ++i)
  {
  	a[i]=0;
  }
i=0;
//seq_city=a;
while(i<dim)
{
j=rand() % dim;
	if(a[j])
	{
	continue;
	}
	else
	{
	seq_city[i]=j;
	a[j]=1;
	i++;	
	}
	
}
 /*for (i = 0; i < dim; ++i)
  {
  	seq_city[i]=i;
  }*/
/*printf("\nIntitial route:\t");
 for (i = 0; i < dim; ++i)
  {
  	printf("%d ",seq_city[i]);
  }*/

  return seq_city;
}

/*void shuffle(int *arry, int n)
{
  int t;
  int i, j;

  srand(time(NULL));
  if (n > 1)
  {
    for (i = 0; i < n; ++i)
    {
      j = rand()%n; // / (RAND_MAX/(n-i)+1);
      t = arry[j];
      arry[j] = arry[i];
      arry[i] = t;
    }
  }
}*/



char* trimWS(char *str)
{
  char *end;

  
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

    end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

int convertExptoInt(char *str)
{
  int num;
  int tmp_exp;
  float tmp_float;
  char *tmp_str;

  trimWS(str);                            // to clean string
  tmp_str = strtok(str, "e");             // to get float part
  tmp_float = (float)atof(tmp_str);       // convert to float
  tmp_str = strtok(NULL, "+");            // to get exp part
  tmp_exp = atoi(tmp_str);                // convert to int

  tmp_float = tmp_float * (float)pow(10, tmp_exp);
  num = (int)tmp_float;

  return num;
}


int* twoOptSwap(int *route, int dim, int m, int n)
{
  /** Neighbor is defined as two of cities swapped
   *  (2-Opt swap)
   **/
  int i, j;
  int *route_new;

  route_new = (int*) malloc(sizeof(int)*dim);

  // Add route[0] to route[m-1] to route_new in order
  for (i = 0; i <=m-1; ++i)
  {
    route_new[i] = route[i];
  }
//route_new[m]=route[n];
//route_new[n]=route[m];


  // Add route[m] to route[n] to route_new in reverse order
  for (i = m, j = n; i <= n; ++i, --j)
  {
    route_new[i] = route[j];
  }

  // Add route[n+1] to route[dim-1] to route_new in order
  for (i = n+1; i < dim; ++i)
  {
    route_new[i] = route[i];
  }

  return route_new;
}

int* bestChild(int *route, float **dist, int dim)
{
  int i, j,INF;
  int *route_new;
  int *route_best;
  float cur_dist;
  float best_dist = (float)INF;

  route_best = twoOptSwap(route, dim, 0, 1);
  best_dist = getDist(route_best, dist, dim);
  //printf("\n%lf\n",best_dist);
   //printf("\n%d\n",*route_best);
  for (i = 0; i < dim; ++i)
  {
    for (j = 0; j < dim; ++j)
    {
      if (i >= j) continue;

      route_new = twoOptSwap(route, dim, i, j);
      cur_dist = getDist(route_new, dist, dim);

      if (cur_dist < best_dist)
      {
        best_dist = cur_dist;
        free(route_best);
        route_best = route_new;
      }
      else // Abandon the new route
      {
        free(route_new);
      }
    }
  }

/*printf("\n Neighbor: ");
for(i=0;i<dim;i++)
{
printf("%d --",route_best[i]);
}
printf("\n%lf\n",best_dist);*/
  return route_best;
}

float HillClimbing(float **dist, int dim)
{
  int *route,i,INF;
  int *route_child_best;
  int improved = 1;
  float best_so_far = (float)INF;
  float child_best_dist = (float)INF;

  // CREATE A RANDOM ROUTE AT START
  route = randRoute(dim);
  best_so_far = getDist(route, dist, dim);

  
  while(improved)
 {
    improved = 0;
    route_child_best = bestChild(route, dist, dim);
    child_best_dist = getDist(route_child_best, dist, dim);
    if (child_best_dist < best_so_far)
    {
      best_so_far = child_best_dist;
      free(route);
      route = route_child_best;
      improved = 1;
    }
    else
    {
      free(route_child_best);
    }
  }
/*printf("\n");
for(i=0;i<dim;i++)
{
printf("%d--",*(route)++);
}
printf("\n");*/
  return best_so_far;
}


int main(int argc, char const *argv[])
{
  int i,INF;
  int dim;
  int **city;
  float **dist;
  float best_dist;
  float run_time;
  char header[LINE_LEN];
  char *tmp;
  float array[100];
  float min;
  clock_t start,start1;
  clock_t end,end1;

  
  float total_best_dist;
  
  FILE *f, *fw;
  
  f = fopen("5.tsp", "r");

  fw = fopen("output", "w");

  // READ HEADER
 for (i = 0; i < 5; ++i)
  {
    fgets(header, LINE_LEN, f);
    tmp = strtok(header, ":");
    trimWS(tmp);
    fprintf(fw, "%s", tmp);

    // get number of cities
    if (strcmp(header, "DIMENSION") == 0)
    {
      tmp = strtok(NULL, "\n");
      trimWS(tmp);
      dim = atoi(tmp);
      fprintf(fw, ": -%d-\n", dim);
    }else{
      tmp = strtok(NULL, "\n");
      fprintf(fw, ": %s\n", tmp);
    }
  }

  
  if (dim == 198) 
  {
    city = readExp(f, dim);
  }
  else 
  {
    city = readNorm(f, dim);
  }

  // CREATE A MATRIX WITH CITY DISTANCE
  dist = getDistMatrix(city, dim);

   
  total_best_dist = 0;
  run_time = 0;
  
  // ITERATE SEVERAL TIMES TO MEASURE PERFORMANCE
  start1=clock();
  for (i = 0; i < ITER_TIMES; ++i)
  {
    start = clock();
    best_dist = HillClimbing(dist, dim);
    array[i]=best_dist;
    end = clock();
    run_time += (float)(end-start);
    total_best_dist += best_dist;
    //printf(".");
    //printf("Shortest distance: %f\n", best_dist);
     
  }
end1=clock();

printf("Run Time: %.2f\n", (float)(end1-start1)/CLOCKS_PER_SEC);
min=INF;
for(i=0;i<ITER_TIMES;i++)
{
//printf("%f\n",array[i]);
	if(array[i]<min)
	{min=array[i];}
}
min=floor(min);
printf("Optimal shortest distance found :%f \n",min);
  printf("\n");
  fprintf(fw, "Search Algorithm: Hill Climbing\n");
  fprintf(fw, "Trials: %d\n", ITER_TIMES);
  fprintf(fw, "Average Best Distance: %.2f\n", ((float)total_best_dist/(float)ITER_TIMES));
  fprintf(fw, "Average Run Time: %.2f\n", (float)(run_time/ITER_TIMES)/CLOCKS_PER_SEC);
 

  free(dist);
  free(city);

  return 0;
}
