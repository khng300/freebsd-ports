--- Telegram/ThirdParty/libtgvoip/audio/AudioOutput.cpp.orig	2020-07-08 10:59:38.946883700 +0800
+++ Telegram/ThirdParty/libtgvoip/audio/AudioOutput.cpp	2021-04-06 00:13:19.367093000 +0800
@@ -83,7 +83,7 @@
 	}
 #endif
 	AudioOutputWASAPI::EnumerateDevices(devs);
-#elif defined(__linux__) && !defined(__ANDROID__)
+#elif (defined(__linux__) || defined(__FreeBSD__)) && !defined(__ANDROID__)
 #if !defined(WITHOUT_PULSE) && !defined(WITHOUT_ALSA)
 	if(!AudioOutputPulse::EnumerateDevices(devs))
 		AudioOutputALSA::EnumerateDevices(devs);
