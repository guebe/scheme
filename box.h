/* (c) guenter.ebermann@htl-hl.ac.at
 * Tiny Scheme Interpreter
 *
 * NaN boxing
 */
#ifndef __BOX_H__
#define __BOX_H__

#define SCM_NIL  0U /* '() - the empty list */
#define SCM_BOOLEAN 1U /* LSB: 0 ... #f, 1 ... #t */
#define SCM_SYMBOL 2U /* index to len/8 cells */
#define SCM_STRING 3U /* index to len/8 cells */
#define SCM_CHARACTER 4U /* LSB: character */
#define SCM_CONS 5U /* cons: index to two cells */

#define SCM_VALUE_MASK 0xffffffffffffULL

static inline double scm_box(unsigned int tag, uint64_t value)
{
	double d;
	value = ((0xfff8ULL | (((uint64_t)tag) & 0x7ULL)) << 48U)
		| (value & SCM_VALUE_MASK);
	memcpy(&d, &value, sizeof(d));
	return d;
}

static inline uint64_t scm_unbox(double d)
{
	uint64_t u;
	memcpy(&u, &(d), sizeof(u));
	return u;
}

#endif
