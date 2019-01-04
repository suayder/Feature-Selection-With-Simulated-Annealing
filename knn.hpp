#ifndef KNN
#define KNN

#include <bitset>
#include <vector>
#include <math.h>
#include <fstream>

#define PATH "breastCancer.data"
#define INSTANCES 699
#define ATTRIBUTES 10
#define K 5

double evaluate(std::bitset<ATTRIBUTES-1>& selected_features, int **data);
int predict(int *instance, int **data, std::bitset<ATTRIBUTES-1>& selected_features); //Return error rate
double euclideanDistance(int* v1, int* v2, std::bitset<ATTRIBUTES-1>& selected_features);
std::vector<std::string> split(const std::string& s, char delimiter);

int** readData(std::string path, unsigned int n_attribute, unsigned int n_instance);

#endif