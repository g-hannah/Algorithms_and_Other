#include <functional>
#include <iostream>
#include <climits>
 
/**
 * Implementation of selection sort in C++.
 *
 * Sort an array of integers in ascending order.
 */
 void
 selection_sort(int arr[], const int len)
 {
     int start_position = 0;
     int min_value = INT_MAX;
     int min_idx = 0;

     while (start_position < len - 1)
     {
         for (int i = min_idx = start_position; i < len; ++i)
         {
             if (arr[i] < min_value)
             {
                 min_value = arr[i];
                 min_idx = i;
             }
         }

         if (min_idx != start_position)
         {
             // A = A ^ B == C
             // B = B ^ C == A
             // C = C ^ A == B
             arr[start_position] ^= arr[min_idx];
             arr[min_idx] ^= arr[start_position];
             arr[start_position] ^= arr[min_idx];
         }

         ++start_position;
         min_value = INT_MAX;
     }
 }
 
 static void
 print_arr(const int arr[], const int len)
 {
     std::cerr << "[ ";

     for (int i = 0; i < len; ++i)
     {
         if (0 == i)
            std::cerr << arr[i];
         else
            std::cerr << ", " << arr[i];
     }

     std::cerr << " ]" << std::endl;
 }

int main()
{
    const int LEN = 8;

    int arr1[] = { 7, 6, 5, 4, 3, 2, 1, 0 };
    int arr1_expected[] = { 0, 1, 2, 3, 4, 5, 6 ,7 };
    int arr2[] = { INT_MAX, 0x7fffffff, 0x30000000, 123456, 0, -0x30000000, -0x7fffffff, INT_MIN };
    int arr2_expected[] = { INT_MIN, -0x7fffffff, -0x30000000, 0, 123456, 0x30000000, 0x7fffffff, INT_MAX };

 /*
  * Introduced in C++11
  */
    std::function<bool(int[],int[],int,int)> are_equal = [](int arr1[], int arr2[], int l1, int l2){

        if (l1 != l2)
            return false;

        for (int i = 0; i < l1; ++i)
        {
            if (arr1[i] != arr2[i])
                return false;
        }

        return true;
    };

    std::cerr << "arr1:" << std::endl;
    print_arr(arr1, LEN);
    std::cerr << "arr2:" << std::endl;
    print_arr(arr2, LEN);

    std::cerr << std::endl;

    std::cerr << "sorting arrays with selection sort" << std::endl;

    selection_sort(arr1, LEN);
    selection_sort(arr2, LEN);

    std::cerr << "arr1 == arr1_expected: " << (are_equal(arr1, arr1_expected, LEN, LEN)) << std::endl;
    std::cerr << "arr2 == arr2_expected: " << (are_equal(arr2, arr2_expected, LEN, LEN)) << std::endl;

    std::cerr << std::endl;

    std::cerr << "arr1:" << std::endl;
    print_arr(arr1, LEN);
    std::cerr << "arr1_expected:" << std::endl;
    print_arr(arr1_expected, LEN);

    std::cerr << std::endl;

    std::cerr << "arr2:" << std::endl;
    print_arr(arr2, LEN);
    std::cerr << "arr2_expected:" << std::endl;
    print_arr(arr2_expected, LEN);

    /*
            ===================== OUTPUT ====================

    arr1:
    [ 7, 6, 5, 4, 3, 2, 1, 0 ]
    arr2:
    [ 2147483647, 2147483647, 805306368, 123456, 0, -805306368, -2147483647, -2147483648 ]

    sorting arrays with selection sort
    arr1 == arr1_expected: 1
    arr2 == arr2_expected: 1

    arr1:
    [ 0, 1, 2, 3, 4, 5, 6, 7 ]
    arr1_expected:
    [ 0, 1, 2, 3, 4, 5, 6, 7 ]

    arr2:
    [ -2147483648, -2147483647, -805306368, 0, 123456, 805306368, 2147483647, 2147483647 ]
    arr2_expected:
    [ -2147483648, -2147483647, -805306368, 0, 123456, 805306368, 2147483647, 2147483647 ]

     */

    return 0;
}
