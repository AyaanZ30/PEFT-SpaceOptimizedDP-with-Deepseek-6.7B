# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <climits>

using namespace std;


// NAIVE 

// OPTIMIZED
// @explanation: Space-optimized 1D DP 
// @complexity: Time: O(n^2) , Space: O(n)
int hyper_optimized_approach(const vector<int> &arr){
    int n = arr.size();
    vector<int> dp_f(n, 1);
    
    // main logic (figuring out LIS from front traversal)
    for(int cur = 0 ; cur < n ; cur++){
        for(int prev = 0 ; prev < cur ; prev++){
            if(arr[cur] > arr[prev] && 1 + dp_f[prev] > dp_f[cur]){   // valid increasing
                dp_f[cur] = 1 + dp_f[prev];
            }
        }
    }

    vector<int> dp_b(n, 1);
    // main logic (figuring out LIS from rear traversal)
    for(int cur = n-1 ; cur >= 0 ; cur--){
        for(int prev = n-1 ; prev > cur ; prev--){
            if(arr[cur] > arr[prev] && 1 + dp_b[prev] > dp_b[cur]){   // valid increasing
                dp_b[cur] = 1 + dp_b[prev];
            }
        }
    }

    vector<int> bitonic(n, 0);
    int maxi = 1;
    for(int i = 0 ; i < n ; i++){
        bitonic[i] = (dp_f[i] + dp_b[i] - 1);   // -1 to avoid double counting of peak element
        maxi = max(maxi, bitonic[i]);
    }
    return maxi;
}

void printArray(const vector<int> &arr){
    cout << "Entered Array: ";
    for(auto x: arr){
        cout << x << " ";
    }
    cout << endl;
}
int main(){
    vector<int> arr = {1, 11, 2, 10, 4, 5, 2, 1}; // Example input
    cout << "Length of LBS : " << hyper_optimized_approach(arr) << endl;
    return 0;
}
