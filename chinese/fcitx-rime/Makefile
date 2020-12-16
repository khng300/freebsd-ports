# Created by: Zhihao Yuan <lichray@gmail.com>
# $FreeBSD$

PORTNAME=	fcitx-rime
DISTVERSION=	0.3.2
PORTREVISION=	2
CATEGORIES=	chinese
MASTER_SITES=	https://download.fcitx-im.org/${PORTNAME}/
DIST_SUBDIR=	fcitx

PATCH_SITES=	https://github.com/fcitx/${PORTNAME}/commit/
PATCHFILES=	a3a9fffb6ee89adc327ad8a7dec05d5f35414f9d.patch:-p1

MAINTAINER=	lichray@gmail.com
COMMENT=	RIME support for Fcitx

LICENSE=	GPLv2

BUILD_DEPENDS=	bash:shells/bash \
		zh-rime-data>=g0:chinese/rime-data
LIB_DEPENDS=	libfcitx-config.so:chinese/fcitx \
		librime.so:chinese/librime
RUN_DEPENDS=	zh-rime-data>=g0:chinese/rime-data

USES=		cmake gettext gnome localbase pkgconfig tar:xz
INSTALLS_ICONS=	yes

.include <bsd.port.mk>
