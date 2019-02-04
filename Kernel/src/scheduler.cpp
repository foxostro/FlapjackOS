#include <scheduler.hpp>
#include <common/logger.hpp>
#include <common/mutex.hpp>
#include <cassert>
#include <utility>

static Scheduler* g_scheduler = nullptr;

extern "C" void thread_start(uintptr_t param, void(*function)(uintptr_t)) noexcept
{
    g_scheduler->unlock_at_thread_start();
    function(param);
}

namespace SchedulerDetail {

void SchedulerLock::lock()
{
    lock_.lock();
    assert(!is_locked_ && "The lock is already taken. Reentrancy is not permitted.");
    is_locked_ = true;
}

void SchedulerLock::unlock()
{
    is_locked_ = false;
    lock_.unlock();
}

void SchedulerUnlocked::add(ThreadPointer thread) noexcept
{
    runnable_.emplace_back(std::move(thread));
}

void SchedulerUnlocked::begin(ThreadPointer init_thread) noexcept
{
    Mutex::yield = ::yield;
    current_thread_ = std::move(init_thread);
}

void SchedulerUnlocked::yield() noexcept
{
    if (can_yield()) {
        Thread& previous_thread = *current_thread_;
        swap_runnable_and_exhausted_if_necessary();
        move_to_next_runnable_thread();
        previous_thread.switch_away(*current_thread_);
    }
}

void SchedulerUnlocked::vanish() noexcept
{
    assert(current_thread_);
    Thread& previous_thread = *current_thread_;
    swap_runnable_and_exhausted_if_necessary();
    take_current_thread_from_runnable_queue();
    previous_thread.switch_away(*current_thread_);
    __builtin_unreachable();
}

bool SchedulerUnlocked::can_yield() noexcept
{
    return current_thread_ && !(runnable_.empty() && exhausted_.empty());
}

void SchedulerUnlocked::take_current_thread_from_runnable_queue() noexcept
{
    current_thread_ = std::move(runnable_.front());
    runnable_.pop_front();
}

void SchedulerUnlocked::swap_runnable_and_exhausted_if_necessary() noexcept
{
    assert(!(runnable_.empty() && exhausted_.empty()));
    if (runnable_.empty()) {
        std::swap(runnable_, exhausted_);
    }
}

void SchedulerUnlocked::move_to_next_runnable_thread() noexcept
{
    if (!runnable_.empty()) {
        exhausted_.emplace_back(std::move(current_thread_));
        take_current_thread_from_runnable_queue();
    }
}

SchedulerMonitor::SchedulerMonitor() noexcept
{
    g_scheduler = this;
}

void SchedulerMonitor::add(ThreadPointer thread) noexcept
{
    LockGuard guard{lock_};
    impl_.add(std::move(thread));
}

void SchedulerMonitor::begin(ThreadPointer init_thread) noexcept
{
    LockGuard guard{lock_};
    impl_.begin(std::move(init_thread));
}

void SchedulerMonitor::yield() noexcept
{
    LockGuard guard{lock_};
    impl_.yield();
}

void SchedulerMonitor::vanish() noexcept
{
    lock_.lock();
    impl_.vanish();

    // The corresponding call to unlock() occurs when the other thread
    // resumes execution in yield() or unlock_at_thread_start().
    __builtin_unreachable();
}

void SchedulerMonitor::unlock_at_thread_start() noexcept
{
    lock_.unlock();
}

} // namespace SchedulerDetail
