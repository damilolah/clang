#include <stdio.h>
#include <stdlib.h>

int main(){

  int w = 0;
  int d;
  int a[3] = {2,1,1};
  int b[3] = {3,1,1};
  int le =15;
  int N = 3;
  w = w+1;
  int i;
   while(true) {
   	d = 0;
   	for(i = 0; i < N; i++){
   		d += a[i] + b[i] * w;
   	}
  	if(d<le-8) break;
  	w = w + d;
  }
  return 0;
}