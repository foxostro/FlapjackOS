#include <scheduler.hpp>
#include <logger.hpp>
#include <cassert>

Scheduler::Scheduler() = default;

void Scheduler::add(ThreadPointer thread)
{
    runnable_.emplace_back(std::move(thread));
}

void Scheduler::begin()
{
    assert(!runnable_.empty());
    take_current_thread_from_runnable_queue();
    current_thread_->switch_to();
}

void Scheduler::yield()
{
    assert(current_thread_);
    Thread& previous_thread = *current_thread_;
    swap_runnable_and_exhausted_if_necessary();
    move_to_next_runnable_thread();
    previous_thread.switch_away(*current_thread_);
}

void Scheduler::vanish()
{
    assert(current_thread_);
    Thread& previous_thread = *current_thread_;
    swap_runnable_and_exhausted_if_necessary();
    take_current_thread_from_runnable_queue();
    previous_thread.switch_away(*current_thread_);
    __builtin_unreachable();
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
        exchange(runnable_, exhausted_);
    }
}

void Scheduler::move_to_next_runnable_thread()
{
    if (!runnable_.empty()) {
        exhausted_.emplace_back(std::move(current_thread_));
        take_current_thread_from_runnable_queue();
    }
}
