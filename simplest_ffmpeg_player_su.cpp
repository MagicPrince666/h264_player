/**
 * 最简单的基于FFmpeg的视频播放器2(SDL升级版)
 * Simplest FFmpeg Player 2(SDL Update)
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 第2版使用SDL2.0取代了第一版中的SDL1.2
 * Version 2 use SDL 2.0 instead of SDL 1.2 in version 1.
 *
 * 本程序实现了视频文件的解码和显示(支持HEVC，H.264，MPEG2等)。
 * 是最简单的FFmpeg视频解码方面的教程。
 * 通过学习本例子可以了解FFmpeg的解码流程。
 * 本版本中使用SDL消息机制刷新视频画面。
 * This software is a simplest video player based on FFmpeg.
 * Suitable for beginner of FFmpeg.
 *
 * 备注:
 * 标准版在播放视频的时候，画面显示使用延时40ms的方式。这么做有两个后果：
 * （1）SDL弹出的窗口无法移动，一直显示是忙碌状态
 * （2）画面显示并不是严格的40ms一帧，因为还没有考虑解码的时间。
 * SU（SDL Update）版在视频解码的过程中，不再使用延时40ms的方式，而是创建了
 * 一个线程，每隔40ms发送一个自定义的消息，告知主函数进行解码显示。这样做之后：
 * （1）SDL弹出的窗口可以移动了
 * （2）画面显示是严格的40ms一帧
 * Remark:
 * Standard Version use's SDL_Delay() to control video's frame rate, it has 2
 * disadvantages:
 * (1)SDL's Screen can't be moved and always "Busy".
 * (2)Frame rate can't be accurate because it doesn't consider the time consumed 
 * by avcodec_decode_video2()
 * SU（SDL Update）Version solved 2 problems above. It create a thread to send SDL 
 * Event every 40ms to tell the main loop to decode and show video frames.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

//#include "h264encoder.h"
//#include "video_capture.h"
//#include "h264_camera.h"
#include "listop.h" 
#include "ringbuffer.h"
//#include "H264_UVC_TestAP.h"
#include "xag_time_tick.h"

//#define __STDC_CONSTANT_MACROS
#define SOFT_H264 0
#if SOFT_H264
#else
#define RTSP_VIDEO 1
#endif

#if RTSP_VIDEO
#include "XagRTSPClient.h"
#endif

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <SDL2/SDL.h>
};


//Refresh Event
#define SFM_REFRESH_EVENT  (SDL_USEREVENT + 1)

#define SFM_BREAK_EVENT  (SDL_USEREVENT + 2)

int thread_exit=0;
int thread_pause=0;


int sfp_refresh_thread(void *opaque){
	thread_exit=0;
	thread_pause=0;

	while (!thread_exit) {
		if(!thread_pause){
			SDL_Event event;
			event.type = SFM_REFRESH_EVENT;
			SDL_PushEvent(&event);
		}
		SDL_Delay(33);
	}
	thread_exit=0;
	thread_pause=0;
	//Break
	SDL_Event event;
	event.type = SFM_BREAK_EVENT;
	SDL_PushEvent(&event);

	return 0;
}

FILE *fp_open = NULL;
//读取数据的回调函数-------------------------  
//AVIOContext使用的回调函数！  
//注意：返回值是读取的字节数  
//手动初始化AVIOContext只需要两个东西：内容来源的buffer，和读取这个Buffer到FFmpeg中的函数  
//回调函数，功能就是：把buf_size字节数据送入buf即可  
//第一个参数(void *opaque)一般情况下可以不用  
int fill_buffer(void * opaque,uint8_t *buf, int buf_size){  
	if(!feof(fp_open)){  
		int true_size=fread(buf,1,buf_size,fp_open);  
		return true_size;  
	}else{  
		return -1;  
	}  
}  

bool sdl_ready = false;

void *video_decoder_Thread(void *arg)
{
	AVFormatContext	*pFormatCtx;
	unsigned int				i, videoindex;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	AVFrame	*pFrame,*pFrameYUV;
	unsigned char *out_buffer;
	AVPacket *packet;
	int ret, got_picture;	

	//------------SDL----------------
	int screen_w,screen_h;
	SDL_Window *screen; 
	SDL_Renderer* sdlRenderer;
	SDL_Texture* sdlTexture;
	SDL_Rect sdlRect;
	SDL_Thread *video_tid;
	SDL_Event event;

	struct SwsContext *img_convert_ctx;	

	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();

	
	//Init AVIOContext
	unsigned char *aviobuffer=(unsigned char *)av_malloc(32768);
#if SOFT_H264
	AVIOContext *avio =avio_alloc_context(aviobuffer, 32768,0,NULL,read_buffer,NULL,NULL);
#else
#if RTSP_VIDEO
	AVIOContext *avio =avio_alloc_context(aviobuffer, 32768,0,NULL,rtsp_read_buf,NULL,NULL);
#else
	AVIOContext *avio =avio_alloc_context(aviobuffer, 32768,0,NULL,read_buf,NULL,NULL);
#endif
#endif
	pFormatCtx->pb=avio;

/*
	if(avformat_open_input(&pFormatCtx,filepath,NULL,NULL)!=0){
		printf("Couldn't open input stream.\n");
		return -1;
	}
*/

	if(avformat_open_input(&pFormatCtx,NULL,NULL,NULL)!=0){
		printf("Couldn't open input stream.\n");
		return NULL;
	}

	if(avformat_find_stream_info(pFormatCtx,NULL)<0){
		printf("Couldn't find stream information.\n");
		return NULL;
	}
	videoindex=-1;
	for(i=0; i < pFormatCtx->nb_streams; i++) 
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
			videoindex=i;
			break;
		}
	if(videoindex==-1){
		printf("Didn't find a video stream.\n");
		return NULL;
	}
	pCodecCtx=pFormatCtx->streams[videoindex]->codec;
	pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
	if(pCodec==NULL){
		printf("Codec not found.\n");
		return NULL;
	}
	if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){
		printf("Could not open codec.\n");
		return NULL;
	}
	pFrame=av_frame_alloc();
	pFrameYUV=av_frame_alloc();

	out_buffer=(unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P,  pCodecCtx->width, pCodecCtx->height,1));
	av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize,out_buffer,
		AV_PIX_FMT_YUV420P,pCodecCtx->width, pCodecCtx->height,1);

	//Output Info-----------------------------
	//printf("---------------- File Information ---------------\n");
	//av_dump_format(pFormatCtx,0,filepath,0);
	//printf("-------------------------------------------------\n");
	
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, 
		pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL); 
	

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {  
		printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
		return NULL;
	} 
	//SDL 2.0 Support for multiple windows
	screen_w = pCodecCtx->width;
	screen_h = pCodecCtx->height;
	screen = SDL_CreateWindow("Prince's ffmpeg player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		screen_w, screen_h,SDL_WINDOW_OPENGL);

	if(!screen) {  
		printf("SDL: could not create window - exiting:%s\n",SDL_GetError());  
		return NULL;
	}
	sdlRenderer = SDL_CreateRenderer(screen, -1, 0);  
	//IYUV: Y + U + V  (3 planes)
	//YV12: Y + V + U  (3 planes)
	sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,pCodecCtx->width,pCodecCtx->height);  

	sdl_ready = true;

	sdlRect.x=0;
	sdlRect.y=0;
	sdlRect.w=screen_w;
	sdlRect.h=screen_h;

	packet=(AVPacket *)av_malloc(sizeof(AVPacket));

	video_tid = SDL_CreateThread(sfp_refresh_thread,NULL,NULL);
	
	//------------SDL End------------
	//Event Loop
	//sleep(10);
	for (;;) {
		//Wait
		SDL_WaitEvent(&event);
		if(event.type==SFM_REFRESH_EVENT){
			while(1){
				if(av_read_frame(pFormatCtx, packet)<0)
					thread_exit=1;

				if(packet->stream_index==videoindex)
					break;
			}
			ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
			if(ret < 0){
				printf("Decode Error.\n");
				return NULL;
			}
			if(got_picture){
				sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
				//SDL---------------------------
				SDL_UpdateTexture( sdlTexture, NULL, pFrameYUV->data[0], pFrameYUV->linesize[0] );  
				SDL_RenderClear( sdlRenderer );  
				//SDL_RenderCopy( sdlRenderer, sdlTexture, &sdlRect, &sdlRect );  
				SDL_RenderCopy( sdlRenderer, sdlTexture, NULL, NULL);  
				SDL_RenderPresent( sdlRenderer );  
				//SDL End-----------------------
			}
			av_free_packet(packet);
		}else if(event.type==SDL_KEYDOWN){
			//Pause
			if(event.key.keysym.sym==SDLK_SPACE)
				thread_pause=!thread_pause;
		}else if(event.type==SDL_QUIT){
			thread_exit=1;
			//capturing = 0;
			XagRtsp::rtsp_ison = false;
		}else if(event.type==SFM_BREAK_EVENT){
			break;
		}
		usleep(1000);
	}

	sws_freeContext(img_convert_ctx);

	SDL_Quit();
	//--------------
	av_free(aviobuffer);
	av_frame_free(&pFrameYUV);
	av_frame_free(&pFrame);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);
	return NULL;
}


//RingBuffer* rbuf;
RingBuffer player_ring;
cycle_buffer* player_buffer = NULL;

void xag_transmit_speed(int data) {
	//printf("timer %s, %d\n", __FUNCTION__, g_sec++);
	printf("live555 in %.2f KB/s\n", (double)XagRtsp::live_cnt/1000.0);
	//printf("aoa out %.2f KB/s\n", (double)A2spipe::aoa_cnt/1000.0);
	XagRtsp::live_cnt = 0;
	//A2spipe::aoa_cnt = 0;
	add_timer(1, xag_transmit_speed);
}

int main(int argc, char* argv[])
{

	signal(SIGALRM, tick);
	alarm(1); // 1s的周期心跳
	add_timer(1, xag_transmit_speed);
	// if(argc >=2)
	// 	fp_open=fopen(argv[1],"rb+");
	// else
	// 	fp_open=fopen("bigbuckbunny_480x272.h264","rb+");
	

	//rbuf = RingBuffer_create(DEFAULT_BUF_SIZE);
	if(player_buffer != NULL){
    printf("init aoa ringbuf\n");
  } else {
    player_buffer = player_ring.ring_init(DEFAULT_BUF_SIZE);
  }

	pthread_t thread[3];

	if((pthread_create(&thread[2], NULL, video_decoder_Thread, NULL)) != 0)   
		printf("video_decoder_Thread create fail!\n");

#if SOFT_H264
	cam = (struct camera *) malloc(sizeof(struct camera));
	if (!cam) {
		printf("malloc camera failure!\n");
		exit(1);
	}
	cam->device_name = (char *)DEVICE;
	cam->buffers = NULL;
	cam->width = SET_WIDTH;
	cam->height = SET_HEIGHT;
	cam->fps = 25;

	framelength = sizeof(unsigned char)*cam->width * cam->height * 2;

	v4l2_init(cam);

	init(Buff);

	//创建线程
	printf("Making thread...\n");

	thread_create();

	if((pthread_create(&thread[2], NULL, video_decoder_Thread, NULL)) != 0)   
                printf("video_decoder_Thread create fail!\n");
	thread_wait();
	if(thread[2] != 0) {   
        pthread_join(thread[2],NULL);
    }
	v4l2_close(cam);

#else

#if RTSP_VIDEO
	if((pthread_create(&thread[1], NULL, XagRtsp::rtsp_thead, NULL)) != 0)   
		printf("rtsp video create fail!\n");
#else
	if((pthread_create(&thread[1], NULL, cap_video, NULL)) != 0)   
		printf("cap_video create fail!\n");
#endif	
	
	if(thread[1] != 0) {   
        pthread_join(thread[1],NULL);
    }
	
#endif

	if(thread[2] != 0) {   
        pthread_join(thread[2],NULL);
    }

	//RingBuffer_destroy(rbuf);
	player_ring.destroy(player_buffer);
    player_buffer = NULL;
	return 0;
}
