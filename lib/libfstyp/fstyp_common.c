/*-
 * Copyright (c) 2016 The DragonFly Project
 * Copyright (c) 2014 The FreeBSD Foundation
 * All rights reserved.
 *
 * This software was developed by Edward Tomasz Napierala under sponsorship
 * from the FreeBSD Foundation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/param.h>
#include <err.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libfstyp.h"

bool show_label = false;

typedef int (*fstyp_function)(FILE *, char *, size_t, const char *);

static struct {
	const char	*name;
	fstyp_function	function;
} fstypes[] = {
	{ "apfs", &fstyp_apfs },
	{ "befs", &fstyp_befs },
	{ "cd9660", &fstyp_cd9660 },
	{ "exfat", &fstyp_exfat },
	{ "ext2fs", &fstyp_ext2fs },
	{ "hfs+", &fstyp_hfsp },
	{ "msdosfs", &fstyp_msdosfs },
	{ "ntfs", &fstyp_ntfs },
	{ "ufs", &fstyp_ufs },
	{ "hammer", &fstyp_hammer },
	{ "hammer2", &fstyp_hammer2 },
	{ NULL, NULL }
};

void *
read_buf(FILE *fp, off_t off, size_t len)
{
	int error;
	size_t nread;
	void *buf;

	error = fseek(fp, off, SEEK_SET);
	if (error != 0) {
		warn("cannot seek to %jd", (uintmax_t)off);
		return (NULL);
	}

	buf = malloc(len);
	if (buf == NULL) {
		warn("cannot malloc %zd bytes of memory", len);
		return (NULL);
	}

	nread = fread(buf, len, 1, fp);
	if (nread != 1) {
		free(buf);
		if (feof(fp) == 0)
			warn("fread");
		return (NULL);
	}

	return (buf);
}

char *
checked_strdup(const char *s)
{
	char *c;

	c = strdup(s);
	if (c == NULL)
		err(1, "strdup");
	return (c);
}

void
rtrim(char *label, size_t size)
{
	ptrdiff_t i;

	for (i = size - 1; i >= 0; i--) {
		if (label[i] == '\0')
			continue;
		else if (label[i] == ' ')
			label[i] = '\0';
		else
			break;
	}
}

const char *
fstyp_identify(FILE *fp)
{
	char label[512];
	int i, error;

	for (i = 0; fstypes[i].function != NULL; i++) {
		memset(label, 0, sizeof(label));
		error = fstypes[i].function(fp, label, sizeof(label), NULL);
		if (error == 0)
			return (fstypes[i].name);
	}

	return (NULL);
}
