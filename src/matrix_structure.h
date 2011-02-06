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

#ifndef _MATRIX_STRUCTURE_H_
#define _MATRIX_STRUCTURE_H_

#include <vector>

// Is structured like a matrix, but can store any type of element.
template<class element_t>
class MatrixStructure{
public:
    MatrixStructure(int height=0, int width=0): height_(height), width_(width),
        elements_(height*width)
    {}

    void resize(int height, int width){
        elements_.resize(height*width);
        height_ = height;
        width_ = width;
    }

    int height() const{ return height_; }
    int width() const{ return width_; }

    // Row dominate accesses.
    element_t& operator()(int i, int j){ return elements_[width_*i + j]; }
    const element_t& operator()(int i, int j) const{ return elements_[width_*i + j]; }
    element_t operator()(int i) const{ return elements_.at(i); }
    element_t& operator()(int i){ return elements_.at(i); }

    void init(const element_t &val){
        fill(elements_.begin(), elements_.end(), val);
    }
    const std::vector<element_t>& elements() const{ return elements_; }

protected:
    std::vector<element_t> elements_;
    int height_, width_;
};

#endif // _MATRIX_STRUCTURE_H_
