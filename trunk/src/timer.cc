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

#include "timer.h"

#include <cstdlib>
#include <iostream>

using namespace std;
using namespace Ocean;

void Timer::start(){ gettimeofday(&start_time_, NULL); }

// returns time in milliseconds.
double Timer::elapse_time(){
    gettimeofday(&end_time_, NULL);
    return (end_time_.tv_sec-start_time_.tv_sec)*1000.0 + 
        (end_time_.tv_usec-start_time_.tv_usec)/1000.0;
}

double Timer::elapse_time_seconds(){
    return elapse_time()/1000.0;
}
	
double Timer::print_elapse(){
    double elapse_time = elapse_time_seconds();
    cout << "elapse time: " << elapse_time << endl;
    return elapse_time;
}
