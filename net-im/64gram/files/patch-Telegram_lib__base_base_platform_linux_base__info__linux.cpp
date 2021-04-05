--- Telegram/lib_base/base/platform/linux/base_info_linux.cpp.orig	2021-03-13 21:41:56.948387900 +0800
+++ Telegram/lib_base/base/platform/linux/base_info_linux.cpp	2021-04-06 00:43:13.415268000 +0800
@@ -22,11 +22,6 @@
 #include <glibmm.h>
 #include <giomm.h>
 
-// this file is used on both Linux & BSD
-#ifdef Q_OS_LINUX
-#include <gnu/libc-version.h>
-#endif // Q_OS_LINUX
-
 namespace Platform {
 namespace {
 
@@ -113,7 +108,7 @@
 	static const auto result = [&] {
 		QStringList resultList{};
 
-#ifdef Q_OS_LINUX
+#if 0
 		resultList << "Linux";
 #else // Q_OS_LINUX
 		resultList << QSysInfo::kernelType();
@@ -198,7 +193,7 @@
 }
 
 QString GetLibcName() {
-#ifdef Q_OS_LINUX
+#if 0
 	return "glibc";
 #endif // Q_OS_LINUX
 
@@ -206,7 +201,7 @@
 }
 
 QString GetLibcVersion() {
-#ifdef Q_OS_LINUX
+#if 0
 	static const auto result = [&] {
 		const auto version = QString::fromLatin1(gnu_get_libc_version());
 		return QVersionNumber::fromString(version).isNull() ? QString() : version;
