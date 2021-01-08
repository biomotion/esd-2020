CXX_X86=g++
CFLAGS_X86=-Wall
OPENCV_INC=-I /usr/include/opencv4
OPENCV_LIB=-lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_datasets -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_line_descriptor -lopencv_optflow -lopencv_video -lopencv_plot -lopencv_reg -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_rgbd -lopencv_viz -lopencv_surface_matching -lopencv_text -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_core

all: face_detector.h face_detector.cpp eye_recognition.cpp
	${CXX_X86} $^ -o main --std=c++11 ${OPENCV_INC} ${OPENCV_LIB}


gui_test: gui_test.cpp gui.hpp
	${CXX_X86} -DDLIB_NO_GUI_SUPPORT gui_test.cpp -o gui_test --std=c++11 ${OPENCV_INC} ${OPENCV_LIB}

example_face_detection: example_face_detection.cpp
	${CXX_X86} -o $@ $^ ${CFLAGS_X86} ${OPENCV_INC} ${OPENCV_LIB}

clean:
	${RM} face_detector.o main.o example_face_detection