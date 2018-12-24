#include <scheduler.hpp>
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

    Thread*& previous_thread = current_thread_;

    if (runnable_.empty()) {
        exchange(runnable_, exhausted_);
    }

    if (!runnable_.empty()) {
        Thread* next_thread = runnable_.pop_front();
        assert(next_thread);

        exhausted_.push_back(current_thread_);
        current_thread_ = next_thread;
    }

    previous_thread->switch_away(*current_thread_);
}
