#include <iostream>
#include <cstdlib>
#include <vector>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "face_detector.h"

using namespace std;
int main(int argc, char *argv[])
{

  if (argc != 4)
  {
    cout << "Usage: " << argv[0] << " <cam number> <face_cascade_file> <eye_cascade_file>" << endl;
    exit(-1);
  }

  cv::VideoCapture camera(atoi(argv[1]));
  FaceDetector detector(argv[2], argv[3]);
  cv::Mat frame;
  std::vector<cv::Rect> face_detections;
  std::vector<cv::Rect> eye_detections;
  while (1)
  {
    camera >> frame;
    detector.detect_faces(frame, face_detections);
    for (cv::Rect face : face_detections)
    {
      detector.detect_eyes_from_face(frame(face), eye_detections);
      // std::cout << eye_detections.size() << std::endl;
      cv::Rect roi;
      cv::Mat face_image, edge;
      uint16_t border = 0;
      roi.x = max(0, face.x - border);
      roi.y = max(0, face.y - border);
      roi.height = min(frame.rows-roi.y, face.height + 2*border);
      roi.width = min(frame.cols-roi.x, face.width + 2*border);
      face_image = frame(roi);
      cv::resize(face_image, face_image, cv::Size(350, 350));
      cv::Canny(face_image, edge, 60, 80);
      // cv::cvtColor(frame(roi), edge, cv::COLOR_RGB2GRAY);
      cv::imshow("Face Edge", edge);
    }

    detector.draw_faces(frame, frame, face_detections);
    // cv::imshow("Face Detections", frame);
    if (cv::waitKey(10) == 27)
      break;
  }

  return 0;
}
