# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <climits>

using namespace std;

// NAIVE
int recursion_approach(int idx, int buy, const vector<int> &prices){
    long maxProfit = 0;
    // base case
    if(idx >= prices.size()){
        return 0;
    }
    // if buy(=1) you still have librety to buy (can/cannot), else you have to sell the stock you previously bought first before buying another
    if(buy){ 
        maxProfit =  max(-prices[idx] + recursion_approach(idx+1, 0, prices), 
                        0 + recursion_approach(idx+1, 1, prices));
    }else{
        maxProfit = max(prices[idx] + recursion_approach(idx+2, 1, prices), 
                        0 + recursion_approach(idx+1, 0, prices));
    }
    return maxProfit;
    cout << "\nTC : O(2^N)" << endl;
    cout << "SC : O(N) (auxiliary stack)" << endl;
}

int tabulation_approach(const vector<int> &prices){
    int n = prices.size();
    vector<vector<int>> dp(n+2, vector<int>(2, 0));

    // analogous to recursion base cases (idx == n)
    dp[n][0] = dp[n][1] = 0;
    for(int idx = n-1 ; idx >= 0 ; idx--){
        for(int buy = 0 ; buy <= 1 ; buy++){    // buy(1) or not buy(0)
            if(buy){
                dp[idx][buy] = max(-prices[idx] + dp[idx+1][0], 0 + dp[idx+1][1]);
            }else{
                dp[idx][buy] = max(prices[idx] + dp[idx+2][1], 0 + dp[idx+1][0]);
            }
        }
    }
    return dp[0][1];    
}

// OPTIMIZED 
// @explanation: Space-optimized 2D DP 
// @complexity: Time: O(N) , Space:  O(N*2)
int hyper_optimized_approach(vector<int> &prices){    // convert s1 -> s2 
    int n = prices.size();
    vector<vector<int>> dp(n+2, vector<int>(2, 0));

    // analogous to recursion base cases (idx == n)
    dp[n][0] = dp[n][1] = 0;
    for(int idx = n-1 ; idx >= 0 ; idx--){
        // buy(1) or not buy(0)
        dp[idx][1] = max(-prices[idx] + dp[idx+1][0], 0 + dp[idx+1][1]);
        dp[idx][0] = max(prices[idx] + dp[idx+2][1], 0 + dp[idx+1][0]);
    }
    return dp[0][1];
}

int main(){
    int n = 5;
    vector<int> stocks = {1,2,3,0,2};

    cout << "\nMax attainable profit : " << hyper_optimized_approach(stocks) << endl;
}
