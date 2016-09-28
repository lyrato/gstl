
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
        }
    }

    return TRUE;
}

gint
main(gint   argc,
     gchar * argv[])
{
    GMainLoop * loop;
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

