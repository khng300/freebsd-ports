Index: interface/cooked_interface.c
===================================================================
RCS file: /home/cvs/cdparanoia/interface/cooked_interface.c,v
retrieving revision 1.1.1.1
retrieving revision 1.8
diff -u -r1.1.1.1 -r1.8
--- interface/cooked_interface.c	2003/01/05 09:46:26	1.1.1.1
+++ interface/cooked_interface.c	2003/01/11 08:58:45	1.8
@@ -1,6 +1,8 @@
 /******************************************************************
  * CopyPolicy: GNU Public License 2 applies
  * Copyright (C) Monty xiphmont@mit.edu
+ * FreeBSD porting (c) 2003
+ * 	Simon 'corecode' Schubert <corecode@corecode.ath.cx>
  *
  * CDROM code specific to the cooked ioctl interface
  *
@@ -10,6 +12,7 @@
 #include "common_interface.h"
 #include "utils.h"
 
+#ifdef Linux
 static int cooked_readtoc (cdrom_drive *d){
   int i;
   int tracks;
@@ -129,6 +132,128 @@
   return(sectors);
 }
 
+#elif defined(__FreeBSD__)
+static int
+cooked_readtoc(cdrom_drive *d)
+{
+	int i;
+	struct ioc_toc_header hdr;
+	struct ioc_read_toc_single_entry entry;
+
+	if (ioctl(d->ioctl_fd, CDIOREADTOCHEADER, &hdr) == -1) {
+		int ret;
+
+		if (errno == EPERM) {
+			ret = -102;
+			cderror(d, "102: ");
+		} else {
+			ret = -4;
+			cderror(d, "004: Unable to read table of contents header: ");
+		}
+		cderror(d, strerror(errno));
+		cderror(d, "\n");
+		return ret;
+	}
+
+	entry.address_format = CD_LBA_FORMAT;
+	for (i = hdr.starting_track; i <= hdr.ending_track; ++i) {
+		entry.track = i;
+		
+		if (ioctl(d->ioctl_fd, CDIOREADTOCENTRY, &entry) == -1) {
+			cderror(d, "005: Unable to read table of contents entry\n");
+			return -5;
+		}
+
+		d->disc_toc[i - hdr.starting_track].bFlags = entry.entry.control;
+		d->disc_toc[i - hdr.starting_track].bTrack = entry.entry.track;
+		d->disc_toc[i - hdr.starting_track].dwStartSector = be32_to_cpu(entry.entry.addr.lba);
+	}
+	
+	entry.track = 0xaa;	/* leadout */
+
+	if (ioctl(d->ioctl_fd, CDIOREADTOCENTRY, &entry) == -1) {
+		cderror(d, "005: Unable to read table of contents entry\n");
+		return -5;
+	}
+
+	d->disc_toc[i - hdr.starting_track].bFlags = entry.entry.control;
+	d->disc_toc[i - hdr.starting_track].bTrack = entry.entry.track;
+	d->disc_toc[i - hdr.starting_track].dwStartSector = be32_to_cpu(entry.entry.addr.lba);
+	
+	d->cd_extra = FixupTOC(d, hdr.ending_track - hdr.starting_track + 2);	/* with TOC */
+
+	return hdr.ending_track - hdr.starting_track + 1;
+}
+
+static int
+cooked_setspeed(cdrom_drive *d, int speed)
+{
+#ifdef CDRIOCREADSPEED
+	speed *= 177;
+	return ioctl(d->ioctl_fd, CDRIOCREADSPEED, &speed);
+#else
+	return -1;
+#endif
+}
+
+
+static long
+cooked_read(cdrom_drive *d, void *p, long begin, long sectors)
+{
+	int retry_count = 0;
+	struct ioc_read_audio arg;
+
+	if (sectors > d->nsectors)
+		sectors = d->nsectors;
+
+	arg.address_format = CD_LBA_FORMAT;
+	arg.address.lba = begin;
+	arg.buffer = p;
+
+	for (;;) {
+		arg.nframes = sectors;
+		if (ioctl(d->ioctl_fd, CDIOCREADAUDIO, &arg) == -1) {
+			if (!d->error_retry)
+				return -7;
+
+			switch (errno) {
+			case ENOMEM:
+				if (sectors == 1) {
+					cderror(d, "300: Kernel memory error\n");
+					return -300;
+				}
+				/* FALLTHROUGH */
+			default:
+				if (sectors == 1) {
+					if (retry_count > MAX_RETRIES - 1) {
+						char b[256];
+						snprintf(b, sizeof(b),
+						    "010: Unable to access sector %ld; "
+						    "skipping...\n", begin);
+						cderror(d, b);
+						return -10;
+					}
+					break;
+				}
+			}
+
+			if (retry_count > 4 && sectors > 1)
+				sectors = sectors * 3 / 4;
+
+			++retry_count;
+
+			if (retry_count > MAX_RETRIES) {
+				cderror(d, "007: Unknown, unrecoverable error reading data\n");
+				return -7;
+			}
+		} else
+			break;
+	}
+
+	return sectors;
+}
+#endif
+
 /* hook */
 static int Dummy (cdrom_drive *d,int Switch){
   return(0);
@@ -193,6 +318,7 @@
 int cooked_init_drive (cdrom_drive *d){
   int ret;
 
+#ifdef Linux
   switch(d->drive_type){
   case MATSUSHITA_CDROM_MAJOR:	/* sbpcd 1 */
   case MATSUSHITA_CDROM2_MAJOR:	/* sbpcd 2 */
@@ -243,6 +369,9 @@
   default:
     d->nsectors=40; 
   }
+#elif defined(__FreeBSD__)
+  d->nsectors = 26;		/* FreeBSD only support 64K I/O transfer size */
+#endif
   d->enable_cdda = Dummy;
   d->read_audio = cooked_read;
   d->set_speed = cooked_setspeed;
