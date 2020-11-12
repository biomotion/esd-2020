#include <fcntl.h> 
#include <fstream>
#include <linux/fb.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sys/ioctl.h>

struct framebuffer_info
{
    uint32_t bits_per_pixel;    // framebuffer depth
    uint32_t xres_virtual;      // how many pixel in a row in virtual screen
};

struct framebuffer_info get_framebuffer_info(const char *framebuffer_device_path);

int main(int argc, const char *argv[])
{
    cv::Mat image;
    cv::Size2i image_size;
    
    framebuffer_info fb_info = get_framebuffer_info("/dev/fb0");
    std::ofstream ofs("/dev/fb0");

    // std::ifstream ifs(argv[1], std::fstream::binary);
    // std::stringstream strs;
    // std::vector<uchar> *data;

    // read image file (sample.bmp) from opencv libs.
    // https://docs.opencv.org/3.4.7/d4/da8/group__imgcodecs.html#ga288b8b3da0892bd651fce07b3bbd3a56
    image = cv::imread(argv[1], cv::IMREAD_UNCHANGED);
    // while(ifs >> strs.rdbuf() ) {printf("r ");}
    // data = new std::vector<uchar>(strs.str().begin(), strs.str().end());
    // printf("%d\n", data->size());
    // image = cv::imdecode(cv::Mat(*data), cv::IMREAD_ANYCOLOR);

    // get image size of the image.
    // https://docs.opencv.org/3.4.7/d3/d63/classcv_1_1Mat.html#a146f8e8dda07d1365a575ab83d9828d1
    image_size.width = image.cols;
    image_size.height = image.rows;
    
    printf("image size = %d x %d x %d\n", image.rows, image.cols, image.channels());

    // transfer color space from BGR to BGR565 (16-bit image) to fit the requirement of the LCD
    // https://docs.opencv.org/3.4.7/d8/d01/group__imgproc__color__conversions.html#ga397ae87e1288a81d2363b61574eb8cab
    // https://docs.opencv.org/3.4.7/d8/d01/group__imgproc__color__conversions.html#ga4e0972be5de079fed4e3a10e24ef5ef0
    cv::cvtColor(image, image, cv::COLOR_BGR2BGR565);

    // output to framebufer row by row
    for (int y = 0; y < image_size.height; y++)
    {
        // char buf[2000] = {0};
        // move to the next written position of output device framebuffer by "std::ostream::seekp()".
        // posisiotn can be calcluated by "y", "fb_info.xres_virtual", and "fb_info.bits_per_pixel".
        // http://www.cplusplus.com/reference/ostream/ostream/seekp/
        ofs.seekp(y*fb_info.xres_virtual*fb_info.bits_per_pixel/8);

        // write to the framebuffer by "std::ostream::write()".
        // you could use "cv::Mat::ptr()" to get the pointer of the corresponding row.
        // you also have to count how many bytes to write to the buffer
        // http://www.cplusplus.com/reference/ostream/ostream/write/
        // https://docs.opencv.org/3.4.7/d3/d63/classcv_1_1Mat.html#a13acd320291229615ef15f96ff1ff738
        ofs.write(image.ptr<char>(y, 0), image_size.width*fb_info.bits_per_pixel/8);
    }

    return 0;
}

struct framebuffer_info get_framebuffer_info(const char *framebuffer_device_path)
{
    struct framebuffer_info fb_info;        // Used to return the required attrs.
    struct fb_var_screeninfo screen_info;   // Used to get attributes of the device from OS kernel.

    int fd;

    // open device with linux system call "open()"
    // https://man7.org/linux/man-pages/man2/open.2.html
    if((fd=open(framebuffer_device_path, O_RDWR))<0){
        printf("DEVICE CAN'T BE OPENED");
        exit(-1);
    }

    // get attributes of the framebuffer device thorugh linux system call "ioctl()".
    // the command you would need is "FBIOGET_VSCREENINFO"
    // https://man7.org/linux/man-pages/man2/ioctl.2.html
    // https://www.kernel.org/doc/Documentation/fb/api.txt
    ioctl(fd, FBIOGET_VSCREENINFO, &screen_info);   

    // put the required attributes in variable "fb_info" you found with "ioctl() and return it."
    fb_info.xres_virtual = screen_info.xres_virtual; // 8
    fb_info.bits_per_pixel = screen_info.bits_per_pixel; // 16
    // printf("xres = %d, bits_per_pixel = %d\n", fb_info.xres_virtual, fb_info.bits_per_pixel);
    // close(fd);
    return fb_info;
};