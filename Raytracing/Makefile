CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT

OBJS =		Raytracing.o Vector.o Ray.o Plane.o Camera.o Sphere.o Material.o

LIBS = `sdl-config --libs` -lGL -lGLU -lstdc++

TARGET =	Raytracing

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
