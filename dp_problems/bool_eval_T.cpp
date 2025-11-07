# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <climits>

using namespace std;


// NAIVE (TC : O(exponential) SC : O(n) auxiliary stack space)
int recursion_approach(int i, int j, int isTrue, const vector<char> &arr){
    // base cases
    if(i > j && isTrue == 0){         // if partitions are over
        return 0;
    }
    if(i == j){
        if(isTrue == 1){
            return (arr[i] == 'T') ? 1 : 0;
        }else{
            return (arr[i] == 'F') ? 1 : 0;
        }
    }
    int total_ways = 0;
    // main logic (since 'T^F&T|T' has both operators at alternate idx (last being j-1), we iterate over operators only as problem can be partitioned at that index only)
    for(int operator_idx = i+1 ; operator_idx <= j-1 ; operator_idx += 2){
        int leftTrue = recursion_approach(i, operator_idx - 1, 1, arr);    // no of ways left partition can be true
        int leftFalse = recursion_approach(i, operator_idx - 1, 0, arr);    // no of ways left partition can be false
        int rightTrue = recursion_approach(operator_idx + 1, j, 1, arr);    // no of ways right partition can be true
        int rightFalse = recursion_approach(operator_idx + 1, j, 0, arr);   // no of ways right partition can be false

        if(arr[operator_idx] == '&'){
            total_ways += (isTrue == 1) ? ((leftTrue * rightTrue)) : ((leftTrue * rightFalse) + (leftFalse * rightTrue) + (leftFalse * rightFalse));     // for AND : (only T & T = T)
        }else if(arr[operator_idx] == '|'){
            total_ways += (isTrue == 1) ? ((leftTrue * rightTrue) + (leftTrue * rightFalse) + (leftFalse * rightTrue)) : ((leftFalse * rightFalse));   // for OR : (T | T = T), (T | F = T), (F | T = T)
        }else{
            total_ways += (isTrue == 1) ? ((leftTrue * rightFalse) + (leftFalse * rightTrue)) : ((leftTrue * rightTrue) + (leftFalse * rightFalse));   // for XOR : (T ^ F = T), (F ^ T = T)
        }
    }
    return total_ways;
}

// OPTIMIZED
// @explanation: Space-optimized 3D DP 
// (BOTTOM-UP approach)
int hyper_optimized_approach(vector<char> &arr){
    int n = arr.size();
    vector<vector<vector<int>>> dp(n, vector<vector<int>>(n, vector<int>(2, 0)));

    // base cases analogy (check recursion)
    for(int i = 0 ; i < n ; i += 2){
        dp[i][i][1] = (arr[i] == 'T') ? 1 : 0;
        dp[i][i][0] = (arr[i] == 'F') ? 1 : 0;
    }
    // main logic (always loop in oppposite fashion as compared to recursion as bottom up)
    for(int i = n - 1 ; i >= 0 ; i -= 2){       // i moves only on operands
        for(int j = i + 2 ; j < n ; j += 2){   // j moves only on operands
            for(int isTrue = 0 ; isTrue <= 1 ; isTrue++){
                if(i > j){continue;}
                int total_ways = 0;

                for(int operator_idx = i + 1 ; operator_idx <= j - 1 ; operator_idx += 2){
                    int leftTrue = dp[i][operator_idx - 1][1];    // no of ways left partition can be true
                    int leftFalse = dp[i][operator_idx - 1][0];   // no of ways left partition can be false
                    int rightTrue = dp[operator_idx + 1][j][1];   // no of ways right partition can be true
                    int rightFalse = dp[operator_idx + 1][j][0];  // no of ways right partition can be false

                    if(arr[operator_idx] == '&'){
                        total_ways += (isTrue == 1) ? ((leftTrue * rightTrue)) : ((leftTrue * rightFalse) + (leftFalse * rightTrue) + (leftFalse * rightFalse));     // for AND : (only T & T = T)
                    }else if(arr[operator_idx] == '|'){
                        total_ways += (isTrue == 1) ? ((leftTrue * rightTrue) + (leftTrue * rightFalse) + (leftFalse * rightTrue)) : ((leftFalse * rightFalse));   // for OR : (T | T = T), (T | F = T), (F | T = T)
                    }else{
                        total_ways += (isTrue == 1) ? ((leftTrue * rightFalse) + (leftFalse * rightTrue)) : ((leftTrue * rightTrue) + (leftFalse * rightFalse));   // for XOR : (T ^ F = T), (F ^ T = T)
                    }
                }
                dp[i][j][isTrue] = total_ways;
            }
        }
    }
    return dp[0][n-1][1];
}

void printArray(const vector<char> &arr){
    cout << "Entered Array: ";
    for(auto x: arr){
        cout << x << " ";
    }
    cout << endl;
}
int main(){
    int n = 5;
    vector<char> arr = {'T', '|', 'F', '&', 'T'};

    cout << "T can be obtained in : " << hyper_optimized_approach(arr) <<" ways." << endl;
    return 0;
}
