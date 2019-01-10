#include <time.h>
#include<iostream>
#include "knn.hpp"

using namespace std;

typedef struct sol{
    double fitness;
    vector<bool> attributes;

    sol(double f, vector<bool> att){
        fitness = f;
        attributes = att;
    }
} _sol;

auto fitness = [](double classifier_error, int cardinality, int features, double alpha){
        return (alpha*classifier_error)+(1.0-alpha)*(cardinality/features);
    };

auto count_bits = [](vector<bool>& v){
        int cont = 0;
        for(auto it:v)
            if(it==true)
                cont++;

        return cont;
    };

_sol generate_new(unsigned int , Knn<>&);
_sol generate_near(int , vector<bool>&, Knn<>&);

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    if(argc!=2){
        cout<<"run like this: \' ./a.out dataset_name \'"<<endl;
        exit(0);
    }
    T_data data(argv[1]);

    Knn<> knn_classifier(&data);

    double t0 = 2*(data.n_attribute),t = 3000*t0; //temperature

    _sol si = generate_new(data.n_attribute, knn_classifier);
    _sol best_sol = si;//best_sol(fitness(evaluate(selected_features, data), selected_features.count(), ATTRIBUTES-1, 0.99),selected_features);

    cout << "ERROR RATE: " << knn_classifier.evaluate(best_sol.attributes)<<endl;
    cout << "SELECTED ATT: " << best_sol.fitness<<endl;
    for(auto it = best_sol.attributes.begin(); it != best_sol.attributes.end(); it++)
        cout << *it;
    cout<<endl;

    int count_iter = 0;
    ofstream file_save("result.dat");
    while(t>t0){

        file_save<<si.fitness<<" "<<count_iter<<endl;
        _sol trial_solution = generate_near(data.n_attribute, si.attributes, knn_classifier); //Generate in the neighbor
        if(trial_solution.fitness<best_sol.fitness){
            si = trial_solution;
            best_sol = trial_solution;
        }
        else if(trial_solution.fitness==best_sol.fitness){
            if(count_bits(trial_solution.attributes)<count_bits(trial_solution.attributes)){
                si = trial_solution;
                best_sol = trial_solution;
            }
        }
        else{
            double theta = trial_solution.fitness-best_sol.fitness;
            double p= (rand()%100)/(100.0);
            if(p<=exp((-theta)/t)){
                si = trial_solution;
            }
        }
        t=0.97*t; //update temperature
        count_iter++;
    }
    cout << "ERROR RATE: " << knn_classifier.evaluate(best_sol.attributes)<<endl;
    cout << "BEST SOL: " << best_sol.fitness<<endl;
    cout << "SELECTED ATT: ";
    for(auto it = best_sol.attributes.begin(); it != best_sol.attributes.end(); it++)
        cout << *it;
    cout<<endl;


    file_save.close();
    system ("gnuplot -p chart.gnu");
    return 0;
}

_sol generate_new(unsigned int n_attribute, Knn<>& knn_classifier){
    
    vector<bool> selected_features;
    do{ //ensure that vector generated will not have all values equals to zero
        for(unsigned int i =0; i<n_attribute; i++) //Initialize a random features selected
            selected_features.push_back((rand()%10<5)?0:1);
    }while(count_bits(selected_features)==0);
    
    return _sol(fitness(knn_classifier.evaluate(selected_features), count_bits(selected_features), 699, 0.93),selected_features);
    //return sol(evaluate(selected_features, data), selected_features);

}

_sol generate_near(int n_attribute, vector<bool>& features, Knn<>& knn_classifier){
    
    int q = rand()%(n_attribute);

    auto flip = [&features](int id){
        features[id] = (features[id]==0)?features[id]=1:features[id]=0;
    };

    do{
        for(int i =0; i<q; i++){ //Initialize a random features selected
            srand(clock());
            int index = rand()%(n_attribute);
            //cout<<index<<endl;
            flip(index);
        }
    }while(count_bits(features)<features.size()*0.2);

    return _sol (fitness(knn_classifier.evaluate(features), count_bits(features), 699, 0.99),features);
    //return sol(evaluate(selected_features, data), selected_features);
}