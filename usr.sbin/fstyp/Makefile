PROG=	fstyp
SRCS=	fstyp.c

MAN=	fstyp.8

WARNS?=	2

CFLAGS+=-I${.CURDIR}/../../sys

LDADD+=	-lfstyp
DPADD+=	${LIBFSTYP}

.include <bsd.prog.mk>
