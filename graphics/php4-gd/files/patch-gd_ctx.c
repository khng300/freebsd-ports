--- gd_ctx.c.orig	Wed Mar 12 05:16:31 2003
+++ gd_ctx.c	Sat Jun 28 15:47:56 2003
@@ -24,17 +24,22 @@
 	
 static void _php_image_output_ctx(INTERNAL_FUNCTION_PARAMETERS, int image_type, char *tn, void (*func_p)()) 
 {
-	zval **imgind, **file, **quality;
+	zval **imgind, **file, **quality, **lo, **to, **del, **dis;
 	gdImagePtr im;
 	char *fn = NULL;
 	FILE *fp = NULL;
 	int argc = ZEND_NUM_ARGS();
 	int q = -1, i;
+#ifdef HAVE_GD_GIF_ANIM
+	int LeftOfs = -1, TopOfs = -1, Delay = -1, Disposal = -1;
+#endif /* HAVE_GD_GIF_ANIM */
 	gdIOCtx *ctx;
 
 	/* The quality parameter for Wbmp stands for the threshold when called from image2wbmp() */
+	/* The quality parameter for GIF animation stands for colormap inclusion. 1==include local/global colormap */
+	/* The LeftOfs parameter for GIF animation begin stands for NETSCAPE2.0 Loop count extension. */
 	
-	if (argc < 1 || argc > 3 || zend_get_parameters_ex(argc, &imgind, &file, &quality) == FAILURE) 
+	if (argc < 1 || argc > 7 || zend_get_parameters_ex(argc, &imgind, &file, &quality, &lo, &to, &del, &dis) == FAILURE) 
 	{
 		WRONG_PARAM_COUNT;
 	}
@@ -44,11 +49,29 @@
 	if (argc > 1) {
 		convert_to_string_ex(file);
 		fn = Z_STRVAL_PP(file);
-		if (argc == 3) {
-			convert_to_long_ex(quality);
-			q = Z_LVAL_PP(quality);
-		}
 	}
+	if (argc >= 3) {
+		convert_to_long_ex(quality);
+		q = Z_LVAL_PP(quality);
+	}
+#ifdef HAVE_GD_GIF_ANIM
+	if (argc >= 4) {
+		convert_to_long_ex(lo);
+		LeftOfs = Z_LVAL_PP(lo);
+	}
+	if (argc >= 5) {
+		convert_to_long_ex(to);
+		TopOfs = Z_LVAL_PP(to);
+	}
+	if (argc >= 6) {
+		convert_to_long_ex(del);
+		Delay = Z_LVAL_PP(del);
+	}
+	if (argc >= 7) {
+		convert_to_long_ex(dis);
+		Disposal = Z_LVAL_PP(dis);
+	}
+#endif /* HAVE_GD_GIF_ANIM */
 
 	if ((argc == 2) || (argc > 2 && Z_STRLEN_PP(file))) {
 		if (!fn || fn == empty_string || php_check_open_basedir(fn TSRMLS_CC)) {
@@ -56,7 +79,7 @@
 			RETURN_FALSE;
 		}
 
-		fp = VCWD_FOPEN(fn, "wb");
+		fp = VCWD_FOPEN(fn, PHP_GDIMG_TYPE_GIFANIMADD == image_type ? "ab" : "wb");
 		if (!fp) {
 			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to open '%s' for writing", fn);
 			RETURN_FALSE;
@@ -94,6 +117,14 @@
 			} 
 			(*func_p)(im, i, ctx);
 			break;
+#ifdef HAVE_GD_GIF_ANIM
+		case PHP_GDIMG_TYPE_GIFANIMBEGIN:
+			(*func_p)(im, ctx, q, LeftOfs);
+			break;
+		case PHP_GDIMG_TYPE_GIFANIMADD:
+			(*func_p)(im, ctx, q, LeftOfs, TopOfs, Delay, Disposal);
+			break;
+#endif /* HAVE_GD_GIF_ANIM */
 		default:
 			(*func_p)(im, ctx);
 			break;
@@ -112,3 +143,72 @@
 	
     RETURN_TRUE;
 }
+
+#ifdef HAVE_GD_GIF_ANIM
+/* {{{ proto int imagegifanimend([string filename])
+   Write end mark to gif animation. */
+PHP_FUNCTION(imagegifanimend)
+{
+	zval **file;
+	char *fn = NULL;
+	FILE *fp = NULL;
+	int argc = ZEND_NUM_ARGS();
+	gdIOCtx *ctx;
+
+	if (argc < 0 || argc > 1 || zend_get_parameters_ex(argc, &file) == FAILURE) {
+		ZEND_WRONG_PARAM_COUNT();
+	}
+
+	if (argc >= 1) {
+		convert_to_string_ex(file);
+		fn = Z_STRVAL_PP(file);
+	}
+
+	if ((argc == 1) || (argc > 1 && Z_STRLEN_PP(file))) {
+		if (!fn || fn == empty_string || php_check_open_basedir(fn TSRMLS_CC)) {
+			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid filename '%s'", fn);
+			RETURN_FALSE;
+		}
+
+		fp = VCWD_FOPEN(fn, "ab");
+		if (!fp) {
+			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to open '%s' for writing", fn);
+			RETURN_FALSE;
+		}
+
+		ctx = gdNewFileCtx(fp);
+	} else {
+		ctx = emalloc(sizeof(gdIOCtx));
+		ctx->putC = _php_image_output_putc;
+		ctx->putBuf = _php_image_output_putbuf;
+#if HAVE_LIBGD204
+		ctx->gd_free = _php_image_output_ctxfree;
+#else
+		ctx->free = _php_image_output_ctxfree;
+#endif		
+
+#if APACHE && defined(CHARSET_EBCDIC)
+		/* XXX this is unlikely to work any more thies@thieso.net */
+		/* This is a binary file already: avoid EBCDIC->ASCII conversion */
+		ap_bsetflag(php3_rqst->connection->client, B_EBCDIC2ASCII, 0);
+#endif
+	}
+
+	/* This could be coded in here, as it only outputs ';' */
+	gdImageGifAnimEndCtx(ctx);
+
+#if HAVE_LIBGD204	
+	ctx->gd_free(ctx);
+#else
+	ctx->free(ctx);
+#endif		
+
+	if(fp) {
+		fflush(fp);
+		fclose(fp);
+	}
+	
+    RETURN_TRUE;
+}
+/* }}} */
+#endif /* HAVE_GD_GIF_ANIM */
