# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <climits>

using namespace std;

// NAIVE

// OPTIMIZED (using Binary Search concept)
// @explanation: Space-optimized 1D (replicating fake LIS space as only goal is to get length of LIS) 
// @complexity: Time: O(n * log(n)) , Space:  O(n)
int hyper_optimized_approach(vector<int> &arr){
    vector<int> temp;
    temp.push_back(arr[0]);
    int len = 1;
    int n = arr.size();

    for(int i = 1 ; i < n ; i++){
        if(arr[i] > temp.back()){
            temp.push_back(arr[i]);
            len++;
        }
        else{
            // find the index of the smallest number >= arr[i]
            int ind = lower_bound(temp.begin(), temp.end(), arr[i]) - temp.begin();
            temp[ind] = arr[i];   // replace that number with arr[i]
        }
    }
    return len;               // length of LIS is size of temp vector
}

// PROMPT: Given an integer array arr, implement a space-optimized approach using binary search to find the length of the longest strictly increasing subsequence in O(n log n) time.
int main() {
    vector<int> arr = {10, 9, 2, 5, 3, 7, 101, 18};

    cout << "Length of LIS : " << hyper_optimized_approach(arr) << endl;
    return 0;
}