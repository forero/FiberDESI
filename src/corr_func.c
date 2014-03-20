#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
Computes the correlation function for a ser of 2D points.
*/
float * load_data(char * filein, int *n_points);
void get_min_max(float *data, int n_points, float *min_x, float *max_x, float *min_y, float *max_y);
float * gen_random(int n_points, float min_x, float max_x, float min_y, float max_y);
float * get_distances(float *data_A, int n_A, float *data_B, int n_B, long long *n_dist);
float min_val(float *data_1D, long long n_points);
float max_val(float *data_1D, long long n_points);

int main(int argc, char **argv){
  float *data_points;
  float *random_points;
  int n_random;  
  int n_data;
  float min_x, min_y, max_x, max_y;
  long long n_DD, n_DR, n_RR;
  float *distance_DD;
  float *distance_DR;
  float *distance_RR;

  /*load data*/
  data_points = load_data(argv[1], &n_data);
  fprintf(stdout, "The number of points to load is: %d\n", n_data);

  /*get min max*/
  get_min_max(data_points, n_data, &min_x, &max_x, &min_y, &max_y);
  fprintf(stdout, "min max x: %f %f\n", min_x, max_x);
  fprintf(stdout, "min max y: %f %f\n", min_y, max_y);
  
  /*generate random points in the same range*/
  n_random = n_data;
  random_points = gen_random(n_random, min_x, max_x, min_y, max_y);
  

  /*compute the list of distances between two different lists*/
  distance_DD = get_distances(data_points, n_data, data_points, n_data, &n_DD);
  distance_DR = get_distances(data_points, n_data, random_points, n_random, &n_DR);
  distance_RR = get_distances(random_points, n_random, random_points, n_random, &n_RR);


  fprintf(stdout, "min max DR: %f %f\n", min_val(distance_DR, n_DR), max_val(distance_DR, n_DR));
  /*get the interval to compute the correlation function*/
  
  /*get the correlation function*/
  

  return 0;
}

float min_val(float *data_1D, long long n_points){
  long long i;
  float min_val;

  min_val = 1E10;
  for(i=0;i<n_points;i++){
    if(data_1D[i]< min_val)
      min_val = data_1D[i];
  }
  return min_val;
}

float max_val(float *data_1D, long long n_points){
  long long i;
  float max_val;

  max_val = -1E10;
  for(i=0;i<n_points;i++){
    if(data_1D[i]> max_val)
      max_val = data_1D[i];
  }
  return max_val;
}

float * get_distances(float *data_A, int n_A, float *data_B, int n_B, long long *n_dist){
  long long n_total;
  float *distances;
  int i, j;
  long long k;
  float delta_x, delta_y;
  n_total = n_A * n_B;


  if(!(distances=malloc(n_total * sizeof(float)))){
    fprintf(stdout, "problem with data allocation\n");
    exit(1);
  }

  k=0;
  for(i=0;i<n_A;i++){
    if(!(i%1000))
      fprintf(stdout, "%d/%d [%lld]\n", i, n_A,k);
    for(j=0;j<n_B;j++){  
      delta_x = data_A[i*2 + 0] - data_B[j*2 + 0];
      delta_y = data_A[i*2 + 1] - data_B[j*2 + 1];
      distances[k] = sqrt( pow(delta_x, 2.0)  + pow(delta_y,2.0)); 
      k++;
    }
  }

  *n_dist = n_total;
  return distances;
}


float * gen_random(int n_points, float min_x, float max_x, float min_y, float max_y){
  float * data;
  int i;
  
  if(!(data=malloc(n_points * 2 * sizeof(float)))){
    fprintf(stdout, "problem with data allocation\n");
    exit(1);
  }

  srand48(n_points);

  for(i=0;i<n_points;i++){
    data[2*i + 0] = drand48() * (max_x - min_x) + min_x;
    data[2*i + 1] = drand48() * (max_y - min_y) + min_y;
  }

  
  return data;
}

float * load_data(char * filein, int *n_points){
  float *data;
  FILE *in;
  int n_lines;
  int i;
  int c;

  if(!(in=fopen(filein, "r"))){
    fprintf(stderr, "problem opening file %s", filein);
    exit(1);
  }
  
  /*count the number of lines*/
  n_lines = 0;
  do{
    c = fgetc(in);
    if(c=='\n')
      n_lines++;
  }while(c!=EOF);
  rewind(in);

  /*malloc and load the data*/
  if(!(data=malloc(n_lines * 2 * sizeof(float)))){
    fprintf(stdout, "problem with data allocation\n");
    exit(1);
  }

  for(i=0;i<n_lines;i++){
    fscanf(in, "%f %f\n", &(data[i*2 + 0 ]), &(data[i*2 +1]));
  }
  fclose(in);
  *n_points  = n_lines;
  return data;
}

void get_min_max(float *data, int n_points, float *min_x, float *max_x, float *min_y, float *max_y){
  float x_min, x_max, y_min, y_max;
  int i;
  
  x_min = 1E10;
  y_min = 1E10;

  x_max = -1E10;
  y_max = -1E10;

  for(i=0;i<n_points;i++){
    if(data[2*i + 0]<x_min)
      x_min = data[2*i + 0];

    if(data[2*i + 0]>x_max)
      x_max = data[2*i + 0];

    if(data[2*i + 1]<y_min)
      y_min = data[2*i + 1];

    if(data[2*i + 1]>y_max)
      y_max = data[2*i + 1];    
  }
  
  *min_x = x_min;
  *min_y = y_min;
  *max_x = x_max;
  *max_y = y_max;
}
