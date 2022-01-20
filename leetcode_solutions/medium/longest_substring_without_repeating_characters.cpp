class Solution
{
    set<char> cs;
public:
    int lengthOfLongestSubstring(string s)
    {
        if (0 == s.length())
            return 0;

        int count = 0, i = 0, start = i;
        while (start < (s.length() - count))
        {
            while (i < s.length() && cs.end() == cs.find(s.at(i)))
            {
                cs.insert(s.at(i++));
            }

            if (cs.size() > count)
                count = cs.size();

            if (i >= s.length())
                return count;

            while (s.at(start) != s.at(i))
            {
                cs.erase(s.at(start++));
            }

            ++start;
            ++i;
        }

        return count;
    }
};
