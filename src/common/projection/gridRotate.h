#ifndef GRIDROTATE_H_
#define GRIDROTATE_H_

#include <vector>
#include <algorithm>
#include <iterator>

namespace grid
{

/**
 * Changes 1d->2d layout mapping to go from upwards to downwards, or downwards to uppwards.
 * 
 * @param grid The grid to be swapped
 * @param xLength Grid's length in x-axis. 
 */
template<typename T>
void swapUpperLowerOrientation(std::vector<T> & grid, typename std::vector<T>::size_type xLength)
{
	std::vector<T> tmp;
	typename std::vector<T>::size_type yLength = grid.size() / xLength;
	
	for ( typename std::vector<T>::size_type y = yLength; y > 0; -- y )
	{
		const T * dat = & grid[(y-1) * xLength];
		std::copy(dat, dat + xLength, std::back_inserter(tmp));
	}
	std::swap(grid, tmp);
}

}

#endif /*GRIDROTATE_H_*/
