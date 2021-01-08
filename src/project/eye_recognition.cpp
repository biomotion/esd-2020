#include <iostream>
#include <cstdlib>
#include <vector>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "face_detector.h"

using namespace std;
int main(int argc, char *argv[])
{

  if (argc != 5)
  {
    cout << "Usage: " << argv[0] << " <cam number> <face_cascade_file> <eye_cascade_file> <nose_cascade_file>" << endl;
    exit(-1);
  }

  cv::VideoCapture camera(atoi(argv[1]));
  cv::CascadeClassifier face_detector, eye_detector, nose_detector;
  cv::Mat frame, frame_gray;
  std::vector<cv::Rect> face_detections;
  std::vector<cv::Rect> eye_detections;
  std::vector<cv::Rect> nose_detections;

  face_detector.load(argv[2]);
  eye_detector.load(argv[3]);
  nose_detector.load(argv[4]);
  while (1)
  {
    camera >> frame;
    cv::cvtColor(frame, frame_gray, cv::COLOR_RGBA2GRAY);
    cv::equalizeHist(frame_gray, frame_gray);
    face_detector.detectMultiScale(frame_gray, face_detections);

    for (cv::Rect face : face_detections)
    {
      float eye_height, eye_dist, nose_height;
      cv::Mat face_with_eye = frame_gray(face);
      cv::Mat face_with_eye_colored = frame(face);

      cv::resize(face_with_eye, face_with_eye, cv::Size(400, 400));
      cv::resize(face_with_eye_colored, face_with_eye_colored, cv::Size(400, 400));
      std::vector<cv::Point2i> eye_anchors, nose_anchors;
      eye_detector.detectMultiScale(face_with_eye, eye_detections);
      nose_detector.detectMultiScale(face_with_eye, nose_detections);
      for (cv::Rect nose : nose_detections)
      {
        cv::rectangle(face_with_eye_colored, nose, cv::Scalar(255, 0, 0));
        nose_anchors.push_back(cv::Point2i(nose.x + nose.width / 2, nose.y + nose.height / 2));
      }
      for (cv::Rect eye : eye_detections)
      {
        eye_anchors.push_back(cv::Point2i(eye.x + eye.width / 2, eye.y + eye.height / 2));
        cv::rectangle(face_with_eye_colored, eye, cv::Scalar(0, 255, 0));
      }
      cv::imshow("Face With Eye", face_with_eye_colored);
      // TODO: try eye height and eye distance
      // TODO: try nose height
      // cout << "# eyes, # nose: " << eye_anchors.size() << ", " << nose_anchors.size() << endl;
      if (eye_anchors.size() < 2 || nose_anchors.size() < 1){
        continue;
      }
      eye_height = (float)(eye_anchors[0].y + eye_anchors[1].y) / 2 / face_with_eye.rows;
      eye_dist = (float)abs(eye_anchors[0].x - eye_anchors[1].x) / face_with_eye.cols;
      nose_height = (float)nose_anchors[0].y / face_with_eye.rows;

      cout << "eye_height | \teye_dist | \tnose_height" << endl;
      cout << eye_height << "\t" << eye_dist << "\t" << nose_height << endl;
    }
    if (face_detections.size() >= 1)
    {
      cv::rectangle(frame, face_detections.at(0), cv::Scalar(255, 255, 0));
    }

    cv::imshow("Face Detections", frame);
    if (cv::waitKey(10) == 27)
      break;
  }

  return 0;
}
