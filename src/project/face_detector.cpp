#include "face_detector.h"

FaceDetector::FaceDetector()
{
    faceCascade.load("haarcascade_frontalface_alt.xml");
    eyeCascade.load("haarcascade_eye_tree_eyeglasses.xml");
}

FaceDetector::FaceDetector(const char *face_cascade_file, const char* eye_cascade_file)
{
    faceCascade.load(face_cascade_file);
    eyeCascade.load(eye_cascade_file);
}

int FaceDetector::detect_faces(cv::InputArray frame, std::vector<cv::Rect> &faces)
{
    cv::Mat frame_gray;
    cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
    // cv::imshow("gray frame", frame_gray);

    cv::equalizeHist(frame_gray, frame_gray);
    // cv::imshow("gray frame equalized", frame_gray);
    faceCascade.detectMultiScale(frame_gray, faces);

    // cv::waitKey(10);
    return faces.size();
}

void FaceDetector::draw_faces(cv::InputArray in_frame, cv::OutputArray out_frame, const std::vector<cv::Rect> &faces)
{
    cv::Mat frame;

    in_frame.copyTo(frame);
    for (cv::Rect face : faces)
    {
        cv::rectangle(frame, face, cv::Scalar(255, 0, 0));
    }
    frame.copyTo(out_frame);
}

int FaceDetector::detect_eyes_from_face(cv::InputArray frame, std::vector<cv::Rect>& eyes){
    cv::Mat frame_gray;
    cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
    // cv::imshow("gray frame", frame_gray);

    cv::equalizeHist(frame_gray, frame_gray);
    // cv::imshow("gray frame equalized", frame_gray);
    eyeCascade.detectMultiScale(frame_gray, eyes);

    // cv::waitKey(10);
    return eyes.size();
}

