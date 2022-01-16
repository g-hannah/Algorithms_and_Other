/*
Roman numerals are represented by seven different symbols: I, V, X, L, C, D and M.

Symbol       Value
I             1
V             5
X             10
L             50
C             100
D             500
M             1000
For example, 2 is written as II in Roman numeral, just two one's added together. 12 is written as XII, which is simply X + II. The number 27 is written as XXVII, which is XX + V + II.

Roman numerals are usually written largest to smallest from left to right. However, the numeral for four is not IIII. Instead, the number four is written as IV. Because the one is before the five we subtract it making four. The same principle applies to the number nine, which is written as IX. There are six instances where subtraction is used:

I can be placed before V (5) and X (10) to make 4 and 9. 
X can be placed before L (50) and C (100) to make 40 and 90. 
C can be placed before D (500) and M (1000) to make 400 and 900.
Given a roman numeral, convert it to an integer.

 

Example 1:

Input: s = "III"
Output: 3
Explanation: III = 3.
Example 2:

Input: s = "LVIII"
Output: 58
Explanation: L = 50, V= 5, III = 3.
Example 3:

Input: s = "MCMXCIV"
Output: 1994
Explanation: M = 1000, CM = 900, XC = 90 and IV = 4.
 */

class Solution
{
private:
    /*
      Array to look up values from char in O(1) time
     */
    int values[26] = {
        -1, -1, 100, 500, -1, -1, -1, -1, 1, -1, -1,
        50, 1000, -1, -1, -1, -1, -1, -1, -1, -1, 5,
        -1, 10, -1, -1
    };
    
    /*
      Helper method to convert from char to index into array
     */
    int _idx(char c)
    {
        return (int)(c - 0x41);
    }
    
    /*
        Algorithm:
        
        total_value := 0
        last_value := INT_MIN
        current_value := 0
        Start from end of input string
        
        1. current_value := value for roman numeral at current position
        2. if current_value is less than last_value
          2.i total_value := total_value - current_value
        3. else
          3.i total_value := total_value + current_value
        4. decrement pointer
        5. if pointer is equal to start of string
          5.i go to 7
        6. else
          6.i go to 1
        7. return total_value
     */
public:
    int romanToInt(string s)
    {        
        const int len = s.length();        
        const char *start = s.c_str();
        const char *e = start + len - 1;
        
        int total_value = 0;
        int last_value = INT_MIN;
        int current_value = 0;
        
        while (e >= start)
        {
            cur = values[_idx(*e)];
            
            if (current_value < last_value)
                total_value -= current_value;
            else
                total_value += current_value;
            
            last_value = current_value;
            --e;
        }
        
        return total_value;
    }
};
