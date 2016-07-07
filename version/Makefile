LIBGST_INC=$(shell pkg-config --cflags --libs gstreamer-0.11)
CFLAGS = -Wall 
CFLAGS += $(LIBGST_INC)
version:	
	gcc -Wall version.c -o version -Wall $(CFLAGS)
clean: 		
	rm -rf *.o 
	rm -rf version
