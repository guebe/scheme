/* (c) guenter.ebermann@htl-hl.ac.at */

#include "scheme.h"
#include <stdio.h>

static char last_error_message[256];

extern scm_obj_t scm_error(const char *message, ...)
{
	va_list ap;

	va_start(ap, message);
	vsnprintf(last_error_message, sizeof last_error_message, message, ap);
	va_end(ap);

	return SCM_ERROR;
}
