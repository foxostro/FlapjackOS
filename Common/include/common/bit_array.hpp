#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_BIT_ARRAY_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_BIT_ARRAY_HPP

#include <cstring>
#include <cassert>
#include <new>
#include <climits> // for CHAR_BIT
#include <cstdint>

// A compact array of bits.
// `CAPACITY' is the number of bits in the array.
template<size_t CAPACITY>
class BitArray {
public:
    using Word = unsigned;
    static constexpr size_t NUMBER_OF_WORD_BITS = sizeof(Word) * 8;
    static constexpr int NOT_FOUND = -1;

    BitArray()
    {
        clear_all();
    }

    void clear_all()
    {
        memset(storage_, 0x00, sizeof(storage_));
    }

    void set_all()
    {
        memset(storage_, 0xFF, sizeof(storage_));
    }

    void set(size_t bit_index)
    {
        get_byte(bit_index) |= get_mask(bit_index);
    }

    void clear(size_t bit_index)
    {
        get_byte(bit_index) &= ~get_mask(bit_index);
    }

    bool test(size_t bit_index)
    {
        return !!(get_byte(bit_index) & get_mask(bit_index));
    }

    size_t get_number_of_bits()
    {
        return CAPACITY;
    }

    void set_region(size_t bit_index_begin, size_t length)
    {
        for (size_t i = 0; i < length; ++i) {
            set(i + bit_index_begin);
        }
    }

    void clear_region(size_t bit_index_begin, size_t length)
    {
        for (size_t i = 0; i < length; ++i) {
            clear(i + bit_index_begin);
        }
    }

    int scan_forward(bool desired_value)
    {
        int begin = scan_forwards_to_find_bit_index_of_matching_word(desired_value);
        if (begin == NOT_FOUND) {
            return NOT_FOUND;
        } else {
            return slow_scan_forward(begin, desired_value);
        }
    }

    int scan_backward(bool desired_value)
    {
#if 0
        int begin = scan_backwards_to_find_bit_index_of_matching_word(desired_value);
        if (begin == NOT_FOUND) {
            return NOT_FOUND;
        } else {
            return slow_scan_backward(begin, desired_value);
        }
#else
        return slow_scan_backward(CAPACITY-1, desired_value);
#endif        
    }

private:
    static constexpr size_t calc_number_of_storage_bytes(size_t number_of_bits)
    {
        return (number_of_bits + NUMBER_OF_WORD_BITS - 1) / NUMBER_OF_WORD_BITS * sizeof(Word);
    }

    static_assert(CAPACITY > 0, "CAPACITY must be greater than zero.");

    uint8_t storage_[calc_number_of_storage_bytes(CAPACITY)];

    size_t get_mask(size_t bit_index)
    {
        return 1 << (bit_index % CHAR_BIT);
    }

    uint8_t& get_byte(size_t bit_index)
    {
        assert(bit_index < CAPACITY);
        assert((bit_index / CHAR_BIT) < sizeof(storage_));
        return storage_[bit_index / CHAR_BIT];
    }

    Word* get_words()
    {
        return (Word*)storage_;
    }

    size_t get_number_of_words()
    {
        return CAPACITY / NUMBER_OF_WORD_BITS;
    }

    int scan_forwards_to_find_bit_index_of_matching_word(bool desired_value)
    {
        for (size_t i = 0; i < get_number_of_words(); ++i) {
            if ((get_words()[i] == 0) == desired_value) {
                return i * NUMBER_OF_WORD_BITS;
            }
        }
        return NOT_FOUND;
    }

    int slow_scan_forward(size_t begin, bool desired_value)
    {
        for (size_t i = begin; i < CAPACITY; ++i) {
            if (test(i) == desired_value) {
                return i;
            }
        }
        return NOT_FOUND;
    }

    int scan_backwards_to_find_bit_index_of_matching_word(bool desired_value)
    {
        // TODO: The last word of storage may be used only partially by the bit array. Need to account for this because the unused bits may be garbage and should not be considered at all.
        for (int i = get_number_of_words()-1; i >= 0; --i) {
            if ((get_words()[i] == 0) == desired_value) {
                return (i+1) * NUMBER_OF_WORD_BITS - 1;
            }
        }
        return NOT_FOUND;
    }

    int slow_scan_backward(size_t begin, bool desired_value)
    {
        for (int i = begin; i >= 0; --i) {
            if (test(i) == desired_value) {
                return i;
            }
        }
        return NOT_FOUND;
    }
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_BIT_ARRAY_HPP
