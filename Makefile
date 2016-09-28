LIBGST_INC=$(shell pkg-config --cflags --libs gstreamer-1.0 )
MINC = -I /opt/MPlayer-1.1.1
MLIB = /opt/MPlayer-1.1.1 
MLIBAVFORMATE =/opt/MPlayer-1.1.1/ffmpeg/libavformat
MLIBAVUTIL =/opt/MPlayer-1.1.1/ffmpeg/libavutil
MLIBAVCODEC =/opt/MPlayer-1.1.1/ffmpeg/libavcodec
MLIBSW =/opt/MPlayer-1.1.1/ffmpeg/libswscale
MLIBPPROCE =/opt/MPlayer-1.1.1/ffmpeg/libpostproc
MLINK = -L$(MLIBAVFORMATE) -lavformat -L$(MLIBAVCODEC) -lavcodec -L$(MLIBSW) -lswscale -L$(MLIBPPROCE) -lpostproc -L$(MLIBAVUTIL) -lavutil   
CFLAGS = -Wall 
CFLAGS += $(LIBGST_INC) 
EXTRALIBS          = -Wl,-z,noexecstack -lm  -ffast-math   -lncurses -lasound -ldl -lpthread -lz -lpthread -ldl -rdynamic  
EXTRALIBS_MPLAYER  =  -lXext -lX11 -lpthread -lXv -lXxf86vm -lXxf86dga
lgst:	
	gcc -Wall main.c -o lgst -g -Wall $(CFLAGS) 
clean: 		
	rm -rf *.o 
	rm -rf lgst
