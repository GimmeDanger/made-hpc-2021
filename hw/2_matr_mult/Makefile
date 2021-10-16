CC=g++
CPPFLAGS=-O3 -g -Wall -Werror
LDFLAGS=

VPATH=%.cpp ../src
VPATH=%.h ../src

SOURCES=main.cpp helpers.cpp mat_mult.cpp mat_mult_strassen.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=mult_bench

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
.cpp .o:
	$(CC) -c $(CPPFLAGS) $< -o $@
clean:
	rm -f *.o mult_bench
