#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <linux/fb.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/hal/interface.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <poll.h>

struct framebuffer_info
{
    uint32_t bits_per_pixel;    // depth of framebuffer
    uint32_t xres_virtual;      // how many pixel in a row in virtual screen
    uint32_t yres_virtual;
};

struct framebuffer_info get_framebuffer_info ( const char *framebuffer_device_path );
int display_image(cv::Mat& img, std::ofstream& fb, struct framebuffer_info& fb_info);

int main ( int argc, const char *argv[] )
{
    cv::Mat frame;
    int ret=0, img_cnt=0;
    char c;
    char img_name[50], vidio_name[50];  

    if(argc < 1) return -1; 
    sprintf(vidio_name, "%s/video.avi", argv[1]);
    // open video stream device
    // https://docs.opencv.org/3.4.7/d8/dfe/classcv_1_1VideoCapture.html#a5d5f5dacb77bbebdcbfb341e3d4355c1
    cv::VideoCapture camera ( 2 );
    cv::VideoWriter writer(vidio_name, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, cv::Size(640, 480));
    framebuffer_info fb_info = get_framebuffer_info("/dev/fb0");
    std::ofstream ofs("/dev/fb0");
    struct termios t;
    struct pollfd fds[1] = {{fd:0, events:POLLIN, 0}};

    // check if video stream device is opened success or not
    // https://docs.opencv.org/3.4.7/d8/dfe/classcv_1_1VideoCapture.html#a9d2ca36789e7fcfe7a7be3b328038585
    if( !camera.isOpened() )
    {
        std::cerr << "Could not open video device." << std::endl;
        return 1;
    }

    // Turn off stdin enter
    tcgetattr(0, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &t);

    while ( true )
    {
        // get video frame from stream
        // https://docs.opencv.org/3.4.7/d8/dfe/classcv_1_1VideoCapture.html#a473055e77dd7faa4d26d686226b292c1
        // https://docs.opencv.org/3.4.7/d8/dfe/classcv_1_1VideoCapture.html#a199844fb74226a28b3ce3a39d1ff6765
        camera >> frame;
        writer << frame;
        printf("image size = %d x %d x %d\n", frame.cols, frame.rows, frame.channels());

        ret = poll(fds, 1, 10);
        if(ret == 1){
            c = std::cin.get();
            // std::cout << c << std::endl;
            if (c == 'c'){
                //save image
                sprintf(img_name, "%s/%06d.png", argv[1], img_cnt++);
                std::cout << "\r" << img_name << std::endl;
                cv::imwrite(img_name, frame);
            }
        }

        display_image(frame, ofs, fb_info);

    }

    // closing video stream
    // https://docs.opencv.org/3.4.7/d8/dfe/classcv_1_1VideoCapture.html#afb4ab689e553ba2c8f0fec41b9344ae6
    camera.release ( );

    return 0;
}

struct framebuffer_info get_framebuffer_info(const char *framebuffer_device_path)
{
    struct framebuffer_info fb_info;        // Used to return the required attrs.
    struct fb_var_screeninfo screen_info;   // Used to get attributes of the device from OS kernel.

    int fd;

    if((fd=open(framebuffer_device_path, O_RDWR))<0){
        printf("DEVICE CAN'T BE OPENED");
        exit(-1);
    }

    ioctl(fd, FBIOGET_VSCREENINFO, &screen_info);   

    fb_info.xres_virtual = screen_info.xres_virtual; // 8
    fb_info.bits_per_pixel = screen_info.bits_per_pixel; // 16
    fb_info.yres_virtual = screen_info.yres_virtual;
    printf("screen_res = %dx%d, bits_per_pixel = %d\n", fb_info.xres_virtual, fb_info.yres_virtual, fb_info.bits_per_pixel);
    // close(fd);
    return fb_info;
};

int display_image(cv::Mat& img, std::ofstream& fb, struct framebuffer_info& fb_info){
    int x_offset=0; // value to make image to the middle
    cv::Size2i img_size;

    img_size.height = fb_info.yres_virtual;
    img_size.width = img.cols * fb_info.yres_virtual / img.rows;
    x_offset = (fb_info.xres_virtual - img_size.width) / 2;

    printf("%dx%d -> %dx%d\n", img.cols, img.rows, img_size.width, img_size.height);
    printf("offset = %d\n", x_offset);
    // Resize the image to fit screen
    cv::resize(img, img, img_size);
    
    cv::cvtColor(img, img, cv::COLOR_BGR2BGR565);
    // output the video frame to framebufer row by row
    for (int y = 0; y < img_size.height; y++ )
    {
        fb.seekp(y*fb_info.xres_virtual*fb_info.bits_per_pixel/8 + x_offset*fb_info.bits_per_pixel/8);

        fb.write(img.ptr<char>(y, 0), img_size.width*fb_info.bits_per_pixel/8);
    }
    return 0;
}