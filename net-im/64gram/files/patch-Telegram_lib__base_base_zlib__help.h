--- Telegram/lib_base/base/zlib_help.h.orig	2020-07-08 11:00:20.083615300 +0800
+++ Telegram/lib_base/base/zlib_help.h	2021-04-06 00:46:32.510692000 +0800
@@ -6,8 +6,8 @@
 //
 #pragma once
 
-#include <zip.h>
-#include <unzip.h>
+#include "minizip/zip.h"
+#include "minizip/unzip.h"
 #include "logs.h"
 
 #ifdef small
