#include <iostream>
#include <climits>

using namespace std;

class Solution
{
    using u32 = uint32_t;
  
    const u32 UVALUE_INT_MAX = (u32)INT32_MAX;
    const u32 UVALUE_INT_MIN = (u32)INT32_MIN;
    const std::string INT_MIN_STRING = std::to_string(INT_MIN);
    bool neg = false;

    inline bool is_digit(const char c)
    {
        return c >= (char)0x30 && c < (char)0x40;
    }

    inline bool is_sign(const char c)
    {
        return c == '+' || c == '-';
    }
    
    inline bool is_whitespace(const char c)
    {
        return c == ' ';
    }

    inline u32 char_to_value(char c)
    {
        return (u32)(c - 0x30);
    }

    inline void push_value(u32 *target, u32 value)
    {
        if (neg)
        {
            if (*target == UVALUE_INT_MIN)
                return;
            
            if (*target > (UVALUE_INT_MIN/10))
            {
                *target = UVALUE_INT_MIN;
                return;
            }

            *target *= 10;

            if ((*target + value) > UVALUE_INT_MIN)
            {
                *target = UVALUE_INT_MIN;
                return;
            }


            *target += value;
        }
        else
        {
            if (*target == UVALUE_INT_MAX)
                return;

            if (*target > (UVALUE_INT_MAX/10))
            {
                *target = UVALUE_INT_MAX;
                return;
            }

            *target *= 10;

            if ((*target + value) > UVALUE_INT_MAX)
            {
                *target = UVALUE_INT_MAX;
                return;
            }

            *target += value;
        }
    }
public:
    int myAtoi(std::string s)
    {
        if (0 == s.length())
            return 0;

        if (INT_MIN_STRING == s)
            return INT_MIN;
        
        int i = 0;
        
        while (i < s.length() && !is_digit(s.at(i)) && !is_sign(s.at(i)) && is_whitespace(s.at(i)))
        {
            ++i;
        }
        
        if (i >= s.length())
            return 0;
        
        if (!is_sign(s.at(i)) && !is_digit(s.at(i)))
            return 0;

        u32 ret = 0;
        const char first = s.at(i);
        if (is_sign(first))
        {
            neg = (first == '-');
            if ((i + 1) < s.length())
            {
                if (!is_digit(s.at(i+1)))
                    return 0;
            }
        }
        else
        {
            push_value(&ret, char_to_value(first));
        }

        for (++i; i < s.length() && is_digit(s.at(i)); ++i)
        {
            push_value(&ret, char_to_value(s.at(i)));
        }

        return neg ? (int)((~ret)+1) : (int)ret;
    }
};
