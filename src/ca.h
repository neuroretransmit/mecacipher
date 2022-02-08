#pragma once

#include <bitset>
#include <unordered_map>
#include <vector>

#include "common.h"

using namespace std;

/**
 * @brief abstract elementary cellular automata
 * @tparam StateSize size of CA state at a single timestep
 */
template<size_t StateSize> class AbstractCA
{
  protected:
    bitset<StateSize> _state;
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
    AbstractCA(const bitset<StateSize>& start_state) : _state(start_state)
    {
        vector<bitset<3>> permutations;
        bitset<3> neighborhood(3);
        generate_partition_states(permutations, neighborhood, 0);

        // Build map for faster lookup
        for (unsigned i = 0; i < permutations.size(); i++)
            _permutations[permutations[i]] = i;
    }

    bitset<StateSize> state() { return _state; }
    virtual void step(unsigned rule) = 0;
};

template<size_t StateSize, boundary_t Boundary> class CA;

/// 32-bit, periodic boundary elementary cellular automata
template<> class CA<32, BOUNDARY_PERIODIC> : public AbstractCA<32>
{
  public:
    CA(const bitset<32>& start_state) : AbstractCA<32>(start_state) {}

    /**
     * @brief evolve @rule for one timestep
     * @param rule rule number
     */
    void step(unsigned rule) override
    {
        bitset<32> new_state;

        for (unsigned cell = 0; cell < _state.size(); cell++) {
            bitset<3> neighborhood = periodic_neighborhood(cell);
            new_state[cell] = (rule >> _permutations[neighborhood]) & 1;
        }

        _state = new_state;
    }
};

/// 64-bit, periodic boundary elementary cellular automata
template<> class CA<64, BOUNDARY_PERIODIC> : public AbstractCA<64>
{
  public:
    CA(const bitset<64>& start_state) : AbstractCA<64>(start_state) {}

    /**
     * @brief evolve @rule for one timestep
     * @param rule rule number
     */
    void step(unsigned rule) override
    {
        bitset<64> new_state;

        for (unsigned cell = 0; cell < _state.size(); cell++) {
            bitset<3> neighborhood = periodic_neighborhood(cell);
            new_state[cell] = (rule >> _permutations[neighborhood]) & 1;
        }

        _state = new_state;
    }
};
