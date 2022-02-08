#pragma once

#include <cassert>
#include <sstream>
#include <string>
#include <vector>

#include "endian.h"

using namespace std;

/**
 * @brief convert key string to vector of words
 * @tparam WordSize the type of the words (quarter block)
 * @param key the key string
 * @return vector of words
 */
template<typename WordSize> vector<WordSize> key_to_words(const string& key)
{
    // Ensure key-length is 255 or less
    assert(key.length() <= 255 && "Key must be <= 255 bytes");

    // Pad key to word length
    ostringstream os;
    os << key;
    while (os.str().length() % sizeof(WordSize))
        os << '\0';
    string key_padded = os.str();

    // Preload key into word array
    vector<WordSize> key_words;
    for (unsigned i = 0; i < key_padded.length(); i += sizeof(WordSize)) {
        WordSize key_word = 0;
        unsigned shift = numeric_limits<WordSize>::digits - 8;
        for (unsigned c = 0; c < sizeof(WordSize); c++, shift -= 8)
            key_word |= key_padded[i + c] << shift;
        key_words.push_back(key_word);
    }

    // Swap endianness on big-endian architectures to ensure key bytes are
    // loaded little - endian
    if (is_big_endian())
        for (size_t i = 0; i < key_words.size(); i++)
            key_words[i] = swap_endian(key_words[i]);

    return key_words;
}
