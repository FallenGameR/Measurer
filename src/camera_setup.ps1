# Camera test
.\vlc.exe dshow:// 
ffmpeg -list_devices true -f dshow -i dummy
ffmpeg -f dshow -list_options true -i video="Integrated Camera"
10.120.157.99

# Server - http://4youngpadawans.com/stream-camera-video-and-audio-with-ffmpeg/
ffmpeg -f dshow -i video="Integrated Camera" -preset ultrafast -vcodec libx264 -tune zerolatency -b 900k -f mpegts udp://127.0.0.1:8080
ffmpeg -f dshow -i video="Integrated Camera" -profile:v high -pix_fmt yuvj420p -level:v 4.1 -preset ultrafast -tune zerolatency -vcodec libx264 -r 10 -b:v 512k -s 640x360 -acodec aac -ac 2 -ab 32k -ar 44100 -f mpegts -flush_packets 0 udp://127.0.0.1:8080?pkt_size=1316
ffmpeg -f video4linux2 -s 640x480 -r 30 -vcodec h264 -i /dev/video0 -an http://localhost:8099/feed1.ffm

# Client
# Args --network-caching 200
.\vlc.exe udp://@0.0.0.0:8080