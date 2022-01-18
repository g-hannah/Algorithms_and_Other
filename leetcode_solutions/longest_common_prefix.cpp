#include <iostream>
#include <vector>

using namespace std;

class Solution
{
private:
    char ret[200];
public:
    string longestCommonPrefix(vector<string>& strs)
    {
        if (1 == strs.size())
            return strs[0];

        string first = strs[0];
        size_t l = first.length();
        for (size_t i = 0; i < l; ++i)
            ret[i] = first.at(i);

        int divergence = l;

        for (int i = 1, n = strs.size(); i < n; ++i)
        {
            const string s = strs[i];
            const int len = s.length();
            int p = 0;

            while (p < len && p < divergence)
            {
                if (s.at(p) != ret[p])
                {
                    if (0 == p)
                    {
                        return "";
                    }

                    divergence = p;
                    break;
                }

                ++p;
            }

            if (p == len)
                divergence = len;
        }

        ret[divergence] = 0;

        string r = "";

        for (int i = 0; i < divergence; ++i)
            r += ret[i];

        return ret;
    }
};

static void
test(vector<string>& v, const string expected)
{
    Solution s;
    const string result = s.longestCommonPrefix(v);
    if (result == expected)
        cerr << result << " == " << expected << " PASSED!";
    else
    {
        cerr << result << " == " << expected << " FAILED! Got result " << result << endl;
    }
}

int
main(void)
{
    vector<string> v;

    v.push_back("flower");
    v.push_back("flow");
    v.push_back("flight");
    v.push_back("cow");
    v.push_back("carry");

    test(v, "");

    return 0;
}
