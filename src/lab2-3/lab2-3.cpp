#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <linux/fb.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/hal/interface.h>
#include <sys/ioctl.h>

#include <string>
#include <dirent.h>

struct framebuffer_info
{
  uint32_t bits_per_pixel; // depth of framebuffer
  uint32_t xres_virtual;   // how many pixel in a row in virtual screen
  uint32_t yres_virtual;
};

struct framebuffer_info get_framebuffer_info(const char *framebuffer_device_path);
int display_image(cv::Mat &img, std::ofstream &fb, struct framebuffer_info &fb_info);
void get_files(const char *dir, std::vector<const char *> &files);

int main(int argc, const char *argv[])
{
  int sleep_time;
  cv::Mat frame;
  char img_name[200];
  std::vector<const char *> files;

  if (argc != 4)
  {
    std::cerr << "Usage: ./camera <framebuffer device> <picture path> <duration(s)>\n";
    return -1;
  }
  sleep_time = atoi(argv[3]);

  cv::VideoCapture camera(2);
  framebuffer_info fb_info = get_framebuffer_info(argv[1]);
  std::ofstream ofs(argv[1]);

  if (!camera.isOpened())
  {
    std::cerr << "Could not open video device." << std::endl;
    return 1;
  }

  // Clear screen
  frame = cv::Mat::zeros(fb_info.xres_virtual, fb_info.yres_virtual, 0);
  display_image(frame, ofs, fb_info);

  get_files(argv[2], files);

  while (true)
  {
    for (std::vector<const char *>::iterator it = files.begin(); it != files.end(); it++)
    {
      sprintf(img_name, "%s/%s", argv[2], *it);
      // std::cout << "image: " << img_name << std::endl;
      frame = cv::imread(img_name, cv::IMREAD_UNCHANGED);
      // std::cout << "(" << frame.rows << ", " << frame.cols << ")\n";
      if (fb_info.bits_per_pixel == 16)
      {
        cv::cvtColor(frame, frame, cv::COLOR_BGR2BGR565);
      }
      display_image(frame, ofs, fb_info);
      sleep(sleep_time);
    }
  }

  camera.release();

  return 0;
}

struct framebuffer_info get_framebuffer_info(const char *framebuffer_device_path)
{
  struct framebuffer_info fb_info;      // Used to return the required attrs.
  struct fb_var_screeninfo screen_info; // Used to get attributes of the device from OS kernel.

  int fd;

  if ((fd = open(framebuffer_device_path, O_RDWR)) < 0)
  {
    printf("DEVICE CAN'T BE OPENED");
    exit(-1);
  }

  ioctl(fd, FBIOGET_VSCREENINFO, &screen_info);

  fb_info.xres_virtual = screen_info.xres_virtual;     // 8
  fb_info.bits_per_pixel = screen_info.bits_per_pixel; // 16
  fb_info.yres_virtual = screen_info.yres_virtual;
  printf("screen_res = %dx%d, bits_per_pixel = %d\n", fb_info.xres_virtual, fb_info.yres_virtual, fb_info.bits_per_pixel);
  return fb_info;
};

int display_image(cv::Mat &img, std::ofstream &fb, struct framebuffer_info &fb_info)
{
  int x_offset = 0; // value to make image to the middle
  cv::Size2i img_size;

  img_size.height = fb_info.yres_virtual;
  img_size.width = img.cols * img.rows / img_size.height;
  x_offset = (fb_info.xres_virtual - img_size.width) / 2;
  // printf("offset = %d\n", x_offset);
  // Resize the image to fit screen
  cv::resize(img, img, img_size);

  // output the video frame to framebufer row by row
  for (int y = 0; y < img.rows; y++)
  {
    fb.seekp(y * fb_info.xres_virtual * fb_info.bits_per_pixel / 8 + x_offset * fb_info.bits_per_pixel / 8);

    fb.write(img.ptr<char>(y, 0), img.cols * fb_info.bits_per_pixel / 8);
  }
  return 0;
}

void get_files(const char *dir, std::vector<const char *> &files)
{
  DIR *dp;
  struct dirent *dirp;
  if ((dp = opendir(dir)) == NULL)
  {
    // std::cerr << "Can't open directory" << std::endl;
  }
  while ((dirp = readdir(dp)) != NULL)
  {
    if((strcmp(dirp->d_name, ".") == 0) || (strcmp(dirp->d_name, "..") == 0)){
      continue;
    }
    files.push_back(dirp->d_name);
    std::cout << "found img: " << dirp->d_name << std::endl;
  }
}