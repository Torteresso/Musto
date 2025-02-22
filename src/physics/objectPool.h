#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <vector>
#include <variant>
#include <limits>
#include <bit>
#include <cassert>
#include <memory>
#include <physics/rigidDisk.h>

inline constexpr size_t kInvalidSlotId = std::numeric_limits<size_t>::max();

struct FreeSlot {
    size_t next;
};



struct ObjectPool {
    using Slot = std::variant<RigidDisk, FreeSlot>;

    ObjectPool() = default;

    ObjectPool(size_t capacity) : m_capacity{ capacity }
    {
        clear();
    }

    void Free(RigidDisk* object) {
        size_t begin_addr = std::bit_cast<size_t>(slots.data());
        size_t obj_addr = std::bit_cast<size_t>(object);
        assert(obj_addr >= begin_addr);
        size_t byte_offset = obj_addr - begin_addr;
        assert((byte_offset % sizeof(Slot)) == 0);
        size_t id = byte_offset / sizeof(Slot);

        assert(std::holds_alternative<RigidDisk>(slots[id]));
        slots[id] = FreeSlot{ .next = first_free };
        first_free = id;
    }

    RigidDisk* Alloc() {
        assert(first_free != kInvalidSlotId);
        size_t id = first_free;
        
        assert(std::holds_alternative<FreeSlot>(slots[id]));
        first_free = std::get<FreeSlot>(slots[id]).next;

        slots[id] = RigidDisk{};
        return &std::get<RigidDisk>(slots[id]);
    }

    void clear()
    {
        slots.clear();
        slots.resize(m_capacity, FreeSlot{});

        for (size_t i = 0; i != m_capacity; ++i) 
        {
            std::get<FreeSlot>(slots[i]).next = i + 1;
        }

        std::get<FreeSlot>(slots.back()).next = kInvalidSlotId;
        first_free = 0;

    }

    std::vector<Slot> slots;
    size_t first_free = kInvalidSlotId;
    size_t m_capacity;
 };

#endif