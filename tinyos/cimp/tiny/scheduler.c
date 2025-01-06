// @Hustler's Project

#include <scheduler.h>
#include <preempt.h>

DEFINE_PERCPU(unsigned int, __preempt_count);
