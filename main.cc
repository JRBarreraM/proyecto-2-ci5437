// Game of Othello -- Example of main
// Universidad Simon Bolivar, 2012.
// Author: Blai Bonet
// Last Revision: 1/11/16
// Modified by: 

#include <iostream>
#include <limits>
#include "othello_cut.h" // won't work correctly until .h is fixed!
#include "utils.h"
#include <algorithm>
#include <unordered_map>
using namespace std;
int INFINITY = numeric_limits<int>::max();


unsigned expanded = 0;
unsigned generated = 0;
int tt_threshold = 32; // threshold to save entries in TT

// Transposition table (it is not necessary to implement TT)
struct stored_info_t {
    int value_;
    int type_;
    enum { EXACT, LOWER, UPPER };
    stored_info_t(int value = -100, int type = LOWER) : value_(value), type_(type) { }
};

struct hash_function_t {
    size_t operator()(const state_t &state) const {
        return state.hash();
    }
};

class hash_table_t : public unordered_map<state_t, stored_info_t, hash_function_t> {
};

hash_table_t TTable[2];

//int maxmin(state_t state, int depth, bool use_tt);
//int minmax(state_t state, int depth, bool use_tt = false);
//int maxmin(state_t state, int depth, bool use_tt = false);
int negamax(state_t state, int depth, int color, bool use_tt = false);
int negamax(state_t state, int depth, int alpha, int beta, int color, bool use_tt = false);
int scout(state_t state, int depth, int color, bool use_tt = false);
int negascout(state_t state, int depth, int alpha, int beta, int color, bool use_tt = false);

int negamax(state_t state, int depth, int color, bool use_tt){
    generated++;
    if (depth == 0 || state.terminal()){
        return color * state.value();
    }
    int alpha = -INFINITY;
    bool col = color + 1;
    for(int move : state.get_valid_moves(col)){
        alpha = max(alpha , -negamax(state.move(col,move) , depth - 1, -color));
    }
    expanded++;
    return alpha;
}

int negamax(state_t state, int depth, int alpha, int beta, int color, bool use_tt){
    generated++;
    if (depth == 0 || state.terminal()){
        return color * state.value();
    }
    int score = -INFINITY;
    bool col = color + 1;
    int val;
    for(int move : state.get_valid_moves(col)){
        val = -negamax(state.move(col,move) , depth - 1, -beta, -alpha, -color);
        score = max(score , val);
        alpha = max(alpha , score);
        if (alpha >= beta) break;
    }
    expanded++;
    return score;
}

bool mayorQue(int a, int b){
    return a > b;
}

bool mayorIgual(int a, int b){
    return a >= b;
}

bool test(state_t state, int depth, int color, int score, bool (*condition)(int,int)){
    generated++;
    if (depth == 0 || state.terminal()){
        return condition(state.value(),score);
    }
    bool col = color + 1;
    for(int move : state.get_valid_moves(col)){
        if (col && test(state.move(col,move) , depth - 1, -color, score , condition))
            return true;
        if (!col && !test(state.move(col,move) , depth - 1, -color, score , condition))
            return false;
    }
    expanded++;
    return !(col);
}

int scout(state_t state, int depth, int color, bool use_tt){
    generated++;
    if (depth == 0 || state.terminal()){
        return state.value();
    }
    int score = 0;
    bool col = color + 1;
    bool first = true;
    for(int move : state.get_valid_moves(col)){
        state_t child = state.move(col,move);
        if (first){
            score = scout(child , depth - 1, -color);
            first = false;
        }
        else{
            if (col && test(child , depth, -color, score , mayorQue))
                score = scout(child , depth - 1, -color);
            if (!col && !test(child , depth, -color, score , mayorIgual))
                score = scout(child , depth - 1, -color);
        }
    }
    expanded++;
    return score;
}

int negascout(state_t state, int depth, int alpha, int beta, int color, bool use_tt){
    if (depth == 0 || state.terminal()){
        return color * state.value();
    }
    bool col = color + 1;
    bool first = true;
    for(int move : state.get_valid_moves(col)){
        generated++;
        state_t child = state.move(col,move);
        int score;
        if (first){
            first = false;
            score = -negascout(child, depth -1, -beta, -alpha, -color);
        }
        else{
            score = -negascout(child, depth -1, -alpha - 1, -alpha, -color);
            if (alpha < score && score < beta){
                score = -negascout(child, depth -1, -beta, -score, -color);
            }
        }
        alpha = max(alpha,score);
        if (alpha >= beta)
            break;
    }
    expanded++;
    return alpha;
}

int main(int argc, const char **argv) {
    state_t pv[128];
    int npv = 0;
    for( int i = 0; PV[i] != -1; ++i ) ++npv;

    int algorithm = 0;
    if( argc > 1 ) algorithm = atoi(argv[1]);
    bool use_tt = argc > 2;

    // Extract principal variation of the game
    state_t state;
    cout << "Extracting principal variation (PV) with " << npv << " plays ... " << flush;
    for( int i = 0; PV[i] != -1; ++i ) {
        bool player = i % 2 == 0; // black moves first!
        int pos = PV[i];
        pv[npv - i] = state;
        state = state.move(player, pos);
    }
    pv[0] = state;
    cout << "done!" << endl;

#if 0
    // print principal variation
    for( int i = 0; i <= npv; ++i )
        cout << pv[npv - i];
#endif

    // Print name of algorithm
    cout << "Algorithm: ";
    if( algorithm == 1 )
        cout << "Negamax (minmax version)";
    else if( algorithm == 2 )
        cout << "Negamax (alpha-beta version)";
    else if( algorithm == 3 )
        cout << "Scout";
    else if( algorithm == 4 )
        cout << "Negascout";
    cout << (use_tt ? " w/ transposition table" : "") << endl;

    // Run algorithm along PV (backwards)
    cout << "Moving along PV:" << endl;
    for( int i = 0; i <= npv; ++i ) {
        // cout << pv[i];
        // cout << "Negro" <<endl;
        // for (int x : pv[i].get_valid_moves(true))
        //     cout << x << " ";
        // cout << endl;
        // cout << "Blanco" <<endl;
        // for (int x : pv[i].get_valid_moves(false))
        //     cout << x << " ";
        // cout << endl;
        int value = 0;
        TTable[0].clear();
        TTable[1].clear();
        float start_time = Utils::read_time_in_seconds();
        expanded = 0;
        generated = 0;
        int color = i % 2 == 1 ? 1 : -1;

        try {
            if( algorithm == 1 ) {
                value = negamax(pv[i], 33, color, use_tt);
            } else if( algorithm == 2 ) {
                value = negamax(pv[i], 33, -INFINITY, INFINITY, color, use_tt);
            } else if( algorithm == 3 ) {
                value = color * scout(pv[i], 33, color, use_tt);
            } else if( algorithm == 4 ) {
                value = negascout(pv[i], 33, -INFINITY, INFINITY, color, use_tt);
            }
        } catch( const bad_alloc &e ) {
            cout << "size TT[0]: size=" << TTable[0].size() << ", #buckets=" << TTable[0].bucket_count() << endl;
            cout << "size TT[1]: size=" << TTable[1].size() << ", #buckets=" << TTable[1].bucket_count() << endl;
            use_tt = false;
        }

        float elapsed_time = Utils::read_time_in_seconds() - start_time;

        cout << npv + 1 - i << ". " << (color == 1 ? "Black" : "White") << " moves: "
             << "value=" << color * value
             << ", #expanded=" << expanded
             << ", #generated=" << generated
             << ", seconds=" << elapsed_time
             << ", #generated/second=" << generated/elapsed_time
             << endl;
    }

    return 0;
}

