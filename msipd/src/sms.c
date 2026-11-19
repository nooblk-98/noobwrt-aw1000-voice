// SPDX-License-Identifier: GPL-2.0
// Copyright (C) 2026 perz3us
#include "sms.h"

int sms_transient(int err, int scode)
{
	if (err || scode <= 0)
		return 1;
	switch (scode) {
	case 408:	/* Request Timeout */
	case 480:	/* Temporarily Unavailable */
	case 486:	/* Busy Here */
	case 500:	/* Server Internal Error */
	case 503:	/* Service Unavailable */
	case 504:	/* Server Time-out */
		return 1;
	default:
		return 0;
	}
}

int sms_should_retry(int err, int scode, int attempts)
{
	if (!sms_transient(err, scode))
		return 0;
	return attempts < SMS_TRIES;
}
