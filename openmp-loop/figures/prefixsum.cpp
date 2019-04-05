void prefixsum (int* arr, int n, int* pr) {
  pr[0] = 0;

  for (int i=0; i<n; ++i) {
    pr[i+1] = pr[i] + arr[i];
  }
}
