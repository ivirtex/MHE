#pragma once

#include <optional>
#include <vector>

/// \brief Loss function for the subset sum problem.
int loss(const std::vector<int>& subset, int target);

/// \brief Returns the subset of the set based on the mask.
std::vector<int> get_subset(const std::vector<int>& set,
                            const std::vector<bool>& set_mask);

/// \brief Generates a near neighbour of a subset by flipping a random mask bit.
std::vector<int> generate_near_neighbour(const std::vector<int>& set,
                                         const std::vector<bool>& set_mask);

/// \brief Generates a random solution mask (random subset).
std::vector<bool> generate_random_solution_mask(const std::vector<int>& set);
