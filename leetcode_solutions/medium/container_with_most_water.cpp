#include <vector>
#include <climits>

class Solution
{
    int max_area = INT_MIN;
public:
    int maxArea(vector<int>& height)
    {
        int i = 0, j = height.size() - 1; 
        while (j > i)
        {
            const int hleft = height.at(i);
            const int hright = height.at(j);
            const int m = std::min(hleft, hright);
            const int a = (j - i) * m;
            
            if (a > max_area)
                max_area = a;

            if (m == hleft)
                ++i;
            else
                --j;
        }

        return max_area;
    }
};
