import subprocess
import json
import matplotlib.pyplot as plt

# C++ programs return results in JSON format:
# # {
# #   "algorithm": "hill_climbing",
# #   "time_ms": 123.456,
# #   "iterations": 100,
# #   "best_subset": [1, 2, 3],
# #   "final_value": 42,
# #   "target": 50,
# #   "loss": 8
# # }

path_to_executables = "./../../build/bin"
path_to_sets = "./../../sets"
target = 2500


def run_program(path: str, args: list) -> dict:
    print(f"Running {path} with arguments: {args}")

    result = subprocess.run([path] + args, capture_output=True, text=True)

    if result.returncode != 0:
        raise RuntimeError(
            f"Error running {path}: {result.stderr} {result.stdout} {result.returncode}"
        )

    return json.loads(result.stdout)


def run_algorithm(algorithm: str, target: int, additional_args: list = []) -> dict:
    set_name = "small_test_set"  # Default set name, can be changed

    path = f"{path_to_executables}/{algorithm}.exe"
    args = [f"{path_to_sets}/{set_name}", str(target)] + additional_args

    return run_program(path, args)


def main():
    # run genetic algorithm with different permutations of parameters: population_count, single_point/two_point crossover, single_bit_flip/probable_bit_flip mutation, max_generations/fitness_threshold
    ga_fitness_threshold_results = []
    ga_fitness_threshold_params = [
        (100, "single_point", "single_bit_flip", "fitness_threshold"),
        (100, "single_point", "probable_bit_flip", "fitness_threshold"),
        (100, "two_point", "single_bit_flip", "fitness_threshold"),
        (100, "two_point", "probable_bit_flip", "fitness_threshold"),
        (200, "single_point", "single_bit_flip", "fitness_threshold"),
        (200, "single_point", "probable_bit_flip", "fitness_threshold"),
        (200, "two_point", "single_bit_flip", "fitness_threshold"),
        (200, "two_point", "probable_bit_flip", "fitness_threshold"),
        (300, "single_point", "single_bit_flip", "fitness_threshold"),
        (300, "single_point", "probable_bit_flip", "fitness_threshold"),
        (300, "two_point", "single_bit_flip", "fitness_threshold"),
        (300, "two_point", "probable_bit_flip", "fitness_threshold"),
    ]

    ga_max_generations_results = []
    ga_max_generations_params = [
        (100, "single_point", "single_bit_flip", "max_generations"),
        (100, "single_point", "probable_bit_flip", "max_generations"),
        (100, "two_point", "single_bit_flip", "max_generations"),
        (100, "two_point", "probable_bit_flip", "max_generations"),
        (200, "single_point", "single_bit_flip", "max_generations"),
        (200, "single_point", "probable_bit_flip", "max_generations"),
        (200, "two_point", "single_bit_flip", "max_generations"),
        (200, "two_point", "probable_bit_flip", "max_generations"),
        (300, "single_point", "single_bit_flip", "max_generations"),
        (300, "single_point", "probable_bit_flip", "max_generations"),
        (300, "two_point", "single_bit_flip", "max_generations"),
        (300, "two_point", "probable_bit_flip", "max_generations"),
    ]

    for (
        population_count,
        crossover,
        mutation,
        termination,
    ) in ga_fitness_threshold_params:
        result = run_algorithm(
            "subset_sum_genetic_algorithm",
            target,
            [
                str(population_count),
                crossover,
                mutation,
                termination,
            ],
        )
        ga_fitness_threshold_results.append(
            (
                {
                    "population_count": population_count,
                    "crossover": crossover,
                    "mutation": mutation,
                    "termination": termination,
                    "fitness_history": result["fitness_history"],
                }
            )
        )

    for (
        population_count,
        crossover,
        mutation,
        termination,
    ) in ga_max_generations_params:
        result = run_algorithm(
            "subset_sum_genetic_algorithm",
            target,
            [
                str(population_count),
                crossover,
                mutation,
                termination,
            ],
        )
        ga_max_generations_results.append(
            (
                {
                    "population_count": population_count,
                    "crossover": crossover,
                    "mutation": mutation,
                    "termination": termination,
                    "fitness_history": result["fitness_history"],
                }
            )
        )

    # plot the fitness history of the genetic algorithm with fitness threshold
    plt.figure(figsize=(10, 6))
    for result in ga_fitness_threshold_results:
        line_width = 1

        if result["population_count"] >= 300:
            line_width = 3
        elif result["population_count"] >= 200:
            line_width = 2
        else:
            line_width = 1

        plt.plot(
            result["fitness_history"],
            linewidth=line_width,
            label=f"GA {result['population_count']} {result['crossover']} {result['mutation']} {result['termination']}",
        )

    plt.title("Genetic Algorithm Fitness History (Fitness Threshold)")
    plt.xlabel("Iteration")
    plt.ylabel("Fitness")
    plt.legend()
    plt.grid()
    plt.show()

    # plot the fitness history of the genetic algorithm with max generations
    plt.figure(figsize=(10, 6))
    for result in ga_max_generations_results:
        line_width = 1

        if result["population_count"] >= 300:
            line_width = 3
        elif result["population_count"] >= 200:
            line_width = 2
        else:
            line_width = 1

        plt.plot(
            result["fitness_history"],
            linewidth=line_width,
            label=f"GA {result['population_count']} {result['crossover']} {result['mutation']} {result['termination']}",
        )
    plt.title("Genetic Algorithm Fitness History (Max Generations)")
    plt.xlabel("Iteration")
    plt.ylabel("Fitness")
    plt.legend()
    plt.grid()
    plt.show()

    # usually the best parameters for the genetic algorithm are:
    # population_count = 300, crossover = two_point, mutation = single_bit_flip, termination = fitness_threshold
    ga_best_result = run_algorithm(
        "subset_sum_genetic_algorithm",
        target,
        ["300", "two_point", "single_bit_flip", "fitness_threshold"],
    )

    ga_parallel_best_result = run_algorithm(
        "subset_sum_genetic_algorithm_parallel",
        target,
        ["300", "two_point", "single_bit_flip", "fitness_threshold"],
    )

    # full search result
    full_search_result = run_algorithm("subset_sum_full_search", target)

    # run hill climbing algorithm
    hill_climbing_result = run_algorithm("subset_sum_hill_climbing", target)

    # run simulated annealing algorithm
    simulated_annealing_linear_result = run_algorithm(
        "subset_sum_sim_annealing", target, ["linear"]
    )

    simulated_annealing_logarithmic_result = run_algorithm(
        "subset_sum_sim_annealing", target, ["logarithmic"]
    )

    # plot simulated annealing results
    plt.figure(figsize=(10, 6))
    plt.plot(
        simulated_annealing_linear_result["fitness_history"],
        label="Simulated Annealing (Linear Cooling)",
    )
    plt.plot(
        simulated_annealing_logarithmic_result["fitness_history"],
        label="Simulated Annealing (Logarithmic Cooling)",
    )
    plt.title("Simulated Annealing Fitness History")
    plt.xlabel("Iteration")
    plt.ylabel("Fitness")
    plt.legend()
    plt.grid()
    plt.show()

    # run tabu search algorithm
    tabu_search_max_tabu_result = run_algorithm(
        "subset_sum_tabu_search", target, ["50"]
    )
    tabu_search_unlimited_tabu_result = run_algorithm("subset_sum_tabu_search", target)

    # plot tabu search results
    plt.figure(figsize=(10, 6))
    plt.plot(
        tabu_search_max_tabu_result["fitness_history"],
        label="Tabu Search (Max Tabu)",
    )
    plt.plot(
        tabu_search_unlimited_tabu_result["fitness_history"],
        label="Tabu Search (Unlimited Tabu)",
    )
    plt.title("Tabu Search Fitness History")
    plt.xlabel("Iteration")
    plt.ylabel("Fitness")
    plt.legend()
    plt.grid()
    plt.show()

    # for tabu search, the best parameters are:
    # unlimited_tabu = True

    tabu_search_best_result = run_algorithm("subset_sum_tabu_search", target)

    # compare all results
    results = {
        "full_search": full_search_result,
        "hill_climbing": hill_climbing_result,
        "simulated_annealing_linear": simulated_annealing_linear_result,
        "simulated_annealing_logarithmic": simulated_annealing_logarithmic_result,
        "tabu_best": tabu_search_best_result,
        "ga_best": ga_best_result,
    }

    # plot all results
    plt.figure(figsize=(10, 6))
    for name, result in results.items():
        plt.plot(
            result["fitness_history"],
            label=name,
        )
    plt.title("Comparison of Algorithms Fitness History")
    plt.xlabel("Iteration")
    plt.ylabel("Fitness")
    plt.legend()
    plt.grid()
    plt.show()

    # plot execution times
    plt.figure(figsize=(10, 6))
    execution_times = {
        "Full Search": full_search_result["time_ms"],
        "Hill Climbing": hill_climbing_result["time_ms"],
        "Simulated Annealing (Linear)": simulated_annealing_linear_result["time_ms"],
        "Simulated Annealing (Logarithmic)": simulated_annealing_logarithmic_result[
            "time_ms"
        ],
        "Tabu Search (Best)": tabu_search_best_result["time_ms"],
        "Genetic Algorithm (Best)": ga_best_result["time_ms"],
        "Parallel Genetic Algorithm (Best)": ga_parallel_best_result["time_ms"],
    }
    plt.bar(execution_times.keys(), execution_times.values())
    plt.title("Execution Times of Algorithms")
    plt.xlabel("Algorithm")
    plt.ylabel("Time (ms)")
    plt.xticks(rotation=45)
    plt.grid(axis="y")
    plt.show()


if __name__ == "__main__":
    main()
