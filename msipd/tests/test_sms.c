// SPDX-License-Identifier: GPL-2.0
// Copyright (C) 2026 perz3us
#include <assert.h>
#include <stdio.h>
#include "../src/sms.h"

int main(void)
{
	/* delivered: 1xx and 2xx are never retried */
	assert(sms_should_retry(0, 200, 1) == 0);
	assert(sms_should_retry(0, 180, 1) == 0);
	assert(sms_transient(0, 200) == 0);

	/* hard refusals are terminal from the first attempt */
	assert(sms_should_retry(0, 415, 1) == 0);
	assert(sms_should_retry(0, 403, 1) == 0);
	assert(sms_should_retry(0, 404, 1) == 0);
	assert(sms_should_retry(0, 405, 1) == 0);
	assert(sms_should_retry(0, 501, 1) == 0);
	assert(sms_should_retry(0, 488, 1) == 0);
	assert(sms_transient(0, 415) == 0);

	/* transient failures are retried while under the cap */
	assert(sms_should_retry(0, 408, 1) == 1);
	assert(sms_should_retry(0, 480, 1) == 1);
	assert(sms_should_retry(0, 486, 1) == 1);
	assert(sms_should_retry(0, 500, 1) == 1);
	assert(sms_should_retry(0, 503, 1) == 1);
	assert(sms_should_retry(0, 504, 1) == 1);
	assert(sms_transient(0, 503) == 1);

	/* no response at all is transient */
	assert(sms_should_retry(1, 0, 1) == 1);
	assert(sms_should_retry(1, 0, SMS_TRIES - 1) == 1);
	assert(sms_transient(1, 0) == 1);

	/* the cap holds for every retryable failure */
	assert(sms_should_retry(0, 503, SMS_TRIES) == 0);
	assert(sms_should_retry(0, 408, SMS_TRIES + 1) == 0);
	assert(sms_should_retry(1, 0, SMS_TRIES) == 0);

	printf("test_sms ok (SMS_TRIES=%d)\n", SMS_TRIES);
	return 0;
}
