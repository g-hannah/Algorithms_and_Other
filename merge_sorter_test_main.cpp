#include <climits>
#include <iostream>
#include <functional>
#include "MergeSorter.h"

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

int
main(void)
{
    const int len1 = 9;
    const int len2 = 8;
    int arr1[] = { 8, 7, 6, 5, 4, 3, 2, 1, 0 };
    int arr1_expected[] = { 0, 1, 2, 3, 4, 5, 6 ,7, 8 };
    int arr2[] = { INT_MAX, 0x7fffffff, 0x30000000, 123456, 0, -0x30000000, -0x7fffffff, INT_MIN };
    int arr2_expected[] = { INT_MIN, -0x7fffffff, -0x30000000, 0, 123456, 0x30000000, 0x7fffffff, INT_MAX };

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

    print_arr(arr1, len1);
    print_arr(arr1_expected, len1);

    MergeSorter *ms = new MergeSorter();

    ms->sort(arr1, 0, len1);
    ms->sort(arr2, 0, len2);

    std::cerr << "arr1 == arr1_expected:  " << are_equal(arr1, arr1_expected, len1, len1) << std::endl;
    std::cerr << "arr2 == arr2_expected:  " << are_equal(arr1, arr1_expected, len2, len2) << std::endl;

    print_arr(arr1, len1);
    print_arr(arr1_expected, len1);
    print_arr(arr2, len2);
    print_arr(arr2_expected, len2);

    delete ms;

    return 0;
}
