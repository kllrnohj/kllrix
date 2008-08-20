#include "video.h"

#define COLOR_BLACK 0
#define COLOR_GRAY 3
#define COLOR_RED 4
#define COLOR_PURPLE 5

static unsigned int screen_x = 0;
static unsigned int screen_y = 0;
extern char* pVGAVirtualPointer;
//static unsigned char* VGA = getVGAVirtualPointer();

static int MiscOutputReg = 0x3c2;
static int DataReg = 0x3c0;
static int AddressReg = 0x3c0;

static char mode13[][32] = {
  { 0x03, 0x01, 0x0f, 0x00, 0x0e },            // 0x3c4, index 0-4
  { 0x5f, 0x4f, 0x50, 0x82, 0x54, 0x80, 0xbf, 0x1f,
    0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x9c, 0x0e, 0x8f, 0x28, 0x40, 0x96, 0xb9, 0xa3,
    0xff },                                    // 0x3d4, index 0-0x18
  { 0, 0, 0, 0, 0, 0x40, 0x05, 0x0f, 0xff },   // 0x3ce, index 0-8
  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
    0x41, 0, 0x0f, 0, 0 }                      // 0x3c0, index 0-0x14
};

char console[2000]; // the actual console for VGA mode
// consider making it 4000 for buffer?

struct FONT_CHAR{
	// images are 4px by 8px
	unsigned char image[32];
};

void drawChar(struct FONT_CHAR toDraw){
	unsigned short px, py, index;
	px = screen_x * 2;
	screen_x += 2;
	py = screen_y * 8;
	index = (py << 8) + (py << 6) + px;
	int x;
	int y;
	for(y = 0; y < 8; y++){
		for(x = 0; x < 4; x++){
			if(toDraw.image[(y * 4) + x]){
				putPixel(px + x, py + y, COLOR_GRAY);
			}else{
				putPixel(px + x, py + y, COLOR_BLACK);
			}
		}
	}
}

struct FONT_CHAR getCharacterImage(unsigned char character){
	struct FONT_CHAR result;
	switch(character){
		case '`':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '1':
		{
			char temp[32] = {
				0, 1, 0, 0,
				1, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '2':
		{
			char temp[32] = {
				0, 1, 1, 0,
				1, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 1, 0, 0,
				1, 0, 0, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '3':
		{
			char temp[32] = {
				1, 1, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				1, 1, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '4':
		{
			char temp[32] = {
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '5':
		{
			char temp[32] = {
				1, 1, 1, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 1, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '6':
		{
			char temp[32] = {
				1, 1, 1, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 1, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '7':
		{
			char temp[32] = {
				1, 1, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '8':
		{
			char temp[32] = {
				1, 1, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '9':
		{
			char temp[32] = {
				1, 1, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '0':
		{
			char temp[32] = {
				0, 1, 0, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				0, 1, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '-':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '=':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '\\':
		{
			char temp[32] = {
				1, 0, 0, 0,
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '[':
		{
			char temp[32] = {
				1, 1, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 1, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case ']':
		{
			char temp[32] = {
				0, 1, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case ';':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				1, 0, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '\'':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case ',':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 1, 0, 0,
				1, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '.':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '/':
		{
			char temp[32] = {
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '~':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 1, 0, 1,
				1, 0, 1, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '!':
		{
			char temp[32] = {
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '@':
		{
			char temp[32] = {
				1, 1, 1, 1,
				1, 0, 0, 1,
				1, 0, 0, 1,
				1, 0, 0, 1,
				1, 0, 1, 1,
				1, 0, 1, 1,
				1, 0, 1, 1,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '#':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 1, 1, 0,
				1, 1, 1, 1,
				0, 1, 1, 0,
				1, 1, 1, 1,
				0, 1, 1, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '$':
		{
			char temp[32] = {
				0, 1, 0, 0,
				1, 1, 1, 0,
				1, 0, 0, 0,
				1, 1, 1, 0,
				0, 0, 1, 0,
				1, 1, 1, 0,
				0, 1, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '%':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 0, 1,
				0, 1, 1, 0,
				1, 0, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '^':
		{
			char temp[32] = {
				0, 1, 0, 0,
				1, 0, 1, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '&':
		{
			char temp[32] = {
				0, 0, 0, 0,
				1, 1, 1, 0,
				1, 0, 0, 0,
				1, 0, 0, 1,
				1, 1, 0, 1,
				1, 0, 1, 0,
				1, 1, 0, 1,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '*':
		{
			char temp[32] = {
				1, 1, 0, 1,
				0, 1, 1, 0,
				0, 1, 1, 0,
				1, 0, 1, 1,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '(':
		{
			char temp[32] = {
				0, 0, 1, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case ')':
		{
			char temp[32] = {
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 1, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '_':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 1, 1,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '+':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				1, 1, 1, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '|':
		{
			char temp[32] = {
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '{':
		{
			char temp[32] = {
				0, 1, 1, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				1, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '}':
		{
			char temp[32] = {
				1, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 1, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				1, 1, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case ':':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 0, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '"':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 1, 1, 0,
				0, 1, 1, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '<':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 1, 0,
				0, 1, 0, 0,
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '>':
		{
			char temp[32] = {
				0, 0, 0, 0,
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 1, 0, 0,
				1, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case '?':
		{
			char temp[32] = {
				1, 1, 1, 0,
				1, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 1, 0, 0,
				0, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case ' ':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'a':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 1, 0,
				0, 0, 1, 0,
				1, 1, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'b':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 1, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'c':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 1, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'd':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				1, 1, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'e':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				1, 0, 0, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'f':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 1, 0,
				1, 0, 0, 0,
				1, 1, 1, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'g':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				1, 1, 1, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'h':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 1, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'i':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'j':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				1, 1, 1, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'k':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 0, 1, 0,
				1, 1, 0, 0,
				1, 0, 0, 0,
				1, 1, 0, 0,
				1, 0, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'l':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 1, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'm':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'n':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 0, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'o':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'p':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'q':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'r':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 1, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 's':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 1, 0,
				1, 0, 0, 0,
				1, 1, 1, 0,
				0, 0, 1, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 't':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				1, 1, 1, 0,
				0, 1, 0, 0,
				0, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'u':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'v':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				0, 1, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'w':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				1, 0, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'x':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 0, 1, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				1, 0, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'y':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				1, 1, 1, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'z':
		{
			char temp[32] = {
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 1, 0,
				0, 0, 1, 0,
				1, 1, 1, 0,
				1, 0, 0, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'A':
		{
			char temp[32] = {
				0, 1, 0, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'B':
		{
			char temp[32] = {
				1, 1, 0, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 1, 0, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 1, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'C':
		{
			char temp[32] = {
				0, 1, 1, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				0, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'D':
		{
			char temp[32] = {
				1, 1, 0, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 1, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'E':
		{
			char temp[32] = {
				1, 1, 1, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 1, 1, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'F':
		{
			char temp[32] = {
				1, 1, 1, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 1, 1, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'G':
		{
			char temp[32] = {
				1, 1, 1, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 1, 1,
				1, 0, 1, 1,
				1, 1, 1, 1,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'H':
		{
			char temp[32] = {
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'I':
		{
			char temp[32] = {
				1, 1, 1, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'J':
		{
			char temp[32] = {
				1, 1, 1, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				1, 1, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'K':
		{
			char temp[32] = {
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 1, 0, 0,
				1, 0, 0, 0,
				1, 1, 0, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'L':
		{
			char temp[32] = {
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'M':
		{
			char temp[32] = {
				1, 0, 1, 0,
				1, 1, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'N':
		{
			char temp[32] = {
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				1, 1, 1, 0,
				1, 1, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'O':
		{
			char temp[32] = {
				1, 1, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'P':
		{
			char temp[32] = {
				1, 1, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'Q':
		{
			char temp[32] = {
				1, 1, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 1,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'R':
		{
			char temp[32] = {
				1, 1, 0, 0,
				1, 0, 1, 0,
				1, 1, 0, 0,
				1, 1, 0, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'S':
		{
			char temp[32] = {
				0, 1, 1, 0,
				1, 0, 0, 0,
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				1, 1, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'T':
		{
			char temp[32] = {
				1, 1, 1, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'U':
		{
			char temp[32] = {
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'V':
		{
			char temp[32] = {
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				0, 1, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'W':
		{
			char temp[32] = {
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 0,
				1, 0, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'X':
		{
			char temp[32] = {
				1, 0, 1, 0,
				1, 0, 1, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'Y':
		{
			char temp[32] = {
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 0, 1, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		case 'Z':
		{
			char temp[32] = {
				1, 1, 1, 0,
				0, 0, 1, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				1, 0, 0, 0,
				1, 1, 1, 0,
				0, 0, 0, 0,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
		default:
		{
			char temp[32] = {
				1, 0, 1, 1,
				1, 0, 1, 0,
				1, 0, 1, 0,
				1, 1, 1, 1,
				0, 1, 0, 1,
				0, 1, 0, 1,
				0, 1, 0, 1,
				1, 1, 0, 1,
			};
			int x;
			for(x = 0; x < 32; x++)
				result.image[x] = temp[x];
		}break;
	}
	return result;
}

void kgetXY(void){
	unsigned char* biosxy = (unsigned char*)0x450;
	screen_x = 0;
	screen_y = (*(biosxy + 1)) + 1;
}

static void copy(void* dest, void* src, int len){
	int x;
	for(x = 0; x < len; x++){
		((char*)dest)[x] = ((char*)src)[x];
	}
}

static void kprint_scroll(void){
	// this functions scrolls the display upwards

	// so start with moving the previous text up
	copy(pVGAVirtualPointer, (void*)(pVGAVirtualPointer + (320 * 8)), (64000-(320 * 8)));
	//copy((void*)0xB8000, (void*)(0xB8000 + 160), 3840);
	// 0xB8000 is video memory
	// 0xB8000 + 160 = 80 * 2 because each character is a byte,
	// then its attribute is another byte, display is 80 characters wide
	// 3840 = 160 * 24 where 24 is height of display minus 1, 160 like before

	// clear bottom line
	int x;
	int y;
	for(x = 0; x < 320; x++)
		for(y = 0; y < 8; y++)
			putPixel(x, y+(200 - 8), 0);
	//unsigned int* tmp = (unsigned int*)(0xB8000 + (160 * 24));
	//unsigned int i;
	//for(i = 0; i < (80 / 2); i++){
	//	// i is 4 bytes, so thats why 80/2, we do two at once
	//	tmp[i] = ((7 | ' ') << 16) | (7 | ' '); // see? 4 bytes
	//}
}

void kprint_putchar(int c){
	switch(c){
		case 0:
		{
		}return;
		case '\b':
		{
			if(screen_x >= 2)
				screen_x -= 2;
			else{
				screen_x = 0;
				screen_y--;
			}
			drawChar(getCharacterImage(' '));
			screen_x-=2;
			/*
			int x = 1;
			unsigned char doLoop = 0;
			if(*(unsigned short*)(0xB8000 + (screen_y * 160) + screen_x-2) == 0)
				doLoop = 1;
			while((*(unsigned short*)(0xB8000 + (screen_y * 160) + screen_x-2) == 0 && doLoop) || x-- == 1){
				if(screen_x > 0){
					screen_x -= 2;
					*(unsigned short*)(0xB8000 + (screen_y * 160) + screen_x) = 0;
				}else if(screen_x == 0 && screen_y == 0){
					// do nothing, because there are no characters
					break;
				}else{
					screen_x = 158;
					screen_y--;
					*(unsigned short*)(0xB8000 + (screen_y * 160) + screen_x) = 0;
				}
			}
			*/
		}break;
		case '\n':
		{
			screen_x = 0;
			if(screen_y == 24)
				kprint_scroll();
			else
				screen_y++;
		}break;
		case '\t':
		{
			screen_x += 4-(screen_x % 4);
			if(screen_x > 158){
				screen_x = 0;
				if(screen_y == 24)
					kprint_scroll();
				else
					screen_y++;	
			}
		}break;
		default:
		{
			struct FONT_CHAR fc = getCharacterImage(c);
			drawChar(fc);
			/*
			*(unsigned short*)(0xB8000 + (screen_y * 160) + screen_x) = (c | (7<<8)); // 7 << 8 is for attribute
			screen_x += 2;*/
	
			if(screen_x > 158){
				screen_x = 0;
		
				if(screen_y == 24){
					kprint_scroll();
				}else{
					screen_y++;
				}
			}
		}break;
	}
}

void kprint(const char* str){ // eventually add argument list
	for(; *str != 0; str++){
		kprint_putchar(*str);
	}
}

void kprintSN(int num, unsigned int radix){
	if(num < 0)
		kprint_putchar('-');
	unsigned int realNum = -num;

	char buffer[33];
	char* where = &buffer[32]; // point to last spot in buffer
	
	*where = 0; // nullify last position

	do{
		unsigned int temp;
		temp = realNum % radix;
		where--; // do this first because we start at last which is now \0
		if(temp < 10)
			*where = temp + '0';
		else
			*where = (temp - 10) + 'A'; // hex

		realNum = (unsigned int)(realNum / radix);
	}while(realNum != 0);

	while(*where != 0){
		kprint_putchar(*where);
		where++;
	}
}

void kprintUN(unsigned int num, unsigned int radix){
	unsigned int realNum = num;

	char buffer[33];
	char* where = &buffer[32]; // point to last spot in buffer
	
	*where = 0; // nullify last position

	do{
		unsigned int temp;
		temp = realNum % radix;
		where--; // do this first because we start at last which is now \0
		if(temp < 10)
			*where = temp + '0';
		else
			*where = (temp - 10) + 'A'; // hex

		realNum = (unsigned int)(realNum / radix);
	}while(realNum != 0);

	while(*where != 0){
		kprint_putchar(*where);
		where++;
	}
}

void clrscr(void){
	// 25 rows by 80 cols
	// 2 bytes a char
	screen_x = 0;
	screen_y = 0;
	
	unsigned int x;
	for(x = 0; x < 64000; x++){
		pVGAVirtualPointer[x] = COLOR_BLACK;
	}
}

void putPixel(int x, int y, unsigned char color){
	/* y << 8 = y * (2 ^ 8) and y << 6 = y * (2^6) = y * 320 (320 * 200) */
	pVGAVirtualPointer[(y << 8) + (y << 6) + x] = color;
}

void setMode(int xres, int yres, int bitdepth){
	int i;
	  
	#define outb_p(a, b) outb(b, a)
	#define outw_p(a, b) outw(b, a)
	#define inb_p(a) inb(a)

	outb_p(0x63, MiscOutputReg);
	outb_p(0x00, 0x3da);

	for (i=0; i < 5; i++) {
		outb_p(i, 0x3c4);
		outb_p(mode13[0][i], 0x3c4+1);
	}
	outw_p(0x0e11, 0x3d4);

	for (i=0; i < 0x19; i++) {
		outb_p(i, 0x3d4);
		outb_p(mode13[1][i], 0x3d4+1);
	}  

	for (i=0; i < 0x9; i++) {
		outb_p(i, 0x3ce);
		outb_p(mode13[2][i], 0x3CF);
	}

	inb_p(0x3da);

	for (i=0; i < 0x15; i++) {
		inw(DataReg);
		outb_p(i, AddressReg);
		outb_p(mode13[3][i], DataReg);
	}

	outb_p(0x20, 0x3c0);
	  
	int x;
	for(x = 0; x < 64000; x++)
		pVGAVirtualPointer[x] = 0;
}
