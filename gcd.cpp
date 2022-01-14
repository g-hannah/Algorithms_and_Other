#include <iostream>
#include <functional>

/*
    C++ implementation of the Euclidian Algorithm, used
    to find the greatest common divisor of two integers,
    using a recursive solution.


    The greatest common divisor, 'd', of a and b is also
    the greatest common divisor of b and (a % b):

    if d | a and d | b, then d | (a % b)

    gcd(a, b) == gcd(b, (a % b))

    gcd(b, (a % b)) == gcd((a % b), b % (a % b))

    gcd((a % b), b % (a % b)) == gcd(b % (a % b), (a % b) % (b % (a % b)))

    .
    .
    .

    In essence, we continually reduce the integers down
    like this, taking the 'b' of the pair, and taking the
    remainder of a / b, until we reach a point at which
    b | a. When we reach that point, the remainder is 0,
    and therefore the dividend, b, is itself the greatest
    common divisor.


    Algorithm:

    1. r := a % b
    2. if r is 0, return abs(b)
    3. a := b
    4. b := r
    5. go to 1 (recursively call gcd(b, r))


    Example:

    gcd(29, 17):

    a = 29, b = 17
    r = 29 % 17 == 12
    r is not 0, continue

    a = 17, b = 12
    r = 17 % 12 == 5
    r is not 0, continue

    a = 12, b = 5
    r = 12 % 5 == 2
    r is not 0, continue

    a = 5, b = 2
    r = 5 % 2 == 1
    r is not 0, continue

    a = 2, b = 1
    r = 2 % 1 == 0
    r is 0, the result is the final divisor, 1
 */

 /**
  * Using C++11 lambda function
  */
static std::function<int(int,int)> gcd = [](int a, int b){

    if (0 == b)
        return 0;

    if (0 == a)
        return b;

    const int r = a % b;

    if (0 == r)
        return abs(b);

    return gcd(b, r);
};

/**
 * C++11 lamda function for testing results
 *
 * The above gcd lambda must be captured in this scope
 */
static std::function<void(int,int,int)> gcd_tester = [gcd](int a, int b, int expected){

    const int result = gcd(a, b);

    std::cerr << "GCD(" << a << ", " << b << ") == " << expected << ": ";
    if (result == expected)
        std::cerr << ": PASSED" << std::endl;
    else
        std::cerr << ": FAILED (Got result " << result << ")" << std::endl;
};

int
main(void)
{
    gcd_tester(16, 10, 2);
    gcd_tester(17, 11, 1);
    gcd_tester(461952, 116298, 18);
    gcd_tester(7966496, 314080416, 32);
    gcd_tester(24826148, 45296490, 526);
    gcd_tester(12, 0, 0);
    gcd_tester(0, 0, 0);
    gcd_tester(0, 9, 9);
    gcd_tester(-17, 11, 1);
    gcd_tester(16, -10, 2);
    gcd_tester(-16, -10, 2);


    /*

    Output from above:


    GCD(16, 10) == 2: : PASSED
    GCD(17, 11) == 1: : PASSED
    GCD(461952, 116298) == 18: : PASSED
    GCD(7966496, 314080416) == 32: : PASSED
    GCD(24826148, 45296490) == 526: : PASSED
    GCD(12, 0) == 0: : PASSED
    GCD(0, 0) == 0: : PASSED
    GCD(0, 9) == 9: : PASSED
    GCD(-17, 11) == 1: : PASSED
    GCD(16, -10) == 2: : PASSED
    GCD(-16, -10) == 2: : PASSED

    */

    return 0;
}
