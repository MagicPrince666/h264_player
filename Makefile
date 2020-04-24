CXX = g++

TARGET	= decodeH264

FFMPEGDIR = ../FFmpeg
DIR		= . ./H264_camera ./listop ./ringbuf
INC		+= -I$(FFMPEGDIR) -I/usr/include -I./listop -I./ringbuf -I./H264_camera

livedir := live/
livemedia := $(livedir)liveMedia
usageenviroment := $(livedir)UsageEnvironment
basicusage := $(livedir)BasicUsageEnvironment
groupsock := $(livedir)groupsock
testProgs := $(livedir)testProgs

LIBDIR = -L$(usageenviroment) -L$(basicusage) -L$(livemedia) -L$(groupsock)

INC		+= -I. -I$(usageenviroment)/include -I$(basicusage)/include -I$(livemedia)/include -I$(groupsock)/include

LDFLAGS := $(LIBDIR) -lliveMedia -lBasicUsageEnvironment -lgroupsock -lUsageEnvironment

FFMPEGLIB = -L$(FFMPEGDIR)/libavformat -L$(FFMPEGDIR)/libavcodec -L$(FFMPEGDIR)/libavutil -L$(FFMPEGDIR)/libswscale -L$(FFMPEGDIR)/libswresample
#FFMPEGLIB += -L$(FFMPEGDIR)/avdevice -L$(FFMPEGDIR)/avfilter -L$(FFMPEGDIR)/avresample
CFLAGS	= -g -Wall -std=c++11 -O2
LDFLAGS += $(FFMPEGLIB) -lavformat -lavcodec -lavutil -lswscale -lswresample #-lavdevice -lavfilter -lavresample
LDFLAGS += -lX11 -lvdpau -lSDL2 -lpthread -ldl -lz -lm -lusb-1.0

OBJPATH	= .

FILES	= $(foreach dir,$(DIR),$(wildcard $(dir)/*.cpp))

OBJS	= $(patsubst %.cpp,%.o,$(FILES))

all:
	cd $(livedir) ; ./genMakefiles linux
	cd $(livemedia) ; $(MAKE) 
	cd $(groupsock) ; $(MAKE) 
	cd $(usageenviroment) ; $(MAKE) 
	cd $(basicusage) ; $(MAKE)
	cd . ; $(MAKE) $(OBJS) $(TARGET)

$(OBJS):%.o:%.cpp
	$(CXX) $(CFLAGS) $(INC) -c -o $(OBJPATH)/$(notdir $@) $< 

$(TARGET):$(OBJPATH)
	$(CXX) -o $@ $(OBJPATH)/*.o $(LDFLAGS) `sdl2-config --cflags --libs`

#$(OBJPATH):
#	mkdir -p $(OBJPATH)

clean:
	-rm -f $(OBJPATH)/*.o
	-rm -f $(TARGET)

