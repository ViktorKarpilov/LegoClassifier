#pragma once

enum action
{
    idle,
    send_image,
    set_exposure_action,
};

struct actions_queue
{
    static constexpr uint8_t capacity = 8;

    volatile uint8_t head = 0;
    volatile uint8_t tail = 0;

    volatile action data[capacity]{};
    // std::array<volatile actions, capacity> data{};

    bool push(const action action)
    {
        if ((head + 1) % capacity == tail % capacity)
        {
            return false;
        }

        data[head] = action;
        head = (head + 1) % capacity;
        return true;
    }

    bool pop(action &out)
    {
        if (head == tail) return false;

        out = data[tail];
        tail = (tail + 1) % capacity;
        return true;
    }
};

inline actions_queue platform_queue{};



