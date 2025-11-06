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
int hyper_optimized_approach(const vector<int> &arr){
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

int main(){
    int n;
    cout << "Enter number of elements: ";
    cin >> n;
    vector<int> arr(n);

    for(int i = 0 ; i < n ; i++){
        cout << "Element " << i + 1 << " : ";
        cin >> arr[i];
    }

    vector<vector<int>> dp(n, vector<int>(n + 1, -1));

    cout << "Length of LIS : " << hyper_optimized_approach(arr) << endl;
    return 0;
}