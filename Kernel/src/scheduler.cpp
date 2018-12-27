#include <scheduler.hpp>
#include <logger.hpp>
#include <cassert>
#include <utility>

static Scheduler* g_scheduler = nullptr;

extern "C" void thread_start()
{
    g_scheduler->unlock_at_thread_start();
}

Scheduler::Scheduler()
{
    g_scheduler = this;
}

void Scheduler::add(ThreadPointer thread)
{
    perform_with_lock(lock_, [&]{
        runnable_.emplace_back(std::move(thread));
    });
}

void Scheduler::begin(ThreadPointer init_thread)
{
    perform_with_lock(lock_, [&]{
        current_thread_ = std::move(init_thread);
    });
}

void Scheduler::yield()
{
    perform_with_lock(lock_, [&]{
        if (can_yield()) {
            Thread& previous_thread = *current_thread_;
            swap_runnable_and_exhausted_if_necessary();
            move_to_next_runnable_thread();
            previous_thread.switch_away(*current_thread_);
        }
    });
}

bool Scheduler::can_yield()
{
    return current_thread_ && !(runnable_.empty() && exhausted_.empty());
}

void Scheduler::vanish()
{
    lock_.lock();
    assert(current_thread_);
    Thread& previous_thread = *current_thread_;
    swap_runnable_and_exhausted_if_necessary();
    take_current_thread_from_runnable_queue();
    previous_thread.switch_away(*current_thread_);

    // The corresponding call to unlock() occurs when the other thread
    // resumes execution in yield() or unlock_at_thread_start().
    __builtin_unreachable();
}

void Scheduler::unlock_at_thread_start()
{
    lock_.unlock();
}

void Scheduler::take_current_thread_from_runnable_queue()
{
    current_thread_ = std::move(runnable_.front());
    runnable_.pop_front();
}

void Scheduler::swap_runnable_and_exhausted_if_necessary()
{
    assert(!(runnable_.empty() && exhausted_.empty()));
    if (runnable_.empty()) {
        std::swap(runnable_, exhausted_);
    }
}

void Scheduler::move_to_next_runnable_thread()
{
    if (!runnable_.empty()) {
        exhausted_.emplace_back(std::move(current_thread_));
        take_current_thread_from_runnable_queue();
    }
}
