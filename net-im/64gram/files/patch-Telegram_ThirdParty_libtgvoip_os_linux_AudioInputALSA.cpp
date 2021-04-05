--- Telegram/ThirdParty/libtgvoip/os/linux/AudioInputALSA.cpp.orig	2020-07-08 10:59:38.969888700 +0800
+++ Telegram/ThirdParty/libtgvoip/os/linux/AudioInputALSA.cpp	2021-04-06 00:18:21.129888000 +0800
@@ -12,6 +12,10 @@
 #include "../../logging.h"
 #include "../../VoIPController.h"
 
+#ifndef typeof
+#define typeof __typeof__
+#endif
+
 using namespace tgvoip::audio;
 
 #define BUFFER_SIZE 960
