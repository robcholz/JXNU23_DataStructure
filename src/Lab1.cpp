#include <iostream>
#include <random>
#include <functional>
#include <chrono>


class RandomGenerator {
public:
	RandomGenerator(const RandomGenerator&) = delete;

	RandomGenerator& operator=(const RandomGenerator&) = delete;

	~RandomGenerator() = default;

	static RandomGenerator& getInstance() {
		static RandomGenerator randomGenerator;
		return randomGenerator;
	}

	template<typename T>
	static T getRange(int a, int b) {
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist6(a, b);
		return T(dist6(rng));
	}

private:
	RandomGenerator() = default;
};

class Sort {
public:
	Sort() = delete;

	Sort(const Sort&) = delete;

	Sort& operator=(const Sort&) = delete;

	~Sort() = delete;

	template<typename T>
	static void bubbleSort(T* array, std::size_t length) {
		for (auto i = 0; i < length - 1; ++i) {
			for (auto j = 0; j < length - i - 1; ++j) {
				if (array[j] < array[j + 1]) {
					T temp;
					temp = array[j];
					array[j] = array[j + 1];
					array[j + 1] = temp;
				}
			}
		}
	}

	template<typename T>
	static void selectionSort(T* array, std::size_t length) {
		std::size_t min_idx;
		for (auto i = 0; i < length - 1; i++) {
			min_idx = i;
			for (auto j = i + 1; j < length; j++) {
				if (array[j] < array[min_idx])
					min_idx = j;
			}
			if (min_idx != i) {
				auto temp = array[min_idx];
				array[min_idx] = array[i];
				array[i] = temp;
			}
		}
	}

	template<typename T>
	static auto generateArray(std::size_t length) {
		return new T[length];
	}

	template<typename T>
	static void fill(T* array, std::size_t length, std::function<T()> fillFunc) {
		for (auto i = 0; i < length; ++i) {
			array[i] = fillFunc();
		}
	}
};

int main() {
	using ArrayT = int;
	std::size_t length = 10000;

	auto array_bubble = Sort::generateArray<ArrayT>(length);
	auto array_quick = Sort::generateArray<ArrayT>(length);
	Sort::fill<ArrayT>(array_bubble, length, []() { return RandomGenerator::getRange<ArrayT>(0, 1e3); });
	Sort::fill<ArrayT>(array_quick, length, []() { return RandomGenerator::getRange<ArrayT>(0, 1e3); });

	const auto start_bubble = std::chrono::high_resolution_clock::now();
	Sort::bubbleSort<ArrayT>(array_bubble, length);
	const auto end_bubble = std::chrono::high_resolution_clock::now();

	const auto start_quick = std::chrono::high_resolution_clock::now();
	Sort::selectionSort<ArrayT>(array_quick, length);
	const auto end_quick = std::chrono::high_resolution_clock::now();

	std::cout << "Bubble Sort: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_bubble - start_bubble).count()
	          << "ms" << std::endl;
	std::cout << "Quick Sort: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_quick - start_quick).count()
	          << "ms" << std::endl;

	delete[] array_bubble;
	delete[] array_quick;

	return 0;
}
