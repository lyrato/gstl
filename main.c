#include <gst/gst.h>  
#include <glib.h>  
#include <libmpdemux/demuxer.h>
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
double video_time_usage;
double vout_time_usage;
static double audio_time_usage;
static int total_time_usage_start;
static int total_frame_cnt;
static int drop_frame_cnt; // total number of dropped frames
int benchmark;
int enable_mouse_movements;
int audio_id  = -1;
int video_id  = -1;
int dvdsub_id = -1;
// this dvdsub_id was selected via slang
// use this to allow dvdnav to follow -slang across stream resets,
// in particular the subtitle ID for a language changes
int dvdsub_lang_id;
int vobsub_id = -1;
char *audio_lang;
char *dvdsub_lang;
char *filename;
int file_filter = 1;

// sub:
char *font_name;
char *sub_font_name;
float font_factor = 0.75;
char **sub_name;
char **sub_paths;
float sub_delay;
float sub_fps;
int sub_auto = 1;
char *vobsub_name;
int subcc_enabled;
int suboverlap_enabled = 1;
char *current_module; // for debugging
int stream_cache_size = -1;

float stream_cache_min_percent      = 1.0;
float stream_cache_seek_min_percent = 10.0;
double force_fps=0;

void exit_player(int how)
{
   
}
void set_osd_subtitle(subtitle *subs)
{
	}
int main(int argc,char *argv[])  
{  
    GMainLoop *loop;  
    GstElement *pipeline,*source,*decoder,*sink;//定义组件  
    GstBus *bus;  
    #if 1
    {
    int file_format = 0;
     stream_t * p_stream = NULL;
     void * p_demuxer;
    p_stream =   open_stream(NULL, 0, &file_format);
    //p_demuxer = demux_mp_open((stream_t *)(p_stream), file_format, -1, -1, -1, NULL);
		}
    
    #endif
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
    printf("\n%s %d %x\n",__FUNCTION__,__LINE__,decoder);
    decoder = gst_element_factory_make("mad","mad-decoder");  
    sink = gst_element_factory_make("autoaudiosink","audio-output");  
    printf("\n%s %d %x\n",__FUNCTION__,__LINE__,source);
    printf("\n%s %d %x\n",__FUNCTION__,__LINE__,decoder);
    printf("\n%s %d %x\n",__FUNCTION__,__LINE__,sink);
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
    usleep(1000*1000);  
    g_print("Returned,stopping playback\n");  
    gst_element_set_state(pipeline,GST_STATE_NULL);  
    gst_object_unref(GST_OBJECT(pipeline));  
    return 0;  
}  
