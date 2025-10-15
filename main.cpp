#include <map>
#include <utility>
#include <vector>
#include "headers/algorithms.hpp"
#include "headers/round.hpp"
#include <string>
#include <ncurses.h>
#include <chrono>
#include <thread>

using namespace std;


const string options[3] = {"scissors", "rock", "paper"};
// Which move wins against another, named in correlation to the indices in options
map<int, int> moves = {{0, 2}, {1, 0}, {2, 1}};

int compare(int user, int computer);

void display_end(vector<Round>* records);

int main() {
    initscr();
    vector<Round> records = {};
    int user_choice;
    pair<vector<int>, int> pc_choices;

    while (true) {
        printw("Choose: \n 1. Scissors\n 2. Rock\n 3. Paper\n 4. exit: ");
        refresh();
        flushinp();
        nocbreak();
        echo();

        user_choice = getch() - 48;
        
        refresh();
        if(user_choice < 1 || user_choice > 4){
            flushinp(); // Clear input on invalid input
            printw("Invalid choice. Try again\n");
            refresh();
            this_thread::sleep_for(chrono::milliseconds(1000));
            clear();
            
            continue;
        }
    
        if (user_choice == 4) {
            break;
        }

        pc_choices = computer_choice(&records);
        printw("You have chosen: %s\n", options[user_choice - 1].c_str()); 
        printw("Computer chooses: %s\n", options[pc_choices.second].c_str());
         
    
        int result = compare(user_choice - 1, pc_choices.second);
        refresh();

        Round round(pc_choices, user_choice - 1, result);
        records.push_back(round);

        this_thread::sleep_for(chrono::milliseconds(1000));
        clear();
    }
    clear();
    display_end(&records);
    refresh();

    flushinp();
    nocbreak();
    getch();
    endwin();
    return 0;
}

int compare(int user, int computer) {
    if(user == computer) {
        printw("Draw \n\n");
        return 0;
    }

    if(moves[user] == computer){
        printw("You win \n\n");
        return -1;
    }
    
    printw("You lose \n\n");
    return 1;
}

void display_end(vector<Round>* records) {
    if(records->size() == 0){
        return;
    }

    printw("\n");
    
    int user_win = 0;
    int pc_win = 0;
    int draw = 0;
    
    for(int i = 0; i < records->size(); i++){
        Round round = (*records)[i];
        switch (round.result) {
            case 0: 
                draw++;
                break;
            case 1:
                pc_win++;
                break;
            case -1:
                user_win++;
                break;
        }

        printw("Round %i | %s | User choice %s | PC choice %s\n", i, round.result == 0 ? "Draw" : round.result == 1 ? "PC wins" : "User wins", options[round.player].c_str(), options[round.algorithm.second].c_str());
    }
    printw("User wins %i | PC wins %i | Draws %i | Win chance %f\n", user_win, pc_win, draw, static_cast<float>(pc_win) / (user_win + draw + pc_win));
}