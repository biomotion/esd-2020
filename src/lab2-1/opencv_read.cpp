#include <iostream>
#include <fstream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace std;

int main(int argc, char** argv){
  cv::Mat img;
  ifstream fd(argv[1], fstream::binary);
  stringstream ss;
  vector<uchar>* data;
  char c;
  while( fd >> ss.rdbuf() ) {}

  data = new vector<uchar>(ss.str().begin(), ss.str().end());
  img = cv::imdecode(cv::Mat(*data), cv::IMREAD_UNCHANGED);
  // img = cv::imread(argv[1]);
  cv::imshow("image", img);
  cout << "size: " << img.size << endl;
  cv::waitKey();
  return 0;

}
