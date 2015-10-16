#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// maximum number of integers in the array
#define MAX_ARRAY_SIZE 640000
#define INPUT_FILE_NAME "MSS_Problems.txt"
#define OUTPUT_FILE_NAME "MSS_Results.txt"
#define NUM_FUNCTIONS 4
#define NUM_TESTS 10

// set it to 1 for debug
int debug = 0;

typedef struct{
  int i,j,max;
} maxij;


int write_array_file(FILE *wfile, int a[], int size_a) {
  int i;

  // print the array
  fprintf(wfile, "[");
  for (i=0; i < size_a; i++) {
    fprintf(wfile, "%d", a[i]);
    if (i+1 == size_a)
      fprintf(wfile, "]\n");
    else
      fprintf(wfile, ", ");
  }  
}


//Algorithm 1: Enumeration
maxij maxsubarray_1(int a[], int n)
{
    int sum_current = 0;
    int sum_max = 0;
    int max_i = 0;
    int max_j = 0;
    int i = 0;
    int j = 0;
    int sumIndex;

    // Algorithm
    for (i=0; i < n; i++) {
      for (j=i; j < n; j++) {
        //compute sum from i to j
        sum_current = 0;
        for (sumIndex=i; sumIndex<=j; ++sumIndex) {
          sum_current += a[sumIndex];
        }

        if (sum_current > sum_max){
          sum_max = sum_current;
          max_i = i;
          max_j = j;
        }

      }
    }

    if (debug) printf("max_i = %d max_j = %d", max_i, max_j);
    if (debug) printf("sum_max=%d\n",sum_max);

    maxij result = {max_i,max_j,sum_max};
    return result;
}

//correct
//Algorithm 2: Better Enumeration
maxij maxsubarray_2(int a[], int n)
{
    int sum_current = 0;
    int sum_max = 0;
    int max_i = 0;
    int max_j = 0;
    int i = 0;
    int j = 0;
    
    // Algorithm
    for (i=0; i < n; i++) {
      sum_current = 0;
      // start with j=i
      for (j=i; j < n; j++) {
        if (i == j) 
          sum_current = a[j];
        else 
          sum_current += a[j]; 
        if (sum_current > sum_max) {
          sum_max = sum_current;
          max_i = i;
          max_j = j;
        }         
      }
    } 

    if (debug) printf("max_i = %d max_j = %d", max_i, max_j);
    if (debug) printf("sum_max=%d\n",sum_max);

    maxij result = {max_i,max_j,sum_max};
    return result;
}


//Algorithm 3: Divide and Conquer

//convinience for comparing 2 structs
maxij max(maxij a, maxij b){
  if (a.max > b.max) return a;
  else return b;
}

//convinience for finding the max of 3 structs
maxij threemax(maxij a, maxij b, maxij c){
  return max(max(a,b),c);
}

//find maximum sub array crosing mid point
maxij max_middle(int a[], int start, int mid, int end)
{
    int sum = a[mid];
    int left_sum = a[mid];
    int right_sum = 0 ;
    int i = 0;
    maxij result = {mid,mid,0};

    for (i = mid-1; i >= start; i--)
    {
      sum = sum + a[i];
      if (sum > left_sum) {
          left_sum = sum;
          result.i = i;
      }
    }
 
    sum = 0;
    for (i = mid+1; i <= end; i++)
    {
      sum = sum + a[i];
      if (sum > right_sum) {
        right_sum = sum;
        result.j = i;
      } 
    }
 
    result.max = left_sum+right_sum;
    return result;
}


maxij r_maxsubarray_3(int a[],int low, int high){
  
  //base case
  if (low == high){
    maxij result = {low,high,a[low]};
    return result;
  }

  //recursive case
  int mid = low + (high-low)/2;

  maxij left = r_maxsubarray_3(a,low,mid);
  maxij right = r_maxsubarray_3(a,mid+1,high);
  maxij middle = max_middle(a,low,mid,high);

  return threemax(left,right,middle);


}

maxij maxsubarray_3(int a[],int n)
{
  return r_maxsubarray_3(a,0,n-1);
}

// Algorithm 4: Linear-time
maxij maxsubarray_4(int a[],int n)
{
  int max = 0;
  int i;
  int temp_i = 0;
  maxij result = {0,0,0};

  // Algorithm
  for(i = 0; i < n; i++) {
    max = max + a[i];
    if (max <= 0){
      max = 0;
      temp_i = i+1;
    }
    if (result.max < max) {
      result.max = max;
      result.i = temp_i;
      result.j = i;
    }
  }

  return result;
} 
 
// a debug routine to print array
int print_a(int a[], int a_dx) {
  int i;

  printf("INPUT ARRAY: [");
  for (i=0; i < a_dx; i++) {
    printf("%i", a[i]);
    if (i+1 == a_dx) {
      printf("]\n");
    } else {
      printf(",");
    }
  }
  printf("ARRAY size : %d\n", a_dx);
}

// read one line; in other words, read one array in
//
int read_a(FILE *rfile, int a[], int *eof_flagp) {

  int i;
  int a_dx = 0;
  unsigned char c;
  int x;

  if ((x = fscanf (rfile, "[%d,", &i)) != 1 ) {
    printf("Done.\n");
    return 0;
  }
  
  if (debug) printf("i=%d\n", i);  
  
  a[a_dx++] = i;
  while ( 1 ) {
    if (fscanf (rfile, "%d,", &i) != 1 ) {
      while (fscanf (rfile, "%c", &c) == 1) {
        if (debug) printf("c = '%c'", c);  
        if ( c == '\n') {
          if (debug) printf("c = \\n");  
          break;
        }
        if ( c == EOF ) {
          printf("c = EOF");  
          *eof_flagp = 1;
          break;
        }
      }
      break;
    }
    if (debug) printf("ei=%d\n", i);  
    a[a_dx++] = i;
  }

  return a_dx;
}

void makeRandomArray(int* a, int n){
  int i;
  for (i = 0; i<n; ++i){
    a[i] = rand()%200 - 100;
  }
}



void experimentalAnalysis( maxij (*func) (int*,int) ){

  int read;
  int n;
  int a[MAX_ARRAY_SIZE];
  int i;
  clock_t begining, end, total;

  srand (time(NULL));

  do{

    //read array size
    read = scanf("%d",&n);
    if (read == EOF) break;
    
    total = 0;

    //test algorithm on x arrays of size n
    for (i=0;i<10;++i){

      makeRandomArray(a,n);

      //record begining time
      begining = clock();

      (*func) (a,n);

      //capture time to execute function
      end = clock();
      total += end-begining;

    }

    //display average time for n executions
    printf("%d,%f\n", n, ((double)total / CLOCKS_PER_SEC) / NUM_TESTS);

  }while(read != EOF);

}

int main(int argc, char const *argv[])
{

  maxij (*func[NUM_FUNCTIONS]) (int*,int) = {maxsubarray_1,maxsubarray_2,maxsubarray_3,maxsubarray_4};

  if (argc > 1){
    experimentalAnalysis(func[atoi(argv[1])]);
    return 0;
  }

  FILE* rfile;
  FILE* wfile;
  int size = 0;
  int a[MAX_ARRAY_SIZE];
  int eof_flag = 0;
  int i;


  if ((rfile = fopen(INPUT_FILE_NAME, "r")) == NULL) {
    printf("Cannot open '%s' for reading\n", INPUT_FILE_NAME);
    exit(-1);
  }

  if ((wfile = fopen(OUTPUT_FILE_NAME, "w")) == NULL) {
    printf("Cannot open '%s' for writing\n", OUTPUT_FILE_NAME);
    exit(-1);
  }

  while (eof_flag == 0) {
    if ((size = read_a(rfile, &a[0], &eof_flag)) == 0) {
      break;
    }
    print_a(a, size);

    write_array_file(wfile,a,size);

    maxij result;

    for(i = 0; i<NUM_FUNCTIONS; ++i){
      result = (*func[i]) (a,size);

      //output returned struct
      //printf("algo %d result max:%d i%d j%d \n", i,result.max,result.i,result.j);

      write_array_file(wfile,a + result.i, result.j - result.i + 1);
      fprintf(wfile, "%d\n", result.max);
    }

    fprintf(wfile, "%s", "\n");

  }

  // TBD close on exits also
  fclose(rfile);
  fclose(wfile);

  return 0;
}
