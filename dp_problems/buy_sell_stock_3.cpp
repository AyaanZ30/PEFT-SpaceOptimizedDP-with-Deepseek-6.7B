# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <climits>

using namespace std;

// NAIVE
int recursion_approach(int idx, int buy, int capacity, const vector<int> &prices){
    long maxProfit = 0;
    // base case
    if(capacity == 0 || idx == prices.size()){
        return 0;
    }
    // if buy(=1) you still have librety to buy (can/cannot), else you have to sell the stock you previously bought first before buying another
    if(buy){
        int ahead_not_buy = recursion_approach(idx+1, 0, capacity, prices);  
        int ahead_buy = recursion_approach(idx+1, 1, capacity, prices);  
        maxProfit =  max(-prices[idx] + ahead_not_buy, 0 + ahead_buy);
    }else{
        int ahead_can_buy = recursion_approach(idx+1, 1, capacity-1, prices);    // if prev bought stock is sold (which means one full transaction is completed) -> reduce the capacity by 1 (as max 2 transacitons allowed)
        int ahead_must_sell = recursion_approach(idx+1, 0, capacity, prices);    // you cannot buy next without selling prev
        maxProfit = max(prices[idx] + ahead_can_buy, 0 + ahead_must_sell);
    }
    cout << "TC : O(2^N)" << endl;
    cout << "SC : O(N) (auxiliary stack)" << endl;
    return maxProfit;
}

int tabulation_approach(const vector<int> &prices, int capacity){
    int n = prices.size();
    int buy_options = 2;      // either 0/1 (not buy/buy)

    // since limited no of transactions(B+S) are 2 (can be 0, 1, 2) we will have a 3D DP(as there are 3 changing params(idx, buy, capacity))
    // declare 3d dp based on 3 changing params dp[n+1][2(0/1)][3(0/1/2)]
    vector<vector<vector<int>>> dp(n+1, vector<vector<int>>(buy_options, vector<int>(3, 0)));

    // 1st base case (when capacity = 0) (analogous to recusive base case)
    for(int idx = 0 ; idx < n ; idx++){
        for(int buy = 0 ; buy < buy_options ; buy++){
            dp[idx][buy][0] = 0;                 
        }
    }
    // 2nd base case (when idx = n)  (analogous to recusive base case)
    for(int buy = 0 ; buy < buy_options ; buy++){
        for(int cap = 0 ; cap < capacity ; cap++){
            dp[n][buy][cap] = 0;
        }
    }

    // core logic
    for(int idx = n-1 ; idx >= 0 ; idx--){
        for(int buy = 0 ; buy < buy_options ; buy++){
            for(int cap = 1 ; cap <= capacity ; cap++){
                if(buy){
                    int ahead_sell = dp[idx+1][0][cap];   // if decided to buy
                    int ahead_buy = dp[idx+1][1][cap];    // if decided to not buy (and look ahead)
                    dp[idx][buy][cap] = max(-prices[idx] + ahead_sell, 0 + ahead_buy);
                }else{
                    int ahead_can_buy = dp[idx+1][1][cap-1];
                    int ahead_must_sell = dp[idx+1][0][cap]; 
                    dp[idx][buy][cap] = max(prices[idx] + ahead_can_buy, 0 + ahead_must_sell);
                }
            }
        }
    }
    cout << "\nTC : O(N*2*3)" << endl;
    cout << "SC : O(N*2*3) (no auxiliary stack by avoiding recursion)" << endl;
    return dp[0][1][2];
}

// OPTIMIZED 
// @explanation: Space-optimized (3D -> 2D)  
// @complexity: Time: O(N*2*3) , Space:  O(const)
int hyper_optimized_approach(const vector<int> &prices, int capacity){    // convert s1 -> s2 
    int n = prices.size();
    int buy_options = 2;      

    vector<vector<int>> after(buy_options, vector<int>(3, 0));
    vector<vector<int>> curr(buy_options, vector<int>(3, 0));

    // core logic
    for(int idx = n-1 ; idx >= 0 ; idx--){
        for(int buy = 0 ; buy < buy_options ; buy++){
            for(int cap = 1 ; cap <= capacity ; cap++){
                if(buy){
                    int ahead_sell = after[0][cap];   // if decided to buy
                    int ahead_buy = after[1][cap];    // if decided to not buy (and look ahead)
                    curr[buy][cap] = max(-prices[idx] + ahead_sell, 0 + ahead_buy);
                }else{
                    int ahead_can_buy = after[1][cap-1];
                    int ahead_must_sell = after[0][cap]; 
                    curr[buy][cap] = max(prices[idx] + ahead_can_buy, 0 + ahead_must_sell);
                }
            }
        }
        after = curr;
    }
    return after[1][2];
}

int main(){
    int n = 6;
    int transaction_cap = 2;
    vector<int> stocks = {7,1,5,3,6,4};

    cout << "\nMax attainable profit : " << hyper_optimized_approach(stocks, transaction_cap) << endl;
}
