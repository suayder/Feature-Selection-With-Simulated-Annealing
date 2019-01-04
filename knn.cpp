#include <iostream>
#include <sstream>
#include <algorithm>
#include <memory>
#include "knn.hpp"

using namespace std;

vector<string> split(const string& s, char delimiter)
{
   vector<string> tokens;
   string token;
   istringstream tokenStream(s);
   while (getline(tokenStream, token, delimiter)){
      tokens.push_back(token);
   }
   tokens.erase(tokens.begin());
   return tokens;
}

int** readData(string path, unsigned int n_attribute, unsigned int n_instance){

    int **data;
    data = (int**) malloc(n_instance*sizeof(int*));
    for (unsigned int i = 0; i<n_instance;i++){
        data[i] = (int*) malloc(n_attribute*sizeof(int));
    }
    ifstream file(path);
    
    string line;
    vector<string> tokens;
    unsigned int c_instance = 0, c_attribute;
    if(file.is_open()){
        while(getline(file, line)){
            tokens = split(line, ',');
            c_attribute = 0;
            for (auto i:tokens){
                try{
                    data[c_instance][c_attribute] = stoi(i);
                }
                catch (exception e){
                    data[c_instance][c_attribute] = -1;
                }
                c_attribute++;
                //cout<<data[c_instance][c_attribute];
            }
            c_instance++;
        }
        file.close();
    }
    else{
        cout << "Something Wrong on reading file"<<endl;
    }
    
    return data;
}

double evaluate(bitset<ATTRIBUTES-1>& selected_features, int **data){
    int hits = 0;
    for (int i = 0; i<100;i++)
        if(predict(data[i],data, selected_features) == data[i][ATTRIBUTES-1])
            hits++;
    return (100-hits)/100.0;
}
struct _data{
    double distance;
    int classe;
};

bool compare_data(_data a, _data b){
    return (a.distance<b.distance);
}

int predict(int *instance, int **data, bitset<ATTRIBUTES-1>& selected_features){
    vector<_data> distance;
    for(int i = 100;i<INSTANCES;i++){
        _data a;
        a.distance = euclideanDistance(instance, data[i], selected_features);
        a.classe = data[i][ATTRIBUTES-1];
        distance.push_back(a);
    }
    sort(distance.begin(), distance.end(), compare_data);
    int c_2 = 0;
    int c_4 = 0;
    for (int i = 0; i<K; i++){
        if(distance[i].classe == 2){
            c_2++;
        }
        else{
            c_4++;
        }
    }
    //cout <<"\n2: "<<c_2<<" 4: "<<c_4;
    return (c_2>c_4)?2:4;
}

double euclideanDistance(int* v1, int* v2, bitset<ATTRIBUTES-1>& selected_features){
    int sfetures_size = selected_features.size();
    double sum = 0.0;
    for (int i =0;i<sfetures_size;i++){
        if(selected_features[i] == 1){
            sum +=(double)pow((v2[i]-v1[i]),2);
        }
    }
    //cout<< "v1: "<<v1[i] <<" v2: "<<v2[i]<<" sum: "<<sum<<" i: "<<i<<endl;
    //cout<<(v2[i]-v1[i])<<endl;
    //cout <<"\n\n\n sum: "<< sum<<endl;
    return sqrt(sum);
}