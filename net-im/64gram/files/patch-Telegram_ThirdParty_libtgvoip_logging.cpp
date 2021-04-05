--- Telegram/ThirdParty/libtgvoip/logging.cpp.orig	2021-04-06 00:15:51.608648000 +0800
+++ Telegram/ThirdParty/libtgvoip/logging.cpp	2021-04-06 00:16:19.084066000 +0800
@@ -13,7 +13,7 @@
 
 #ifdef __ANDROID__
 #include <sys/system_properties.h>
-#elif defined(__linux__)
+#elif defined(__linux__) || defined(__FreeBSD__)
 #include <sys/utsname.h>
 #endif
 
@@ -51,7 +51,7 @@
 #else
 			char* systemVersion="Windows RT";
 #endif
-#elif defined(__linux__)
+#elif defined(__linux__) || defined(__FreeBSD__)
 #ifdef __ANDROID__
 		char systemVersion[128];
 		char sysRel[PROP_VALUE_MAX];
