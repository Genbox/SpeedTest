import java.util.Random;
import java.util.random.RandomGenerator;
import java.util.stream.IntStream;

class SpeedTest 
{
    private static final RandomGenerator rng = new Random();

    public static int Calculate(int[] nums, int size) {
        var onesCount = 0;
        for (int i = 0; i < size; i++) {
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
                ++windowCount;
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

            maxCount = Math.max(maxCount, windowCount);

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

    private static void FillArray(int[] arr, int size) {
        for (var i = 0; i < size; i++) {
            if (rng.nextDouble() < 0.5) {
                arr[i] = 1;
            }
            else {
                arr[i] = rng.nextInt();
            }
        }
    }
    
    private static void FillArrayTest(int[] arr, int size) {
        for (var i = 0; i < size; i++) {
            arr[i] = i;
        }
    }

    public static void main(String[] args) {
        var TRIAL_COUNT = 100;
        var testSize = 1000;
        
        int[] testArr = new int[testSize];
        FillArrayTest(testArr, testSize);
        System.out.println("Test: " + Calculate(testArr, testSize));

    	var accumulator = 0; // Prevent compiler from removing calls

        // Warmup
        for (var i = 0; i < 10_000; i++) {
            accumulator += Calculate(testArr, testSize);
        }

        int pad = 0;
        for (final var size : new int[]{ 1, 10, 100, 1000, 10_000, 100_000, 1_000_000 }) {
            int[] nums = new int[size]; 
            FillArray(nums, size);
            System.out.print("n = " + size + " ".repeat(10 - pad++));

            var begin = System.nanoTime();
            for (var i = 0; i < TRIAL_COUNT; i++)
                accumulator += Calculate(nums, size);
            var end = System.nanoTime();
            
            System.out.println(String.format("%.2f ns", ((end - begin) / (double)TRIAL_COUNT)));
        }
        
        System.out.println("Accumulator: " + accumulator);
    }
}