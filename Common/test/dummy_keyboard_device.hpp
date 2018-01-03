#pragma once

#include <common/keyboard_device.hpp>
#include <vector>

class dummy_keyboard_device : public keyboard_device {
    std::vector<keyboard_event> _events;

public:
    virtual ~dummy_keyboard_device() noexcept = default;

    const char* keycode_name(keycode_t) override
    {
        return "Unknown Key Code";
    }

    keyboard_event get_event() override
    {
        assert(!_events.empty());
        keyboard_event ev = _events.front();
        _events.erase(_events.begin());
        return ev;
    }

    void set_events(std::vector<keyboard_event> &&events)
    {
        _events = std::move(events);
    }
};
