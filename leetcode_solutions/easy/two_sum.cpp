/*
Given an array of integers nums and an integer target, return indices of the two numbers such that they add up to target.

You may assume that each input would have exactly one solution, and you may not use the same element twice.

You can return the answer in any order.

 

Example 1:

Input: nums = [2,7,11,15], target = 9
Output: [0,1]
Explanation: Because nums[0] + nums[1] == 9, we return [0, 1].
Example 2:

Input: nums = [3,2,4], target = 6
Output: [1,2]
Example 3:

Input: nums = [3,3], target = 6
Output: [0,1]
 */

class Solution
{
    using miter = std::map<int,int>::iterator;
public:
    vector<int> twoSum(vector<int>& nums, int target)
    {
        std::vector<int> ret;
        std::map<int,int> map;
        /*
           Do a single pass over the input array to both
           insert into the map and also check if the
           solution already exists, at which point stop.
           
           The mapping is (target - nums[i]) -> i
           
           Therefore, the key is the number required to
           add to nums[i] to make 'target'. Thus, we can
           for each element in the array check if there
           is a mapping for that value. If so, the index
           stored there is the index of the number to add
           to sum to target. Return the current index
           and the index mapped to.
         */
        for (int i = 0; i < nums.size(); ++i)
        {
            const miter other = map.find(nums.at(i));
            if (map.end() != other && i != other->second)
            {
                ret.push_back(i);
                ret.push_back(other->second);
                
                return ret;
            }
            map.insert(pair<int,int>(target - nums.at(i), i));
        }

        return ret;
    }
};
