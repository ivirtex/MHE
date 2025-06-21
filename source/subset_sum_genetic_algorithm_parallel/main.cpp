#include <algorithm>
#include <chrono>
#include <execution>
#include <new>
#include <numeric>
#include <print>
#include <ranges>
#include <utility>
#include <vector>

#include "helpers.h"
#include "subset_sum.h"

enum class CrossoverMethod {
  SinglePoint,
  TwoPoint,
};

enum class MutationMethod {
  SingleBitFlip,
  ProbableBitFlip,
};

enum class TerminationMethod {
  MaxGenerations,
  FitnessThreshold,
};

std::pair<std::vector<bool>, std::vector<bool>> crossover(
    const std::vector<bool>& parent1,
    const std::vector<bool>& parent2,
    CrossoverMethod method) {
  std::vector<bool> child1 = parent1;
  std::vector<bool> child2 = parent2;

  switch (method) {
    case CrossoverMethod::SinglePoint: {
      int crossover_point = get_random_int(0, parent1.size() - 1);
      for (int i = crossover_point; i < parent1.size(); ++i) {
        child1[i] = parent2[i];
        child2[i] = parent1[i];
      }
      break;
    }
    case CrossoverMethod::TwoPoint: {
      int point1 = get_random_int(0, parent1.size() - 2);
      int point2 = get_random_int(point1 + 1, parent1.size() - 1);
      for (int i = point1; i <= point2; ++i) {
        child1[i] = parent2[i];
        child2[i] = parent1[i];
      }
      break;
    }
  }

  return {child1, child2};
}

// mutation
std::vector<bool> mutate(const std::vector<bool>& mask, MutationMethod method) {
  std::vector<bool> mutated_mask = mask;

  switch (method) {
    case MutationMethod::SingleBitFlip: {
      int flip_index = get_random_int(0, mutated_mask.size() - 1);
      mutated_mask[flip_index] = !mutated_mask[flip_index];
      break;
    }
    case MutationMethod::ProbableBitFlip: {
      for (size_t i = 0; i < mutated_mask.size(); ++i) {
        if (get_random_double(0.0, 1.0) < 0.1) {  // 10% mutation chance
          mutated_mask[i] = !mutated_mask[i];
        }
      }
      break;
    }
  }

  return mutated_mask;
}

std::vector<bool> tournament_selection(
    const std::vector<std::vector<bool>>& population,
    const std::vector<double>& fitness_values,
    int tournament_size = 2) {
  int pop_size = static_cast<int>(population.size());
  int best_idx = get_random_int(0, pop_size - 1);

  for (int i = 1; i < tournament_size; ++i) {
    int idx = get_random_int(0, pop_size - 1);
    if (fitness_values[idx] > fitness_values[best_idx]) {
      best_idx = idx;
    }
  }

  return population[best_idx];
}

int main(int argc, char* argv[]) {
  auto [file, target, population_count, crossover_method_str,
        mutation_method_str, termination_method_str] =
      parse_args<std::string, int, int, std::string, std::string, std::string>(
          argc, argv,
          "<file> <target> <population_count> <crossover_method: "
          "single_point/two_point> "
          "<mutation_method: single_bit_flip/probable_bit_flip> "
          "<termination_method: max_generations/fitness_threshold>");

  // Convert crossover method string to enum
  CrossoverMethod crossover_method;
  if (crossover_method_str == "single_point") {
    crossover_method = CrossoverMethod::SinglePoint;
  } else if (crossover_method_str == "two_point") {
    crossover_method = CrossoverMethod::TwoPoint;
  } else {
    std::print("Invalid crossover method: {}\n", crossover_method_str);
    return 1;
  }

  // Convert mutation method string to enum
  MutationMethod mutation_method;
  if (mutation_method_str == "single_bit_flip") {
    mutation_method = MutationMethod::SingleBitFlip;
  } else if (mutation_method_str == "probable_bit_flip") {
    mutation_method = MutationMethod::ProbableBitFlip;
  } else {
    std::print("Invalid mutation method: {}\n", mutation_method_str);
    return 1;
  }

  // Convert termination method string to enum
  TerminationMethod termination_method;
  if (termination_method_str == "max_generations") {
    termination_method = TerminationMethod::MaxGenerations;
  } else if (termination_method_str == "fitness_threshold") {
    termination_method = TerminationMethod::FitnessThreshold;
  } else {
    std::print("Invalid termination method: {}\n", termination_method_str);
    return 1;
  }

  auto should_terminate = [&](int generation, double best_fitness) {
    if (termination_method == TerminationMethod::MaxGenerations) {
      return generation >= 10000;
    } else if (termination_method == TerminationMethod::FitnessThreshold) {
      return best_fitness > 0.99;  // Assuming fitness is normalized to [0, 1]
    }
    return false;
  };

  solve(
      "Genetic parallel", file, target,
      [&](const std::vector<int>& set, int target) {
        std::vector<double> fitness_history;
        std::vector<std::vector<bool>> population(population_count);

        // Generate initial population in parallel
        std::generate(std::execution::par, population.begin(), population.end(),
                      [&]() { return generate_random_solution_mask(set); });

        int generation = 0;
        double best_fitness = 0.0;
        std::vector<bool> best_mask;

        while (!should_terminate(generation, best_fitness)) {
          std::vector<double> population_fitness(population_count, 0.0);

          // Use parallel execution to calculate fitness values
          std::transform(
              std::execution::par_unseq, population.begin(), population.end(),
              population_fitness.begin(), [&](const std::vector<bool>& mask) {
                double fitness_value = fitness(get_subset(set, mask), target);

                if (fitness_value > best_fitness) {
                  best_fitness = fitness_value;
                  best_mask = mask;
                }

                return fitness_value;
              });

          fitness_history.push_back(best_fitness);

          std::vector<std::vector<bool>> offspring;
          offspring.reserve(population_count);

          if (!best_mask.empty()) {
            offspring.push_back(best_mask);
          }

          while (offspring.size() < population_count) {
            // Select parents using tournament selection
            auto parent1 = tournament_selection(population, population_fitness);
            auto parent2 = tournament_selection(population, population_fitness);

            // Crossover
            auto [child1, child2] =
                crossover(parent1, parent2, crossover_method);

            // Mutate children
            child1 = mutate(child1, mutation_method);
            child2 = mutate(child2, mutation_method);

            offspring.push_back(child1);
            if (offspring.size() < population_count) {
              offspring.push_back(child2);
            }
          }

          population = std::move(offspring);
          generation++;
        }

        SubsetSumResult result{
            .best_subset = get_subset(set, best_mask),
            .fitness_history = fitness_history,
            .iterations = generation,
        };

        return result;
      });
}
