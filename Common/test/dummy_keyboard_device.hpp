#ifndef FLAPJACKOS_COMMON_TEST_DUMMY_KEYBOARD_DEVICE_HPP
#define FLAPJACKOS_COMMON_TEST_DUMMY_KEYBOARD_DEVICE_HPP

#include <common/keyboard_device.hpp>
#include <vector>

class DummyKeyboardDevice : public KeyboardDevice {
    std::vector<KeyboardEvent> _events;

public:
    virtual ~DummyKeyboardDevice() noexcept = default;

    const char* keycode_name(Keycode) override
    {
        return "Unknown Key Code";
    }

    KeyboardEvent get_event() override
    {
        assert(!_events.empty());
        KeyboardEvent ev = _events.front();
        _events.erase(_events.begin());
        return ev;
    }

    void set_events(std::vector<KeyboardEvent> &&events)
    {
        _events = std::move(events);
    }
};

#endif // FLAPJACKOS_COMMON_TEST_DUMMY_KEYBOARD_DEVICE_HPP
