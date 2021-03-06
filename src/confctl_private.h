/*-
 * Copyright (c) 2012 Edward Tomasz Napierala <trasz@FreeBSD.org>
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

#ifndef CONFCTL_PRIVATE_H
#define	CONFCTL_PRIVATE_H

#include "queue.h"

struct buf {
	char	*b_buf;
	size_t	b_allocated;
	size_t	b_len;
};

/*
 * Tree of configuration variables.  For each element, we store the variable
 * name, it's value, subvalues (children), "junk text" (comments, whitespace,
 * newlines, curly brackets etc) stored in the configuration file before the
 * variable name (cv_before), between the name and value or child variables
 * (cv_middle), and after value or child variables (cv_after).
 */
struct confctl_var {
	TAILQ_ENTRY(confctl_var)	cv_next;
	struct buf			*cv_before;
	struct buf			*cv_name;
	struct buf			*cv_middle;
	struct buf			*cv_value;
	struct buf			*cv_after;
	struct confctl_var		*cv_parent;
	void				*cv_uptr;
	bool				cv_implicit_container:1;
	bool				cv_needs_reindent:1;
	TAILQ_HEAD(confctl_var_head, confctl_var)	cv_children;
};

/*
 * Root of the configuration tree.  Apart from being root, it also contains
 * variables that control configuration file syntax.
 */
struct confctl {
	struct confctl_var	*cc_root;
	bool			cc_equals_sign;
	bool			cc_rewrite_in_place;
	bool			cc_semicolon;
	bool			cc_slash_slash_comments;
	bool			cc_slash_star_comments;
};

#endif /* !CONFCTL_PRIVATE_H */
