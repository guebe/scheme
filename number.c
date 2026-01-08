/* (c) guenter.ebermann@htl-hl.ac.at */

#include "scheme.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

/* R7RS, section 6.2.7, Numerical input and output */
extern scm_obj_t scm_string_to_number(const char *string, int radix)
{
	char *end;
	double value;
	scm_obj_t obj;

	errno = 0;

	if (radix > 0)
		value = (double)strtol(string, &end, radix);
	else
		value = strtod(string, &end);

	if ((end == string) || (*end != '\0') || (errno != 0)) return scm_false();

	memcpy(&obj, &value, sizeof obj);

	return obj;
}
