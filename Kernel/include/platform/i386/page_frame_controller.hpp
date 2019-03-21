#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_FRAME_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_FRAME_CONTROLLER_HPP

#include <paging_topology/page_frame_controller.hpp>
#include <page_frame_allocator.hpp>
#include <common/interrupt_lock.hpp>
#include <common/lock_guard.hpp>

namespace i386 {

// Owns a page frame.
class PageFrameController final : public PagingTopology::PageFrameController {
public:
    ~PageFrameController()
    {
        release();
    }

    PageFrameController()
     : allocator_(nullptr), page_frame_(0)
    {}

    explicit PageFrameController(uintptr_t page_frame)
     : allocator_(nullptr),
       page_frame_(page_frame)
    {}

    explicit PageFrameController(PageFrameAllocator& allocator)
     : allocator_(&allocator),
       page_frame_(allocator.allocate(PAGE_SIZE))
    {}

    PageFrameController(PageFrameController&& other)
     : allocator_(nullptr), page_frame_(0)
    {
        LockGuard guard{other.lock_};
        std::swap(other.allocator_, allocator_);
        std::swap(other.page_frame_, page_frame_);
    }

    PageFrameController(const PageFrameController&) = delete;

    PageFrameController& operator=(PageFrameController&& other)
    {
        LockGuard guard{lock_, other.lock_};
        if (this != &other) {
            release();
            std::swap(other.allocator_, allocator_);
            std::swap(other.page_frame_, page_frame_);
        }
        return *this;
    }

    uintptr_t get_page_frame() const override
    {
        LockGuard guard{lock_};
        return page_frame_;
    }

private:
    mutable InterruptLock lock_;
    PageFrameAllocator* allocator_;
    uintptr_t page_frame_;

    void release()
    {
        if (allocator_ != nullptr) {
            allocator_->deallocate(page_frame_);
        }
        page_frame_ = 0;
    }
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_FRAME_CONTROLLER_HPP
