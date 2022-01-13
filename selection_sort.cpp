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
         for (int i = start_position; i < len; ++i)
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
    int arr[LEN] = { 1711, 28, -194, 81, 29, -41, 227, -4567 };

    print_arr(arr, LEN);
    selection_sort(arr, LEN);
    print_arr(arr, LEN);

    return 0;
}
