# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <climits>

using namespace std;

bool checkPossible(const string &s1, const string &s2){
    if(s1.size() != s2.size() + 1){
        return false;
    }
    int first = 0;
    int second = 0;

    // s1 is progressive element (by 1 character) of s2 (bigger in length by 1)
    while(first < s1.size()){
        if(s1[first] == s2[second]){
            first++;
            second++;
        }else{
            first++;    // skip the extra character in s1   
        }
    }
    return (first == s1.size() && second == s2.size());
}

bool comp(const string &a, const string &b){
    return (a.size() < b.size());
}
// NAIVE 

// OPTIMIZED
// @explanation: Space-optimized 1D DP 
// @complexity: Time: O(n^2 * len(s)) , Space:  O(n) + O(nlogn) for sorting
int hyper_optimized_approach(vector<string> &arr){
    int n = arr.size();
    vector<int> dp(n, 1);
    int maxi = 1;
    
    // main logic (hash array to help in printing LIS sequence)
    sort(arr.begin(), arr.end(), comp);     // as sequence can start from any element (progressing by 1 character) and not just in chronological order

    for(int cur = 0 ; cur < n ; cur++){
        for(int prev = 0 ; prev < cur ; prev++){
            if(checkPossible(arr[cur], arr[prev])){   // valid increasing
                if(1 + dp[prev] > dp[cur]){
                    dp[cur] = 1 + dp[prev];
                }
            }
        }
        maxi = max(maxi, dp[cur]);
    }
    return maxi;
}

// PROMPT: Given a list of strings where each string can be formed by adding one character to another string in the list, implement a space-optimized dynamic programming approach to find the length of the longest possible string chain.
int main() {
    vector<string> arr = {"abc", "def", "ghi", "ab", "cdef", "gh"}; 

    cout << "Length of LSC : " << hyper_optimized_approach(arr) << endl;
    return 0;
}
