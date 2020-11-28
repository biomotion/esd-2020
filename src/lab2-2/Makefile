CC=arm-linux-gnueabihf-g++
CFLAGS=-Wall -I /opt/EmbedSky/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/include/ \
-I /usr/local/arm-opencv/install/include/ -L /usr/local/arm-opencv/install/lib/ \
-Wl,-rpath-link=/opt/EmbedSky/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/lib/ \
-Wl,-rpath-link=/opt/EmbedSky/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/qt5.5/rootfs_imx6q_V3_qt5.5_env/lib/ \
-Wl,-rpath-link=/opt/EmbedSky/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/qt5.5/rootfs_imx6q_V3_qt5.5_env/qt5.5_env/lib/ \
-Wl,-rpath-link=/opt/EmbedSky/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf/qt5.5/rootfs_imx6q_V3_qt5.5_env/usr/lib/ \
-lpthread -lopencv_world

CC_X86=g++
CFLAGS_X86=-Wall -I /usr/include/opencv4 \
-lopencv_core \
-lopencv_videoio \
-lopencv_highgui \
-lopencv_imgproc \
-lncurses

EXE_FILE=lab2-2

all: 
	${CC} lab2-2.cpp ${CFLAGS} -o ${EXE_FILE}
x86: 
	${CC_X86} read_camera.cpp ${CFLAGS_X86} -o x86_${EXE_FILE}
clean: 
	${RM} ${EXE_FILE} x86_${EXE_FILE}