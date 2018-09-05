#pragma once

#include <cstring>
#include <cassert>
#include <new>
#include <climits> // for CHAR_BIT

// A compact array of bits.
// Either use the factory method to create this object, or else use
// placement-new to construct this in a suitable buffer.
class bit_array {
public:
    using word_t = unsigned;
    static constexpr size_t NUMBER_OF_WORD_BITS = sizeof(word_t) * 8;
    static constexpr int NOT_FOUND = -1;

    static size_t calc_number_of_storage_bytes(size_t number_of_bits)
    {
        return sizeof(bit_array) + (number_of_bits + bit_array::NUMBER_OF_WORD_BITS - 1) / bit_array::NUMBER_OF_WORD_BITS * sizeof(bit_array::word_t);
    }

    static bit_array* create(size_t number_of_bits)
    {
        size_t size = calc_number_of_storage_bytes(number_of_bits);
        char* buffer = new char[size];
        if (buffer) {
            bit_array *result = new (buffer) bit_array(number_of_bits, size);
            return result;
        } else {
            return nullptr;
        }
    }

    bit_array(size_t number_of_bits, size_t length)
     : number_of_bits(number_of_bits),
       length(length)
    {
        clear_all();
    }

    void clear_all()
    {
        memset(storage, 0x00, length);
    }

    void set_all()
    {
        memset(storage, 0xFF, length);
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
        return number_of_bits;
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
        return slow_scan_backward(number_of_bits-1, desired_value);
#endif        
    }

private:
    size_t number_of_bits, length;
    char storage[];

    size_t get_mask(size_t bit_index)
    {
        return 1 << (bit_index % CHAR_BIT);
    }

    char& get_byte(size_t bit_index)
    {
        assert(bit_index < number_of_bits);
        assert((bit_index / CHAR_BIT) < length);
        return storage[bit_index / CHAR_BIT];
    }

    word_t* get_words()
    {
        return (word_t*)storage;
    }

    size_t get_number_of_words()
    {
        return number_of_bits / NUMBER_OF_WORD_BITS;
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
        for (size_t i = begin; i < number_of_bits; ++i) {
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
