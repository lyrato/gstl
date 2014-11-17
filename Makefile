LIBGST_INC=$(shell pkg-config --cflags --libs gstreamer-1.0)
CFLAGS = -Wall 
CFLAGS += $(LIBGST_INC)
lgst:	
	gcc -Wall main.c -o lgst -Wall $(CFLAGS)
clean: 		
	rm -rf *.o 
	rm -rf lgst
