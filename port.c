/* (c) guenter.ebermann@htl-hl.ac.at */

#include "scm754.h"
#include <stdio.h>

static int peek_buffer = EOF;

extern int scm_read_char(void)
{
	int c;

	if (peek_buffer == EOF) {
		c = getchar();
	} else {
		c = peek_buffer;
		peek_buffer = EOF;
	}

	return c;
}

extern int scm_peek_char(void)
{
	if (peek_buffer == EOF) {
		peek_buffer = getchar();
	}

	return peek_buffer;
}
