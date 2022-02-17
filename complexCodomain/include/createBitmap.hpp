#ifndef _CREATE_BITMAP_H
#define _CREATE_BITMAP_H

#include <cstdio>
#include <cstdint>
#include <string>

#pragma pack(push, 2)

struct BITMAPFILEHEADER{
	uint16_t type; 			//type of file, should be BM for bitmap
	uint32_t size; 			//size of file, set at end
	uint16_t reserved1;		//
	uint16_t reserved2;		//
	uint32_t offset; 		//offset should be 14+40=54 bytes
};

struct BITMAPINFOHEADER{
	uint32_t size; 			//size of the info header i.e sizeof(BITMAPINFOHEADER) should be 40
	uint32_t width; 		//
	uint32_t height;		//
	uint16_t planes; 		//one plane as defualt for test images
	uint16_t bitcount;	 	//should be 24bit for 3channel rgb
	uint32_t compression;	 	//set as 0 for no compression
	uint32_t imagesize;		//
	uint32_t x_pixels_per_meter;	//
	uint32_t y_pixels_per_meter;	//
	uint32_t color_used;	 	//color index used, but we arent using one so its 0
	uint32_t color_important; 	//also zero

};

struct Pixel {
	uint8_t blue;
	uint8_t green;
	uint8_t red;
};

#pragma pack(pop)

void writeBitmap(int h, int w, Pixel* pixarray, std::string fn){
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	size_t i, bytes;

	unsigned int padding = (4 - (w * sizeof(Pixel)) % 4) % 4;
	if(padding > 3 || padding < 0){
		printf("Bitmap Padding calculated incorecctly, calculated as %d, should be 0, 1, 2, or 3\n", padding);
		return;
	}


	bfh.type = 19778;
	bfh.size = 54 + (h * w) + (padding * h);
	bfh.reserved1 = 0;
	bfh.reserved2 = 0;
	bfh.offset = 54;
	bih.size = 40;
	bih.width = w;
	bih.height = h;
	bih.planes = 1;
	bih.bitcount = 24;
	bih.compression = 0;
	bih.imagesize = 0;
	bih.color_important = 0;


	FILE* fout;
	fout = fopen(fn.c_str(), "wb");

	bytes = fwrite(&bfh, 1, sizeof(bfh), fout);
	if(bytes != sizeof(bfh)) printf("bfh broke");

	bytes = fwrite(&bih, 1, sizeof(bih), fout);
	if(bytes != sizeof(bih)) printf("bih broke");

	fseek(fout, bfh.offset, SEEK_SET);

	uint32_t c = 0;
	for(i = 0; i < h; i++){
		bytes = fwrite(pixarray + (i * w), 1, w * sizeof(Pixel), fout);
		if(bytes != (size_t)(w * sizeof(Pixel))) printf("pix at %lu broke", i);

		bytes = fwrite(&c, 1, padding, fout);
		if(bytes != padding) printf("pix at %lu broke", i);
	}

	fclose(fout);
}

#endif
