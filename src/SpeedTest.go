package main

import (
    "fmt"
    "math/rand"
    "time"
)

func Calculate(nums []int32, size int) int {
    onesCount := 0
    for i := 0; i < size; i++ {
        if nums[i] == 1 {
            onesCount++
        }
    }

    if onesCount == size {
        return 0
    }

    windowCount := 0
    for i := 0; i < onesCount; i++ {
        if nums[i] == 1 {
            windowCount++
        }
    }

    maxCount := windowCount
    ii := 0
    j := onesCount
    
    for {
        if nums[ii] == 1 {
            windowCount--
        }

        if nums[j] == 1 {
            windowCount++
        }

        maxCount = max(maxCount, windowCount)

        ii++
        if ii == size {
            break
        }

        j++
        if j == size {
            j = 0
        }
    }

    return onesCount - maxCount
}

func FillArray(arr []int32, size int) {
   for i := 0; i < size; i++ {
        if rand.Float64() < 0.5 {
            arr[i] = 1
        } else {
            arr[i] = rand.Int31()
        }
    }
}

func FillArrayTest(arr []int32, size int) {
   for i := 0; i < size; i++ {
        arr[i] = int32(i)
    }
}

func main() {
    TRIAL_COUNT := 100
    testSize := 1000
    
    testArr := make([]int32, testSize)
    FillArrayTest(testArr, testSize);
    fmt.Printf("Test: %d\n", Calculate(testArr, testSize))

	accumulator := 0; // Prevent compiler from removing calls

    // Warmup
    for i := 0; i < 10_000; i++ {
        accumulator += Calculate(testArr, testSize)
    }

    for _, size := range []int{1, 10, 100, 1000, 10_000, 100_000, 1_000_000} {
        nums := make([]int32, size)
        FillArray(nums, size);
        fmt.Printf("n = %-10d", size)

        begin := time.Now().UnixNano()
        for range TRIAL_COUNT {
            accumulator += Calculate(nums, size)
        }
        end := time.Now().UnixNano()
        
        fmt.Printf("%.2f ns\n", float64(end - begin)/float64(TRIAL_COUNT))
    }
    
    fmt.Printf("Accumulator: %d\n", accumulator)
}