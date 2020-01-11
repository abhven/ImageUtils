#include<iostream>
#include<jpeglib.h>
#include<string>
#include<stdio.h>
#include"image.h"

using namespace std;

int main(int argc, char *argv[])
{
	if(argc > 1) {
		Image im(argv[1]);
		im.print_info();
		if(argc > 2){
			vector<vector<float>> kern = {{0.1f, 0.2f, 0.3f, 0.6f, 0.3f, 0.2f, 0.1f},
						      {0.2f, 0.3f, 0.6f, 0.8f, 0.6f, 0.3f, 0.2f},
						      {0.3f, 0.6f, 0.8f, 0.9f, 0.8f, 0.6f, 0.3f},
						      {0.6f, 0.8f, 0.9f, 1.0f, 0.9f, 0.8f, 0.6f},
						      {0.3f, 0.6f, 0.8f, 0.9f, 0.8f, 0.6f, 0.3f},
						      {0.2f, 0.3f, 0.6f, 0.8f, 0.6f, 0.3f, 0.2f},
						      {0.1f, 0.2f, 0.3f, 0.6f, 0.3f, 0.2f, 0.1f}};
			im.blur_image(kern);
			im.blur_image(kern);
			im.write_ppm(argv[2]);
		}
	}
	return 0;
}


