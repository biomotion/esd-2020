#include "gui.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
using namespace cv;

// #include <dlib/image_processing/frontal_face_detector.h>
// #include <dlib/image_processing/render_face_detections.h>
// #include <dlib/image_processing.h>
// #include <dlib/gui_widgets.h>
// #include <dlib/opencv.h>
// #include <dlib/image_io.h>
// using namespace dlib;

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    Layout layout;
    cv::Mat frame, frame_gray;
    CascadeClassifier face_cascade;
    std::vector<cv::Rect> face_detections;
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " <cam number> <face_cascade_file>" << endl;
        exit(-1);
    }

    cv::VideoCapture camera(atoi(argv[1]));
    face_cascade.load(argv[2]);

    // camera.set(cv::CAP_PROP_FRAME_WIDTH, IMAGE_SIZE_X);
    // camera.set(cv::CAP_PROP_FRAME_HEIGHT, IMAGE_SIZE_Y);

    std::vector<cv::Point2d> points;
    points.push_back(cv::Point2d(80, 80));
    points.push_back(cv::Point2d(80, 95));
    points.push_back(cv::Point2d(80, 90));
    points.push_back(cv::Point2d(60, 70));
    points.push_back(cv::Point2d(100, 70));
    // points.push_back(cv::Point2d(80, 90));

    while (1)
    {
        camera >> frame;
        cout << frame.size() << endl;
        layout.setCamera(frame);

        cv::cvtColor(frame, frame_gray, cv::COLOR_RGBA2GRAY);
        cv::equalizeHist(frame_gray, frame_gray);
        face_cascade.detectMultiScale(frame_gray, face_detections);
        for (cv::Rect face : face_detections)
        {
            layout.setFace(frame(face));
            layout.setFaceFeature(points);
        }
        layout.update();
    }
}