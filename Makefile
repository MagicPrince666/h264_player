CPP = g++

TARGET	= decodeH264

DIR		= . ./camera ./H264_camera ./listop ./ringbuf
INC		= -I./include -I/usr/include -I./listop -I./ringbuf -I./H264_camera
CFLAGS	= -g -Wall -O2
LDFLAGS += -L./lib -lx264 -lavformat -lavcodec -lavutil -lswscale -lswresample 
LDFLAGS += -lSDL2 -lpthread -ldl -lz -lm

OBJPATH	= ./objs

FILES	= $(foreach dir,$(DIR),$(wildcard $(dir)/*.cpp))

OBJS	= $(patsubst %.cpp,%.o,$(FILES))

all:$(OBJS) $(TARGET)

$(OBJS):%.o:%.cpp
	$(CPP) $(CFLAGS) $(INC) -c -o $(OBJPATH)/$(notdir $@) $< 

$(TARGET):$(OBJPATH)
	$(CPP) -o $@ $(OBJPATH)/*.o $(LDFLAGS) `sdl2-config --cflags --libs`

#$(OBJPATH):
#	mkdir -p $(OBJPATH)

clean:
	-rm -f $(OBJPATH)/*.o
	-rm -f $(TARGET)