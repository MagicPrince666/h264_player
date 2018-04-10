CPP = g++

TARGET	= decodeH264

DIR		= . ./camera ./H264_camera ./listop ./ringbuf
INC		= -I./camera -I./include -I/usr/include -I./listop -I./ringbuf -I./H264_camera

FFMPEGDIR = ../wifi/ffmpeg
FFMPEGLIB = -L$(FFMPEGDIR)/libavformat -L$(FFMPEGDIR)/libavcodec -L$(FFMPEGDIR)/libavutil -L$(FFMPEGDIR)/libswscale -L$(FFMPEGDIR)/libswresample
CFLAGS	= -g -Wall -O2
LDFLAGS += $(FFMPEGLIB) -L./lib -lx264 -lavformat -lavcodec -lavutil -lswscale -lswresample
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