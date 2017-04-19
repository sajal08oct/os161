/*
 * Copyright (c) 2001, 2002, 2009
 *	The President and Fellows of Harvard College.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Driver code is in kern/tests/synchprobs.c We will
 * replace that file. This file is yours to modify as you see fit.
 *
 * You should implement your solution to the whalemating problem below.
 */

#include <types.h>
#include <lib.h>
#include <thread.h>
#include <test.h>
#include <synch.h>

/*
 * Called by the driver during initialization.
 */

struct cv* male_cv = NULL;
struct cv* female_cv = NULL;
struct cv* matchmaker_cv = NULL;
struct lock* testlock1 = NULL;
struct lock* testlock2 = NULL;
struct lock* testlock3 = NULL;

void whalemating_init() {
	male_cv = cv_create("male_cv");
	female_cv = cv_create("female_cv");
	matchmaker_cv = cv_create("matchmaker_cv");
	testlock1 = lock_create("testlock1");
	testlock2 = lock_create("testlock2");
	testlock3 = lock_create("testlock3");

	return;
}

/*
 * Called by the driver during teardown.
 */

void
whalemating_cleanup() {
	cv_destroy(male_cv);
	cv_destroy(female_cv);
	cv_destroy(matchmaker_cv);
	lock_destroy(testlock1);
	lock_destroy(testlock2);
	lock_destroy(testlock3);
	
	return;
}

void
male(uint32_t index)
{
	(void)index;
	/*
	 * Implement this function by calling male_start and male_end when
	 * appropriate.
	 */

	male_start(index);
	
	lock_acquire(testlock1);
	cv_wait(male_cv , testlock1);
				
	male_end(index);
	lock_release(testlock1);
	return;
}

void
female(uint32_t index)
{
	(void)index;
	/*
	 * Implement this function by calling female_start and female_end when
	 * appropriate.
	 */

	female_start(index);
	lock_acquire(testlock2);
	
	cv_wait(female_cv , testlock2);
	
	
	female_end(index);
	lock_release(testlock2);
	
	return;
}

void
matchmaker(uint32_t index)
{
	(void)index;
	/*
	 * Implement this function by calling matchmaker_start and matchmaker_end
	 * when appropriate.
	 */
	
	matchmaker_start(index);
	lock_acquire(testlock1);
	cv_signal(male_cv , testlock1);
	lock_release(testlock1);
	lock_acquire(testlock2);
	cv_signal(female_cv, testlock2);
	lock_release(testlock2);		
	matchmaker_end(index);
	return;
}
