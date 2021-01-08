#ifndef __FACE_DETECTOR_H__
#define __FACE_DETECTOR_H__

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <vector>

class FaceDetector{
cv::CascadeClassifier faceCascade;
cv::CascadeClassifier eyeCascade;
public:
FaceDetector();
FaceDetector(const char* face_cascade_file, const char* eye_cascade_file);
int detect_faces(cv::InputArray frame, std::vector<cv::Rect>& faces);
void draw_faces(cv::InputArray in_frame, cv::OutputArray out_frame, const std::vector<cv::Rect>& faces);
int detect_eyes_from_face(cv::InputArray frame, std::vector<cv::Rect>& eyes);
};
#endif