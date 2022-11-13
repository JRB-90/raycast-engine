#include "engine_subsystems.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <termios.h>

const static char* FB_PATH = "/dev/fb0";
const static char* TTY_PATH = "/dev/tty1";
const static char* EVNT_PATH = "/dev/input/event0";

typedef struct fb_fix_screeninfo fb_fix_screeninfo;
typedef struct fb_var_screeninfo fb_var_screeninfo;
typedef struct input_event input_event;

typedef struct RpiDisplay {
	int fbfd;
	int ttyfd;
	unsigned int screenPixels;
	unsigned int screenSizeBytes;
	unsigned int vscreenPixels;
	unsigned int vscreenSizeBytes;
	fb_fix_screeninfo finfo;
	fb_var_screeninfo vinfo;
	fb_var_screeninfo originalVinfo;
	uint8_t fbReadPage;
	uint8_t fbWritePage;
	uint8_t* fbPixels;
} RpiDisplay;

RpiDisplay display;
int evfd;
input_event events[64];
bool lctrl;
struct termios old;
struct termios new;

int pan_display(screen_buffer* const screen);
void print_scrinfo(
	const fb_fix_screeninfo* const finfo,
	const fb_var_screeninfo* const vinfo
);

int init_render_subsystem(
	const screen_format* const format,
	screen_buffer* const screen)
{
	display.fbfd = -1;
	display.ttyfd = -1;
	display.screenSizeBytes = 0;
	display.screenPixels = 0;
	display.fbReadPage = 0;
	display.fbWritePage = 1;
	display.fbPixels = NULL;

	display.fbfd = open(FB_PATH, O_RDWR);
	if (display.fbfd == -1)
	{
		fprintf(stderr, "Failed to open %s fd\n", FB_PATH);
		return -1;
	}

	if (ioctl(display.fbfd, FBIOGET_FSCREENINFO, &display.finfo))
	{
		fprintf(stderr, "Failed to get fixed screen info\n");
		return -1;
	}

	if (ioctl(display.fbfd, FBIOGET_VSCREENINFO, &display.vinfo))
	{
		fprintf(stderr, "Failed to get variable screen info\n");
		return -1;
	}

	display.originalVinfo = display.vinfo;
	display.screenPixels = display.vinfo.xres * display.vinfo.yres;
	display.screenSizeBytes = display.screenPixels * (display.vinfo.bits_per_pixel / 8);
	display.vscreenPixels = display.vinfo.xres_virtual * display.vinfo.yres_virtual;
	display.vscreenSizeBytes = display.vscreenPixels * (display.vinfo.bits_per_pixel / 8);

	printf("Starting screen info\n");
	print_scrinfo(&display.finfo, &display.vinfo);

	display.vinfo.xres = format->width;
	display.vinfo.xres_virtual = format->width;
	display.vinfo.yres = format->height;
	display.vinfo.yres_virtual = format->height * 2;
	display.vinfo.yoffset = 0;

	if (format->format == CF_ARGB)
	{
		display.vinfo.bits_per_pixel = 32;
	}
	else if (format->format == CF_RGBA)
	{
		display.vinfo.bits_per_pixel = 32;
	}
	else if (format->format == CF_RGB565)
	{
		display.vinfo.bits_per_pixel = 16;
	}
	else
	{
		display.vinfo.bits_per_pixel = 32;
	}

	if (ioctl(display.fbfd, FBIOPUT_VSCREENINFO, &display.vinfo))
	{
		fprintf(stderr, "Failed to set variable screen info\n");
		return -1;
	}

	display.screenPixels = display.vinfo.xres * display.vinfo.yres;
	display.screenSizeBytes = display.screenPixels * (display.vinfo.bits_per_pixel / 8);
	display.vscreenPixels = display.vinfo.xres_virtual * display.vinfo.yres_virtual;
	display.vscreenSizeBytes = display.vscreenPixels * (display.vinfo.bits_per_pixel / 8);

	printf("\nUpdated screen info\n");
	print_scrinfo(&display.finfo, &display.vinfo);

	display.ttyfd = open(TTY_PATH, O_RDWR);
	if (display.ttyfd == -1)
	{
		fprintf(stderr, "Failed to open %s fd\n", TTY_PATH);
		return -1;
	}

	if (ioctl(display.ttyfd, KDSETMODE, KD_GRAPHICS))
	{
		fprintf(stderr, "Failed to set %s to graphics mode\n", TTY_PATH);
		return -1;
	}

	pan_display(screen);

	screen->width = display.vinfo.xres;
	screen->height = display.vinfo.yres;
	screen->bitsPP = display.vinfo.bits_per_pixel;
	screen->bytesPP = display.vinfo.bits_per_pixel / 8;
	screen->sizeInPixels = display.vinfo.xres * display.vinfo.yres;
	screen->stride = display.vinfo.xres * (display.vinfo.bits_per_pixel / 8);
	screen->sizeInBytes = display.screenSizeBytes;
	screen->colorFormat = format->format;

	screen->pixels = malloc(screen->sizeInBytes);
	if (screen->pixels == NULL)
	{
		fprintf(stderr, "Failed to allocate screen pixel buffer\n");
		return -1;
	}

	display.fbPixels =
		(uint8_t*)mmap(
			0,
			display.vscreenSizeBytes,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			display.fbfd,
			0
		);

	if (display.fbPixels == (void*)-1)
	{
		fprintf(stderr, "Failed to create the fb buffer, mem map failed\n");
		return -1;
	}

	return 0;
}

int destroy_render_subsystem(screen_buffer* const screen)
{
	display.fbReadPage = 1;
	render_screen(screen);
	free(screen->pixels);

	if (display.fbPixels != NULL)
	{
		munmap(display.fbPixels, display.vscreenSizeBytes);
	}

	if (ioctl(display.ttyfd, KDSETMODE, KD_TEXT))
	{
		fprintf(stderr, "Failed to reset console to text mode\n");
	}

	if (ioctl(display.fbfd, FBIOPUT_VSCREENINFO, &display.originalVinfo))
	{
		fprintf(stderr, "Failed to reset screen settings\n");
	}

	close(display.ttyfd);
	close(display.fbfd);

	return 0;
}

int render_screen(screen_buffer* const screen)
{
	unsigned int offset = display.fbWritePage * display.screenSizeBytes;
	memcpy(display.fbPixels + offset, screen->pixels, screen->sizeInBytes);

	if (display.fbReadPage == 0)
	{
		display.fbReadPage = 1;
		display.fbWritePage = 0;
	}
	else
	{
		display.fbReadPage = 0;
		display.fbWritePage = 1;
	}

	return pan_display(screen);
}

int init_input_subsystem()
{
	lctrl = false;

	evfd = open(EVNT_PATH, O_RDONLY | O_NONBLOCK);
	if (evfd == -1)
	{
		fprintf(stderr, "Failed to open event fd\n");
		return -1;
	}

	int version;
	if (ioctl(evfd, EVIOCGVERSION, &version))
	{
		fprintf(stderr, "Failed to get input version\n");
		return -1;
	}

	printf(
		"Input driver version: %d.%d.%d\n",
		version >> 16,
		(version >> 8) & 0xff,
		version & 0xff
	);

	unsigned short id[4];
	if (ioctl(evfd, EVIOCGID, id))
	{
		fprintf(stderr, "Failed to get input id\n");
		return -1;
	}

	printf(
		"Input device ID: bus 0x%x vendor 0x%x product 0x%x version 0x%x\n",
		id[ID_BUS],
		id[ID_VENDOR],
		id[ID_PRODUCT],
		id[ID_VERSION]
	);

	if (tcgetattr(STDIN_FILENO, &old))
	{
		fprintf(stderr, "Failed to get termios attribute\n");
		return -1;
	}

	new = old;
	new.c_lflag &= ~(ICANON | ECHO);
	if (tcsetattr(STDIN_FILENO, TCSANOW, &new))
	{
		fprintf(stderr, "Failed to set termios attribute\n");
		return -1;
	}

	return 0;
}

int destroy_input_subsystem()
{
	tcsetattr(STDIN_FILENO, TCSANOW, &old);
	tcflush(STDIN_FILENO, TCIOFLUSH);
	close(evfd);

	return 0;
}

int update_input_state(input_state* const state)
{
	state->toggleDebug = false;
	state->toggleRenderMode = false;

	int bytesRead = read(evfd, events, sizeof(input_event) * 64);
	if (bytesRead < (int)sizeof(input_event))
	{
		return 0;
	}

	for (int i = 0; i < (bytesRead / sizeof(input_event)); i++)
	{
		if (events[i].type == EV_KEY)
		{
			if (events[i].value == 1)
			{
				switch (events[i].code)
				{
				case KEY_ESC:
					state->quit = true;
					break;
				case KEY_V:
					state->toggleDebug = true;
					break;
				case KEY_B:
					state->toggleRenderMode = true;
					break;
				case KEY_W:
					state->forwards = true;
					break;
				case KEY_S:
					state->backwards = true;
					break;
				case KEY_D:
					if (lctrl)
					{
						state->rotRight = false;
						state->right = true;
					}
					else
					{
						state->rotRight = true;
						state->right = false;
					}
					break;
				case KEY_A:
					if (lctrl)
					{
						state->rotLeft = false;
						state->left = true;
					}
					else
					{
						state->rotLeft = true;
						state->left = false;
					}
					break;
				case KEY_LEFTCTRL:
					lctrl = true;
					break;
				}
			}
			else if (events[i].value == 0)
			{
				switch (events[i].code)
				{
				case KEY_W:
					state->forwards = false;
					break;
				case KEY_S:
					state->backwards = false;
					break;
				case KEY_D:
					state->rotRight = false;
					state->right = false;
					break;
				case KEY_A:
					state->rotLeft = false;
					state->left = false;
					break;
				case KEY_LEFTCTRL:
					lctrl = false;
					break;
				}
			}
		}
	}

	return 0;
}

int pan_display(screen_buffer* const screen)
{
	display.vinfo.yoffset = display.fbReadPage * display.vinfo.yres;
	display.vinfo.activate = FB_ACTIVATE_VBL;

	if (ioctl(display.fbfd, FBIOPAN_DISPLAY, &display.vinfo))
	{
		printf("Failed to pan display\n");
		return -1;
	}

	/*if (ioctl(display.fbfd, FBIO_WAITFORVSYNC, 0))
	{
		printf("Failed to wait for vsync\n");
		return -1;
	}*/

	return 0;
}

void print_scrinfo(
	const fb_fix_screeninfo* const finfo,
	const fb_var_screeninfo* const vinfo)
{
	printf(
		"Finfo: %s, %u smem len, %u\n",
		finfo->id,
		finfo->smem_len,
		finfo->capabilities
	);

	printf(
		"Vinfo: %dx%d res, %dx%d vres, %d bpp\n",
		vinfo->xres,
		vinfo->yres,
		vinfo->xres_virtual,
		vinfo->yres_virtual,
		vinfo->bits_per_pixel
	);

	printf("SSize: %d\n", display.screenSizeBytes);
	printf("VSize: %d\n", display.vscreenSizeBytes);
}
