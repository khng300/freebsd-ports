--- Telegram/ThirdParty/libtgvoip/webrtc_dsp/rtc_base/stringutils.h.orig	2020-07-08 10:59:39.102918600 +0800
+++ Telegram/ThirdParty/libtgvoip/webrtc_dsp/rtc_base/stringutils.h	2021-04-06 00:38:30.342878000 +0800
@@ -25,7 +25,7 @@
 #endif  // WEBRTC_WIN
 
 #if defined(WEBRTC_POSIX)
-#ifdef BSD
+#ifdef __FreeBSD__
 #include <stdlib.h>
 #else  // BSD
 #include <alloca.h>
