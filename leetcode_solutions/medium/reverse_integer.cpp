class Solution
{
    const unsigned int UVALUE_OF_INT_MAX = (unsigned int)INT_MAX;

    inline bool safe_add(unsigned int value, unsigned int to_add, unsigned int *result)
    {
        if (value > UVALUE_OF_INT_MAX || to_add > UVALUE_OF_INT_MAX)
            return false;

        *result = value + to_add;

        if (*result > UVALUE_OF_INT_MAX || (*result < value || *result < to_add))
            return false;

        return true;
    }

    inline bool safe_multiply(unsigned int value, unsigned int by, unsigned int *result)
    {
        if (value > UVALUE_OF_INT_MAX || by > UVALUE_OF_INT_MAX)
            return false;

        *result = value * by;

        if ((0 != by
            && 0 != value
            && 1 != by
            && 1 != value
            && (*result <= value
            || *result <= by))
            || *result > UVALUE_OF_INT_MAX)
                return false;

        return true;
    }
public:
    int reverse(int x)
    {
        if (x == INT_MIN)
            return 0;

        unsigned int ret = 0, d = 1;
        int tmp = x;
        int neg = 0 > x;

        tmp = neg ? ((~tmp) + 1) : tmp;
        x = neg ? ((~x) + 1) : x;

        while (10 <= tmp)
        {
            if (!safe_multiply(d, 10, &d))
                return 0;

            tmp = tmp / 10;
        }

        while (0 < x)
        {
            unsigned int next;
            unsigned int digit = (unsigned int)x % 10;
            
            if ((unsigned int)1000000000 == d && 2 < digit)
                return 0;
            
            if (!safe_multiply(digit, d, &next))
                return 0;

            if (!safe_add(ret, next, &ret))
                return 0;

            d /= 10;
            x /= 10;
        }

        if (neg)
            ret = (~ret)+1;

        return (int)ret;
    }
};
