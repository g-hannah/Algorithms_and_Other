class Solution
{
public:
    string convert(string s, int numRows)
    {
        int len = s.length();
        
        if (1 == numRows || numRows >= s.length())
            return s;
        
        string result = "";
        int skip, fwd, period = (numRows - 1) << 1;
        for (int current = 0; current < numRows; ++current)
        {
            result += s.at(current);
            fwd = current;
            
            while (true)
            {
                skip = (numRows - (current + 1)) << 1;
                if (0 == skip)
                    skip = period;
                
                fwd += skip;
                
                if (fwd >= len)
                    break;
                
                result += s.at(fwd);
                if (0 == (period - skip))
                    continue;
                
                fwd += (period - skip);
                
                if (fwd >= len)
                    break;
                
                result += s.at(fwd);
            }
        }
        
        return result;
    }
};
