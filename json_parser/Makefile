all: parser
	@echo "Compile Done."

parser: valuedfa.o intdfa.o stringdfa.o arraydfa.o dfa.o value.o main.o 
	g++ -g -o labeler.exe intdfa.o stringdfa.o arraydfa.o dfa.o value.o main.o 

intdfa.o: intdfa.h intdfa.cpp
	g++ -c intdfa.cpp

stringdfa.o: stringdfa.h stringdfa.cpp
	g++ -c stringdfa.cpp

value.o: value.hpp value.cpp
	g++ -c value.cpp

valuedfa.o: valuedfa.h valuedfa.cpp
	g++ -c valuedfa.cpp
	
arraydfa.o: arraydfa.h arraydfa.cpp
	g++ -c arraydfa.cpp

dfa.o: dfa.h dfa.cpp
	g++ -c dfa.cpp

main.o: config_parser.cpp
	g++ -c config_parser.cpp -o main.o
clean:
	rm *.o
