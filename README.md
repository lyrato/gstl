gstl
====

gst study
./gst-launch-1.0 filesrc location=/opt/share/1.ape ! avdemux_ape ! avdec_ape ! audioresample ! audioconvert ! alsasink
./gst-launch-1.0 filesrc location=/opt/share/1.ape ! avdemux_ape ! avdec_ape ! alsasink
./gst-launch-1.0 filesrc location=/opt/share/1.avi ! decodebin ! videoconvert ! ximagesink sync=false
