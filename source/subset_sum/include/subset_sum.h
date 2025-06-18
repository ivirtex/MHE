#pragma once

#include <functional>
#include <optional>
#include <string>
#include <vector>

/// \brief Loss function for the subset sum problem.
int loss(const std::vector<int>& subset, int target);

/// \brief Fitness function for the subset sum problem.
double fitness(const std::vector<int>& subset, int target);

/// \brief Returns the subset of the set based on the mask.
std::vector<int> get_subset(const std::vector<int>& set,
                            const std::vector<bool>& set_mask);

/// \brief Generates a near neighbour of a subset by flipping a random mask bit
/// and returning the new subset.
std::vector<int> generate_near_neighbour(const std::vector<int>& set,
                                         const std::vector<bool>& set_mask);

/// \brief Generates a near neighbour mask by flipping a random bit in the mask.
std::vector<bool> generate_near_neighbour_mask(
    const std::vector<bool>& set_mask);

std::vector<std::vector<bool>> generate_near_neighbour_masks(
    const std::vector<bool>& set_mask);

/// \brief Generates a random solution mask (random subset).
std::vector<bool> generate_random_solution_mask(const std::vector<int>& set);

struct SubsetSumResult {
  std::vector<int> best_subset;
  std::vector<double> fitness_history;
  int iterations;
};

void solve(const std::string& algoritm_name,
           const std::string& file,
           int target,
           const std::function<SubsetSumResult(const std::vector<int>& set,
                                               int target)>& algoritm);
