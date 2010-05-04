
#include "video.h"
 
int HRES = 0;
int VRES = 0;

static __dpmi_meminfo map; ///< mapping information
static char *base = NULL; ///< apontador gen�rico para aceder � mem�ria gr�fica

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

	__dpmi_regs regs; /*__dpmi_regs � uma estrutura que representa os registos do CPU */
	
	__djgpp_nearptr_enable(); /* permite acesso � mem�ria f�sica usando apontadores */
	
	// Obter endere�o do video
	dosbuf = __tb & 0xFFFFF; /* dosbuf � a �rea de transfer�ncia */
	dosmemput(&address, 4, dosbuf+40); /*apaga bytes 40 a 43 dessa �rea*/

	regs.x.ax = 0x4F01; /* VBE get mode info */
	regs.x.di = dosbuf & 0xF; /* LSB da �rea de transfer�ncia */
	regs.x.es = (dosbuf >> 4) & 0xFFFF; /*MSB da �rea de transfer�ncia*/
	regs.x.cx = mode; /*informa��o sobre o modo 105h*/
	__dpmi_int(0x10, &regs); /* DOS int 10h */
	
	if (regs.h.ah) {
		return NULL;
	} /* tratamento de erro, modo n�o suportado */

	dosmemget(dosbuf+40, 4, &address); /* l� o endere�o da mem�ria v�deo */

	if (!address) {
		return NULL;
	} /* tratamento de erro, modo n�o suportado */
	
	
	regs.x.ax = 0x4F02; /* registo AX do CPU com valor 0x4f02 */
	regs.x.bx = BIT14ON | mode; /* registo BX do CPU com valor 0x4105 */
	__dpmi_int(0x10, &regs); /* gera interrup��o de software 0x10, activando o modo gr�fico */
	

	/* endere�o f�sico da mem�ria v�deo, usar o valor correcto */
	map.address = address;

	/* tamanho do bloco de mem�ria, usar apenas o necess�rio */
	map.size = 0x80000;
	__dpmi_physical_address_mapping(&map); /* video_mem cont�m o endere�o gen�rico a usar */
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
	__dpmi_regs regs; /*__dpmi_regs � uma estrutura que representa os registos do CPU */

	__dpmi_free_physical_address_mapping(&map); /* liberta mapeamento */
	__djgpp_nearptr_disable();
		
	regs.x.ax = 0x0003; /* registo AX do CPU com valor 0x03 */
	__dpmi_int(0x10, &regs); /* gera interrup��o software 0x10, entrando no modo texto */
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
