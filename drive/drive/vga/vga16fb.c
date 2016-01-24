/*
 * linux/drivers/video/vga16.c -- VGA 16-color framebuffer driver
 * 
 * Copyright 1999 Ben Pfaff <pfaffben@debian.org> and Petr Vandrovec <VANDROVE@vc.cvut.cz>
 * Based on VGA info at http://www.goodnet.com/~tinara/FreeVGA/home.htm
 * Based on VESA framebuffer (c) 1998 Gerd Knorr <kraxel@goldbach.in-berlin.de>
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License.  See the file COPYING in the main directory of this
 * archive for more details.  
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/fb.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/screen_info.h>

#include <asm/io.h>
#include <video/vga.h>

#define VGA_FB_PHYS 0xA0000
#define VGA_FB_PHYS_LEN 65536

#define MODE_SKIP4	1
#define MODE_8BPP	2
#define MODE_CFB	4
#define MODE_TEXT	8

/* --------------------------------------------------------------------- */

static struct fb_var_screeninfo vga16fb_defined __initdata = {
	.xres		= 640,
	.yres		= 480,
	.xres_virtual	= 640,
	.yres_virtual	= 480,
	.bits_per_pixel	= 4,	
	.activate	= FB_ACTIVATE_TEST,
	.height		= -1,
	.width		= -1,
	.pixclock	= 39721,
	.left_margin	= 48,
	.right_margin	= 16,
	.upper_margin	= 33,
	.lower_margin	= 10,
	.hsync_len 	= 96,
	.vsync_len	= 2,
	.vmode		= FB_VMODE_NONINTERLACED,
};

/* name should not depend on EGA/VGA */
static struct fb_fix_screeninfo vga16fb_fix __initdata = {
	.id		= "VGA16 VGA",
	.smem_start	= VGA_FB_PHYS,
	.smem_len	= VGA_FB_PHYS_LEN,
	.type		= FB_TYPE_VGA_PLANES,
	.type_aux	= FB_AUX_VGA_PLANES_VGA4,
	.visual		= FB_VISUAL_PSEUDOCOLOR,
	.xpanstep	= 8,
	.ypanstep	= 1,
	.line_length	= 640 / 8,
	.accel		= FB_ACCEL_NONE
};

/* The VGA's weird architecture often requires that we read a byte and
   write a byte to the same location.  It doesn't matter *what* byte
   we write, however.  This is because all the action goes on behind
   the scenes in the VGA's 32-bit latch register, and reading and writing
   video memory just invokes latch behavior.

   To avoid race conditions (is this necessary?), reading and writing
   the memory byte should be done with a single instruction.  One
   suitable instruction is the x86 bitwise OR.  The following
   read-modify-write routine should optimize to one such bitwise
   OR. */
static inline void rmw(volatile char __iomem *p)
{
	readb(p);
	writeb(1, p);
}

/* Set the Graphics Mode Register, and return its previous value.
   Bits 0-1 are write mode, bit 3 is read mode. */
static inline int setmode(int mode)
{
	int oldmode;
	
	oldmode = vga_io_rgfx(VGA_GFX_MODE);
	vga_io_w(VGA_GFX_D, mode);
	return oldmode;
}

/* Select the Bit Mask Register and return its value. */
static inline int selectmask(void)
{
	return vga_io_rgfx(VGA_GFX_BIT_MASK);
}

/* Set the value of the Bit Mask Register.  It must already have been
   selected with selectmask(). */
static inline void setmask(int mask)
{
	vga_io_w(VGA_GFX_D, mask);
}

/* Set the Data Rotate Register and return its old value. 
   Bits 0-2 are rotate count, bits 3-4 are logical operation
   (0=NOP, 1=AND, 2=OR, 3=XOR). */
static inline int setop(int op)
{
	int oldop;
	
	oldop = vga_io_rgfx(VGA_GFX_DATA_ROTATE);
	vga_io_w(VGA_GFX_D, op);
	return oldop;
}

/* Set the Enable Set/Reset Register and return its old value.  
   The code here always uses value 0xf for thsi register. */
static inline int setsr(int sr)
{
	int oldsr;

	oldsr = vga_io_rgfx(VGA_GFX_SR_ENABLE);
	vga_io_w(VGA_GFX_D, sr);
	return oldsr;
}

/* Set the Set/Reset Register and return its old value. */
static inline int setcolor(int color)
{
	int oldcolor;

	oldcolor = vga_io_rgfx(VGA_GFX_SR_VALUE);
	vga_io_w(VGA_GFX_D, color);
	return oldcolor;
}

/* Return the value in the Graphics Address Register. */
static inline int getindex(void)
{
	return vga_io_r(VGA_GFX_I);
}

/* Set the value in the Graphics Address Register. */
static inline void setindex(int index)
{
	vga_io_w(VGA_GFX_I, index);
}

static void vga16fb_pan_var(struct fb_info *info, 
			    struct fb_var_screeninfo *var)
{
	struct vga16fb_par *par = info->par;
	u32 xoffset, pos;

	xoffset = var->xoffset;
	if (info->var.bits_per_pixel == 8) {
		pos = (info->var.xres_virtual * var->yoffset + xoffset) >> 2;
	} else if (par->mode & MODE_TEXT) {
		int fh = 16; // FIXME !!! font height. Fugde for now.
		pos = (info->var.xres_virtual * (var->yoffset / fh) + xoffset) >> 3;
	} else {
		if (info->var.nonstd)
			xoffset--;
		pos = (info->var.xres_virtual * var->yoffset + xoffset) >> 3;
	}
	vga_io_wcrt(VGA_CRTC_START_HI, pos >> 8);
	vga_io_wcrt(VGA_CRTC_START_LO, pos & 0xFF);
	/* if we support CFB4, then we must! support xoffset with pixel
	 * granularity if someone supports xoffset in bit resolution */
	vga_io_r(VGA_IS1_RC);		/* reset flip-flop */
	vga_io_w(VGA_ATT_IW, VGA_ATC_PEL);
	if (var->bits_per_pixel == 8)
		vga_io_w(VGA_ATT_IW, (xoffset & 3) << 1);
	else
		vga_io_w(VGA_ATT_IW, xoffset & 7);
	vga_io_r(VGA_IS1_RC);
	vga_io_w(VGA_ATT_IW, 0x20);
}

static void vga16fb_update_fix(struct fb_info *info)
{
}

static void vga16fb_clock_chip(struct vga16fb_par *par,
			       unsigned int pixclock,
			       const struct fb_info *info,
			       int mul, int div)
{

}
			       
#define FAIL(X) return -EINVAL

static int vga16fb_open(struct fb_info *info, int user)
{

	return 0;
}

static int vga16fb_release(struct fb_info *info, int user)
{
	return 0;
}

static int vga16fb_check_var(struct fb_var_screeninfo *var,
			     struct fb_info *info)
{
	return 0;
}

static int vga16fb_set_par(struct fb_info *info)
{
	return 0;
}

static void ega16_setpalette(int regno, unsigned red, unsigned green, unsigned blue)
{

}

static void vga16_setpalette(int regno, unsigned red, unsigned green, unsigned blue)
{

}

static int vga16fb_setcolreg(unsigned regno, unsigned red, unsigned green,
			     unsigned blue, unsigned transp,
			     struct fb_info *info)
{

	/*
	 *  Set a single color register. The values supplied are
	 *  already rounded down to the hardware's capabilities
	 *  (according to the entries in the `var' structure). Return
	 *  != 0 for invalid regno.
	 */
	

	return 0;
}

static int vga16fb_pan_display(struct fb_var_screeninfo *var,
			       struct fb_info *info) 
{
	return 0;
}

/* The following VESA blanking code is taken from vgacon.c.  The VGA
   blanking code was originally by Huang shi chao, and modified by
   Christoph Rimek (chrimek@toppoint.de) and todd j. derr
   (tjd@barefoot.org) for Linux. */

static void vga_vesa_blank(struct vga16fb_par *par, int mode)
{

}

static void vga_vesa_unblank(struct vga16fb_par *par)
{

}

static void vga_pal_blank(void)
{
	int i;

	for (i=0; i<16; i++) {
		outb_p(i, VGA_PEL_IW);
		outb_p(0, VGA_PEL_D);
		outb_p(0, VGA_PEL_D);
		outb_p(0, VGA_PEL_D);
	}
}

/* 0 unblank, 1 blank, 2 no vsync, 3 no hsync, 4 off */
static int vga16fb_blank(int blank, struct fb_info *info)
{
	struct vga16fb_par *par = info->par;

	switch (blank) {
	case FB_BLANK_UNBLANK:				/* Unblank */
		if (par->vesa_blanked) {
			vga_vesa_unblank(par);
			par->vesa_blanked = 0;
		}
		if (par->palette_blanked) {
			par->palette_blanked = 0;
		}
		break;
	case FB_BLANK_NORMAL:				/* blank */
		vga_pal_blank();
		par->palette_blanked = 1;
		break;
	default:			/* VESA blanking */
		vga_vesa_blank(par, blank);
		par->vesa_blanked = 1;
		break;
	}
	return 0;
}

static void vga_8planes_fillrect(struct fb_info *info, const struct fb_fillrect *rect)
{
	u32 dx = rect->dx, width = rect->width;
        char oldindex = getindex();
        char oldmode = setmode(0x40);
        char oldmask = selectmask();
        int line_ofs, height;
        char oldop, oldsr;
        char __iomem *where;

        dx /= 4;
        where = info->screen_base + dx + rect->dy * info->fix.line_length;

        if (rect->rop == ROP_COPY) {
                oldop = setop(0);
                oldsr = setsr(0);

                width /= 4;
                line_ofs = info->fix.line_length - width;
                setmask(0xff);

                height = rect->height;

                while (height--) {
                        int x;

                        /* we can do memset... */
                        for (x = width; x > 0; --x) {
                                writeb(rect->color, where);
                                where++;
                        }
                        where += line_ofs;
                }
        } else {
                char oldcolor = setcolor(0xf);
                int y;

                oldop = setop(0x18);
                oldsr = setsr(0xf);
                setmask(0x0F);
                for (y = 0; y < rect->height; y++) {
                        rmw(where);
                        rmw(where+1);
                        where += info->fix.line_length;
                }
                setcolor(oldcolor);
        }
        setmask(oldmask);
        setsr(oldsr);
        setop(oldop);
        setmode(oldmode);
        setindex(oldindex);
}

static void vga16fb_fillrect(struct fb_info *info, const struct fb_fillrect *rect)
{
	int x, x2, y2, vxres, vyres, width, height, line_ofs;
	char __iomem *dst;

	vxres = info->var.xres_virtual;
	vyres = info->var.yres_virtual;

	if (!rect->width || !rect->height || rect->dx > vxres || rect->dy > vyres)
		return;

	/* We could use hardware clipping but on many cards you get around
	 * hardware clipping by writing to framebuffer directly. */

	x2 = rect->dx + rect->width;
	y2 = rect->dy + rect->height;
	x2 = x2 < vxres ? x2 : vxres;
	y2 = y2 < vyres ? y2 : vyres;
	width = x2 - rect->dx;

	switch (info->fix.type) {
	case FB_TYPE_VGA_PLANES:
		if (info->fix.type_aux == FB_AUX_VGA_PLANES_VGA4) {

			height = y2 - rect->dy;
			width = rect->width/8;

			line_ofs = info->fix.line_length - width;
			dst = info->screen_base + (rect->dx/8) + rect->dy * info->fix.line_length;

			switch (rect->rop) {
			case ROP_COPY:
				setmode(0);
				setop(0);
				setsr(0xf);
				setcolor(rect->color);
				selectmask();

				setmask(0xff);

				while (height--) {
					for (x = 0; x < width; x++) {
						writeb(0, dst);
						dst++;
					}
					dst += line_ofs;
				}
				break;
			case ROP_XOR:
				setmode(0);
				setop(0x18);
				setsr(0xf);
				setcolor(0xf);
				selectmask();

				setmask(0xff);
				while (height--) {
					for (x = 0; x < width; x++) {
						rmw(dst);
						dst++;
					}
					dst += line_ofs;
				}
				break;
			}
		} else 
			vga_8planes_fillrect(info, rect);
		break;
	case FB_TYPE_PACKED_PIXELS:
	default:
		cfb_fillrect(info, rect);
		break;
	}
}

static void vga_8planes_copyarea(struct fb_info *info, const struct fb_copyarea *area)
{
        char oldindex = getindex();
        char oldmode = setmode(0x41);
        char oldop = setop(0);
        char oldsr = setsr(0xf);
        int height, line_ofs, x;
	u32 sx, dx, width;
	char __iomem *dest;
	char __iomem *src;

        height = area->height;

        sx = area->sx / 4;
        dx = area->dx / 4;
        width = area->width / 4;

        if (area->dy < area->sy || (area->dy == area->sy && dx < sx)) {
                line_ofs = info->fix.line_length - width;
                dest = info->screen_base + dx + area->dy * info->fix.line_length;
                src = info->screen_base + sx + area->sy * info->fix.line_length;
                while (height--) {
                        for (x = 0; x < width; x++) {
                                readb(src);
                                writeb(0, dest);
                                src++;
                                dest++;
                        }
                        src += line_ofs;
                        dest += line_ofs;
                }
        } else {
                line_ofs = info->fix.line_length - width;
                dest = info->screen_base + dx + width +
			(area->dy + height - 1) * info->fix.line_length;
                src = info->screen_base + sx + width +
			(area->sy + height - 1) * info->fix.line_length;
                while (height--) {
                        for (x = 0; x < width; x++) {
                                --src;
                                --dest;
                                readb(src);
                                writeb(0, dest);
                        }
                        src -= line_ofs;
                        dest -= line_ofs;
                }
        }

        setsr(oldsr);
        setop(oldop);
        setmode(oldmode);
        setindex(oldindex);
}

static void vga16fb_copyarea(struct fb_info *info, const struct fb_copyarea *area)
{
	u32 dx = area->dx, dy = area->dy, sx = area->sx, sy = area->sy; 
	int x, x2, y2, old_dx, old_dy, vxres, vyres;
	int height, width, line_ofs;
	char __iomem *dst = NULL;
	char __iomem *src = NULL;

	vxres = info->var.xres_virtual;
	vyres = info->var.yres_virtual;

	if (area->dx > vxres || area->sx > vxres || area->dy > vyres ||
	    area->sy > vyres)
		return;

	/* clip the destination */
	old_dx = area->dx;
	old_dy = area->dy;

	/*
	 * We could use hardware clipping but on many cards you get around
	 * hardware clipping by writing to framebuffer directly.
	 */
	x2 = area->dx + area->width;
	y2 = area->dy + area->height;
	dx = area->dx > 0 ? area->dx : 0;
	dy = area->dy > 0 ? area->dy : 0;
	x2 = x2 < vxres ? x2 : vxres;
	y2 = y2 < vyres ? y2 : vyres;
	width = x2 - dx;
	height = y2 - dy;

	if (sx + dx < old_dx || sy + dy < old_dy)
		return;

	/* update sx1,sy1 */
	sx += (dx - old_dx);
	sy += (dy - old_dy);

	/* the source must be completely inside the virtual screen */
	if (sx + width > vxres || sy + height > vyres)
		return;

	switch (info->fix.type) {
	case FB_TYPE_VGA_PLANES:
		if (info->fix.type_aux == FB_AUX_VGA_PLANES_VGA4) {
			width = width/8;
			height = height;
			line_ofs = info->fix.line_length - width;

			setmode(1);
			setop(0);
			setsr(0xf);

			if (dy < sy || (dy == sy && dx < sx)) {
				dst = info->screen_base + (dx/8) + dy * info->fix.line_length;
				src = info->screen_base + (sx/8) + sy * info->fix.line_length;
				while (height--) {
					for (x = 0; x < width; x++) {
						readb(src);
						writeb(0, dst);
						dst++;
						src++;
					}
					src += line_ofs;
					dst += line_ofs;
				}
			} else {
				dst = info->screen_base + (dx/8) + width + 
					(dy + height - 1) * info->fix.line_length;
				src = info->screen_base + (sx/8) + width + 
					(sy + height  - 1) * info->fix.line_length;
				while (height--) {
					for (x = 0; x < width; x++) {
						dst--;
						src--;
						readb(src);
						writeb(0, dst);
					}
					src -= line_ofs;
					dst -= line_ofs;
				}
			}
		} else 
			vga_8planes_copyarea(info, area);
		break;
	case FB_TYPE_PACKED_PIXELS:
	default:
		cfb_copyarea(info, area);
		break;
	}
}

#define TRANS_MASK_LOW  {0x0,0x8,0x4,0xC,0x2,0xA,0x6,0xE,0x1,0x9,0x5,0xD,0x3,0xB,0x7,0xF}
#define TRANS_MASK_HIGH {0x000, 0x800, 0x400, 0xC00, 0x200, 0xA00, 0x600, 0xE00, \
			 0x100, 0x900, 0x500, 0xD00, 0x300, 0xB00, 0x700, 0xF00}

#if defined(__LITTLE_ENDIAN)
static const u16 transl_l[] = TRANS_MASK_LOW;
static const u16 transl_h[] = TRANS_MASK_HIGH;
#elif defined(__BIG_ENDIAN)
static const u16 transl_l[] = TRANS_MASK_HIGH;
static const u16 transl_h[] = TRANS_MASK_LOW;
#else
#error "Only __BIG_ENDIAN and __LITTLE_ENDIAN are supported in vga-planes"
#endif

static void vga_8planes_imageblit(struct fb_info *info, const struct fb_image *image)
{
        char oldindex = getindex();
        char oldmode = setmode(0x40);
        char oldop = setop(0);
        char oldsr = setsr(0);
        char oldmask = selectmask();
        const char *cdat = image->data;
	u32 dx = image->dx;
        char __iomem *where;
        int y;

        dx /= 4;
        where = info->screen_base + dx + image->dy * info->fix.line_length;

        setmask(0xff);
        writeb(image->bg_color, where);
        readb(where);
        selectmask();
        setmask(image->fg_color ^ image->bg_color);
        setmode(0x42);
        setop(0x18);
        for (y = 0; y < image->height; y++, where += info->fix.line_length)
                writew(transl_h[cdat[y]&0xF] | transl_l[cdat[y] >> 4], where);
        setmask(oldmask);
        setsr(oldsr);
        setop(oldop);
        setmode(oldmode);
        setindex(oldindex);
}

static void vga_imageblit_expand(struct fb_info *info, const struct fb_image *image)
{
	char __iomem *where = info->screen_base + (image->dx/8) +
		image->dy * info->fix.line_length;
	struct vga16fb_par *par = info->par;
	char *cdat = (char *) image->data;
	char __iomem *dst;
	int x, y;

	switch (info->fix.type) {
	case FB_TYPE_VGA_PLANES:
		if (info->fix.type_aux == FB_AUX_VGA_PLANES_VGA4) {
			if (par->isVGA) {
				setmode(2);
				setop(0);
				setsr(0xf);
				setcolor(image->fg_color);
				selectmask();
				
				setmask(0xff);
				writeb(image->bg_color, where);
				rmb();
				readb(where); /* fill latches */
				setmode(3);
				wmb();
				for (y = 0; y < image->height; y++) {
					dst = where;
					for (x = image->width/8; x--;) 
						writeb(*cdat++, dst++);
					where += info->fix.line_length;
				}
				wmb();
			} else {
				setmode(0);
				setop(0);
				setsr(0xf);
				setcolor(image->bg_color);
				selectmask();
				
				setmask(0xff);
				for (y = 0; y < image->height; y++) {
					dst = where;
					for (x=image->width/8; x--;){
						rmw(dst);
						setcolor(image->fg_color);
						selectmask();
						if (*cdat) {
							setmask(*cdat++);
							rmw(dst++);
						}
					}
					where += info->fix.line_length;
				}
			}
		} else 
			vga_8planes_imageblit(info, image);
		break;
	case FB_TYPE_PACKED_PIXELS:
	default:
		cfb_imageblit(info, image);
		break;
	}
}

static void vga_imageblit_color(struct fb_info *info, const struct fb_image *image)
{
	/*
	 * Draw logo 
	 */
	struct vga16fb_par *par = info->par;
	char __iomem *where =
		info->screen_base + image->dy * info->fix.line_length +
		image->dx/8;
	const char *cdat = image->data;
	char __iomem *dst;
	int x, y;

	switch (info->fix.type) {
	case FB_TYPE_VGA_PLANES:
		if (info->fix.type_aux == FB_AUX_VGA_PLANES_VGA4 &&
		    par->isVGA) {
			setsr(0xf);
			setop(0);
			setmode(0);
			
			for (y = 0; y < image->height; y++) {
				for (x = 0; x < image->width; x++) {
					dst = where + x/8;

					setcolor(*cdat);
					selectmask();
					setmask(1 << (7 - (x % 8)));
					fb_readb(dst);
					fb_writeb(0, dst);

					cdat++;
				}
				where += info->fix.line_length;
			}
		}
		break;
	case FB_TYPE_PACKED_PIXELS:
		cfb_imageblit(info, image);
		break;
	default:
		break;
	}
}
				
static void vga16fb_imageblit(struct fb_info *info, const struct fb_image *image)
{
	if (image->depth == 1)
		vga_imageblit_expand(info, image);
	else
		vga_imageblit_color(info, image);
}

static struct fb_ops vga16fb_ops = {
	.owner		= THIS_MODULE,
	.fb_open        = vga16fb_open,
	.fb_release     = vga16fb_release,
	.fb_check_var	= vga16fb_check_var,
	.fb_set_par	= vga16fb_set_par,
	.fb_setcolreg 	= vga16fb_setcolreg,
	.fb_pan_display = vga16fb_pan_display,
	.fb_blank 	= vga16fb_blank,
	.fb_fillrect	= vga16fb_fillrect,
	.fb_copyarea	= vga16fb_copyarea,
	.fb_imageblit	= vga16fb_imageblit,
};

#ifndef MODULE
static int vga16fb_setup(char *options)
{
	char *this_opt;
	
	if (!options || !*options)
		return 0;
	
	while ((this_opt = strsep(&options, ",")) != NULL) {
		if (!*this_opt) continue;
	}
	return 0;
}
#endif

static int __init vga16fb_probe(struct platform_device *dev)
{
	struct fb_info *info;
	struct vga16fb_par *par;
	int i;
	int ret = 0;

	printk(KERN_DEBUG "vga16fb: initializing\n");
	info = framebuffer_alloc(sizeof(struct vga16fb_par), &dev->dev);

	if (!info) {
		ret = -ENOMEM;
		goto err_fb_alloc;
	}

	/* XXX share VGA_FB_PHYS and I/O region with vgacon and others */
//	info->screen_base = (void __iomem *)VGA_MAP_MEM(VGA_FB_PHYS, 0);

	if (!info->screen_base) {
		printk(KERN_ERR "vga16fb: unable to map device\n");
		ret = -ENOMEM;
		goto err_ioremap;
	}

	printk(KERN_INFO "vga16fb: mapped to 0x%p\n", info->screen_base);
	par = info->par;

	mutex_init(&par->open_lock);
	par->isVGA = screen_info.orig_video_isVGA;
	par->palette_blanked = 0;
	par->vesa_blanked = 0;

	i = par->isVGA? 6 : 2;
	
	vga16fb_defined.red.length   = i;
	vga16fb_defined.green.length = i;
	vga16fb_defined.blue.length  = i;	

	/* name should not depend on EGA/VGA */
	info->fbops = &vga16fb_ops;
	info->var = vga16fb_defined;
	info->fix = vga16fb_fix;
	/* supports rectangles with widths of multiples of 8 */
	info->pixmap.blit_x = 1 << 7 | 1 << 15 | 1 << 23 | 1 << 31;
	info->flags = FBINFO_FLAG_DEFAULT |
		FBINFO_HWACCEL_YPAN;

	i = (info->var.bits_per_pixel == 8) ? 256 : 16;
	ret = fb_alloc_cmap(&info->cmap, i, 0);
	if (ret) {
		printk(KERN_ERR "vga16fb: unable to allocate colormap\n");
		ret = -ENOMEM;
		goto err_alloc_cmap;
	}

	if (vga16fb_check_var(&info->var, info)) {
		printk(KERN_ERR "vga16fb: unable to validate variable\n");
		ret = -EINVAL;
		goto err_check_var;
	}

	vga16fb_update_fix(info);

	if (register_framebuffer(info) < 0) {
		printk(KERN_ERR "vga16fb: unable to register framebuffer\n");
		ret = -EINVAL;
		goto err_check_var;
	}

	printk(KERN_INFO "fb%d: %s frame buffer device\n",
	       info->node, info->fix.id);
	platform_set_drvdata(dev, info);

	return 0;

 err_check_var:
	fb_dealloc_cmap(&info->cmap);
 err_alloc_cmap:
	iounmap(info->screen_base);
 err_ioremap:
	framebuffer_release(info);
 err_fb_alloc:
	return ret;
}

static int vga16fb_remove(struct platform_device *dev)
{
	struct fb_info *info = platform_get_drvdata(dev);

	if (info) {
		unregister_framebuffer(info);
		iounmap(info->screen_base);
		fb_dealloc_cmap(&info->cmap);
	/* XXX unshare VGA regions */
		framebuffer_release(info);
	}

	return 0;
}

static struct platform_driver vga16fb_driver = {
	.probe = vga16fb_probe,
	.remove = vga16fb_remove,
	.driver = {
		.name = "vga16fb",
	},
};

static struct platform_device *vga16fb_device;

static int __init vga16fb_init(void)
{
	int ret;
#ifndef MODULE
	char *option = NULL;

	if (fb_get_options("vga16fb", &option))
		return -ENODEV;

	vga16fb_setup(option);
#endif
	ret = platform_driver_register(&vga16fb_driver);

	if (!ret) {
		vga16fb_device = platform_device_alloc("vga16fb", 0);

		if (vga16fb_device)
			ret = platform_device_add(vga16fb_device);
		else
			ret = -ENOMEM;

		if (ret) {
			platform_device_put(vga16fb_device);
			platform_driver_unregister(&vga16fb_driver);
		}
	}

	return ret;
}

static void __exit vga16fb_exit(void)
{
	platform_device_unregister(vga16fb_device);
	platform_driver_unregister(&vga16fb_driver);
}

MODULE_DESCRIPTION("Legacy VGA framebuffer device driver");
MODULE_LICENSE("GPL");
module_init(vga16fb_init);
module_exit(vga16fb_exit);


/*
 * Overrides for Emacs so that we follow Linus's tabbing style.
 * ---------------------------------------------------------------------------
 * Local variables:
 * c-basic-offset: 8
 * End:
 */

