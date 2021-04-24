sudo apt-get install libgstreamer1.0-0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio

git clone https://github.com/GStreamer/gst-rtsp-server.git

cd gst-rtsp-server

git checkout 1.14

sudo apt-get install gtk-doc-tools

sudo ./autogen.sh

sudo make

sudo make install