# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <climits>

using namespace std;

// if max 3 transactions allowed : B,S,B,S -> (T1,T2,T3,T4) (buy = even(T1, T3), sell = odd(T2,T4)) (1 tran == 1B & 1S)
// NAIVE (max k transactions allowed) (tran_No => current transaction in action)
int recurse(int idx, int tran_No, int k, const vector<int> &prices){
    // base cases 
    if(tran_No == 2*k || idx == prices.size()){
        return 0;
    }
    if(tran_No % 2 == 0){    // BUY (all even tranNo are always buy(check example above function))
        return max(-prices[idx] + recurse(idx+1, tran_No+1, k, prices), 
                0 + recurse(idx+1, tran_No, k, prices));
    }
    // SELL (if tranNo not even)
    return max(prices[idx] + recurse(idx+1, tran_No+1, k, prices),
                0 + recurse(idx+1, tran_No, k, prices));
}

int tabulation_approach(const vector<int> &prices, int k){
    int n = prices.size();
    vector<vector<int>> dp(n+1, vector<int>(2*k+1, 0));   // since if there are 2 trans(=k) allowed => 4 operations(B, S, B, S)

    for(int idx = n-1 ; idx >= 0 ; idx--){
        for(int tran_No = 2*k-1 ; tran_No >= 0 ; tran_No--){
            if((tran_No % 2) == 0){
                dp[idx][tran_No] = max(-prices[idx] + dp[idx+1][tran_No+1], 
                                        0 + dp[idx+1][tran_No]);
            }else{
                dp[idx][tran_No] = max(prices[idx] + dp[idx+1][tran_No+1], 
                                        0 + dp[idx+1][tran_No]); 
            }
        }
    }return dp[0][0];
}

// OPTIMIZED 
// @explanation: Space-optimized (2D -> 1D)  
// @complexity: Time: O(N*2*k) , Space:  O(4k + 2)
int hyper_optimized_approach(vector<int> &prices, int k){    // convert s1 -> s2 
    int n = prices.size();      
    vector<int> after(2*k + 1, 0);
    vector<int> curr(2*k + 1, 0);

    // core logic
    for(int idx = n-1 ; idx >= 0 ; idx--){
        for(int tran_No = 2*k-1 ; tran_No >= 0 ; tran_No--){
            if((tran_No % 2) == 0){
                curr[tran_No] = max(-prices[idx] + after[tran_No+1], 
                                    0 + after[tran_No]);
            }else{
                curr[tran_No] = max(prices[idx] + after[tran_No+1], 
                                    0 + after[tran_No]);
            }
        }
        after = curr;
    }
    return after[0];
}

// PROMPT: You are given at most k transactions and an array of prices. Implement a space-optimized dynamic programming solution to find the maximum achievable profit with at most k buy-sell operations.
int main(){
    int n = 6;
    int transaction_cap = 2;
    vector<int> stocks = {7,1,5,3,6,4};

    cout << "\nMax attainable profit : " << hyper_optimized_approach(stocks, transaction_cap) << endl;
}
