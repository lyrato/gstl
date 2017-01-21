

#if 0
#include <gst/gst.h>
#include <glib.h>
//定义消息处理函数,
static gboolean bus_call(GstBus *bus,GstMessage *msg,gpointer data)
{
    GMainLoop *loop = (GMainLoop *) data;//这个是主循环的指针，在接受EOS消息时退出循环
    switch (GST_MESSAGE_TYPE(msg))
    {
        case GST_MESSAGE_EOS:
            g_print("End of stream\n");
            g_main_loop_quit(loop);
            break;
        case GST_MESSAGE_ERROR:
        {
                           gchar *debug;
                           GError *error;

                           gst_message_parse_error(msg,&error,&debug);
                           g_free(debug);
                           g_printerr("ERROR:%s\n",error->message);
                           g_error_free(error);
                           g_main_loop_quit(loop);
                           break;
        }
        default:
             break;
    }
    return TRUE;
}

int main(int argc,char *argv[])
{
    GMainLoop *loop;
    GstElement *pipeline,*source,*decoder,*sink;//定义组件
    GstBus *bus;

    gst_init(&argc,&argv);
    loop = g_main_loop_new(NULL,FALSE);//创建主循环，在执行 g_main_loop_run后正式开始循环

    if(argc != 2)
    {
        g_printerr("Usage:%s <mp3 filename>\n",argv[0]);
        return -1;
    }
    //创建管道和组件
    pipeline = gst_pipeline_new("audio-player");
    source = gst_element_factory_make("filesrc","file-source");
    decoder = gst_element_factory_make("mad","mad-decoder");
    sink = gst_element_factory_make("autoaudiosink","audio-output");

    if(!pipeline||!source||!decoder||!sink){
        g_printerr("One element could not be created.Exiting.\n");
        return -1;
    }
    //设置 source的location 参数。即 文件地址.
    g_object_set(G_OBJECT(source),"location",argv[1],NULL);
    //得到 管道的消息总线
    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
   //添加消息监视器
    gst_bus_add_watch(bus,bus_call,loop);
    gst_object_unref(bus);
    //把组件添加到管道中.管道是一个特殊的组件，可以更好的让数据流动
    gst_bin_add_many(GST_BIN(pipeline),source,decoder,sink,NULL);
   //依次连接组件
   gst_element_link_many(source,decoder,sink,NULL);
   //开始播放
    gst_element_set_state(pipeline,GST_STATE_PLAYING);
    g_print("Running\n");
    //开始循环
    g_main_loop_run(loop);
    g_print("Returned,stopping playback\n");
    gst_element_set_state(pipeline,GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(pipeline));
    return 0;
}
#endif
#include <string.h>
#include <stdio.h>
#include <gst/gst.h>
//定义消息处理函数,
static gboolean bus_call(GstBus * bus, GstMessage * msg, gpointer data)
{
    GMainLoop * loop = (GMainLoop *) data; //这个是主循环的指针，在接受EOS消息时退出循环

    switch (GST_MESSAGE_TYPE(msg)) {
    case GST_MESSAGE_EOS:
        g_print("End of stream\n");
        g_main_loop_quit(loop);
        break;
    case GST_MESSAGE_ERROR: {
        gchar * debug;
        GError * error;
        gst_message_parse_error(msg, &error, &debug);
        g_free(debug);
        g_printerr("ERROR:%s\n", error->message);
        g_error_free(error);
        g_main_loop_quit(loop);
        break;
    }
    default:
        break;
    }

    return TRUE;
}
static int cnt = 1 ;
static GMainLoop * loop;
static gboolean handle_keyboard(GIOChannel * source, GIOCondition cond, GstElement * play)
{
    gchar * str = NULL;

    if (g_io_channel_read_line(source, &str, NULL, NULL, NULL) != G_IO_STATUS_NORMAL) {
        return TRUE;
    }

    switch (g_ascii_tolower(str[0])) {
    case 'p': {
        printf("\n%s %d\n", __FUNCTION__, __LINE__);

        if (cnt == 1) {
            printf("\n%s %d\n", __FUNCTION__, __LINE__);
            gst_element_set_state(play, GST_STATE_PAUSED);
        } else {
            printf("\n%s %d\n", __FUNCTION__, __LINE__);
            gst_element_set_state(play, GST_STATE_PLAYING);
        }

        cnt = 1 - cnt;
        break;
    }
	case 'q': {
        printf("\n%s %d\n", __FUNCTION__, __LINE__);
        gst_element_set_state (play, GST_STATE_NULL);  
        g_main_loop_quit (loop);  
        break;
    }
    case 's': {
        printf("\n%s %d\n", __FUNCTION__, __LINE__);
        GstEvent *event;

        event = gst_event_new_seek (1.0,
                                    GST_FORMAT_TIME,
                                    GST_SEEK_FLAG_FLUSH,
                                    GST_SEEK_TYPE_SET, 50 * GST_SECOND,
                                    GST_SEEK_TYPE_NONE, 0* GST_SECOND);

        gst_element_send_event (play, event);
        break;

    }
        case 'f': {
        
        GstEvent *event;
        static double frate = 1;
   
        frate*=2;
        if(frate-32.f>0)
        frate = 1;
        printf("\n%s %d rate :%f\n", __FUNCTION__, __LINE__,frate);
   
        event = gst_event_new_seek (frate, 
        GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH , GST_SEEK_TYPE_NONE, 0, GST_SEEK_TYPE_SET, -1);   

        gst_element_send_event (play, event);
        break;

    }
      case 'b': {
        
        GstEvent *event;
        static double brate = 1;
  
        brate*=2;
        if(brate-32.f>0)
        brate = 1;
        
        printf("\n%s %d rate :%f\n", __FUNCTION__, __LINE__,brate);
        event = gst_event_new_seek (-brate, 
        GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH ,GST_SEEK_TYPE_SET, 0, GST_SEEK_TYPE_NONE, 0);

        gst_element_send_event (play, event);
        break;

    }
    }

    return TRUE;
}

gint
main(gint   argc,
     gchar * argv[])
{
    
    GstElement * play;
    GstBus * bus;
    GIOChannel * io_stdin;
    /* init GStreamer */
    gst_init(&argc, &argv);
    loop = g_main_loop_new(NULL, FALSE);

    /* make sure we have a URI */
    if (argc != 2) {
        g_print("Usage: %s <URI>\n", argv[0]);
        return -1;
    }

    /* set up */
    play = gst_element_factory_make("playbin", "play");
    g_object_set(G_OBJECT(play), "uri", argv[1], NULL);
    bus = gst_pipeline_get_bus(GST_PIPELINE(play));
    gst_bus_add_watch(bus, bus_call, loop);
    gst_object_unref(bus);
    io_stdin = g_io_channel_unix_new(fileno(stdin));
    g_io_add_watch(io_stdin, G_IO_IN, (GIOFunc)handle_keyboard, play);
    gst_element_set_state(play, GST_STATE_PLAYING);
    printf("\n%s %d \n", __FUNCTION__, __LINE__);
    /* now run */
    g_main_loop_run(loop);
    printf("\n%s %d \n", __FUNCTION__, __LINE__);
    /* also clean up */
    gst_element_set_state(play, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(play));
    return 0;
}

