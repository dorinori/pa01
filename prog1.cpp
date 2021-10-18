#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <random>
using namespace std::chrono;
using namespace std;

int aboveLine(vector<double> p, vector<double> p1, vector<double> p2){
    double slope = (p1[1] - p2[1]) / (p1[0]-p2[0]);
    double intercept = p1[1] - slope*p1[0];

    if (p[1] > (p[0]*slope + intercept)){
        return 1;
    }
    else if (p[1] == (p[0]*slope + intercept)){
        return 0;
    }
    else{
        return -1;
    }
}

vector<vector<double>> merge(vector<vector<double>> left_hull, vector<vector<double>> right_hull){
    //rightmost point in left_hull
    int l = left_hull.size();
    int r = right_hull.size(); 
    int rp = 0; //index of rightmost point
    for (int i = 0; i < l; i++){
        if (left_hull[i][0] > left_hull[rp][0] || ((left_hull[i][0] == left_hull[rp][0]) && (left_hull[i][1] < left_hull[rp][1]))){ //x is larger = more right
            rp = i;
        }
    }
    //leftmost point in right_hull
    int lp = 0; //index of leftmostpoint
    for (int i = 0; i < r; i++){
        if (right_hull[i][0] < right_hull[lp][0] || ((right_hull[i][0] == right_hull[lp][0]) && (right_hull[i][1] < right_hull[lp][1])) ){
            lp = i;
        }
    }

    int temp1 = rp;
    int temp2 = lp;

    //assume stored c-cw and smallest y first
    // find upper tangent
    while(true){
        if(aboveLine(left_hull[(rp + 1) % l], left_hull[rp], right_hull[lp]) >= 0 && (left_hull.size() > 1)){ //c-cw of rp lies above rp-lp{
            //set rp to c-cw rp
            rp = (rp + 1) % l;
        }
        else if(aboveLine(right_hull[(r + lp - 1) % r], left_hull[rp], right_hull[lp]) >= 0 && (right_hull.size() > 1)){ //cw of lp lies above rp-lp{
            //set lp to cw-lp
            lp = (r + lp - 1) % r;
        }
        else{
            break;
        }
    }

    int upper_left = rp;
    int upper_right = lp;
    rp = temp1; 
    lp = temp2;

    //find lower tangent
    while(true){
        if(aboveLine(left_hull[(l + rp - 1) % l], left_hull[rp], right_hull[lp]) <= 0 && (left_hull.size() > 1)){ //cw of rp lies below rp-lp{
            //set rp to cw rp
            rp = (l + rp - 1) % l;
        }
        else if(aboveLine(right_hull[(lp + 1) % r], left_hull[rp], right_hull[lp]) <= 0 && (right_hull.size() > 1)){ //c-cw of lp lies below rp-lp{
            //set lp to c-cw lp
            lp = (lp + 1) % r;
        }
        else{
            break;
        }
    }

    int lower_left = rp;
    int lower_right = lp;
    int index = lower_right;
    vector<vector<double>> merged;
    //push lower right to vector, circle around
    merged.push_back(right_hull[lower_right]);

    //c-cw rotation through right hull
    while(index != upper_right){
        index = (index + 1) % r;
        merged.push_back(right_hull[index]);
    }

    //c-cw rotattion through left hull
    merged.push_back(left_hull[upper_left]);
    index = upper_left;
    while(index != lower_left){
        index = (index + 1) % l;
        merged.push_back(left_hull[index]);
    }
    //compare lower right with lower left
    if(left_hull[lower_left][1] < right_hull[lower_right][1] || (left_hull[lower_left][1] == right_hull[lower_right][1] && left_hull[lower_left][0] < right_hull[lower_right][0])){
        merged.pop_back();
        merged.insert(merged.begin(), left_hull[lower_left]);
    }

    return merged;
}


vector<vector<double>> convexHull(vector<vector<double>> points){
    if(points.size() == 1){
        return points;
    }

    vector<vector<double>>left, right;
    for (int i = 0; i < points.size()/2; i++){
        left.push_back(points[i]);
    }
    for (int i = points.size()/2; i < points.size(); i++)
        right.push_back(points[i]);

    vector<vector<double>>left_side =  convexHull(left);
    vector<vector<double>>right_side =  convexHull(right);
    // merging the convex hulls
    return merge(left_side, right_side);
    // return points;
}

int main(){
    auto start = high_resolution_clock::now();
    
    //take in index and coordinate inputs
    vector<vector<double>> points;
    string input;
    cin >> input;
    int num = stoi(input);
    for(int i = 0; i < num; i++){
        double x , y;
        cin >> input;
        input.pop_back();
        x = stod(input);
        cin >> input;
        y = stod(input);
        vector<double> coord;
        coord.push_back(x);
        coord.push_back(y);
        coord.push_back(i);
        points.push_back(coord);
    }

    // cout << points.size() << endl;
    // for (int i = 0; i < points.size(); i++){
    //     cout << points[i][2] << ", " << points[i][1] << endl;
    // }

    sort(points.begin(), points.end());

    vector<vector<double>> convex = convexHull(points);
    cout << convex.size() << endl;
    for (int i = 0; i < convex.size(); i++){
        cout << convex[i][2] << ", " << convex[i][0] << ", " << convex[i][1] << endl;
    }

    // auto stop = high_resolution_clock::now();

    // auto duration = duration_cast<microseconds>(stop - start);
    // cout << duration.count() << endl;

    // cout << "1000000" << endl;
    // for (int i = 0; i < 1000000; i++){
    //     double x = static_cast<double>(rand() % 99999 + 1) / (double)100000; 
    //     double y = static_cast<double>(rand() % 99999 + 1) / (double)100000;  
    //     cout << x << ", " << y << endl;
    // }   

    return 0;
}