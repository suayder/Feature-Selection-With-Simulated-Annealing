#include <iostream>
#include <time.h>
#include "knn.hpp"

using namespace std;

struct sol{
    double fitness;
    bitset<ATTRIBUTES-1> attributes;

    sol(double f, bitset<ATTRIBUTES-1> att){
        fitness = f;
        attributes = att;
    }
};

auto fitness = [](double classifier_error, int cardinality, int features, double alpha){
        return (alpha*classifier_error)+(1.0-alpha)*(cardinality/features);
    };

sol generate_new(int **);
sol generate_near(int **, bitset<ATTRIBUTES-1>&);

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    int** data;
    data = readData(PATH, ATTRIBUTES, INSTANCES);

    double t0 = 2*(ATTRIBUTES-1),t = 3000*t0; //temperature

    sol si = generate_new(data);
    sol best_sol = si;//best_sol(fitness(evaluate(selected_features, data), selected_features.count(), ATTRIBUTES-1, 0.99),selected_features);

    cout << "ERROR RATE: " << evaluate(best_sol.attributes, data)<<endl;
    cout << "BEST SOL: " << best_sol.fitness<<endl;
    cout << "SELECTED ATT: "<<best_sol.attributes<<endl;

    int count_iter = 0;
    ofstream file_save("result.dat");
    while(t>t0){

        file_save<<si.fitness<<" "<<count_iter<<endl;
        sol trial_solution = generate_near(data, si.attributes); //Generate in the neighbor
        if(trial_solution.fitness<best_sol.fitness){
            si = trial_solution;
            best_sol = trial_solution;
        }
        else if(trial_solution.fitness==best_sol.fitness){
            if(trial_solution.attributes.count()<trial_solution.attributes.count()){
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
    cout << "ERROR RATE: " << evaluate(best_sol.attributes, data)<<endl;
    cout << "BEST SOL: " << best_sol.fitness<<endl;
    cout << "SELECTED ATT: "<<best_sol.attributes<<endl;

    file_save.close();
    system ("gnuplot -p chart.gnu");
    return 0;
}

sol generate_new(int **data){
    
    bitset<ATTRIBUTES-1> selected_features;
    
    do{
        for(int i =0; i<ATTRIBUTES; i++) //Initialize a random features selected
            selected_features[i] = (rand()%10<5)?0:1;
    }while(selected_features.count()==0);

    return sol (fitness(evaluate(selected_features, data), selected_features.count(), 699, 0.99),selected_features);
    //return sol(evaluate(selected_features, data), selected_features);

}

sol generate_near(int **data, bitset<ATTRIBUTES-1>& features){
    
    int q = rand()%(ATTRIBUTES-1);
    do{
        for(int i =0; i<q; i++){ //Initialize a random features selected
            srand(clock());
            int index = rand()%(ATTRIBUTES-1);
            cout<<index<<endl;
            features.flip(index);
        }
    }while(features.count()<features.size()*0.2);

    return sol (fitness(evaluate(features, data), features.count(), 699, 0.99),features);
    //return sol(evaluate(selected_features, data), selected_features);
}