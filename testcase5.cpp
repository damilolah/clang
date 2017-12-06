#include <stdio.h>
#include <stdlib.h>

int main(){

  int w = 0;
  int x[10][10] = {{},{}};
  int i = 0;
  int j = 0;
  int N = 10;
  for(i = 0; i < N; i++){
  	for(j = 0; j < N; j++){
  		w = w * x[i][j];
  	}
  }
  return 0;
}
