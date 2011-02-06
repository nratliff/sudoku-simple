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

#ifndef _SUDOKU_H_
#define _SUDOKU_H_

#include "matrix_structure.h"
#include <vector>
#include <iostream>

template<class element_t>
class SudokuMatrix: public MatrixStructure<element_t>{
public:
    SudokuMatrix(int height=0, int width=0); 

    std::vector<element_t> get_row(int i) const;
    std::vector<element_t> get_col(int j) const;
    MatrixStructure<element_t> get_set(int s_i, int s_j) const;

    MatrixStructure<element_t> get_set_from_element_indices(int e_i, int e_j) const;
protected:
};

class Board: public SudokuMatrix<int>{
public:
    Board();
    ~Board();

    static void set_dirty(const std::vector<int> &elements, std::vector<bool> &dirty);
    static std::vector<int> extract_clean(const std::vector<bool> &dirty);

    std::vector<int> compute_moves(int i, int j);
    SudokuMatrix<std::vector<int> > compute_moves();

    void set_elements(int set_i, int set_j, const SudokuMatrix<int> &set);
    static bool is_valid(const std::vector<int> &elements);

    bool is_valid_row(int i) const;
    bool is_valid_col(int j) const;
    bool is_valid_set(int s_i, int s_j) const;
    bool is_valid() const;

    void load_board(const std::string &filepath);
};
std::ostream& operator<<(std::ostream &os, const Board &board);

class Position{
public:
    Position(int i = 0, int j = 0): 
        i_(i), j_(j)
    {}

    int i() const{ return i_; }
    int j() const{ return j_; }

protected:
    int i_, j_;
};
std::ostream& operator<<(std::ostream &os, const Position &p);

class SudokuState{
public:
    SudokuState(std::string filepath="file.txt");
    SudokuState(const SudokuState &rhs);
    SudokuState& operator=(const SudokuState &rhs);

    const Board& board() const;
    const SudokuMatrix<std::vector<int> >& move_matrix() const;
    const std::vector<int>& move_matrix(const Position &p) const;

    // Note that this function changes the state.
    void test();

    void print_row_moves(int i) const;
    void print_moves(const std::vector<std::vector<int> > &moves) const;

    // Currently, this function is inefficiently implemented.  It's 
    // easier and safer this way for debugging.
    void make_move(const Position &p, int value);
    void make_move(int i, int j, int value);

    bool is_consistent(const std::vector<std::vector<int> > &moves, 
        std::vector<bool> dirty) const;

    std::vector<bool> get_row_dirty(int i) const;
    std::vector<bool> get_col_dirty(int j) const;

    std::vector<bool> get_set_dirty(int s_i, int s_j) const;

    bool is_consistent_row(int i) const;
    bool is_consistent_col(int j) const;
    bool is_consistent_set(int s_i, int s_j) const;

    // Verifies whether there is the possibility for every row, column, and 
    // set to contain digits 1 through 9.
    bool is_consistent() const;

protected:
    Board board_;
    SudokuMatrix<std::vector<int> > move_matrix_;
};

void search(SudokuState &state, const std::vector<Position> &positions, int p_i);

template<class element_t>
std::ostream& operator<<(std::ostream &os, const std::vector<element_t> &vec);

template<class element_t>
std::vector<element_t>& operator++(std::vector<element_t> &vec);

#endif // _SUDOKU_H_
