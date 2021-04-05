--- Telegram/ThirdParty/libtgvoip/VoIPController.cpp.orig	2020-08-23 06:46:43.811845300 +0800
+++ Telegram/ThirdParty/libtgvoip/VoIPController.cpp	2021-04-05 23:54:40.182010000 +0800
@@ -3005,7 +3005,7 @@
 #endif
 
 double VoIPController::GetCurrentTime(){
-#if defined(__linux__)
+#if defined(__linux__) || defined(__FreeBSD__)
 	struct timespec ts;
 	clock_gettime(CLOCK_MONOTONIC, &ts);
 	return ts.tv_sec+(double)ts.tv_nsec/1000000000.0;
