#include <iostream>
#include "createBitmap.hpp"
using namespace std;

int main(){
	int ht = 500;
	int wd = 500;
	uint8_t image[ht][wd][3];


	for(int i = 0; i < ht; i++){
		for(int j = 0; j < wd; j++){
			//cout << "";
			image[i][j][2] = (uint8_t)(i*255/ht);
			image[i][j][1] = (uint8_t)(j*255/wd);
			image[i][j][0] = (uint8_t)((i+j)*255/(ht+wd));
		}
	}

	unsigned char* ret = createBitmapInMemory(wd,ht,3,(uint8_t*)image);

	return 0;
}
