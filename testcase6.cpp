#include <stdio.h>
#include <stdlib.h>

int main(){

  int w = 0;
  int z = 0;
  int x[] = {};
  int y[][]={{},{}};
  int i = 0;
  int N = 10;
  for(i = 0; i < N; i++){
   		w = w + x[i];
   		z = w + 1;
   		for(j = 0; j < N; j++){
  		w = w + y[i][j];
  	}
  	
  }
  return 0;
}