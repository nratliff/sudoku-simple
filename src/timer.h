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

#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>

namespace Ocean{

/** @brief Simple lightweight timing class.
 */
class Timer{
public:
    /** @brief Starts the timer by recording the current time.
     */
	void start();

    /** @brief Computes the time in milliseconds since the timer started.
     *
     *  This method does can be called multiple time referencing the same
     *  start time.  It does not stop the clock.
     *  @return The number of milliseconds since the timer was started.
     */
	double elapse_time();

    /** @brief Computes the time in seconds since the timer started.
     *
     *  This method does can be called multiple time referencing the same
     *  start time.  It does not stop the clock.
     *  @return The number of seconds since the timer was started.
     */
    double elapse_time_seconds();

    /** @brief Calculates and prints the elapse time in seconds.
     *  @return The number of seconds since the timer was started.
     */
    double print_elapse();
	
protected:
	struct timeval start_time_;
	struct timeval end_time_;
};

} // end namespace ocean

#endif // TIMER_H

