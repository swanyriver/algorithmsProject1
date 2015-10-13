#include <stdio.h>
#include <stdlib.h>

// maximum number of integers in the array
#define MAX_ARRAY_SIZE 640000
#define INPUT_FILE_NAME "i.txt"  // TBD
#define OUTPUT_FILE_NAME "o.txt"  // TBD

// set it to 1 for debug
int debug = 0;

//globals for capturing sub array indices in algorithm 3  
int maximum_i;
int maximum_j;

struct maxij
{
  int i,j,max;
};

int write_file(FILE *wfile, int a[], int size_a, int begin, int end, int sum_max) {
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
    // print the sub array
    fprintf(wfile, "[");
    for (i=begin; i <= end; i++) {
  fprintf(wfile, "%d", a[i]);
  if (i == end)
    fprintf(wfile, "]\n");
  else
    fprintf(wfile, ", ");
    }
    // print the sum of sub array
    fprintf(wfile, "%d\n\n", sum_max);
    
}

//
//
//Algorithm 1: Enumeration
int maxsubarray_1(FILE *wfile, int a[], int n)
{
    int sum_current = 0;
    int sum_max = 0;
    int max_i = 0;
    int max_j = 0;
    int i = 0;
    int j = 0;
    
    // Algorithm
    for (j=0; j < n; j++) {
        sum_current = 0;
        for (i=0; i < n; i++) {
      if (j == i) 
    sum_current = a[i];
      else 
              sum_current = sum_current + a[i]; 
            if (sum_current > sum_max) {
                sum_max = sum_current;
                max_i = i;
                max_j = j;
            }         
        }
    } 
    if (debug) printf("max_i = %d max_j = %d", max_i, max_j);
    if (debug) printf("sum_max=%d\n",sum_max);

    // write the array, sub_array, and sum_max to oupt file
    write_file(wfile, &a[0], n, max_j, max_i, sum_max); 

    return sum_max;
}

//Algorithm 2: Better Enumeration
int maxsubarray_2(FILE *wfile, int a[], int begin, int n, int wflag)
{
    int sum_current = 0;
    int sum_max = 0;
    int max_i = 0;
    int max_j = 0;
    int i = 0;
    int j = 0;
    
    // Algorithm
    for (j=begin; j < n; j++) {
        sum_current = 0;
        // start with i=j
        for (i=j; i < n; i++) {
      if (j == i) 
    sum_current = a[i];
      else 
              sum_current = sum_current + a[i]; 
            if (sum_current > sum_max) {
                sum_max = sum_current;
                max_i = i;
                max_j = j;
            }         
        }
    } 

    if (!wflag) {
  maximum_i = max_i;
  maximum_j = max_j;
  return sum_max;
    }

    if (debug) printf("max_i = %d max_j = %d", max_i, max_j);
    if (debug) printf("sum_max=%d\n",sum_max);

    // write the array, sub_array, and sum_max to oupt file
    write_file(wfile, &a[0], n, max_j, max_i, sum_max); 

    return sum_max;
}

//Algorithm 3: Divide and Conquer

int max_middle(int a[], int start, int mid, int end)
{
    int sum = 0;
    int left_sum = 0;
    int right_sum = 0 ;
    int i = 0;

    for (i = mid; i >= start; i--)
    {
        sum = sum + a[i];
        if (sum > left_sum) {
            left_sum = sum;
            maximum_i = i;
  }

    }
 
    sum = 0;
    for (i = mid+1; i <= end; i++)
    {
        sum = sum + a[i];
        if (sum > right_sum) {
          right_sum = sum;
          maximum_j = i;
        } 
    }
 
    return left_sum + right_sum;

}


int maxsubarray_3(FILE *wfile, int a[],int n)
{
    int max_sum = 0;
    int sub[MAX_ARRAY_SIZE];
    int size_sub;
    int i;
    int middle;
    int max_begin = 0;
    int max_begin_i = 0;
    int max_begin_j = 0;
    int max_mid = 0;
    int max_mid_i = 0;
    int max_mid_j = 0;
    int max_end = 0;
    int max_end_i = 0;
    int max_end_j = 0;

    middle = ((n-1)/2);
    // first half 
    max_begin = maxsubarray_2(wfile, &a[0], 0, middle, 0);
    max_begin_i = maximum_i;
    max_begin_j = maximum_j; 
    // in between 
    max_mid = max_middle(&a[0],0,middle,n-1);
    max_mid_i = maximum_i;
    max_mid_j = maximum_j; 
    // second half
    // max_end = maxsubarray_2(wfile, &a[0], middle+1, (n-(middle+1)), 0);
    max_end = maxsubarray_2(wfile, &a[0], middle+1, n, 0);
    max_end_i = maximum_i;
    max_end_j = maximum_j; 
    if (debug)
    {
       printf("max_begin_i= %d max_begin_j = %d\n" , max_begin_i, max_begin_j);
       printf("max_mid_i= %d max_mid_j = %d\n" , max_mid_i, max_mid_j);
       printf("max_end_i= %d max_end_j = %d\n" , max_end_i, max_end_j);
    }

    printf("max_begin=%d, max_end = %d, max_mid=%d\n", \
  max_begin, max_end, max_mid);
    if ((max_begin >= max_end) && (max_begin >= max_mid))
    {
        // max is in the begining
  if (debug) printf("beginning\n");
        write_file(wfile, &a[0], n, max_begin_j, max_begin_i, max_begin);
        return max_begin;
    }

    if ((max_end >= max_begin) && (max_end >= max_mid))
    {
        // max is in the end
  if (debug) printf("end\n");
        write_file(wfile, &a[0], n, max_end_j, max_end_i, max_end);
        return max_end;
    }
    
     // max is in the middle
     if (debug) printf("emiddle\n");
     write_file(wfile, &a[0], n, max_mid_i, max_mid_j, max_mid);
     return max_mid;
}

// Algorithm 4: Linear-time
int maxsubarray_4(FILE *wfile, int a[],int n)
{
  int max_real = 0;
  int max = 0;
  int i;
  int max_i = 0;

  // Algorithm
  for(i = 0; i < n; i++) {
    max = max + a[i];
    if (max < 0)
      max = 0;
    if (max_real < max) {
      max_real = max;
      max_i = i;
    }
  }

  // find the start of the sub array
  if (debug) printf("max_i=%d\n", max_i);
  for (max=max_real, i = max_i; i > 0; i--) {
    max = max - a[i];
    if (debug) printf("max=%d", max);
    if (max <= 0) {
      break;
    }
  }

  // write the array, sub_array, and max_real to oupt file
  write_file(wfile, &a[0], n, i, max_i, max_real); 

  return max_real;
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

int main(){

  FILE* rfile;
  FILE* wfile;
  int size = 0;
  int a[MAX_ARRAY_SIZE];
  int eof_flag = 0;

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

    //algorithms for max subarray

    // maxsubarray_1(wfile, a, size);
    // maxsubarray_2(wfile, a, 0, size, 1);
    maxsubarray_3(wfile, a, size);
    // maxsubarray_4(wfile, a, size);

  }

  // TBD close on exits also
  fclose(rfile);
  fclose(wfile);

  return 0;
}
