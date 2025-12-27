#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>

int file_descriptor = -1;
struct fb_fix_screeninfo fix_info;
struct fb_var_screeninfo var_info;

int limit255(int num)
{
    if(num < 0) return 0;
    if(num > 255) return 255;
    else return num;
}

int main(int argc, char **argv)
{
    int R_val = limit255(atoi(argv[1]));
    int G_val = limit255(atoi(argv[2]));
    int B_val = limit255(atoi(argv[3]));

    file_descriptor = open("/dev/fb0",O_RDWR);
    if(file_descriptor >= 0){
        printf("ok\n");

        ioctl(file_descriptor,FBIOGET_FSCREENINFO,&fix_info);
        ioctl(file_descriptor,FBIOGET_VSCREENINFO,&var_info);
        printf("%i x %i x %i\n",var_info.xres, var_info.yres, var_info.bits_per_pixel);
        int fb_width = var_info.xres;
        int fb_height = var_info.yres;
        int fb_bpp = var_info.bits_per_pixel;
        uint32_t r_offset = var_info.red.offset;
        uint32_t g_offset = var_info.green.offset;
        uint32_t b_offset = var_info.blue.offset;
        printf("OFFSET %iR %iG %iB\n",r_offset, g_offset, b_offset);

        int fb_data_size = fb_width * fb_height * fb_bpp / 8;
        
        if(fb_bpp == 16)
        {
            uint16_t* fbdata = (uint16_t*)mmap(0, fb_data_size, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);
            
            uint16_t color_bitfield = 
            (R_val << r_offset) | 
            (G_val << g_offset) | 
            (B_val << b_offset);
            
            while(1){
                for(size_t pixel = 0; pixel < fb_width * fb_height; pixel++)
                {
                    fbdata[pixel] = color_bitfield;
                }
                sleep(5);
            } //run foreva
        }
        else {
            uint32_t* fbdata = (uint32_t*)mmap(0, fb_data_size, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);
            
            uint32_t color_bitfield = 
            (R_val << r_offset) | 
            (G_val << g_offset) | 
            (B_val << b_offset);

            while(1){
                for(size_t pixel = 0; pixel < fb_width * fb_height; pixel++)
                {
                    fbdata[pixel] = color_bitfield;
                }
                sleep(5);
            } //run foreva
        }
        
    }
    else {
        printf("Couldn't open /dev/fb0");  
    }
    return 0;
}