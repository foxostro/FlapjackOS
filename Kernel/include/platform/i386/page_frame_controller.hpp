#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_FRAME_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_FRAME_CONTROLLER_HPP

#include <paging_topology/page_frame_controller.hpp>
#include <interrupt_lock.hpp>
#include <common/spin_lock.hpp> // for perform_with_lock()

namespace i386 {

// Owns a page frame.
template<typename PageFrameAllocator>
class UnlockedPageFrameController : public PagingTopology::PageFrameController {
public:
    ~UnlockedPageFrameController()
    {
        release();
    }

    UnlockedPageFrameController()
     : allocator_(nullptr), page_frame_(0)
    {}

    explicit UnlockedPageFrameController(PageFrameAllocator& allocator)
     : allocator_(&allocator),
       page_frame_(allocator.allocate_span(PAGE_SIZE))
    {}

    UnlockedPageFrameController(UnlockedPageFrameController&& other)
     : allocator_(nullptr), page_frame_(0)
    {
        std::swap(other.allocator_, allocator_);
        std::swap(other.page_frame_, page_frame_);
    }

    UnlockedPageFrameController(const UnlockedPageFrameController&) = delete;

    UnlockedPageFrameController& operator=(UnlockedPageFrameController&& other)
    {
        if (this != &other) {
            release();
            std::swap(other.allocator_, allocator_);
            std::swap(other.page_frame_, page_frame_);
        }
        return *this;
    }

    void release()
    {
        if (allocator_ != nullptr) {
            allocator_->deallocate(page_frame_);
        }
        page_frame_ = 0;
    }

    uintptr_t get_page_frame() const override { return page_frame_; }

private:
    PageFrameAllocator* allocator_;
    uintptr_t page_frame_;
};

// Owns a page frame. Provides synchronized access.
template<typename PageFrameAllocator>
class PageFrameController : public PagingTopology::PageFrameController {
public:
    explicit PageFrameController(PageFrameAllocator& allocator)
     : impl_(allocator)
    {}

    PageFrameController(PageFrameController&& other)
    {
        perform_with_lock(lock_, other.lock_, [&]{
            impl_ = std::move(other.impl_);
        });
    }

    PageFrameController(const PageFrameController&) = delete;

    PageFrameController& operator=(PageFrameController&& other)
    {
        if (this != &other) {
            perform_with_lock(lock_, other.lock_, [&]{
                impl_ = std::move(other.impl_);
            });
        }
        return *this;
    }

    uintptr_t get_page_frame() const override
    {
        uintptr_t result;
        perform_with_lock(lock_, [&]{
            result = impl_.get_page_frame();
        });
        return result;
    }

private:
    mutable InterruptLock lock_;
    UnlockedPageFrameController<PageFrameAllocator> impl_;
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_FRAME_CONTROLLER_HPP
