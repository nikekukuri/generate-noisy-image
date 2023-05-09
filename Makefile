CC = c++
CFLAGS =  -I/usr/local/Cellar/opencv/4.7.0_2/include/opencv4/
LDFLAGS = -L/usr/local/Cellar/opencv/4.7.0_2/lib/
LDLIBS =  -lopencv_core -lopencv_highgui -lopencv_imgcodecs
VERSION = -std=c++11

main: main.cpp
	$(CC) $< -o $@ $(CFLAGS) $(VERSION) $(LDFLAGS) $(LDLIBS)

clean :
	rm main


