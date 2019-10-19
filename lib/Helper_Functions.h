#include "Library.h"

using namespace std;

int Read_point_files(vector<vector<int>>*, vector<vector<int>>*, char*, char* );
int Read_curve_files(vector<vector<double*>>*, vector<vector<double*>>*, char*, char*);
double point_dist(double*, double*, int);
int dist(vector<int>*, vector<int>*, int, int=1);
void brute_force(vector<vector<int>>*, vector<vector<int>>*, vector<int>*, vector<double>*);
int* arg_min(double**, vector<int>*, int, int);