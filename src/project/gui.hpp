#ifndef __GUI_HPP__
#define __GUI_HPP__

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

#define FRAME_SIZE_X 800
#define FRAME_SIZE_Y 480
#define IMAGE_SIZE_X 640
#define IMAGE_SIZE_Y 480
#define FACE_SIZE 160

const char WINDOW_NAME[] = "result";
class Layout
{

  cv::Mat frame;
  cv::Rect camera_roi, face_roi, face_feat_roi;

public:
  Layout() : frame(FRAME_SIZE_Y, FRAME_SIZE_X, 16)
  {
    camera_roi.x = 0;
    camera_roi.y = 0;
    camera_roi.height = IMAGE_SIZE_Y;
    camera_roi.width = IMAGE_SIZE_X;

    face_roi.x = IMAGE_SIZE_X;
    face_roi.y = 0;
    face_roi.height = FACE_SIZE;
    face_roi.width = FACE_SIZE;

    face_feat_roi.x = IMAGE_SIZE_X;
    face_feat_roi.y = FACE_SIZE;
    face_feat_roi.height = FACE_SIZE;
    face_feat_roi.width = FACE_SIZE;
  }
  void setCamera(cv::InputArray in_frame)
  {
    cv::resize(in_frame, frame(camera_roi), cv::Size2i(camera_roi.width, camera_roi.height));
  }
  void setFace(cv::InputArray in_frame)
  {
    cv::resize(in_frame, frame(face_roi), cv::Size2i(face_roi.width, face_roi.height));
  }
  void setFaceFeature(const std::vector<cv::Point2d> &points)
  {
    frame(face_roi).copyTo(frame(face_feat_roi));
    for (std::vector<cv::Point2d>::const_iterator it = points.begin(); it != points.end(); it++)
    {
      cv::circle(frame(face_feat_roi), *it, 1, cv::Scalar(0, 255, 0));
    }
  }
  void update()
  {
    cv::imshow(WINDOW_NAME, frame);
    cv::waitKey(1);
  }
};

#endif