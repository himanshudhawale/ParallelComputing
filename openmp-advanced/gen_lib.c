#include <stdlib.h>
#include <stdio.h>


void generateReduceData (int* arr, size_t n) {
  for (size_t i=0; i<n; ++i) {
    if (i%2 ==0 )
      arr[i] = i/2;
    else
      arr[i] = -(i/2);
  }
}


void generateMergeSortData (int* arr, size_t n) {
  for (size_t i=0; i<n; ++i) {
    arr[i] = i;
  }

  for (int i = n-1; i>0; --i) {
    int swapwith = rand()%i;
    int temp = arr[i];
    arr[i] = arr[swapwith];
    arr[swapwith] = temp;
  }  
}


void generatePrefixSumData (int* arr, int n) {
  for (int i=0; i<n; ++i) {
    if (i%2 == 0 )
      arr[i] = i/2;
    else
      arr[i] = -(i/2);
  }
}


void checkPrefixSumResult (const int* arr, size_t n) {
  int correct = 1;

  int *data = malloc(sizeof(int)*n);
  generatePrefixSumData (data, n);

  correct = correct && (arr[0] == 0);

  int sum = 0;
  for (int i=0; i<n; ++i) {
    sum += data[i];
    if (arr[i+1] != sum) {
      correct = 0;
    }
  }
  
  if (correct) {  
    printf ("checked\n");
  }
  else {
    printf ("incorrect\n");
  }
  free(data);
}


//this is based on how MS data is generated
void checkMergeSortResult (const int* arr, size_t n) {
  int correct = 1;
  
  for (int i=0; i< n-1; ++i) {
    correct = correct && (arr[i] == i);
  }

  if (correct) {  
    printf ("checked\n");
  }
  else {
    printf ("incorrect\n");
  }
}


// assumes n and m are powers of 10, 
//
void generateLCS(char* X, int m, char* Y, int n) {

  const char A = 'A'; 
  const char B = 'B';
  const char C = 'C'; 
  
  int x = (n>m) ? m : n;

  // X ~ AABBAAAAAA
  for (int i=0; i<(2*x/10); ++i) {
    X[i] = A;
    X[i+(2*x/10)] = B;
  }
  for (int i=(4*x/10); i<m; ++i) {
    X[i] = A;
  }

  // Y ~ CCCCCCCCBB
  for (int i=0; i<(n-(2*x/10)); ++i) {
    Y[i] = C;
  }
  for (int i=n-(2*x/10); i<n; ++i) {
    Y[i] = B;
  }
}


void checkLCS(char* X, int m, char* Y, int n, int result) {
  int x = (n>m) ? m : n;

  if ((x*2/10) == result) {
    printf ("checked\n");
  }
  else {
    printf ("incorrect\n");
  }
}
