#!/bin/sh
#
# $FreeBSD$
#

if ! PREFIX=$(expr $0 : "\(/.*\)/etc/rc\.d/$(basename $0)\$"); then
    echo "$0: Cannot determine the PREFIX" >&2
    exit 1
fi

case "$1" in
start)
	/sbin/ldconfig -m ${PREFIX}/%%SHLIB_SUBDIR%%
	;;
stop)
	;;
*)
	echo "Usage: `basename $0` {start|stop}" 2>&1
	exit 64
	;;
esac
