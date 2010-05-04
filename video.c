
#include "video.h"
 
int HRES = 0;
int VRES = 0;

static __dpmi_meminfo map; ///< mapping information
static char *base = NULL; ///< apontador genérico para aceder à memória gráfica

/** Enter graphics mode, enabling near pointers and mapping video physical memory
 * to program virtual address.
 *
 * Returns a generic pointer pointing to video memory address or NULL on error. 
 * "mode" specifies the VESA graphics mode to use, and
 * the mapping information is returned through "map".
 *
 * Also initializes two global variables, VRES and HRES,
 */
char * enter_graphics(int mode) {
	unsigned long dosbuf, address=0;

	__dpmi_regs regs; /*__dpmi_regs é uma estrutura que representa os registos do CPU */
	
	__djgpp_nearptr_enable(); /* permite acesso à memória física usando apontadores */
	
	// Obter endereço do video
	dosbuf = __tb & 0xFFFFF; /* dosbuf é a área de transferência */
	dosmemput(&address, 4, dosbuf+40); /*apaga bytes 40 a 43 dessa área*/

	regs.x.ax = 0x4F01; /* VBE get mode info */
	regs.x.di = dosbuf & 0xF; /* LSB da área de transferência */
	regs.x.es = (dosbuf >> 4) & 0xFFFF; /*MSB da área de transferência*/
	regs.x.cx = mode; /*informação sobre o modo 105h*/
	__dpmi_int(0x10, &regs); /* DOS int 10h */
	
	if (regs.h.ah) {
		return NULL;
	} /* tratamento de erro, modo não suportado */

	dosmemget(dosbuf+40, 4, &address); /* lê o endereço da memória vídeo */

	if (!address) {
		return NULL;
	} /* tratamento de erro, modo não suportado */
	
	
	regs.x.ax = 0x4F02; /* registo AX do CPU com valor 0x4f02 */
	regs.x.bx = BIT14ON | mode; /* registo BX do CPU com valor 0x4105 */
	__dpmi_int(0x10, &regs); /* gera interrupção de software 0x10, activando o modo gráfico */
	

	/* endereço físico da memória vídeo, usar o valor correcto */
	map.address = address;

	/* tamanho do bloco de memória, usar apenas o necessário */
	map.size = 0x80000;
	__dpmi_physical_address_mapping(&map); /* video_mem contém o endereço genérico a usar */
	base = (char *)(map.address + __djgpp_conventional_base);
	
	switch (mode) {
		case 0x101:
			HRES = 640;
			VRES = 480;
			break;
		case 0x103:
			HRES = 800;
			VRES = 600;
			break;
		case 0x105:
			HRES = 1024;
			VRES = 768;
			break;
		case 0x107:
			HRES = 1280;
			VRES = 1024;
			break;
		default:
			HRES = 0;
			VRES = 0;
	}
	
	return base;
}

/** Unmap video memory, disable near pointer and returns to text mode
 */
void leave_graphics() {
	__dpmi_regs regs; /*__dpmi_regs é uma estrutura que representa os registos do CPU */

	__dpmi_free_physical_address_mapping(&map); /* liberta mapeamento */
	__djgpp_nearptr_disable();
		
	regs.x.ax = 0x0003; /* registo AX do CPU com valor 0x03 */
	__dpmi_int(0x10, &regs); /* gera interrupção software 0x10, entrando no modo texto */
}

/** Draws a pixel of color "color" at screen coordinates x, y at memory address "base"
 */
void set_pixelb(int x, int y, int color, char *base) {
	if (base != NULL && x >= 0 && y >= 0 && x < HRES && y < VRES)
		*(base + y*HRES + x) = color;
}

void set_pixel(int x, int y, int color) {
	set_pixelb(x, y, color, base);
}

/** Returns the pixel color at screen coordinates x, y at memory address "base"
 */
int get_pixelb(int x, int y, char *base) {
	return *(base + y*HRES + x);
}

int get_pixel(int x, int y) {
	return get_pixelb(x, y, base);
}

/** Set graphics memory at memory address "base" to "color".
 */
void clear_screenb(char color, char *base) {
	int k = 0;
	int i = 0;
	
	for (k = 0; k < HRES; k++) {
		for (i = 0; i < VRES; i++) {
			set_pixelb(k, i, color, base);
		}
	}
}

void clear_screen(char color) {
	clear_screenb(color, base);
}

/** Draw a line of color "color" between point (xi,yi) and (xf,yf) at memory address "base".
	Algoritmo de Bresenham
*/
void draw_line(int xi, int yi, int xf, int yf, int color) {
	int temp;
	int deltax, deltay;
	double error = 0;
	double deltaerr;
	int ystep;
	int y;
	int x;
	Bool steep = abs(yf - yi) > abs(xf - xi);
	if (steep) {
		temp = xi;
		xi = yi;
		yi = temp;
		
		// swap(x1, y1)
		temp = xf;
		xf = yf;
		yf = temp;
	}
	if (xi > xf) {
		temp = xi;
		xi = xf;
		xf = temp;
		
		temp = yi;
		yi = yf;
		yf = temp;
	}
	deltax = xf - xi;
	deltay = abs(yf - yi);
	
	deltaerr = (double)deltay / (double)deltax;

	y = yi;
	if (yi < yf)
		ystep = 1;
	else
		ystep = -1;
		
	for (x = xi; x != xf; x++) {
		if (steep)
			set_pixel(y, x, color);
		else
			set_pixel(x, y, color);
		
		error += deltaerr;
		if (error >= 0.5) {
			y += ystep;
			error -= 1.0;
		}
	}
}

void wait_for_retrace() {
    /* wait until done with vertical retrace */
    while  ((inp(INPUT_STATUS) & VRETRACE)) {};
    /* wait until done refreshing */
    while (!(inp(INPUT_STATUS) & VRETRACE)) {};
}
