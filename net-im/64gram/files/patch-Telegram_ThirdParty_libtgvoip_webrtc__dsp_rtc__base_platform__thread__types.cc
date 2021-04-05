--- Telegram/ThirdParty/libtgvoip/webrtc_dsp/rtc_base/platform_thread_types.cc.orig	2020-08-23 06:46:43.814845800 +0800
+++ Telegram/ThirdParty/libtgvoip/webrtc_dsp/rtc_base/platform_thread_types.cc	2021-04-06 00:37:25.198845000 +0800
@@ -10,6 +10,9 @@
 
 #include "rtc_base/platform_thread_types.h"
 
+#ifdef __FreeBSD__
+#include <sys/thr.h>
+#endif
 #if defined(WEBRTC_LINUX)
 #include <sys/prctl.h>
 #include <sys/syscall.h>
@@ -27,6 +30,10 @@
   return gettid();
 #elif defined(WEBRTC_FUCHSIA)
   return zx_thread_self();
+#elif defined(__FreeBSD__)
+  long tid;
+  thr_self(&tid);
+  return tid;
 #elif defined(WEBRTC_LINUX)
   return syscall(__NR_gettid);
 #else
