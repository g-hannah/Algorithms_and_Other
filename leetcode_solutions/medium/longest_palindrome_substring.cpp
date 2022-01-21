class Solution
{
public:
    string longestPalindrome(string s)
    {
        if (1 == s.length())
            return s;

        int count = 0, current;
        string result = "";
        int len = s.length();

        for (current = 0; current < len; ++current)
        {
            int i = current, j = current;
            while (i >= 0 && j < len && s.at(i) == s.at(j))
                --i, ++j;
            
            if (j - i - 1 > count)
            {
                count = j - i - 1;
                result = s.substr(i + 1, count);
            }
            
            i = current, j = current + 1;
            while (i >= 0 && j < len && s.at(i) == s.at(j))
                --i, ++j;
            
            if (j - i - 1 > count)
            {
                count = j - i - 1;
                result = s.substr(i + 1, count);
            }
        }

        if (0 == count)
            return s.substr(0,1);

        return result;
    }
};
