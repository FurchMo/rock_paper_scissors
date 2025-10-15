#include <algorithm>
#include <limits>
#include <random>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include "../headers/round.hpp"

using namespace std;


float sum_vector(const vector<float>* scores);

float get_model_score(int model, const vector<Round>* records, int drop_first);

int beats(int choice);

int vectorize(int choice1, int choice2);

int get_frequent(const vector<int>* vectors, bool desc=true);

int model0(const vector<Round>* records);

int model1(const vector<Round>* records);

int model2(const vector<Round>* records);

int model3(const vector<Round>* records);

int get_weighted_av(const pair<vector<int>, int>* choices, const vector<pair<int, float>>* scores);


pair<vector<int>, int> computer_choice(const vector<Round>* records) {
    int chosen_model;

    vector<pair<int, float>> model_scores = {};
    pair<vector<int>, int> model_choices;

    if(records->size() <= 1){
        model_choices.first = {model0(records), 0, 0, 0};
        model_choices.second = model_choices.first[0];
        return model_choices;
    }
    model_choices.first = {model0(records), model1(records), model2(records), model3(records)};
    model_scores.push_back({0, get_model_score(0, records,0)});

    if(records->size() >= 2){
        model_scores.push_back({1, get_model_score(1, records, 2)});
        model_scores.push_back({2, get_model_score(2, records, 1)});
        model_scores.push_back({3, get_model_score(3, records, 1)});
    }
    
    model_choices.second = get_weighted_av(&model_choices, &model_scores);

    return model_choices;
}

float sum_vector(const vector<float>* scores){
    float sum = 0;
    for(float score: (*scores)){
        sum += score;
    }
    return sum;
}

float get_model_score(int model, const vector<Round>* records, int drop_first=1){
    vector<float> score_record = {};
    float max_score = 0;
    int model_result;

    string model_key = "model" + to_string(model); 
    for(int i = drop_first; i < records->size(); i++){
        float points = i * i;
        Round round = (*records)[i];
        
        model_result = round.algorithm.first[model];
           
        if(round.result == 1) {
            if(model_result == round.algorithm.second){
                score_record.push_back(points);
            } else {
                score_record.push_back(-(points / 2 ));
            }
            
        } else if(round.result == -1){
            if(model_result == round.algorithm.second){
                score_record.push_back(-points);
            } else {
                score_record.push_back(points / 2);
            }

        } else if(round.result == 0){
            if(round.player == model_result){
                score_record.push_back(0);
            } else if(beats(round.player) == model_result){
                score_record.push_back(points / 2);
            } else {
                score_record.push_back(-(points/  2)); 
            }
        }
        max_score += points;
    }
    return !score_record.empty() ? sum_vector(&score_record) / max_score : 0.3;
}

int beats(int choice) {
    if(choice == 2) {
        return 0;
    }
    return choice + 1;
}

int vectorize(int choice1, int choice2){
    if(choice1 == beats(choice2)){
        return 1;
    } else if(choice1 == beats(beats(choice2))) {
        return -1;
    }
    return 0;
}

int get_frequent(const vector<int>* vectors, bool desc){
    if(vectors->empty()) {
        return 0;
    }

    map<int, int> values = {};
    for(int i = 0; i < vectors->size(); i++){
        values[(*vectors)[i]]++;
    }
    
    vector<pair<int, int>> valuesVec;
    for(auto& q: values) {
        valuesVec.push_back({q.first, q.second});
    };

    auto sort_fn = desc ? 
    [](pair<int, int>& a, pair<int, int>& b) {
        return a.second < b.second; 
    } : [](pair<int, int>& a, pair<int, int>& b) {
        return a.second > b.second; 
    };

    sort(valuesVec.begin(), valuesVec.end(), sort_fn);
    
    return valuesVec[0].first;
}

// Chooses based on players last choice. Beats last choice if player hast not changed his choice the last three rounds,
// looses to last choice if player changes his choice often
int model0(const vector<Round>* records) {
    int size = records->size();
    if(size > 3) {
        int repeats = 0;
        for(int i = size - 1; i > size - 4; i--){
            repeats += (int)((*records)[i].player == (*records)[i - 1].player);
        }
        if(repeats > 1) {
            return beats((*records)[size - 1].player);
        } 
        return beats(beats((*records)[size - 1].player)); // Two times beats == loses
    }

    if(size > 0) {
        return beats(beats((*records)[size - 1].player));
    }

    // If no records (first round) take random choice
    random_device os_seed;
    const uint_least32_t seed = os_seed();
    
    mt19937 generator( seed );
    uniform_int_distribution< uint_least32_t > distribute( 0, 2 );
    
    return distribute(generator);
}

//Chooses based on the past three to seven choices
int model1(const vector<Round>* records){
    int size = records->size();

    if(size > 1){
        int _vector;

        if(size > 5){
            vector<int> vectors= {};

            for(int i = size - 1; i > size - 6; i--){
                vectors.push_back(vectorize((*records)[i].player, (*records)[i-1].player));
            }
            _vector = get_frequent(&vectors, false);
            
        } else {
            _vector = vectorize((*records)[size - 1].player, (*records)[size - 2].player);
        }

        if(_vector == 1){
            return beats(beats((*records)[size - 1].player));
        } 
        else if(_vector == -1){
            return (*records)[size - 1].player;
        }

        return beats((*records)[size - 1].player);
    }

    return 0;
}

// Beat the players most frequent choice
int model2(const vector<Round>* records){
    int size = records->size();
    vector<int> player_choices = {};

    for(auto i: (*records)){
        player_choices.push_back(i.player);
    }

    if(size > 0){
        return beats(get_frequent(&player_choices, false));
    }
    return 0;
}

// Beat the players least frequent choice
int model3(const vector<Round>* records){
    int size = records->size();

    vector<int> player_choices = {};

    for(auto i: (*records)){
        player_choices.push_back(i.player);
    }

    if(size > 0){
        return beats(get_frequent(&player_choices));
    }
    return 0;
}

int get_weighted_av(const pair<vector<int>, int>* choices, const vector<pair<int, float>>* scores){
    if(choices->first.empty() || scores->empty()){
        return 0;
    }

    map<int, float> weighted_scores = {};
    for(int i = 0; i < choices->first.size(); i++){
        weighted_scores[choices->first[i]] += (*scores)[i].second;
    }

    int best_choice = 0;
    float max_score = -numeric_limits<float>::max();

    for(const auto [choice, score]: weighted_scores){
        if(score > max_score){
            max_score = score;
            best_choice = choice;
        }
    }
    
    return best_choice;
}