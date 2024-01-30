TARGET = main
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	g++ -o programa $(TARGET).cpp $(LIBS)

clean:
	rm -f $(TARGET)
