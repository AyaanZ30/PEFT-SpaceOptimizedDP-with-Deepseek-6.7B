# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <climits>

using namespace std;

// NAIVE 

// OPTIMIZED 
// @explanation: Space-optimized 1D DP 
// @complexity: Time: O(N) , Space:  O(1)
int hyper_optimized_approach(vector<int> &prices){    // convert s1 -> s2 
    int maxProfit = 0;
    int mini = prices[0];

    for(int i = 0 ; i < prices.size() ; i++){
        int current_profit = (prices[i] - mini);
        maxProfit = max(current_profit, maxProfit);
        mini = min(prices[i], mini);
    }
    return maxProfit;
}

// PROMPT: Given an array where each element represents the stock price on a given day, implement a space-optimized dynamic programming approach to find the maximum profit you can achieve from a single buy and a single sell operation.
int main(){
    int n = 6;
    vector<int> stocks = {7,1,5,3,6,4};

    cout << "\nMax attainable profit : " << hyper_optimized_approach(stocks) << endl;
}
