#include <scheduler.hpp>
#include <logger.hpp>
#include <cassert>

Scheduler::Scheduler()
    : current_thread_(nullptr)
{}

void Scheduler::add(Thread* thread)
{
    runnable_.push_back(thread);
}

void Scheduler::begin()
{
    assert(!runnable_.empty());
    current_thread_ = runnable_.pop_front();
    current_thread_->switch_to();
}

void Scheduler::yield()
{
    assert(current_thread_);
    Thread* previous_thread = current_thread_;
    swap_runnable_and_exhausted_if_necessary();
    move_to_next_runnable_thread();
    previous_thread->switch_away(*current_thread_);
}

void Scheduler::vanish()
{
    // AFOX_TODO: Scheduler::vanish() leaks threads
    assert(current_thread_);
    Thread* previous_thread = current_thread_;
    swap_runnable_and_exhausted_if_necessary();
    current_thread_ = runnable_.pop_front();
    previous_thread->switch_away(*current_thread_);
}

void Scheduler::swap_runnable_and_exhausted_if_necessary()
{
    assert(!(runnable_.empty() && exhausted_.empty()));
    if (runnable_.empty()) {
        exchange(runnable_, exhausted_);
    }
}

void Scheduler::move_to_next_runnable_thread()
{
    if (!runnable_.empty()) {
        exhausted_.push_back(current_thread_);
        current_thread_ = runnable_.pop_front();
    }
}
