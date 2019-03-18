#include <scheduler.hpp>
#include <common/logger.hpp>
#include <cassert>
#include <utility>

static Scheduler* g_scheduler = nullptr;

extern "C" void thread_start(unsigned param, void(*function)(unsigned)) noexcept
{
    g_scheduler->unlock_at_thread_start();
    function(param);
}

Scheduler::Scheduler() noexcept
{
    g_scheduler = this;
}

void Scheduler::add(ThreadPointer thread) noexcept
{
    LockGuard guard{lock_};
    runnable_.emplace_back(std::move(thread));
}

void Scheduler::begin(ThreadPointer init_thread) noexcept
{
    LockGuard guard{lock_};
    current_thread_ = std::move(init_thread);
}

void Scheduler::yield() noexcept
{
    LockGuard guard{lock_};
    if (can_yield()) {
        Thread& previous_thread = *current_thread_;
        swap_runnable_and_exhausted_if_necessary();
        move_to_next_runnable_thread();
        previous_thread.switch_away(*current_thread_);
    }
}

bool Scheduler::can_yield() noexcept
{
    return current_thread_ && !(runnable_.empty() && exhausted_.empty());
}

void Scheduler::vanish() noexcept
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

void Scheduler::unlock_at_thread_start() noexcept
{
    lock_.unlock();
}

void Scheduler::take_current_thread_from_runnable_queue() noexcept
{
    current_thread_ = std::move(runnable_.front());
    runnable_.pop_front();
}

void Scheduler::swap_runnable_and_exhausted_if_necessary() noexcept
{
    assert(!(runnable_.empty() && exhausted_.empty()));
    if (runnable_.empty()) {
        std::swap(runnable_, exhausted_);
    }
}

void Scheduler::move_to_next_runnable_thread() noexcept
{
    if (!runnable_.empty()) {
        exhausted_.emplace_back(std::move(current_thread_));
        take_current_thread_from_runnable_queue();
    }
}
