all: labeler
	@echo "Compile Done."

labeler: frame_labeler.o video_labeler.o config_file_reader.o label.o main.o global.o
	g++ -o labeler.exe main.o frame_labeler.o video_labeler.o config_file_reader.o label.o global.o

global.o: global.cpp global.hpp
	g++ -c global.cpp

os.o: os.cpp os.hpp
	g++ -c os.cpp

utils.o: utils.cpp
	g++ -c utils.cpp

main.o: main.cpp
	g++ -c main.cpp -lm -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_features2d -lopencv_objdetect

frame_labeler.o: ./labeler/frame_labeler.cpp ./labeler/frame_labeler.hpp
	g++ -c ./labeler/frame_labeler.cpp -lm -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_features2d -lopencv_objdetect

video_labeler.o: ./labeler/video_labeler.cpp ./labeler/video_labeler.hpp
	g++ -c ./labeler/video_labeler.cpp -lm -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_features2d -lopencv_objdetect

label.o: ./labeler/label.cpp ./labeler/label.hpp
	g++ -c ./labeler/label.cpp -lm -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_features2d -lopencv_objdetect

config_file_reader.o: ./config/ConfigFileReader.cpp ./config/ConfigFileReader.hpp
	g++ -c ./config/ConfigFileReader.cpp -lm -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_features2d -lopencv_objdetect -o config_file_reader.o
clean:
	rm *.o
