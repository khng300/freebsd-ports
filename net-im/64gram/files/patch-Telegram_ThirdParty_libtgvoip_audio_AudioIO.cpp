--- Telegram/ThirdParty/libtgvoip/audio/AudioIO.cpp.orig	2021-04-06 00:10:23.860403000 +0800
+++ Telegram/ThirdParty/libtgvoip/audio/AudioIO.cpp	2021-04-06 00:11:36.619498000 +0800
@@ -65,7 +65,7 @@
 		return new ContextlessAudioIO<AudioInputWave, AudioOutputWave>(inputDevice, outputDevice);
 #endif
 	return new ContextlessAudioIO<AudioInputWASAPI, AudioOutputWASAPI>(inputDevice, outputDevice);
-#elif defined(__linux__)
+#elif defined(__linux__) || defined(__FreeBSD__)
 #ifndef WITHOUT_ALSA
 #ifndef WITHOUT_PULSE
 	if(AudioPulse::Load()){
