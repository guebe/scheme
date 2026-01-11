/* (c) guenter.ebermann@htl-hl.ac.at */

#include "scheme.h"

static scm_obj_t eval_list(scm_obj_t list, scm_obj_t environment_specifier)
{
	scm_obj_t first, rest;

	if (scm_is_empty_list(list)) return list;
	first = scm_eval(scm_car(list), environment_specifier);
	if (scm_is_error_object(first)) return first;
	rest = eval_list(scm_cdr(list), environment_specifier);
	if (scm_is_error_object(rest)) return rest;
	return scm_cons(first, rest);
}

extern scm_obj_t scm_eval(scm_obj_t expr_or_def, scm_obj_t environment_specifier)
{
	scm_obj_t op, args, string, evaled_args;
	const char *symbol;
	size_t symbol_length;

	if (scm_is_empty_list(expr_or_def)) {
		return scm_error("eval: can not apply empty-list-object ()");
	}
	else if (scm_is_symbol(expr_or_def)) {
		/* variable, note: also procedures are mapped in the environment */
		string = scm_symbol_to_string(expr_or_def);
		symbol = scm_string_value(string);
		symbol_length = scm_string_length(string);

		if ((symbol_length == 1) && (memcmp(symbol, "+", 1) == 0)) {
			return expr_or_def; /* return the symbol as "procedure" */
		}

		return scm_error("lookup(environment_specifier, expr_or_def) not yet supported");
	}
	else if (scm_is_pair(expr_or_def)) {
		op = scm_car(expr_or_def);
		args = scm_cdr(expr_or_def);

		if (scm_is_symbol(op)) {
			/* special forms */
			string = scm_symbol_to_string(op);
			symbol = scm_string_value(string);
			symbol_length = scm_string_length(string);

			/* if */
			if ((symbol_length == 2) && (memcmp(symbol, "if", 2) == 0)) {
				return scm_error("eval: if not implemented");
			}
			/* quoted */
			if ((symbol_length == 5) && (memcmp(symbol, "quote", 5) == 0)) {
				return scm_is_empty_list(scm_cdr(args)) ? scm_car(args) : scm_error("eval: bad quote form");
			}
			/* definition */
			if ((symbol_length == 6) && (memcmp(symbol, "define", 6) == 0)) {
				return scm_error("eval: define not implemented");
			}
			/* lambda */
			if ((symbol_length == 6) && (memcmp(symbol, "lambda", 6) == 0)) {
				return scm_error("eval: lambda not implemented");
			}
		}

		/* application */
		scm_obj_t proc = scm_eval(op, environment_specifier);
		if (scm_is_error_object(proc)) return proc;
		evaled_args = eval_list(args, environment_specifier);
		if (scm_is_error_object(evaled_args)) return evaled_args;
		return scm_apply(proc, evaled_args, environment_specifier);
	}
	else {
		/* self-evaluating */
	       	return expr_or_def;
	}
}

extern scm_obj_t scm_apply(scm_obj_t proc, scm_obj_t args, scm_obj_t environment_specifier)
{
	scm_obj_t string;
	const char *symbol;
	size_t symbol_length;
	(void)environment_specifier;

	if (scm_is_symbol(proc)) {
		/* primitive */
		string = scm_symbol_to_string(proc);
		symbol = scm_string_value(string);
		symbol_length = scm_string_length(string);

		/* + */
		if ((symbol_length == 1) && (memcmp(symbol, "+", 1) == 0)) {
			double result = 0;
			while (!scm_is_empty_list(args)) {
				scm_obj_t arg = scm_car(args);
				if (scm_is_number(arg)) {
					double value = scm_number_value(arg);
					result += value;
				}
				else return scm_error("+: argument is not a number");
				args = scm_cdr(args);
			}
			return scm_number(result);
		}
	}
	return scm_error("apply: not implemented");
}
