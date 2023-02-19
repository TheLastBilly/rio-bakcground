# Background patch for rio
This is a patch for the [rio](https://en.wikipedia.org/wiki/Rio_(windowing_system)) windowing system. It's based on [the mod created by 100rabits](https://wiki.xxiivv.com/site/rio.html), with a modification that allows for user specific background images.

# Installation
Since [data.c](./data.c) is the only file that needs to be modified by the patch, you just need to copy this repository's version of that file into the `/sys/src/cmd/rio` directory on your Plan9/9front installation. Alternatively, you can modify the `iconinit(void)` function near the end of `/sys/src/cmd/rio/data.c` to include the following modifications. 

***Note: Keep in mind you will also need to #include <stdio.h> at the beginning of the file.***
```c
	int fd;
	long r = 0;
	char p[100];
	Image *bimg = nil;
	
	fd = open("/env/home", OREAD);
	if(fd < 0 || (r = read(fd, p, 40)) < 1)
		memcpy(p, (void *)"/usr/glenda", (r=12));
	else
		close(fd);
	
	/* ugly, but it works™ */	
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
```

Either way, you have to recompile and install rio in order to apply this patch. You can do this with the `cd /sys/src/cmd/rio; mk install` command.

# Usage
In order to set a background for your user account, you need to convert your image into raw format using one of the image conversion programs available on Plan9. In the case of this example we'll be using a JPEG image, thus we will use the `jpg` command, however the process should be identical with the other programs. You can learn more about them using the `man jpg` command.

First, download the image you'd like to use as a background image. You can do this from programs like `hget`, or simply by copying them from your root mount on `drawterm`. Then, convert the image to raw format and scaled it to fit your desktop. 
```rc
jpg -t9 mybackground.jpg | resample -x 1920 -y 1080 > $home/lib/background
```

The above command will also place your image into your `lib/` directory, which is where rio will look for your background image at launch.

***Note: You do not need to set a background image for rio to work. If it does not find the image, or it can't open it, it will use the default background color.***