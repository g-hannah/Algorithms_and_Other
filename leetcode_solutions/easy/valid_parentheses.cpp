/*

Given a string s containing just the characters '(', ')', '{', '}', '[' and ']', determine if the input string is valid.

An input string is valid if:

Open brackets must be closed by the same type of brackets.
Open brackets must be closed in the correct order.
 

Example 1:

Input: s = "()"
Output: true
Example 2:

Input: s = "()[]{}"
Output: true
Example 3:

Input: s = "(]"
Output: false
 

Constraints:

1 <= s.length <= 104
s consists of parentheses only '()[]{}'.

*/

class Solution
{
private:
    stack<char> sk;
    unordered_map<char,char> compliment = {

        { ']', '[' }, { '}', '{' }, { ')', '(' }
    };

    inline bool is_opening(const char c)
    {
      /*
          All the closing parentheses end in
          binary 01, whereas the opening ones
          end in either 11 or 00.
          
          So we logical AND with mask 00000011
          and if the result is not 00000001,
          it's an opening parenthesis.
          
          (The constraints stipulate that
          the string will ONLY contain
          parentheses, and therefore for
          this problem it is acceptable to
          check this way).
       */
        return (c & 0x03) != 0x01;
    }

public:
    bool isValid(string s)
    {
        for (size_t i = 0, n = s.length(); i < n; ++i)
        {
            const char c = s.at(i);
            if (is_opening(c))
            {
                sk.push(c);
            }
            else
            {
                if (sk.empty())
                    return false;

                if (compliment[c] != sk.top())
                    return false;

                sk.pop();
            }
        }

        return sk.empty();
    }
};
