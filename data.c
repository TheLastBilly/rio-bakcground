#include <u.h>
#include <stdio.h>
#include <libc.h>
#include <draw.h>
#include <thread.h>
#include <cursor.h>
#include <mouse.h>
#include <keyboard.h>
#include <frame.h>
#include <fcall.h>
#include "dat.h"
#include "fns.h"

Cursor crosscursor = {
	{-7, -7},
	{0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0,
	 0x03, 0xC0, 0x03, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF,
	 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0xC0, 0x03, 0xC0,
	 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, },
	{0x00, 0x00, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
	 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x7F, 0xFE,
	 0x7F, 0xFE, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
	 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x00, 0x00, }
};

Cursor boxcursor = {
	{-7, -7},
	{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	 0xFF, 0xFF, 0xF8, 0x1F, 0xF8, 0x1F, 0xF8, 0x1F,
	 0xF8, 0x1F, 0xF8, 0x1F, 0xF8, 0x1F, 0xFF, 0xFF,
	 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, },
	{0x00, 0x00, 0x7F, 0xFE, 0x7F, 0xFE, 0x7F, 0xFE,
	 0x70, 0x0E, 0x70, 0x0E, 0x70, 0x0E, 0x70, 0x0E,
	 0x70, 0x0E, 0x70, 0x0E, 0x70, 0x0E, 0x70, 0x0E,
	 0x7F, 0xFE, 0x7F, 0xFE, 0x7F, 0xFE, 0x00, 0x00, }
};

Cursor sightcursor = {
	{-7, -7},
	{0x1F, 0xF8, 0x3F, 0xFC, 0x7F, 0xFE, 0xFB, 0xDF,
	 0xF3, 0xCF, 0xE3, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF,
	 0xFF, 0xFF, 0xFF, 0xFF, 0xE3, 0xC7, 0xF3, 0xCF,
	 0x7B, 0xDF, 0x7F, 0xFE, 0x3F, 0xFC, 0x1F, 0xF8, },
	{0x00, 0x00, 0x0F, 0xF0, 0x31, 0x8C, 0x21, 0x84,
	 0x41, 0x82, 0x41, 0x82, 0x41, 0x82, 0x7F, 0xFE,
	 0x7F, 0xFE, 0x41, 0x82, 0x41, 0x82, 0x41, 0x82,
	 0x21, 0x84, 0x31, 0x8C, 0x0F, 0xF0, 0x00, 0x00, }
};

Cursor whitearrow = {
	{0, 0},
	{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFC, 
	 0xFF, 0xF0, 0xFF, 0xF0, 0xFF, 0xF8, 0xFF, 0xFC, 
	 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFC, 
	 0xF3, 0xF8, 0xF1, 0xF0, 0xE0, 0xE0, 0xC0, 0x40, },
	{0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x06, 0xC0, 0x1C, 
	 0xC0, 0x30, 0xC0, 0x30, 0xC0, 0x38, 0xC0, 0x1C, 
	 0xC0, 0x0E, 0xC0, 0x07, 0xCE, 0x0E, 0xDF, 0x1C, 
	 0xD3, 0xB8, 0xF1, 0xF0, 0xE0, 0xE0, 0xC0, 0x40, }
};

Cursor query = {
	{-7,-7},
	{0x0f, 0xf0, 0x1f, 0xf8, 0x3f, 0xfc, 0x7f, 0xfe, 
	 0x7c, 0x7e, 0x78, 0x7e, 0x00, 0xfc, 0x01, 0xf8, 
	 0x03, 0xf0, 0x07, 0xe0, 0x07, 0xc0, 0x07, 0xc0, 
	 0x07, 0xc0, 0x07, 0xc0, 0x07, 0xc0, 0x07, 0xc0, },
	{0x00, 0x00, 0x0f, 0xf0, 0x1f, 0xf8, 0x3c, 0x3c, 
	 0x38, 0x1c, 0x00, 0x3c, 0x00, 0x78, 0x00, 0xf0, 
	 0x01, 0xe0, 0x03, 0xc0, 0x03, 0x80, 0x03, 0x80, 
	 0x00, 0x00, 0x03, 0x80, 0x03, 0x80, 0x00, 0x00, }
};

Cursor tl = {
	{-4, -4},
	{0xfe, 0x00, 0x82, 0x00, 0x8c, 0x00, 0x87, 0xff, 
	 0xa0, 0x01, 0xb0, 0x01, 0xd0, 0x01, 0x11, 0xff, 
	 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 
	 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0x1f, 0x00, },
	{0x00, 0x00, 0x7c, 0x00, 0x70, 0x00, 0x78, 0x00, 
	 0x5f, 0xfe, 0x4f, 0xfe, 0x0f, 0xfe, 0x0e, 0x00, 
	 0x0e, 0x00, 0x0e, 0x00, 0x0e, 0x00, 0x0e, 0x00, 
	 0x0e, 0x00, 0x0e, 0x00, 0x0e, 0x00, 0x00, 0x00, }
};

Cursor skull = {
	{-7,-7},
	{0x00, 0x00, 0x00, 0x00, 0xc0, 0x03, 0xe7, 0xe7, 
	 0xff, 0xff, 0xff, 0xff, 0x3f, 0xfc, 0x1f, 0xf8, 
	 0x0f, 0xf0, 0x3f, 0xfc, 0xff, 0xff, 0xff, 0xff, 
	 0xef, 0xf7, 0xc7, 0xe3, 0x00, 0x00, 0x00, 0x00,},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x03,
	 0xE7, 0xE7, 0x3F, 0xFC, 0x0F, 0xF0, 0x0D, 0xB0,
	 0x07, 0xE0, 0x06, 0x60, 0x37, 0xEC, 0xE4, 0x27,
	 0xC3, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,}
};

Cursor t = {
	{-7, -8},
	{0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x06, 0xc0, 
	 0x1c, 0x70, 0x10, 0x10, 0x0c, 0x60, 0xfc, 0x7f, 
	 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0xff, 0xff, 
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 
	 0x03, 0x80, 0x0f, 0xe0, 0x03, 0x80, 0x03, 0x80, 
	 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x00, 0x00, 
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, }
};

Cursor tr = {
	{-11, -4},
	{0x00, 0x7f, 0x00, 0x41, 0x00, 0x31, 0xff, 0xe1, 
	 0x80, 0x05, 0x80, 0x0d, 0x80, 0x0b, 0xff, 0x88, 
	 0x00, 0x88, 0x0, 0x88, 0x00, 0x88, 0x00, 0x88, 
	 0x00, 0x88, 0x00, 0x88, 0x00, 0x88, 0x00, 0xf8, },
	{0x00, 0x00, 0x00, 0x3e, 0x00, 0x0e, 0x00, 0x1e, 
	 0x7f, 0xfa, 0x7f, 0xf2, 0x7f, 0xf0, 0x00, 0x70, 
	 0x00, 0x70, 0x00, 0x70, 0x00, 0x70, 0x00, 0x70, 
	 0x00, 0x70, 0x00, 0x70, 0x00, 0x70, 0x00, 0x00, }
};

Cursor r = {
	{-8, -7},
	{0x07, 0xc0, 0x04, 0x40, 0x04, 0x40, 0x04, 0x58, 
	 0x04, 0x68, 0x04, 0x6c, 0x04, 0x06, 0x04, 0x02, 
	 0x04, 0x06, 0x04, 0x6c, 0x04, 0x68, 0x04, 0x58, 
	 0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x07, 0xc0, },
	{0x00, 0x00, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 
	 0x03, 0x90, 0x03, 0x90, 0x03, 0xf8, 0x03, 0xfc, 
	 0x03, 0xf8, 0x03, 0x90, 0x03, 0x90, 0x03, 0x80, 
	 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x00, 0x00, }
};

Cursor br = {
	{-11, -11},
	{0x00, 0xf8, 0x00, 0x88, 0x00, 0x88, 0x00, 0x88, 
	 0x00, 0x88, 0x00, 0x88, 0x00, 0x88, 0x00, 0x88, 
	 0xff, 0x88, 0x80, 0x0b, 0x80, 0x0d, 0x80, 0x05, 
	 0xff, 0xe1, 0x00, 0x31, 0x00, 0x41, 0x00, 0x7f, },
	{0x00, 0x00, 0x00, 0x70, 0x00, 0x70, 0x00, 0x70, 
	 0x0, 0x70, 0x00, 0x70, 0x00, 0x70, 0x00, 0x70, 
	 0x00, 0x70, 0x7f, 0xf0, 0x7f, 0xf2, 0x7f, 0xfa, 
	 0x00, 0x1e, 0x00, 0x0e, 0x00, 0x3e, 0x00, 0x00, }
};

Cursor b = {
	{-7, -7},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	 0xff, 0xff, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 
	 0xfc, 0x7f, 0x0c, 0x60, 0x10, 0x10, 0x1c, 0x70, 
	 0x06, 0xc0, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, },
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	 0x00, 0x00, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 
	 0x03, 0x80, 0x03, 0x80, 0x0f, 0xe0, 0x03, 0x80, 
	 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, }
};

Cursor bl = {
	{-4, -11},
	{0x1f, 0x00, 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 
	 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 
	 0x11, 0xff, 0xd0, 0x01, 0xb0, 0x01, 0xa0, 0x01, 
	 0x87, 0xff, 0x8c, 0x00, 0x82, 0x00, 0xfe, 0x00, },
	{0x00, 0x00, 0x0e, 0x00, 0x0e, 0x00, 0x0e, 0x00, 
	 0x0e, 0x00, 0x0e, 0x00, 0x0e, 0x00, 0x0e, 0x00, 
	 0x0e, 0x00, 0x0f, 0xfe, 0x4f, 0xfe, 0x5f, 0xfe, 
	 0x78, 0x00, 0x70, 0x00, 0x7c, 0x00, 0x00, 0x0, }
};

Cursor l = {
	{-7, -7},
	{0x03, 0xe0, 0x02, 0x20, 0x02, 0x20, 0x1a, 0x20, 
	 0x16, 0x20, 0x36, 0x20, 0x60, 0x20, 0x40, 0x20, 
	 0x60, 0x20, 0x36, 0x20, 0x16, 0x20, 0x1a, 0x20, 
	 0x02, 0x20, 0x02, 0x20, 0x02, 0x20, 0x03, 0xe0, },
	{0x00, 0x00, 0x01, 0xc0, 0x01, 0xc0, 0x01, 0xc0, 
	 0x09, 0xc0, 0x09, 0xc0, 0x1f, 0xc0, 0x3f, 0xc0, 
	 0x1f, 0xc0, 0x09, 0xc0, 0x09, 0xc0, 0x01, 0xc0, 
	 0x01, 0xc0, 0x01, 0xc0, 0x01, 0xc0, 0x00, 0x00, }
};

Cursor *corners[9] = {
	&tl,	&t,	&tr,
	&l,	nil,	&r,
	&bl,	&b,	&br,
};

void
iconinit(void)
{
	int fd;
	long r = 0;
	char p[100];
	Image *bimg = nil;
	
	fd = open("/env/home", OREAD);
	if(fd < 0 || (r = read(fd, p, 40)) < 1)
		memcpy(p, (void *)"/usr/glenda", (r=12));
	else
		close(fd);
	
	/* ugly, but it works??? */	
	p[r] = 0;
	snprintf(&p[r+1], 99-r, "lib/background", p);
	p[r] = '/';
	
	fd = open(p, OREAD);
	if(fd > 0)
	{
		bimg = readimage(display, fd, 0);
		close(fd);				
	}
	
	if(bimg)
	{
		background = allocimage(display, Rect(0, 0, Dx(bimg->r), Dy(bimg->r)), RGB24, 1, 0x000000FF);
		draw(background, background->r, bimg, 0, bimg->r.min);
	}
	else
		background = allocimage(display, Rect(0,0,1,1), screen->chan, 1, 0x777777FF);

	/* greys are multiples of 0x11111100+0xFF, 14* being palest */
	cols[BACK] = allocimage(display, Rect(0,0,1,1), CMAP8, 1, 0xFFFFFFFF^reverse);
	cols[BORD] = allocimage(display, Rect(0,0,1,1), CMAP8, 1, 0x999999FF^reverse);
	cols[TEXT] = allocimage(display, Rect(0,0,1,1), CMAP8, 1, 0x000000FF^reverse);
	cols[HTEXT] = allocimage(display, Rect(0,0,1,1), CMAP8, 1, 0x000000FF);
	if(!reverse) {
		cols[HIGH] = allocimage(display, Rect(0,0,1,1), CMAP8, 1, 0xCCCCCCFF);
		titlecol = allocimage(display, Rect(0,0,1,1), CMAP8, 1, DGreygreen);
		lighttitlecol = allocimage(display, Rect(0,0,1,1), CMAP8, 1, DPalegreygreen);
	} else {
		cols[HIGH] = allocimage(display, Rect(0,0,1,1), CMAP8, 1, DPurpleblue);
		titlecol = allocimage(display, Rect(0,0,1,1), CMAP8, 1, DPurpleblue);
		lighttitlecol = allocimage(display, Rect(0,0,1,1), CMAP8, 1, 0x222222FF);
	}
	dholdcol = allocimage(display, Rect(0,0,1,1), CMAP8, 1, DMedblue);
	lightholdcol = allocimage(display, Rect(0,0,1,1), CMAP8, 1, DGreyblue);
	paleholdcol = allocimage(display, Rect(0,0,1,1), CMAP8, 1, DPalegreyblue);
	paletextcol = allocimage(display, Rect(0,0,1,1), CMAP8, 1, 0x666666FF^reverse);
	sizecol = allocimage(display, Rect(0,0,1,1), CMAP8, 1, DRed);

	if(reverse == 0)
		holdcol = dholdcol;
	else
		holdcol = paleholdcol;
}
