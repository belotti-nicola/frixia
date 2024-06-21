// SPDX-License-Identifier: BSD-3-Clause
#include <uv.h>
#include <stdio.h>
 
int counter = 0;
 
static void timer_cb(uv_timer_t *handle)
{
  // The void* data poiner has our counter
  int *counter = (int*) handle->data;
  printf("Timer event fired!\n");
 
  // Increment the counter
  (*counter)++;
 
  // When the counter his 20 fires this should stop the timer and cause the
  // application to exit
  if (*counter > 20)
  {
    uv_timer_stop(handle);
  }
}
 
int main()
{
  // Create a timer handler and a counter to be passed
  uv_timer_t timer_handle;
  int counter = 0;
 
  // libuv creates a default loop, you can use this or initialize a new one.
  // This call initializes the timer handle and adds it to the main loop.
  uv_timer_init(uv_default_loop(), &timer_handle);
 
  // Give the counter as callback data. This is a void* pointer for your
  // application to use.
  timer_handle.data = &counter;
 
  // Start the timer, note that it doesn't start at this point because the
  // main loop isn't running yet. This triggers after the first 1000ms and
  // every 100ms after that.
  uv_timer_start(&timer_handle, timer_cb, 1000, 100);
 
  // Run the event loop and block until all events are finished. In our case
  // the timer has a repeat so will block until the timer is stopped
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
 
  // Cleanup and exit
  uv_loop_close(uv_default_loop());
  return 0;
}