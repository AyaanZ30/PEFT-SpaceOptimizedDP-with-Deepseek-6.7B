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
    if(idx == prices.size()){
        return 0;
    }
    // if buy(=1) you still have librety to buy (can/cannot), else you have to sell the stock you previously bought first before buying another
    if(buy){
        int ahead_not_buy = recursion_approach(idx+1, 0, prices);  
        int ahead_buy = recursion_approach(idx+1, 1, prices);  
        maxProfit =  max(-prices[idx] + ahead_not_buy, 0 + ahead_buy);
    }else{
        int ahead_can_buy = recursion_approach(idx+1, 1, prices);    // only if you have sold prev, you can buy next
        int ahead_must_sell = recursion_approach(idx+1, 0, prices);    // you cannot buy next without selling prev
        maxProfit = max(prices[idx] + ahead_can_buy, 0 + ahead_must_sell);
    }
    cout << "TC : O(2^N)" << endl;
    cout << "SC : O(N) (auxiliary stack)" << endl;
    return maxProfit;
}

int tabulation_approach(const vector<int> &prices){
    int n = prices.size();
    vector<vector<int>> dp(n+1, vector<int>(2, 0));

    // analogous to recursion base cases (idx == n)
    dp[n][0] = dp[n][1] = 0;
    for(int idx = n-1 ; idx >= 0 ; idx--){
        for(int buy = 0 ; buy <= 1 ; buy++){    // buy(1) or not buy(0)
            if(buy){
                dp[idx][buy] = max(-prices[idx] + dp[idx+1][0], 0 + dp[idx+1][1]);
            }else{
                dp[idx][buy] = max(prices[idx] + dp[idx+1][1], 0 + dp[idx+1][0]);
            }
        }
    }
    return dp[0][1];    
}

// OPTIMIZED 
// @explanation: Space-optimized 1D DP 
// @complexity: Time: O(N) , Space:  O(1)
int hyper_optimized_approach(vector<int> &prices){    // convert s1 -> s2 
    int n = prices.size();
    vector<int> ahead(2, 0), curr(2, 0); // since for computing current state (idx) we only ahead values (idx+1) (refer tabulation logic)

    ahead[0] = ahead[1] = 0;
    for(int idx = n-1 ; idx >= 0 ; idx--){
        for(int buy = 0 ; buy <= 1 ; buy++){    // buy(1) or not buy(0)
            if(buy){
                curr[buy] = max(-prices[idx] + ahead[0], 0 + ahead[1]);
            }else{
                curr[buy] = max(prices[idx] + ahead[1], 0 + ahead[0]);
            }
        }
        ahead = curr;
    }
    return ahead[1];
}

// PROMPT: You are allowed to complete as many transactions as you like, but you must sell the stock before you buy again. Implement a space-optimized dynamic programming solution to maximize the total profit.
int main(){
    int n = 6;
    vector<int> stocks = {7,1,5,3,6,4};

    cout << "\nMax attainable profit : " << hyper_optimized_approach(stocks) << endl;
}
