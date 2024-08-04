using System;
using System.Diagnostics;
using System.Linq;

internal class SpeedTest
{
    private static Random rng = new Random();
    
    public static int Calculate(int[] nums, int size)
    {
        var onesCount = 0;
        for (var i = 0; i < size; i++) {
            if (nums[i] == 1) {
                onesCount++;
			}
        }

        if (onesCount == size) {
            return 0;
		}

        var windowCount = 0;
        for (var i = 0; i < onesCount; i++) {
            if (nums[i] == 1) {
                windowCount++;
			}
        }

        var maxCount = windowCount;
		var ii = 0;
        var j = onesCount;
        
        for (;;)
        {
            if (nums[ii] == 1) {
                windowCount--;
			}

            if (nums[j] == 1) {
                windowCount++;
			}

            maxCount = Math.Max(maxCount, windowCount);

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

    private static void FillArray(int[] arr, int size)
    {
        for (int i = 0; i < size; i++) {
            if (rng.NextDouble() < 0.5) {
                arr[i] = 1;
            }
            else {
                arr[i] = rng.Next();
            }
        }
    }
    
    private static void FillArrayTest(int[] arr, int size)
    {
        for (int i = 0; i < size; i++) {
            arr[i] = i;
        }
    }
  
    private static void Main(string[] args)
    {
        var TRIAL_COUNT = 100;
        var testSize = 1000;
        
        int[] testArr = new int[testSize];
        FillArrayTest(testArr, testSize);
        Console.WriteLine($"Test: {Calculate(testArr, testSize)}");

        var accumulator = 0; // Prevent compiler from removing calls

        // Warmup
        for (var i = 0; i < 10_000; i++) {
            accumulator += Calculate(testArr, testSize);
        }

        int pad = 0;
        foreach (var size in (int[]) [1, 10, 100, 1000, 10_000, 100_000, 1_000_000])
        {
            int[] nums = new int[size];
            FillArray(nums, size);
            Console.Write("n = " + size + "".PadLeft(10 - pad++));

            var begin = Stopwatch.GetTimestamp();
            for (var i = 0; i < TRIAL_COUNT; i++) {
                accumulator += Calculate(nums, size);
            }
            var end = Stopwatch.GetTimestamp();
            
            Console.WriteLine(((end - begin) * 100) / (double)TRIAL_COUNT + " ns");
        }
        
        Console.WriteLine("Accumulator: " + accumulator);
    }
}