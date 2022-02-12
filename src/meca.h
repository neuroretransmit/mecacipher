#pragma once

#include <bitset>
#include <unordered_map>
#include <vector>

#include "common.h"

using namespace std;

/**
 * @brief abstract second-order cellular automata
 * @tparam StateSize size of state for a single timestep
 */
template<size_t StateSize> class AbstractMECA
{
  protected:
    bitset<StateSize> _state;
    bitset<StateSize> _prev;
    unordered_map<bitset<3>, unsigned> _permutations;

    /**
     * Recursive function to generate all permutations of specified neighborhood
     * cells. Used for finding the index of a partition within the exhaustive set
     * and using it as a bit-shift within qualifying rules.
     * @param permutations: vector to store generated permutations in
     * @param neighborhood: starting neighborhood to permute
     * @param cell: cell to operate on
     */
    void generate_partition_states(vector<bitset<3>>& permutations, bitset<3>& neighborhood, size_t cell)
    {
        if (cell == 3) {
            permutations.push_back(neighborhood);
            return;
        }

        neighborhood[cell] = 0;
        generate_partition_states(permutations, neighborhood, cell + 1);
        neighborhood[cell] = 1;
        generate_partition_states(permutations, neighborhood, cell + 1);
    }

    /**
     * @brief retrieve the periodic (cyclic) boundary neighborhood
     * @param cell number of cell in state
     * @return the cell's 3 neighbors as bitset
     */
    bitset<3> periodic_neighborhood(unsigned cell)
    {
        bitset<3> neighborhood;

        // Create a neighborhood with periodic boundary
        if (cell == 0) {
            neighborhood[0] = _state[_state.size() - 1];
            neighborhood[1] = _state[cell];
            neighborhood[2] = _state[cell + 1];
        } else if (cell == (_state.size() - 1)) {
            neighborhood[0] = _state[cell - 1];
            neighborhood[1] = _state[cell];
            neighborhood[2] = _state[0];
        } else {
            neighborhood[0] = _state[cell - 1];
            neighborhood[1] = _state[cell];
            neighborhood[2] = _state[cell + 1];
        }

        return neighborhood;
    }

    /**
     * @brief retrieve the zero (null) boundary neighborhood
     * @param cell number of cell in state
     * @return the cell's 3 neighbors as bitset
     */
    bitset<3> zero_neighborhood(unsigned cell)
    {
        bitset<3> neighborhood;

        // Create a neighborhood with periodic boundary
        if (cell == 0) {
            neighborhood[0] = 0;
            neighborhood[1] = _state[cell];
            neighborhood[2] = _state[cell + 1];
        } else if (cell == (_state.size() - 1)) {
            neighborhood[0] = _state[cell - 1];
            neighborhood[1] = _state[cell];
            neighborhood[2] = 0;
        } else {
            neighborhood[0] = _state[cell - 1];
            neighborhood[1] = _state[cell];
            neighborhood[2] = _state[cell + 1];
        }

        return neighborhood;
    }

  public:
    AbstractMECA(const bitset<StateSize * 2>& start_state, bool reverse)
    {
        vector<bitset<3>> permutations;
        bitset<3> neighborhood(3);
        generate_partition_states(permutations, neighborhood, 0);

        // Build map for faster lookup
        for (unsigned i = 0; i < permutations.size(); i++)
            _permutations[permutations[i]] = i;

        // Split start state into pre-initial/initial conditions
        bitset<StateSize* 2> mask = (StateSize == 32 ? uint32_t(~uint32_t(0)) : uint64_t(~uint64_t(0)));

        // TODO: Convert logic to handle a 128-bit state or pass to constructor split
        //       (to_ullong() is 64 bits on most platforms)
        if (!reverse) {
            _prev = (start_state >> StateSize).to_ullong();
            _state = (start_state & mask).to_ullong();
        } else {
            _prev = (start_state & mask).to_ullong();
            _state = (start_state >> StateSize).to_ullong();
        }
    }

    void xor_state(const bitset<StateSize>& x) { _state ^= x; }
    void xor_prev(const bitset<StateSize>& x) { _prev ^= x; }

    bitset<StateSize> state() { return _state; }
    bitset<StateSize> prev() { return _prev; }
    virtual void step(unsigned rule) = 0;
};

template<size_t StateSize, boundary_t Boundary> class MECA : public AbstractMECA<StateSize>
{
  public:
    MECA(const bitset<StateSize * 2>& start_state, bool reverse = false)
        : AbstractMECA<StateSize>(start_state, reverse)
    {
    }

    /**
     * @brief evolve @rule for one timestep
     * @param rule rule number
     */
    void step(unsigned rule) override
    {
        bitset<StateSize> new_state;

        for (unsigned cell = 0; cell < this->_state.size(); cell++) {
            bitset<3> neighborhood = this->periodic_neighborhood(cell);
            bool first_order_state = (rule >> this->_permutations[neighborhood]) & 1;
            new_state[cell] = this->_prev[cell] != first_order_state ? true : false;
        }

        this->_prev = this->_state;
        this->_state = new_state;
    }
};
