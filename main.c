/* (c) guenter.ebermann@htl-hl.ac.at */

#include "scm754.h"
#include <stdio.h>

int main(void)
{
	scm_obj_t obj;

	while (1) {
		fputs("> ", stdout);
		fflush(stdout);

		obj = scm_read();
		if (scm_is_eof_object(obj)) { break; }
		else if (scm_is_error_object(obj)) { puts(scm_error_object_message(obj)); continue; }

#ifndef SCM_NO_EVAL
		obj = scm_eval(obj, scm_empty_list());
		if (scm_is_error_object(obj)) { puts(scm_error_object_message(obj)); continue; }
#endif

		scm_write(obj);
		putchar('\n');
	}

	putchar('\n');
	return 0;
}
