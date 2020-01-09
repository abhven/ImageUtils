#ifndef IMAGE_H
#define IMAGE_H

#include<vector>
#include<string>
#include<jpeglib.h>
#include<string>
#include<fstream>

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
	int blur_image(std::vector<std::vector<float>> kernel);

};

void Image::print_info(){
	std::cout<<"\n---- IMAGE INFO ----\n";
	std::cout<<"Height: "<<this->height<<"\nWidth: "<<this->width<<"\nFormat: "<<this->format;
	std::cout<<"\n--------------------\n";
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

Image::Image (char * filename)
{
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

#endif  /* IMAGE_H */
