#include <map>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
#include <set>

using namespace std;

/*

  Make a single pass over the input vector to process them.
  
  Data structures used:
  
  Hashtable (std::unordered_map) [for mapping substrings to count]
  Hashset (std::set) [for keeping track of substrings with incremented counts]
  Priority queue [to order key -> value pairs by value in descending order]
    
  highest_cnt := 0
  highest_len := 0
  
  Algorithm:
  
  1. Get next string from vector
  2. For each substring of length 1 to length of string
  3. If length of substring is less than highest_len
    3.i Ignore substring and continue
  4. Else
    4.i Check for existence of given substring in hashtable
    4.ii If substring exists
      4.ii.a Increment its count in the table
      4.ii.b If count is greater than highest count seen and length is greater than the corresponding length
        4.ii.b.i Iterate hashset of previously entered substrings whose count was incremented, and remove from hashtable
        4.ii.b.ii Clear the hashset
      4.ii.c Add substring to hashset
    4.iii Else
      4.iii.a Add new entry with count 1 in the table
  5. If reached end of vector
    5.i Stop
  6. Else
    6.i Go to 1

 */

class Solution
{
    using iter_t = unordered_map<string,int>::iterator;
    using value_t = pair<string,int>;
    using container_t = vector<value_t>;
private:
    unordered_map<string,int> m;
public:
    string longestCommonPrefix(vector<string>& strs)
    {
        int highest_cnt = 0;
        size_t highest_len = 0;
        set<string> inserted;

        for (int i = 0, n = strs.size(); i < n; ++i)
        {
            const string s = strs[i];
            const int len = s.length();
            int p = 0, e = 1;

            while (e <= len)
            {
                if ((e - p) < highest_len)
                {
                    e++;
                    continue;
                }

                string t = s.substr(p, e);
                iter_t iter = m.find(t);

                if (iter != m.end())
                {
                    int count = iter->second;
                    if (++count > highest_cnt)
                    {
                        if (t.length() > highest_len)
                        {
                            highest_len = t.length();
                            highest_cnt = count;

                            for (const string& key : inserted)
                                m.erase(key);

                            inserted.clear();
                        }
                    }

                    m.insert_or_assign(t, count);
                    inserted.insert(t);
                }
                else
                {
                    m.insert(pair<string,int>(t, 1));
                }

                ++e;
            }
        }

        string result;
        int max_count = INT_MIN;
        size_t max_len = 0;

        /*
            Use a priority queue with custom comparator
            to store the elements in descending order
            (by the count).

            Otherwise, we can start with something with
            a count of 1 but a large length and then
            our condition of a given element having both
            a greater count and a greater length won't
            be satisfied and we won't return the correct
            result.
         */
        auto comp_t = [](const value_t& a, const value_t& b) {
            return a.second < b.second;
        };

        priority_queue<value_t, container_t, decltype(comp_t)> q {comp_t};

        iter_t iter = m.begin();
        while (iter != m.end())
        {
            q.push(*iter);
            ++iter;
        }

        for ( ; !q.empty(); q.pop())
        {
            const auto& elem = q.top();
            const string prefix = elem.first;
            const int cnt = elem.second;
            const size_t len = prefix.length();

            if (cnt >= max_count)
            {
                if (len > max_len)
                {
                    max_count = cnt;
                    max_len = prefix.length();
                    result = prefix;
                }
            }
        }

        if (1 == max_count)
            return "";

        return result;
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

    v.push_back("f3");
    v.push_back("f2");
    v.push_back("f1");
    v.push_back("flower");
    v.push_back("flow");
    v.push_back("flight");
    v.push_back("for");
    v.push_back("fo234");
    v.push_back("fo489");
    v.push_back("foig");

    test(v, "fo");

    return 0;
}
