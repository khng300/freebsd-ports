--- cmake/Findzstd.cmake.orig	2020-11-22 20:00:22 UTC
+++ cmake/Findzstd.cmake
@@ -41,7 +41,12 @@ if(ZSTD_FROM_INTERNET)
 
   set(zstd_FOUND TRUE)
 else()
-  find_library(ZSTD_LIBRARY zstd)
+  set(zstd_library_name zstd)
+  if (ZSTD_SYSTEM_STATIC)
+    string(CONCAT zstd_library_name
+      ${CMAKE_STATIC_LIBRARY_PREFIX} zstd ${CMAKE_STATIC_LIBRARY_SUFFIX})
+  endif()
+  find_library(ZSTD_LIBRARY ${zstd_library_name})
   find_path(ZSTD_INCLUDE_DIR zstd.h)
 
   include(FindPackageHandleStandardArgs)
