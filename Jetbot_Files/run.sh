./gst-rtsp-server/examples/test-launch "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720, format=(string)NV12, framerate=(fraction)60/1 ! nvvidconv ! omxh265enc ! rtph265pay name=pay0 pt=96 config-interval=1"