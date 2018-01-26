/*
 * Copyright © 2017 Collabora, Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "config.h"

#include <time.h>

#include "shared/timespec-util.h"
#include "weston-test-client-helper.h"
#include "wayland-server-protocol.h"

static const struct timespec t1 = { .tv_sec = 1, .tv_nsec = 1000001 };
static const struct timespec t2 = { .tv_sec = 2, .tv_nsec = 2000001 };
static const struct timespec t3 = { .tv_sec = 3, .tv_nsec = 3000001 };

static struct client *
create_touch_test_client(void)
{
	struct client *cl = create_client_and_test_surface(0, 0, 100, 100);
	assert(cl);
	return cl;
}

static void
send_touch(struct client *client, const struct timespec *time,
	   uint32_t touch_type)
{
	uint32_t tv_sec_hi, tv_sec_lo, tv_nsec;

	timespec_to_proto(time, &tv_sec_hi, &tv_sec_lo, &tv_nsec);
	weston_test_send_touch(client->test->weston_test, tv_sec_hi, tv_sec_lo,
			       tv_nsec, 1, 1, 1, touch_type);
	client_roundtrip(client);
}

TEST(touch_events)
{
	struct client *client = create_touch_test_client();
	struct touch *touch = client->input->touch;

	send_touch(client, &t1, WL_TOUCH_DOWN);
	assert(touch->down_time_msec == timespec_to_msec(&t1));

	send_touch(client, &t2, WL_TOUCH_MOTION);
	assert(touch->motion_time_msec == timespec_to_msec(&t2));

	send_touch(client, &t3, WL_TOUCH_UP);
	assert(touch->up_time_msec == timespec_to_msec(&t3));
}