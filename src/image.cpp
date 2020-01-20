#include<iostream>
#include<vector>
#include<string>
#include<jpeglib.h>
#include<fstream>
#include<cassert>

#include "image.h"

Image::Image (char * filename) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    /* More stuff */
    FILE * infile;                /* source file */
    JSAMPARRAY buffer;            /* Output row buffer */
    int row_stride;               /* physical row widthdd in output buffer */

    if ((infile = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    this->state = -1; 
    return;
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);


    (void) jpeg_read_header(&cinfo, TRUE);

    (void) jpeg_start_decompress(&cinfo);
    this->width  = cinfo.output_width;
    this->height = cinfo.output_height; 
    this->stride = this->width * cinfo.output_components;
    this->bitmap.reserve(this->height);

    while (cinfo.output_scanline < this->height) {
        std::vector<uint8_t> line(this->stride);
        uint8_t *buffer = line.data();
            jpeg_read_scanlines(&cinfo, &buffer, 1);
        this->bitmap.push_back(line);
    }


    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    fclose(infile);

    this->state = 1;
    this->format = "RGB";
}

void Image::blur_image(std::vector<std::vector<float>>& kernel){
	int k_height = kernel.size();
	assert(k_height);
	int k_width  = kernel[0].size();
	assert(k_width);
	std::vector<std::vector<uint8_t>> out;
	for(int i = 0; i< this->height; i++) {
		std::vector<uint8_t> out_row(this->stride);
		for(int j = 0; j< this->width; j++) {
			float val[3] = {0.f, 0.f, 0.f};
			float wt  = 0.f;
			for(int k_i = 0; k_i < k_height; k_i++)
			for(int k_j = 0; k_j< k_width; k_j++){
				int x = i - k_i + k_height/2;
				int y = j - k_j + k_width/2;
				if(x> 0 && y>0 && x< this->height && y< this->width) {
					val[0] += this->bitmap[x][y*3 + 0] * kernel[k_i][k_j];
					val[1] += this->bitmap[x][y*3 + 1] * kernel[k_i][k_j];
					val[2] += this->bitmap[x][y*3 + 2] * kernel[k_i][k_j];
					wt  += kernel[k_i][k_j];
				}
			}
			out_row[j*3 + 0] = val[0]/ wt;
			out_row[j*3 + 1] = val[1]/ wt;
			out_row[j*3 + 2] = val[2]/ wt;
		}
		out.push_back(out_row);
	}
	this->bitmap = out;
	return;
}

void Image::write_ppm(const std::string filename) {

    std::ofstream ofs( filename, std::ios::out | std::ios::binary );
    if ( ! ofs )
        throw std::runtime_error("Could not open " + filename + " for saving");

    //header
    ofs << "P6 " << width << " " << height << " 255\n";
    for ( auto& v : bitmap )
    {
        ofs.write( reinterpret_cast<const char *>(v.data()), v.size() );
    }
    ofs.close();
}

void Image::print_info(){
	std::cout<<"\n---- IMAGE INFO ----\n";
	std::cout<<"Height: "<<this->height<<"\nWidth: "<<this->width<<"\nFormat: "<<this->format;
	std::cout<<"\n--------------------\n";
}
