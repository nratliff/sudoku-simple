// This file is part of SudokuSover, a simple implementation of search for
// solving sudoku puzzles.
// 
// Copyright (C) 2011 Nathan Ratliff <ratliff.nathan@gmail.com>
// 
// This program is free software: you can redistribute it and/or modify it under
// the terms of the LGNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the LGNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "sudoku.h"
#include "timer.h"
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

static Ocean::Timer timer;


////////////////////////////////////////////////////////////////////
// SudokuMatrix implementation
////////////////////////////////////////////////////////////////////

template<class element_t>
SudokuMatrix<element_t>::SudokuMatrix(int height, int width): 
    MatrixStructure<element_t>(height, width)
{}

template<class element_t>
vector<element_t> 
SudokuMatrix<element_t>::get_row(int i) const{
    int w = this->width();
    vector<element_t> row(w);
    for (int j = 0; j < w; ++j){
        row.at(j) = this->operator()(i,j);
    }
    return row;
}

template<class element_t>
vector<element_t> 
SudokuMatrix<element_t>::get_col(int j) const{
    int h = this->height();
    vector<element_t> col(h);
    for (int i = 0; i < h; ++i){
        col.at(i) = this->operator()(i,j);
    }
    return col;
}

template<class element_t>
MatrixStructure<element_t> 
SudokuMatrix<element_t>::get_set(int s_i, int s_j) const{
    MatrixStructure<element_t> set(3,3);
    for (int i = 0; i < 3; ++i){
        int e_i = 3*s_i + i; 
        for (int j = 0; j < 3; ++j){
            int e_j = 3*s_j + j; 
            set(i,j) = this->operator()(e_i, e_j);
        }
    }
    return set;
}

template<class element_t>
MatrixStructure<element_t> 
SudokuMatrix<element_t>::get_set_from_element_indices(int e_i, int e_j) const{
    return get_set(e_i/3, e_j/3);
}


////////////////////////////////////////////////////////////////////
// Board implementation
////////////////////////////////////////////////////////////////////

Board::Board(): SudokuMatrix<int>(9,9){
    init(-1);
}
Board::~Board(){
}

void Board::set_dirty(const vector<int> &elements, vector<bool> &dirty){
    assert(dirty.size() == 9);
    for (int i = 0; i < elements.size(); ++i){
        int index = elements.at(i);
        if (index != -1) {
            dirty.at(index) = true;
        }
    }
}

vector<int> Board::extract_clean(const vector<bool> &dirty){
    vector<int> clean;
    for (int i = 0; i < dirty.size(); ++i){
        if (!dirty.at(i)) clean.push_back(i);
    }
    return clean;
}

vector<int> Board::compute_moves(int i, int j){
    vector<int> actions;

    if (operator()(i,j) == -1){
        vector<int> row_elements = get_row(i);
        vector<int> col_elements = get_col(j);
        MatrixStructure<int> set = get_set_from_element_indices(i,j);
        vector<int> set_elements = set.elements();

        vector<bool> dirty(9, false);
        set_dirty(row_elements, dirty);
        set_dirty(col_elements, dirty);
        set_dirty(set_elements, dirty);

        actions = extract_clean(dirty);
    }
    return actions;
}

SudokuMatrix<vector<int> > Board::compute_moves(){
    SudokuMatrix<vector<int> > moves(height(), width());
    for (int i = 0; i < height(); ++i){
        for (int j = 0; j < width(); ++j){
            moves(i,j) = compute_moves(i,j);
        }
    }
    return moves;
}

void Board::set_elements(int set_i, int set_j, const SudokuMatrix<int> &set){
    assert(set.height() == 3);
    assert(set.width() == 3);

    for (int i = 0; i < 3; ++i){
        for (int j = 0; j < 3; ++j){
        int e_i = 3*set_i + i; 
        int e_j = 3*set_j + j; 
            operator()(e_i, e_j) = set.operator()(i,j);
        }
    }
}

bool Board::is_valid(const vector<int> &elements){
    vector<bool> dirty(9, false);
    for (int i = 0; i < elements.size(); ++i){
        int num = elements.at(i);
        if (num != -1){
            if (dirty.at(num)) return false;
            dirty.at(num) = true;
        }
    }
    return true;
}

bool Board::is_valid_row(int i) const{
    vector<int> row = get_row(i);
    return is_valid(row);
}

bool Board::is_valid_col(int j) const{
    vector<int> col = get_col(j);
    return is_valid(col);
}

bool Board::is_valid_set(int s_i, int s_j) const{
    MatrixStructure<int> set = get_set(s_i, s_j);
    const vector<int> elements = set.elements();
    return is_valid(elements);
}

bool Board::is_valid() const{
    for (int i = 0; i < 9; ++i){
        if (!is_valid_row(i) || !is_valid_col(i)) return false;
    }
    for (int s_i = 0; s_i < 3; ++s_i){
        for (int s_j = 0; s_j < 3; ++s_j){
            if (!is_valid_set(s_i, s_j)) return false;
        }
    }
    return true;
}

void Board::load_board(const string &filepath){
    ifstream in(filepath.c_str());
    for (int s_i = 0; s_i < 3; ++s_i){
        for (int s_j = 0; s_j < 3; ++s_j){
            SudokuMatrix<int> set(3,3);
            for (int i = 0; i < 9; ++i){
                int val;
                in >> val;
                set(i) = (val!=-1)?(val-1):-1; // internally represent vector indices
            }
            set_elements(s_i, s_j, set);
        }
    }
    in.close();
}

ostream& operator<<(ostream &os, const Board &board){
    int index = 0;
    cout << "   ";
    for (int i = 0; i < 3; ++i){
        for (int j = 0; j < 3; ++j){
            cout << " " << index << " ";
            ++index;
        }
        cout << " ";
    }
    //cout << endl; // print upper boarder
    //for (int i = 0; i < 33; ++i) cout << "-";

    for (int s_i = 0; s_i < 3; ++s_i){
        cout << endl;
        for (int r_i = 0; r_i < 3; ++r_i){
            cout << s_i*3 + r_i << " ";
            for (int e_j = 0; e_j < 9; ++e_j){
                if (e_j % 3 == 0) cout << "|";
                int val = board(s_i*3 + r_i, e_j);
                cout << " ";
                if (val != -1) cout << val+1;
                else cout << 'x';
                cout << " ";
            }
            cout << "|" << endl;
        }
    }
    cout << endl;
}

////////////////////////////////////////////////////////////////////
// Position implementation
////////////////////////////////////////////////////////////////////

ostream& operator<<(ostream &os, const Position &p){
    os << "(" << p.i() << "," << p.j() << ")";
    return os;
}

////////////////////////////////////////////////////////////////////
// SudokuState implementation
////////////////////////////////////////////////////////////////////

SudokuState::SudokuState(string filepath): move_matrix_(9,9){
    board_.load_board(filepath);
    move_matrix_ = board_.compute_moves();
}

SudokuState::SudokuState(const SudokuState &rhs){
    *this = rhs;
}
SudokuState& SudokuState::operator=(const SudokuState &rhs){
    board_ = rhs.board_;
    move_matrix_ = rhs.move_matrix_;
}

const Board& SudokuState::board() const{ return board_; }
const SudokuMatrix<vector<int> >& SudokuState::move_matrix() const{ return move_matrix_; }
const vector<int>& SudokuState::move_matrix(const Position &p) const{ 
    return move_matrix_(p.i(), p.j()); 
}

// Note that this function changes the state.
void SudokuState::test(){
    cout << board_ << endl;
    print_row_moves(3);
    cout << "is consistent: " << is_consistent() << endl;

    cout << "changing board..." << endl;
    bool finished = false;
    for (int i = 0; i < 9 && !finished; ++i){
        for (int j = 0; j < 9 && !finished; ++j){
            if (move_matrix_(i,j).size() > 0){
                int value = move_matrix_(i,j).front();
                cout << "setting (" << i << "," << j << ") = " << value << ", ";
                make_move(i, j, value);
                cout << (is_consistent()?"is":"is not") << " consistent: " << endl;
                cout << board_ << endl;
                finished = !is_consistent();
                if (finished){
                    cout << "------------------\nrow moves:" << endl;
                    print_moves(move_matrix_.get_row(i));
                    cout << "------------------\ncol moves:" << endl;
                    print_moves(move_matrix_.get_col(j));
                    cout << "------------------\nset moves:" << endl;
                    print_moves(move_matrix_.get_set(i/3, j/3).elements());
                }
            }
        }
    }
    cout << "is consistent: " << is_consistent() << endl;
}

void SudokuState::print_row_moves(int i) const{
    for (int j = 0; j < board_.width(); ++j){
        vector<int> actions = move_matrix_(i,j);
        cout << "(" << i << "," << j << ") " << ++actions << endl;
    }
}
void SudokuState::print_moves(const vector<vector<int> > &moves) const{
    for (int i = 0; i < moves.size(); ++i){
        vector<int> actions = moves.at(i);
        ++actions;
        cout << actions << endl;
    }
}

// Currently, this function is inefficiently implemented.  It's 
// easier and safer this way for debugging.
void SudokuState::make_move(const Position &p, int value){
    make_move(p.i(), p.j(), value);
}
void SudokuState::make_move(int i, int j, int value){
    board_(i,j) = value;
    move_matrix_ = board_.compute_moves();
}

bool SudokuState::is_consistent(const vector<vector<int> > &moves, vector<bool> dirty) const{
    for (int j = 0; j < moves.size(); ++j){
        const vector<int> &actions = moves.at(j);
        for (int m_i = 0; m_i < actions.size(); ++m_i){
            dirty.at(actions.at(m_i)) = true;
        }
    }
    // If any number is impossible, then this board is inconsistent.
    for (int d_i = 0; d_i < dirty.size(); ++d_i){
        if (!dirty.at(d_i)) return false;
    }
    return true;
}

vector<bool> SudokuState::get_row_dirty(int i) const{
    vector<bool> dirty(9,false);
    vector<int> row = board_.get_row(i);
    board_.set_dirty(row, dirty);
    return dirty;
}

vector<bool> SudokuState::get_col_dirty(int j) const{
    vector<bool> dirty(9,false);
    vector<int> col = board_.get_col(j);
    board_.set_dirty(col, dirty);
    return dirty;
}

vector<bool> SudokuState::get_set_dirty(int s_i, int s_j) const{
    vector<bool> dirty(9,false);
    MatrixStructure<int> set = board_.get_set(s_i, s_j);
    board_.set_dirty(set.elements(), dirty);
    return dirty;
}

bool SudokuState::is_consistent_row(int i) const{
    vector<vector<int> > moves = move_matrix_.get_row(i);
    vector<bool> dirty = get_row_dirty(i);
    return is_consistent(moves, dirty);
}
bool SudokuState::is_consistent_col(int j) const{
    vector<vector<int> > moves = move_matrix_.get_col(j);
    vector<bool> dirty = get_col_dirty(j);
    return is_consistent(moves, dirty);
}
bool SudokuState::is_consistent_set(int s_i, int s_j) const{
    MatrixStructure<vector<int> > move_set = move_matrix_.get_set(s_i, s_j);
    const vector<vector<int> > &move_set_vector = move_set.elements();
    vector<bool> dirty = get_set_dirty(s_i, s_j);
    return is_consistent(move_set_vector, dirty);
}

// Verifies whether there is the possibility for every row, column, and 
// set to contain digits 1 through 9.
bool SudokuState::is_consistent() const{
    for (int r_i = 0; r_i < board_.height(); ++r_i){
        if (!is_consistent_row(r_i)) return false;
    }
    for (int c_i = 0; c_i < board_.width(); ++c_i){
        if (!is_consistent_col(c_i)) return false;
    }
    for (int s_i = 0; s_i < 3; ++s_i){
        for (int s_j = 0; s_j < 3; ++s_j){
            if (!is_consistent_set(s_i, s_j)) return false;
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////
// Sudoku search implementation
////////////////////////////////////////////////////////////////////

void search(SudokuState &state, const vector<Position> &positions, int p_i){
    while (p_i < positions.size() && state.move_matrix(positions.at(p_i)).size() == 0) ++p_i;
    if (p_i == positions.size()){
        if (state.is_consistent()){
            cout << "consistent solution found." << endl;
            cout << state.board() << endl;
            timer.print_elapse();
            exit(0);
        }
        return;
    }
    assert(p_i < positions.size());
    Position p = positions.at(p_i);
    const vector<int> &actions = state.move_matrix(p);
    for (int i = 0; i < actions.size(); ++i){
        SudokuState new_state(state);
        new_state.make_move(p, actions.at(i));
        if (new_state.is_consistent()){
            search(new_state, positions, p_i + 1);
        }
    }
}


////////////////////////////////////////////////////////////////////
// Utility implementation
////////////////////////////////////////////////////////////////////

template<class element_t>
std::ostream& operator<<(std::ostream &os, const std::vector<element_t> &vec){
    if (vec.size() == 0) os << "[empty]";
    for (int i = 0; i < vec.size(); ++i){
        os << "[" << vec[i] << "]";
    }
    return os;
}
template<class element_t>
vector<element_t>& operator++(vector<element_t> &vec){
    for (int i = 0; i < vec.size(); ++i){
        ++vec.at(i);
    }
    return vec;
}


////////////////////////////////////////////////////////////////////
// Main implementation
////////////////////////////////////////////////////////////////////

int main(int argc, char **argv){
    if (argc != 2) {
        cout << "requires filepath argment." << endl;
        return 1;
    }
    string filepath(argv[1]);
    cout << "filepath: " << filepath << endl;
    SudokuState sudoku_state(filepath);
    cout << "initial board state:\n" << sudoku_state.board() << endl;

    vector<Position> positions;
    for (int i = 0; i < 9; ++i){
        for (int j = 0; j < 9; ++j){
            positions.push_back(Position(i,j));
        }
    }

    timer.start();
    search(sudoku_state, positions, 0);
    return 0;
}
