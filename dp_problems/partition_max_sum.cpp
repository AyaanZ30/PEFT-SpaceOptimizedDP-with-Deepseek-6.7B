# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <climits>

using namespace std;

bool isPalidrome(string s){
    string ps = s;
    reverse(ps.begin(), ps.end());
    return (ps == s);
}

// NAIVE (TC : O(exponential) SC : O(n) auxiliary stack space)
int recursion_approach(int i, int k, const vector<int> &arr){    // front partitioning
    // base case
    if(i == arr.size()){
        return 0;
    }
    int maxAns = INT_MIN;
    int maxi = INT_MIN;
    int len = 0;
    for(int j = i ; j < (i + k) && j < arr.size() ; j++){
        len++;
        maxi = max(maxi, arr[j]);
        // finding max locally for a partition (ex : [1 15 7] -> max = 15 -> [15 15 15] -> sum = 15(maxi) * 3(len) = 45)
        int sum = (maxi * len) + recursion_approach(j + 1, k, arr);     // recurse for next partition (independent)
        maxAns = max(maxAns, sum);
    }
    return maxAns;
}

// OPTIMIZED
// @explanation: Space-optimized 1D DP 
// (BOTTOM-UP approach) (TC : O(n*k), SC : O(n))
int hyper_optimized_approach(int k, const vector<int> &arr){
    int n = arr.size();
    vector<int> dp(n + 1, 0);  // dp[i] -> max sum for subarray arr[0...i-1]

    dp[n] = 0; // base case

    for(int i = n-1 ; i >= 0 ; i--){
        int maxAns = INT_MIN;
        int maxi = INT_MIN;
        int len = 0;
        for(int j = i ; j < (i + k) && j < n ; j++){
            len++;
            maxi = max(maxi, arr[j]);
            int sum = (maxi * len) + dp[j + 1]; // dp[j + 1] -> max sum for subarray arr[0...j]
            maxAns = max(maxAns, sum);
        }
        dp[i] = maxAns;
    }
    return dp[0];
}

int main(){
    int n, k;
    cout << "Enter number of elements : ";
    cin >> n;
    cout << "Enter max partition size : ";
    cin >> k;

    vector<int> arr(n);
    cout << "Enter elements : ";
    for(int i = 0 ; i < n ; i++){
        cin >> arr[i];
    }

    cout << "Max sum : " << hyper_optimized_approach(k, arr) << endl;
    return 0;
}