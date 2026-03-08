#pragma once

enum actions
{
    idle,
    send_image,
};

struct actions_queue
{
    static constexpr uint8_t capacity = 8;

    uint8_t head = 0;
    uint8_t tail = 0;

    std::array<actions, capacity> data{};

    bool push(const actions action)
    {
        if ((head + 1) % capacity == tail % capacity)
        {
            return false;
        }

        data[head] = action;
        head = (head + 1) % capacity;
        return true;
    }

    bool pop(actions &out)
    {
        if (head == tail) return false;

        tail = (tail + 1) % capacity;
        out = data[tail];
        return true;
    }
};

inline actions_queue platform_queue{};



