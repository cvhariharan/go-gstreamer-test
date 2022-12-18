package main

/*
#cgo pkg-config: gstreamer-1.0 gstreamer-app-1.0

#include "gst.h"
*/
import "C"
import (
	"log"
	"os"
	"time"
	"unsafe"
)

var video *os.File

func main() {
	C.gst_create_pipeline(C.CString("test-pipeline"))

	video, _ = os.Create("desktop2.ogg")
	defer video.Close()

	time.Sleep(10 * time.Second)
}

//export goHandleSinkBuffer
func goHandleSinkBuffer(buffer unsafe.Pointer, bufferLen C.int, elementID C.int) {
	_, err := video.Write(C.GoBytes(buffer, bufferLen))
	if err != nil {
		log.Println(err)
	}
}
