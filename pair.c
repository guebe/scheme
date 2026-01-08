/* (c) guenter.ebermann@htl-hl.ac.at */

#include "scheme.h"
#include <string.h>

#define SCM_CELL_NUM 8192U
static scm_obj_t cell[SCM_CELL_NUM];
static uint32_t cell_idx = 0;

extern unsigned char scm_string_ref(scm_obj_t string, size_t k)
{
	size_t i, l;
       
	i = string & 0xFFFFFFFF;
	l = string >> 32U & 0xFFFF;

	if (k < l)
		return cell[i+k];
	return '\0';
}

extern scm_obj_t scm_string(const char *s, uint32_t k)
{
	if ((k >> 16U) != 0)
		return scm_error("string too long");

	uint32_t i = cell_idx;
	uint32_t free = SCM_CELL_NUM - i;
	uint32_t needed = (k + 7) / 8;

	if (needed <= free) {
		memcpy(&cell[i], s, k);
		cell_idx = i + needed;
	        return SCM_STRING | ((scm_obj_t)(k & 0xFFFF) << 32U) | i;
	}

	return scm_error("out of memory");
}

extern scm_obj_t scm_cons(scm_obj_t obj1, scm_obj_t obj2)
{
	scm_obj_t pair;

	if (cell_idx + 2 >= SCM_CELL_NUM)
	       return scm_error("out of memory");

	pair = SCM_PAIR | cell_idx;
	cell[cell_idx++] = obj1;
	cell[cell_idx++] = obj2;

	return pair;
}

extern scm_obj_t scm_car(scm_obj_t pair)
{
	return scm_is_pair(pair) ? cell[(uint32_t)pair] : scm_error("car: bad argument type");
}

extern scm_obj_t scm_cdr(scm_obj_t pair)
{
	return scm_is_pair(pair) ? cell[(uint32_t)pair+1] : scm_error("cdr: bad argument type");
}

extern void scm_set_car(scm_obj_t pair, scm_obj_t obj)
{
	if (scm_is_pair(pair)) cell[(uint32_t)pair] = obj;
}

extern void scm_set_cdr(scm_obj_t pair, scm_obj_t obj)
{
	if (scm_is_pair(pair)) cell[(uint32_t)pair+1] = obj;
}

