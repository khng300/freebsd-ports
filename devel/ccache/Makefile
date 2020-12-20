# Created by: Dominic Marks <d.marks@student.umist.ac.uk>
# $FreeBSD$

PORTNAME=	ccache
#PORTVERSION must be set later due to depending on PORT_OPTIONS.
PORTREVISION=	1
CATEGORIES=	devel
MASTER_SITES=	https://github.com/ccache/ccache/releases/download/v${DISTVERSION}/ \
		LOCAL/bdrewery

MAINTAINER=	bdrewery@FreeBSD.org
COMMENT=	Tool to minimize the compile time of C/C++ programs

LICENSE=	GPLv3+
LICENSE_FILE=	${WRKSRC}/GPL-3.0.txt

CONFLICTS_INSTALL?=	ccache-static-[0-9]* ccache-memcached-[0-9]* ccache-memcached-static-[0-9]*

HOWTO=		ccache-howto-freebsd.txt
CCLINKDIR=	libexec/ccache
SUB_FILES=	${HOWTO} world-ccache pkg-message ccache-update-links.sh

PORTDOCS=	ccache-howto-freebsd.txt MANUAL.html MANUAL.adoc NEWS.adoc \
		NEWS.html

OPTIONS_DEFINE=	CLANGLINK LLVMLINK STATIC DOCS TINDERBOX MEMCACHED
OPTIONS_DEFAULT=CLANGLINK LLVMLINK

CLANGLINK_DESC=	Create clang compiler links if clang is installed
LLVMLINK_DESC=	Create llvm compiler links if llvm is installed
TINDERBOX_DESC=	Create tarball for tinderbox usage
MEMCACHED_DESC=	Build in experimental Memcached support

MEMCACHED_EXTRA_PATCHES=	\
				${FILESDIR}/extra-patch-memcached-configure.ac \
				${FILESDIR}/extra-patch-memcached-Makefile.in
MEMCACHED_CONFIGURE_ENABLE=	memcached
MEMCACHED_USES=			compiler autoreconf gmake pkgconfig
MEMCACHED_USES_OFF=		compiler:c++11-lang cmake
MEMCACHED_BUILD_DEPENDS_OFF=	zstd>=1.1.2:archivers/zstd
MEMCACHED_LIB_DEPENDS=		libmemcached.so:databases/libmemcached
MEMCACHED_LDFLAGS=		-L${LOCALBASE}/lib
MEMCACHED_CFLAGS=		-I${LOCALBASE}/include
MEMCACHED_CMAKE_ON=		-DZSTD_SYSTEM_STATIC:BOOL=On

STATIC_CONFIGURE_ENABLE=	static

.if defined(WITH_CCACHE_BUILD) && empty(OPTIONS_SLAVE:MMEMCACHED) && \
    !defined(NO_CCACHE_DEPEND)
MEMCACHED_IGNORE=		MEMCACHED cannot be combined with WITH_CCACHE_BUILD.  Use devel/ccache-memcached
# XXX: This needs more testing with Poudriere before enabling. Also bsd.options.mk support.
#MEMCACHED_DEPENDS_ARGS+= NO_CCACHE_DEPEND=1
.endif

# Support WITH_CCACHE_BUILD but don't depend on it.
NO_CCACHE_DEPEND=	yes

OPTIONS_SUB=	yes

STATIC_LDFLAGS+=	-static

.include <bsd.port.options.mk>

.if ${PORT_OPTIONS:MDOCS}
BUILD_DEPENDS+=	a2x:textproc/asciidoc
.endif

.if ${PORT_OPTIONS:MMEMCACHED}
# Hack for libsasl2.a requiring HMAC_CTX_reset from hidden libntlm.a.
STATIC_LDFLAGS+= -L${LOCALBASE}/lib/sasl2
PORTVERSION=	3.5.dev.20180926
USE_GITHUB=	yes
GH_TAGNAME=	f7712082106bef085515f86ae47e7dbbd2a48b57
MASTER_SITES=
DISTINFO_FILE=	${MASTERDIR}/distinfo.memcached

GNU_CONFIGURE=	yes
EXTRA_PATCHES+=	${FILESDIR}/extra-patch-src__ccache.c
.if ${PORT_OPTIONS:MDOCS}
ALL_TARGET=	all docs
.endif
.else
PORTVERSION=	4.1

#PATCH_SITES is set later due to depending on PORT_OPTIONS.
PATCH_SITES=	https://github.com/ccache/ccache/commit/
PATCHFILES+=	5504310039c09dd21919817aa93ef67647a5d758.patch:-p1	\
		6426ae47c902f3061dbc3c4c816a4bbbe692fa92.patch:-p1

.if ${PORT_OPTIONS:MDOCS}
ALL_TARGET=	all doc
.endif
.endif

.include <bsd.port.pre.mk>

.  if ${COMPILER_TYPE} == clang
CPPFLAGS+=	-DCC_IS_CLANG
.  elif ${COMPILER_TYPE} == gcc
CPPFLAGS+=	-DCC_IS_GCC
.  endif

PLIST_SUB+=	CCLINKDIR="${CCLINKDIR}"

.if ${ARCH}=="i386"
CCACHE_COMPILERS+=	icc icpc
.endif

GNU_COMPILERS+=		34 42 43 44 45 46 47 48 49 5 6 7 8 9 10
CCACHE_COMPILERS+=	cc c++ CC gcc g++ ${GNU_COMPILERS:S|^|gcc|} \
			${GNU_COMPILERS:S|^|g++|} \
			${GNU_COMPILERS:S|^|cpp|}

.if ${PORT_OPTIONS:MCLANGLINK}
CLANG_COMPILERS+=	33 34 35 36 37 38 39 40 50 60 70 80 90 10 -devel
CCACHE_COMPILERS+=	clang clang++ ${CLANG_COMPILERS:S|^|clang|} \
			${CLANG_COMPILERS:S|^|clang++|} \
			${CLANG_COMPILERS:S|^|cpp|}
.endif

.if ${PORT_OPTIONS:MLLVMLINK}
CCACHE_COMPILERS+=	llvm-gcc llvm-c++ llvm-g++
.endif

CCACHE_COMPILERS+=	${EXTRA_COMPILERS}
SUB_LIST+=	CCACHE_COMPILERS="${CCACHE_COMPILERS}" \
		CCLINKDIR="${CCLINKDIR}" \
		ICCPREFIX="${LOCALBASE}/intel_cc_80/bin" \
		HOWTO="${HOWTO}"

post-build-TINDERBOX-on:
	@${MKDIR} ${WRKDIR}/tb/opt
	${INSTALL_PROGRAM} ${WRKSRC}/${PORTNAME} ${WRKDIR}/tb/opt
.for l in ${CCACHE_COMPILERS}
	@${LN} -sf ${PORTNAME} ${WRKDIR}/tb/opt/${l}
.endfor
	@${TAR} -C ${WRKDIR}/tb -cpf ${WRKSRC}/${PORTNAME}.tar opt

do-install:
	${INSTALL_PROGRAM} ${BUILD_WRKSRC}/ccache ${STAGEDIR}${PREFIX}/bin
	${MKDIR} ${STAGEDIR}${PREFIX}/${CCLINKDIR}/world
	${INSTALL_SCRIPT} ${WRKDIR}/world-ccache \
		${STAGEDIR}${PREFIX}/${CCLINKDIR}/world/ccache
	${INSTALL_SCRIPT} ${WRKDIR}/ccache-update-links.sh \
		${STAGEDIR}${PREFIX}/bin/ccache-update-links

do-install-TINDERBOX-on:
	${MKDIR} ${STAGEDIR}${DATADIR}
	${INSTALL_DATA} ${WRKSRC}/${PORTNAME}.tar ${STAGEDIR}${DATADIR}

do-install-DOCS-on:
	${MKDIR} ${STAGEDIR}${DOCSDIR}
	${INSTALL_DATA} ${BUILD_WRKSRC}/doc/ccache.1 ${STAGEDIR}${MAN1PREFIX}/man/man1
	${INSTALL_DATA} ${BUILD_WRKSRC}/doc/MANUAL.html ${STAGEDIR}${DOCSDIR}
	${INSTALL_DATA} ${WRKSRC}/doc/MANUAL.adoc ${STAGEDIR}${DOCSDIR}
	${INSTALL_DATA} ${WRKSRC}/doc/NEWS.adoc ${STAGEDIR}${DOCSDIR}
	${INSTALL_DATA} ${BUILD_WRKSRC}/doc/NEWS.html ${STAGEDIR}${DOCSDIR}
	${INSTALL_DATA} ${WRKDIR}/${HOWTO} ${STAGEDIR}${DOCSDIR}

.include <bsd.port.post.mk>
