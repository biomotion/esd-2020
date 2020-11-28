#include <iostream>
#include <fstream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <cstdlib>
#include <poll.h>
#include <termios.h>

using namespace std;

int main(int argc, char** argv){
  cv::Mat img;
  int cam_index;
  cout << argc << endl;
  if(argc > 1){
    cam_index = atoi(argv[1]);
  }else{
    cam_index = 0;
  }
  cv::VideoCapture cam(cam_index);
  struct pollfd pfds[1] = {{ fd:0, events:POLLIN, 0 }};
  int ret = 0;
  struct termios t;

  tcgetattr(0, &t);
  t.c_lflag &= ~ICANON;
  tcsetattr(0, TCSANOW, &t);

  while(true){
    cam >> img;
    cv::imshow("image", img);
    cv::waitKey(1);
    ret = poll(pfds, 1, 10);
    if(ret == 1){
      cout << cin.get() << endl;
    }
  }

  return 0;

}
