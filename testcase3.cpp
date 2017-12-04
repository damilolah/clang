#include <stdio.h>
#include <stdlib.h>



 int u(int a, int b, int c, int d){
    return 0;
  }

int main(){
  int x = 0;
  int k = 0;
  int NK = 10;
  int j = 0; 
  int NJ = 10;
  int i = 0;
  int NI = 10;
  int m = 0;
  int NM = 10;
  int px = 0;
  int py = 0;
  int pz = 0;
 

   for(k = 2; k < NK; k++){
	  z = (k + 1)/(NK + 1);
  	for(j = 2; j < NJ; j++){
		  y = (j+1)/(NJ + 1);
		  for(i = 2; i < NI; i++){
			 x = (i+1)/(NI+1);
			 for( m = 0 ; m<NM ; m++ ) {
				px = (1-x)*u(m,1,j,k) + x*u(m,NI,j,k); 
				py = (1-y)*u(m,x,1,k) + y*u(m,x,NJ,k); 
				pz = (1-z)*u(m,x,j,1) + z*u(m,x,j,NK); 
				u(m,i,j,k) = px+py+pz − px*py − py*pz − pz*px + px*py*pz;
			}
		}
	}
}
  return 0;
}
