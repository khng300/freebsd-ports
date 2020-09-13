# Created by: Ka Ho Ng <khng300@gmail.com>
# $FreeBSD$

PORTNAME=	pam_pkcs11
PORTVERSION=	0.6.11
CATEGORIES=	security

MAINTAINER=	khng300@gmail.com
COMMENT=	PAM module using crypto tokens for auth

LICENSE=	LGPL21

RUN_DEPENDS=	bash:shells/bash

USES=		autoreconf gmake libtool pkgconfig shebangfix

USE_GITHUB=	yes
GH_ACCOUNT=	OpenSC
GH_TAGNAME=	${PORTNAME}-${PORTVERSION}

GNU_CONFIGURE=	yes

CONFIGURE_ARGS=	--with-confdir=${ETCDIR}
CONFIGURE_ENV=	OPENSSL_CFLAGS="-I${OPENSSLINC}" \
		OPENSSL_LIBS="-L${OPENSSLLIB} -lcrypto"

SHEBANG_FILES=	tools/pkcs11_make_hash_link

SUB_FILES=	pkg-message

OPTIONS_DEFINE=		CURL DEBUG DOCS LDAP NLS NSS PCSC SSL
OPTIONS_DEFAULT=	PCSC SSL
OPTIONS_SUB=		yes

CURL_LIB_DEPENDS=	libcurl.so:ftp/curl
CURL_CONFIGURE_WITH=	curl

LDAP_USE=		OPENLDAP=yes
LDAP_CONFIGURE_WITH=	ldap

# Users can only choose either NSS or SSL
NSS_DESC=		Enable Mozilla's NSS support
NSS_CONFIGURE_WITH=	nss
NSS_LIB_DEPENDS=	libnss3.so:security/nss \
			libnspr4.so:devel/nspr
NSS_PREVENTS=		SSL

PCSC_DESC=		Enable PC/SC support
PCSC_LIB_DEPENDS=	libpcsclite.so:devel/pcsc-lite
PCSC_CONFIGURE_WITH=	pcsclite

SSL_USES=		ssl

NLS_CONFIGURE_ENABLE=	nls
NLS_USES=	gettext

DOCS_CONFIGURE_ENABLE=	doc

PORTDOCS=	*

.include <bsd.port.options.mk>

.if ! ${PORT_OPTIONS:MDEBUG}
INSTALL_TARGET=		install-strip
.else
CONFIGURE_ARGS+=	--with-debug=yes
.endif

.if ${PORT_OPTIONS:MDOCS}
BUILD_DEPENDS+=	xsltproc:textproc/libxslt \
		${LOCALBASE}/share/xsl/docbook/html/docbook.xsl:textproc/docbook-xsl
CONFIGURE_ENV+=	XSLTPROC="${LOCALBASE}/bin/xsltproc"
CONFIGURE_ARGS+=--with-xsl-stylesheetsdir=${LOCALBASE}/share/xsl/docbook
.else
CONFIGURE_ENV+=	XSLTPROC="${FALSE}"
CONFIGURE_ARGS+=--without-xsl-stylesheetsdir
.endif

pre-configure:
	@(cd ${WRKSRC} && ./bootstrap)

.include <bsd.port.mk>
