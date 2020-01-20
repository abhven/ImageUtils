#ifndef IMAGE_H
#define IMAGE_H

#include<vector>
#include<string>

class Image{
    private:
		std::vector< std::vector <uint8_t>> bitmap;
		size_t     width;  /* Width of the image  */
		size_t     height; /* Height of the image */
		size_t     stride; /* physical row width in output buffer. Typically for RGB image stride is 3*width */
		std::string 	   format; /* format of the bitmap */
		int        state;
    public:
		Image (char * filename);
		void print_info();
		void write_ppm(const std::string filename);
		void blur_image(std::vector<std::vector<float>>& kernel);
};

#endif  /* IMAGE_H */
