--- Telegram/ThirdParty/libtgvoip/audio/AudioInput.cpp.orig	2020-07-08 10:59:38.945883800 +0800
+++ Telegram/ThirdParty/libtgvoip/audio/AudioInput.cpp	2021-04-06 00:06:24.166719000 +0800
@@ -72,7 +72,7 @@
 	}
 #endif
 	AudioInputWASAPI::EnumerateDevices(devs);
-#elif defined(__linux__) && !defined(__ANDROID__)
+#elif (defined(__linux__) || defined(__FreeBSD__)) && !defined(__ANDROID__)
 #if !defined(WITHOUT_PULSE) && !defined(WITHOUT_ALSA)
 	if(!AudioInputPulse::EnumerateDevices(devs))
 		AudioInputALSA::EnumerateDevices(devs);
