#pragma once

#include <climits>
#include <cmath>
#include <limits>

#include "ca.h"
#include "meca.h"

using namespace std;

/**
 * @brief metamorphic cipher using second-order cellular automata
 * @tparam WordSize the size of a word in a 4-way partitioned block (i.e. uint32_t for a 128-bit block size)
 * @tparam Rounds number of rounds to evolve each half-block
 */
template<typename WordSize, size_t Rounds> class MECACipher
{
  private:
    /// Binary expansion of e - 2
    const WordSize P = WordSize(ceil((M_E - 2) * pow(2, numeric_limits<WordSize>::digits)));
    /// Binary expansion of the golden ratio - 1
    const WordSize Q = WordSize(((1.618033988749895 - 1) * pow(2, numeric_limits<WordSize>::digits)));

    /**
     * @brief metamorphic timestep depending on current state
     * @param meca second-order cellular automata to evolve
     */
    void morph(MECA<numeric_limits<WordSize>::digits, BOUNDARY_PERIODIC>& meca)
    {
        switch (meca.state().to_ullong() % 6) {
        case 0:
            meca.step(75);
            break;
        case 1:
            meca.step(86);
            break;
        case 2:
            meca.step(89);
            break;
        case 3:
            meca.step(149);
            break;
        case 4:
            meca.step(166);
            break;
        case 5:
            meca.step(173);
            break;
        }
    }

  public:
    /**
     * @brief create key schedule using globally chaotic (class 4) elementary CA rules
     * @return vector of words to xor with previous and current state
     */
    vector<WordSize> schedule_keys(const vector<WordSize>& key)
    {
        const unsigned num_keys = 2 * Rounds;
        vector<WordSize> keys(num_keys);

        // Initialize keys with magic constants
        keys[0] = P;
        for (unsigned i = 1; i < num_keys; i++)
            keys[i] = keys[i - 1] + Q;

        unsigned iterations = 3 * max((unsigned) key.size(), num_keys);

        for (unsigned s = 1, i = 0, j = 0; s < iterations;
             s++, i += 1 % num_keys - 1, j += 1 % key.size() - 1) {
            // Evolve supplied key using globally chaotic (class 4) rules
            CA<numeric_limits<WordSize>::digits, BOUNDARY_PERIODIC> ca(key[j]);
            ca.step(54);
            ca.step(110);
            ca.step(137);
            keys[i] = ca.state().to_ullong();
        }

        return keys;
    }

    /**
     * @brief encrypt a block
     * @param plaintext a block of plaintext
     * @param round_keys the key schedule
     */
    vector<WordSize> encrypt(const vector<WordSize>& plaintext, vector<WordSize>& round_keys)
    {
        const size_t bitsize = numeric_limits<WordSize>::digits;
        vector<WordSize> output_block(4);
        MECA<bitsize, BOUNDARY_PERIODIC> a((uint64_t) (plaintext[0]) << bitsize | plaintext[1]);
        MECA<bitsize, BOUNDARY_PERIODIC> b((uint64_t) (plaintext[2]) << bitsize | plaintext[3]);

        for (unsigned round = 0, key = 0; round < Rounds; round++, key += 2) {
            a.xor_state(round_keys[key]);
            morph(a);
            b.xor_state(round_keys[key + 1]);
            morph(b);
        }

        output_block[0] = a.prev().to_ullong();
        output_block[1] = a.state().to_ullong();
        output_block[2] = b.prev().to_ullong();
        output_block[3] = b.state().to_ullong();
        return output_block;
    }

    /**
     * @brief decrypt a block
     * @param ciphertext a block of ciphertext
     * @param round_keys the key schedule
     */
    vector<WordSize> decrypt(const vector<WordSize>& ciphertext, const vector<WordSize>& round_keys)
    {
        const size_t bitsize = numeric_limits<WordSize>::digits;
        vector<WordSize> output_block(4);
        MECA<bitsize, BOUNDARY_PERIODIC> a((uint64_t) (ciphertext[0]) << bitsize | ciphertext[1], true);
        MECA<bitsize, BOUNDARY_PERIODIC> b((uint64_t) (ciphertext[2]) << bitsize | ciphertext[3], true);

        for (unsigned round = 0, key = Rounds * 2 - 1; round < Rounds; round++, key -= 2) {
            morph(b);
            b.xor_prev(round_keys[key]);
            morph(a);
            a.xor_prev(round_keys[key - 1]);
        }

        output_block[0] = a.prev().to_ullong();
        output_block[1] = a.state().to_ullong();
        output_block[2] = b.prev().to_ullong();
        output_block[3] = b.state().to_ullong();
        return output_block;
    }
};
