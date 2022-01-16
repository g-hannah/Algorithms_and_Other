/*
Given an integer x, return true if x is palindrome integer.

An integer is a palindrome when it reads the same backward as forward.

For example, 121 is a palindrome while 123 is not.
 

Example 1:

Input: x = 121
Output: true
Explanation: 121 reads as 121 from left to right and from right to left.
Example 2:

Input: x = -121
Output: false
Explanation: From left to right, it reads -121. From right to left, it becomes 121-. Therefore it is not a palindrome.
Example 3:

Input: x = 10
Output: false
Explanation: Reads 01 from right to left. Therefore it is not a palindrome.
*/

class Solution
{
    /*
    
        if x is negative
            return false
            
        if x is 0
          return true

        i := 0
        r := 0
        l := 0
        
        while true
            r := x % 10
            if r is 0 and i is 0
                return false
            l := l * 10 + r
            if x is less than or equal to l
                break
            x := x / 10
            if x is less than or equals to 1
                break
            i := i + 1
            
        return x is equal to l
     */
public:
    bool isPalindrome(int x)
    {
        if (0 > x)
            return false;
        
        if (0 == x)
            return true;
        
        int l = 0;
        int i = 0;
        int r = 0;
        
        while (1)
        {
            r = x % 10;
            if (0 == r && 0 == i)
                return false;
            
            l = l * 10 + r;
            
            if (x <= l)
                break;
            
            x = x / 10;
            
            if (x <= l)
                break;
            
            ++i;
        }
        
        return x == l;
    }
};
