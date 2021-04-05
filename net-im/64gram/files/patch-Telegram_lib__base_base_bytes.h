--- Telegram/lib_base/base/bytes.h.orig	2020-07-08 11:00:20.061610700 +0800
+++ Telegram/lib_base/base/bytes.h	2021-04-06 00:41:11.881647000 +0800
@@ -145,7 +145,7 @@
 vector concatenate(Args &&...args) {
 	const auto size = details::spansLength(args...);
 	auto result = vector(size);
-	details::spansAppend(make_span(result), args...);
+	details::spansAppend(bytes::make_span(result), args...);
 	return result;
 }
 
@@ -157,7 +157,7 @@
 		size += bytes::make_span(arg).size();
 	}
 	auto result = vector(size);
-	auto buffer = make_span(result);
+	auto buffer = bytes::make_span(result);
 	for (const auto &arg : args) {
 		const auto part = bytes::make_span(arg);
 		bytes::copy(buffer, part);
