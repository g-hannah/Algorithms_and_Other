#include "MergeSorter.h"
#include <iostream>
#include <cstring>

MergeSorter::MergeSorter()
{
    //ctor
}

MergeSorter::~MergeSorter()
{
    //dtor
}

/**
 * Implementation of merge sort in C++
 *
 * @author Gary Hannah
 */
void MergeSorter::sort(int arr[], const int left, const int right)
{
    const int diff = right - left;

    /*
     * The stopping condition for recursive function calls.
     * An array of size 1 is sorted. Now we can return
     * and the previous instance of sort() will then merge.
     */
    if (1 == diff)
        return;

    /*
     * Split the array into two subsegments
     */
    const int mid = (left + right) >> 1;

    /*
     * Recursively call sort() on subsegments
     */
    sort(arr, left, mid);
    sort(arr, mid, right);

    /*
     * Temporary array for merging process
     */
    int tmp[diff];
    int k = 0, i = left, j = mid;

    for ( ; i < mid && j < right; )
    {
        if (arr[j] < arr[i])
            tmp[k++] = arr[j++];
        else
            tmp[k++] = arr[i++];
    }

    /*
     * Catch the rest that may have been missed
     * at the end of one of the subsegments
     */
    while (i < mid)
        tmp[k++] = arr[i++];

    while (j < right)
        tmp[k++] = arr[j++];

    /*
     * Now overwrite subsegment [left:right] with
     * the sorted values in tmp[]
     */
    std::memcpy(arr + left, tmp, diff * sizeof(int));

    return;
}
