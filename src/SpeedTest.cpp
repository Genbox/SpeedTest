#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>
#include <type_traits>
#include <algorithm>
#include <iomanip>

std::random_device rd;
std::seed_seq ss{ rd(), rd(), rd(), rd() };
std::mt19937_64 rng(ss);

std::int32_t Calculate(std::int32_t* nums, std::int32_t size) noexcept
{
    auto onesCount = 0;
    for (auto i = 0; i < size; i++) {
        if (nums[i] == 1) {
            onesCount++;
        }
    }

	if (onesCount == size) {
        return 0;
    }

    auto windowCount = 0;
    for (auto i = 0; i < onesCount; i++) {
        if (nums[i] == 1) {
            windowCount++;
        }
    }

    auto maxCount = windowCount;
    auto ii = 0;
    auto j = onesCount;
	
    for (;;)
    {
		if (nums[ii] == 1) {
            windowCount--;
        }

        if (nums[j] == 1) {
            windowCount++;
        }

        maxCount = std::max(maxCount, windowCount);

		ii++;
		if (ii == size) {
            break;
        }

		j++;
        if (j == size) {
            j = 0;
        }
    }

    return onesCount - maxCount;
}

void FillArray(std::int32_t* arr, std::int32_t size)
{
    std::uniform_real_distribution<double> dist_double(0.0, 1.0);
    std::uniform_int_distribution<int> dist_int;

    for (std::int32_t i = 0; i < size; i++) {
        if (dist_double(rng) < 0.5) {
            arr[i] = 1;
        }
        else {
            arr[i] = dist_int(rng);
        }
    }
}

void FillArrayTest(std::int32_t* arr, std::int32_t size)
{
    for (std::int32_t i = 0; i < size; i++) {
        arr[i] = i;
    }
}

void main()
{
    auto TRIAL_COUNT = 100;
    auto testSize = 1000;

    std::int32_t* testArr = new std::int32_t[testSize];
    FillArrayTest(testArr, testSize);
    std::cout << "Test: " << Calculate(testArr, testSize) << std::endl;

	auto accumulator = 0; // Prevent compiler from removing calls
 
    // Warmup
    for (auto i = 0; i < 10'000; i++) {
        accumulator += Calculate(testArr, testSize);
    }

    delete[] testArr;

    for (auto size : { 1, 10, 100, 1000, 10'000, 100'000, 1'000'000 })
    {
        std::int32_t* nums = new std::int32_t[size];
        FillArray(nums, size);
        std::cout << "n = " << size << std::setw(14);

        auto begin = std::chrono::high_resolution_clock::now();
        for (int i = TRIAL_COUNT; i--;) {
            accumulator += Calculate(nums, size);
        }
        auto end = std::chrono::high_resolution_clock::now();
		
        std::cout << std::fixed << std::setprecision(2)
		<< std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() / static_cast<double>(TRIAL_COUNT)
            << " ns"
            << std::endl;
			
		delete[] nums;
    }

	std::cout << "Accumulator: " << accumulator;
}