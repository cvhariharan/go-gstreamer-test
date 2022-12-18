#include <gst/gst.h>
#include "gst.h"
#ifdef __APPLE__
#include <TargetConditionals.h>
#endif



GstFlowReturn gstreamer_new_sample_handler(GstElement *object, gpointer user_data) {
  GstSample *sample = NULL;
  GstBuffer *buffer = NULL;
  gpointer copy = NULL;
  gsize copy_size = 0;

  g_signal_emit_by_name (object, "pull-sample", &sample);
  if (sample) {
    buffer = gst_sample_get_buffer(sample);
    if (buffer) {
      gst_buffer_extract_dup(buffer, 0, gst_buffer_get_size(buffer), &copy, &copy_size);
      goHandleSinkBuffer(copy, copy_size, 0);
    }
    gst_sample_unref (sample);
  }

  return GST_FLOW_OK;
}

int gst_create_pipeline(char *name) {
    gst_init(NULL, NULL);

    const char *pipelineStr = "ximagesrc ! video/x-raw,framerate=10/1 ! videoconvert ! theoraenc ! oggmux ! appsink name=appsink";
    GstElement *pipeline = gst_parse_launch(pipelineStr, NULL);
    GstElement *appsink = gst_bin_get_by_name(GST_BIN(pipeline), "appsink");
    g_object_set(appsink, "emit-signals", TRUE, NULL);
    g_signal_connect(appsink, "new-sample", G_CALLBACK(gstreamer_new_sample_handler), NULL);

    GstStateChangeReturn ret;
    ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
      g_printerr ("Unable to set the pipeline to the playing state.\n");
      gst_object_unref (pipeline);
      return -1;
    }
    return 0;
}
