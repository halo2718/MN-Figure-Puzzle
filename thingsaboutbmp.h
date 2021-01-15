#pragma pack(2)

struct puzzle_info
{
	int number;	
};

struct bmp_header
{
	unsigned short btype;
	unsigned int size;
	unsigned short reserve1;
	unsigned short reserve2;
	unsigned int startposit;
};

struct BMPinfoheader
{
	unsigned int ssize;
	int width;
	int height;
	unsigned short planes;
	unsigned short bitcount;
	unsigned int compression;
	unsigned int sizeimage;
	int xpixel;
	int ypixel;
	unsigned int used;
	unsigned int important;
};

struct RGB_space
{
	unsigned short B;
	unsigned short G;
	unsigned short R;
}image[50000000],imagecpy[5000000];

