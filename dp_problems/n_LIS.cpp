# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <climits>

using namespace std;


// NAIVE 

// OPTIMIZED
// @explanation: Space-optimized 1D DP 
// @complexity: Time: O(n^2) , Space:  O(n)
int hyper_optimized_approach(const vector<int> &arr){
    int n = arr.size();
    vector<int> dp(n, 1);
    vector<int> count(n, 1);
    int maxi = 1;
    
    // main logic (hash array to help in printing LIS sequence)
    for(int cur = 0 ; cur < n ; cur++){
        for(int prev = 0 ; prev < cur ; prev++){
            if(arr[cur] > arr[prev] && 1 + dp[prev] > dp[cur]){   // valid increasing
                dp[cur] = 1 + dp[prev];
                count[cur] = count[prev];               // inherit count
            }else if(arr[cur] > arr[prev] && 1 + dp[prev] == dp[cur]){   //(when duplicate value of LIS length found in dp)
                count[cur] += count[prev];              // add count
            }
        }
        maxi = max(maxi, dp[cur]);
    }
    int totalCount = 0;
    for(int i = 0 ; i < n ; i++){
        if(dp[i] == maxi){
            totalCount += count[i];
        }
    }
    return totalCount;
}

void printArray(const vector<int> &arr){
    cout << "Entered Array: ";
    for(auto x: arr){
        cout << x << " ";
    }
    cout << endl;
}
int main() {
    vector<int> arr = {1, 3, 5, 4, 7};   

    cout << "N LIS : " << hyper_optimized_approach(arr) << endl;
    return 0;
}
