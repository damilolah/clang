#include <stdio.h>
#include <stdlib.h>

int main(){
  int w[] = {};
  int r, jj, k, i, S, l, j, m, ii;
  int D = 10;
  int NK = 10;
  int M = 10;
  int d[] = {};
  int x[][] = {{},{}};
  int le =15;
  int alpha = 0.5;
  int mu = 1;
 

int r = −alpha/mu;
  for(jj = 0; jj <D; jj++ ){
    w[jj] = 0;
  }
  for(k = 0; k < NK; k++){
    for(j = 0; j < D; j++){ 
      d[j] = 0;
    }
    for (i = 0; i <M; i++){
      S=0;
      for(l = 0; j < D; j++){
        S = S + x[i][l] * w[l]; 
      }
      for (m = 0; m < D; m++){
        d[m] = d[m] + S * x[i][m];
      } 
      
    }// i loop
    for(ii = 0; ii < D; j++){
      w[ii] = w[ii] + r * d[ii];
    }
 }

  return 0;
}
