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
 * Driver code is in kern/tests/synchprobs.c We will replace that file. This
 * file is yours to modify as you see fit.
 *
 * You should implement your solution to the stoplight problem below. The
 * quadrant and direction mappings for reference: (although the problem is, of
 * course, stable under rotation)
 *
 *   |0 |
 * -     --
 *    01  1
 * 3  32
 * --    --
 *   | 2|
 *
 * As way to think about it, assuming cars drive on the right: a car entering
 * the intersection from direction X will enter intersection quadrant X first.
 * The semantics of the problem are that once a car enters any quadrant it has
 * to be somewhere in the intersection until it call leaveIntersection(),
 * which it should call while in the final quadrant.
 *
 * As an example, let's say a car approaches the intersection and needs to
 * pass through quadrants 0, 3 and 2. Once you call inQuadrant(0), the car is
 * considered in quadrant 0 until you call inQuadrant(3). After you call
 * inQuadrant(2), the car is considered in quadrant 2 until you call
 * leaveIntersection().
 *
 * You will probably want to write some helper functions to assist with the
 * mappings. Modular arithmetic can help, e.g. a car passing straight through
 * the intersection entering from direction X will leave to direction (X + 2)
 * % 4 and pass through quadrants X and (X + 3) % 4.  Boo-yah.
 *
 * Your solutions below should call the inQuadrant() and leaveIntersection()
 * functions in synchprobs.c to record their progress.
 */

#include <types.h>
#include <lib.h>
#include <thread.h>
#include <test.h>
#include <synch.h>

/*
 * Called by the driver during initialization.
 */

int arr[4];
struct lock* locks[4];
void quad_to_go_right(uint32_t );
void quad_to_go_straight(uint32_t );
void quad_to_go_left(uint32_t );

void
stoplight_init() {
	for(int i=0;i<4;i++)
		locks[i] =lock_create("stoplight lock");
//	lock1 = lock_create("lock1");
//	lock2 = lock_create("lock2");
//	lock3 = lock_create("lock3");
//	lock4 = lock_create("lock4");
	
	return;
}
/*
 * Called by the driver during teardown.
 */

void stoplight_cleanup() {
	for(int i=0;i<4;i++)
		lock_destroy(locks[i]);
		return;
}

void 
quad_to_go_right(uint32_t direction){
//	int arr[4];
	for(int i=0;i<4;i++) arr[i]=0;
	arr[direction] = 1;		
//	return arr;
}

void quad_to_go_straight(uint32_t direction){
	for(int i=0;i<4;i++){
		arr[i]=0;
	}
	arr[direction] = 1;
	arr[(direction+3)%4] =1;

}

void quad_to_go_left(uint32_t direction){
	for(int i=0;i<4;i++){
		arr[i]=0;
	}
	arr[direction] = 1;
	arr[(direction+2)%4] = 1;
	arr[(direction+3)%4] = 1;

}
void
turnright(uint32_t direction, uint32_t index)
{
	(void)direction;
	(void)index;
	
	//int quad1 = direction;
	quad_to_go_right(direction);
	
	lock_acquire(locks[direction]);
	inQuadrant(direction,index);
	leaveIntersection(index);
	lock_release(locks[direction]);
	/*
	 * Implement this function.
	 */
	
	return;
}
void
gostraight(uint32_t direction, uint32_t index)
{
	(void)direction;
	(void)index;
	while(true){
		if(locks[direction]->holder == NULL)
			lock_acquire(locks[direction]);
		if(locks[(direction+3)%4]->holder == NULL)
			lock_acquire(locks[(direction+3)%4]);

		if(lock_do_i_hold(locks[direction]) && lock_do_i_hold(locks[(direction+3)%4 ] )){
			break;
		}
		else{
		if(lock_do_i_hold(locks[(direction+3)%4]))
			lock_release(locks[(direction+3)%4]);
		else if(lock_do_i_hold(locks[direction]))
			lock_release(locks[(direction)]);
		}	
	}	

	inQuadrant(direction,index);
	inQuadrant((direction+3)%4 , index);
	leaveIntersection(index);
	
	lock_release(locks[direction]);
	lock_release(locks[(direction+3)%4]);

	
	
	/*
	 * Implement this function.
	 */
	return;
}
void
turnleft(uint32_t direction, uint32_t index)
{
	(void)direction;
	(void)index;
	while(true){
		if(locks[direction]->holder == NULL)	
			lock_acquire(locks[direction]);
		if(locks[(direction+3)%4]->holder == NULL)
			lock_acquire(locks[(direction+3)%4]);
		if(locks[(direction+2)%4]->holder == NULL)
			lock_acquire(locks[(direction+2)%4]);
	
		if(lock_do_i_hold(locks[(direction+2)%4]) && lock_do_i_hold(locks[direction]) && lock_do_i_hold(locks[(direction+3)%4 ] )){
                        break;
                }
                else{
                if(lock_do_i_hold(locks[(direction+3)%4]))
                        lock_release(locks[(direction+3)%4]);
                if(lock_do_i_hold(locks[direction]))
                        lock_release(locks[(direction)]);
		if(lock_do_i_hold(locks[(direction+2)%4]))
			lock_release(locks[(direction+2)%4]);
		}
	}
	inQuadrant(direction,index);
        inQuadrant((direction+3)%4 , index);
        inQuadrant((direction+2)%4 , index);
	leaveIntersection(index);

        lock_release(locks[direction]);
        lock_release(locks[(direction+3)%4]);
	lock_release(locks[(direction+2)%4]);

		
	/*
	 * Implement this function.
	 */
	return;
}
