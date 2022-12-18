#pragma once

#include <stdio.h>
#include <gst/gst.h>

extern void goHandleSinkBuffer(void *buffer, int bufferLen, int elementId);
int gst_create_pipeline(char *name);
