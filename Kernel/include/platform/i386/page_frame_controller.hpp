#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_FRAME_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_FRAME_CONTROLLER_HPP

#include <page_size.hpp>

namespace i386 {

// Owns a page frame.
template<typename PageFrameAllocator>
class PageFrameController {
public:
    ~PageFrameController()
    {
        release();
    }

    explicit PageFrameController(PageFrameAllocator& allocator)
     : allocator_(&allocator),
       page_frame_(allocator.allocate_span(PAGE_SIZE))
    {}

    PageFrameController(PageFrameController&& other)
     : allocator_(nullptr), page_frame_(0)
    {
        std::swap(other.allocator_, allocator_);
        std::swap(other.page_frame_, page_frame_);
    }

    PageFrameController(const PageFrameController&) = delete;

    PageFrameController& operator=(PageFrameController&& other)
    {
        if (this != &other) {
            release();
            std::swap(other.allocator_, allocator_);
            std::swap(other.page_frame_, page_frame_);
        }
        return *this;
    }

    inline uintptr_t get_page_frame() const { return page_frame_; }

private:
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
