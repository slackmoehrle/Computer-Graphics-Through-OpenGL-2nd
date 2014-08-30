#include <fstream>

#include "getbmp.h"

using namespace std;

// Routine to read an uncompressed 24-bit color RGB bmp file into a 
// 32-bit color RGBA bitmap file (A value being set to 1).
BitMapFile *getbmp(string filename)
{
    int offset, headerSize;
	
	// Initialize bitmap files for RGB (input) and RGBA (output).
	BitMapFile *bmpRGB = new BitMapFile;
	BitMapFile *bmpRGBA = new BitMapFile;

	// Read input bmp file name.
	ifstream infile(filename.c_str(), ios::binary);

	// Get starting point of image data in bmp file.
	infile.seekg(10);
	infile.read((char *)&offset, 4); 

	// Get header size of bmp file.
	infile.read((char *)&headerSize,4);

	// Get image width and height values from bmp file header.
	infile.seekg(18);
	infile.read((char *)&bmpRGB->sizeX, 4);
	infile.read((char *)&bmpRGB->sizeY, 4);

	// Determine the length of zero-byte padding of the scanlines 
	// (each scanline of a bmp file is 4-byte aligned by padding with zeros).
	int padding = (3 * bmpRGB->sizeX) % 4 ? 4 - (3 * bmpRGB->sizeX) % 4 : 0;

	// Add the padding to determine size of each scanline.
	int sizeScanline = 3 * bmpRGB->sizeX + padding;

	// Allocate storage for image in input bitmap file.
	int sizeStorage = sizeScanline * bmpRGB->sizeY;
	bmpRGB->data = new unsigned char[sizeStorage];

	// Read bmp file image data into input bitmap file.
	infile.seekg(offset);
	infile.read((char *) bmpRGB->data , sizeStorage);
 
	// Reverse color values from BGR (bmp storage format) to RGB.
	int startScanline, endScanlineImageData, temp;
	for (int y = 0; y < bmpRGB->sizeY; y++)
	{
       startScanline = y * sizeScanline; // Start position of y'th scanline.
	   endScanlineImageData = startScanline + 3 * bmpRGB->sizeX; // Image data excludes padding.
       for (int x = startScanline; x < endScanlineImageData; x += 3)
	   {
	      temp = bmpRGB->data[x];
		  bmpRGB->data[x] = bmpRGB->data[x+2];
		  bmpRGB->data[x+2] = temp;     
	   }
	}

	// Set image width and height values and allocate storage for image in output bitmap file.
	bmpRGBA->sizeX = bmpRGB->sizeX;
	bmpRGBA->sizeY = bmpRGB->sizeY;
	bmpRGBA->data = new unsigned char[4*bmpRGB->sizeX*bmpRGB->sizeY];

	// Copy RGB data from input to output bitmap files, set output A to 1.
	for(int j = 0; j < 4*bmpRGB->sizeY * bmpRGB->sizeX; j+=4)
	{
		bmpRGBA->data[j] = bmpRGB->data[(j/4)*3];
		bmpRGBA->data[j+1] = bmpRGB->data[(j/4)*3+1];
		bmpRGBA->data[j+2] = bmpRGB->data[(j/4)*3+2];
		bmpRGBA->data[j+3] = 0xFF;
	}

	return bmpRGBA;
}
