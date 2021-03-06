/****************************************************************
 *								*
 *	Copyright 2001, 2014 Fidelity Information Services, Inc	*
 *								*
 *	This source code contains the intellectual property	*
 *	of its copyright holder(s), and is made available	*
 *	under a license.  If you do not know the terms of	*
 *	the license, please stop and do not read further.	*
 *								*
 ****************************************************************/

#include "mdef.h"

#include "gtm_string.h"

#include "gdsroot.h"
#include "gtm_facility.h"
#include "fileinfo.h"
#include "gdsbt.h"
#include "gdsfhead.h"

#include "gtm_stdio.h"
#include <stdarg.h>

#include "error.h"
#include "fao_parm.h"
#include "util.h"
#include "gtmmsg.h"
#include "sgtm_putmsg.h"

/*
**  WARNING:    For chained error messages, all messages MUST be followed by an fao count;
**  =======     zero MUST be specified if there are no parameters.
*/

/* This routine is a variation on the unix version of rts_error, and has an identical interface */

void sgtm_putmsg(char *out_str, ...)
{
	va_list	var;
	int	arg_count, dummy, fao_actual, fao_count, fao_list[MAX_FAO_PARMS + 1], i, msg_id;
	char	msg_buffer[1024];
	mstr	msg_string;
	int	util_outbufflen;
	DCL_THREADGBL_ACCESS;

	SETUP_THREADGBL_ACCESS;
	VAR_START(var, out_str);
	va_count(arg_count);
	arg_count--;
	assert(arg_count > 0);
	util_out_print(NULL, RESET);

	for (;;)
	{
		msg_id = va_arg(var, int);
		--arg_count;

		msg_string.addr = msg_buffer;
		msg_string.len = SIZEOF(msg_buffer);
		gtm_getmsg(msg_id, &msg_string);

		if (arg_count > 0)
		{
			fao_actual = va_arg(var, int);
			--arg_count;

			fao_count = fao_actual;
			if (fao_count > MAX_FAO_PARMS)
			{
				assert(FALSE);
				fao_count = MAX_FAO_PARMS;
			}
		}
		else
			fao_actual = fao_count
				   = 0;

		memset(fao_list, 0, SIZEOF(fao_list));

		for (i = 0;  i < fao_count;  ++i)
		{
			fao_list[i] = va_arg(var, int);
			--arg_count;
		}

		/* Currently there are a max of 34 fao parms (MAX_FAO_PARMS) allowed, hence passing upto fao_list[33].
		 * An assert is added to ensure this code is changed whenever the macro MAX_FAO_PARMS is changed.
		 * The # of arguments passed below should change accordingly.
		 */
		assert(MAX_FAO_PARMS == 34);
		util_out_print(msg_string.addr, NOFLUSH, fao_list[0], fao_list[1], fao_list[2], fao_list[3], fao_list[4],
			fao_list[5], fao_list[6], fao_list[7], fao_list[8], fao_list[9], fao_list[10], fao_list[11], fao_list[12],
			fao_list[13], fao_list[14], fao_list[15], fao_list[16], fao_list[17], fao_list[18], fao_list[19],
			fao_list[20], fao_list[21], fao_list[22], fao_list[23], fao_list[24], fao_list[25], fao_list[26],
			fao_list[27], fao_list[28], fao_list[29], fao_list[30], fao_list[31], fao_list[32], fao_list[33]);

		if (arg_count < 1)
			break;

		util_out_print("!/", NOFLUSH);
	}
	va_end(var);
	util_out_print(NULL, SPRINT);
	util_outbufflen = STRLEN(TREF(util_outbuff_ptr));
	memcpy(out_str, TREF(util_outbuff_ptr), util_outbufflen);
	out_str[util_outbufflen] = '\n';
	out_str[util_outbufflen + 1] = '\0';
}
