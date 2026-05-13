LIB=	fstyp
SHLIB_MAJOR= 1

SRCS=	fstyp_common.c apfs.c befs.c cd9660.c exfat.c ext2fs.c \
	hfsplus.c msdosfs.c ntfs.c ufs.c hammer.c hammer2.c

INCS=	libfstyp.h

CFLAGS+= -I${.CURDIR}/../../sys

WARNS?=	2

.include <bsd.lib.mk>
