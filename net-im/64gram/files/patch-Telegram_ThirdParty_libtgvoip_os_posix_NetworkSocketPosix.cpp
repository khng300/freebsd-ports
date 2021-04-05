--- Telegram/ThirdParty/libtgvoip/os/posix/NetworkSocketPosix.cpp.orig	2020-07-08 10:59:38.972889400 +0800
+++ Telegram/ThirdParty/libtgvoip/os/posix/NetworkSocketPosix.cpp	2021-04-06 00:32:27.389000000 +0800
@@ -64,6 +64,7 @@
 	if(res<0){
 		LOGE("error setting darwin-specific net priority: %d / %s", errno, strerror(errno));
 	}
+#elif defined(__FreeBSD__)
 #elif defined(__linux__)
 	int prio=6;
 	int res=setsockopt(fd, SOL_SOCKET, SO_PRIORITY, &prio, sizeof(prio));
