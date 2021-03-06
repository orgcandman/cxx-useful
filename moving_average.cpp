#include "cyclic_iterator.hpp"

#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>

using cxx_utils::iterator::cyclic_iterator;

void moving_average_main()
{
    // create a simple vector of int
    std::vector<long int> cSlidingWindowAverage(5, 0);
    // The sliding window average will be over 5 elements.

    cyclic_iterator<std::vector<long int>::iterator> iCycles(cSlidingWindowAverage.begin(), cSlidingWindowAverage.end());

    std::cout << "Input numbers with the last one being 0." << std::endl;
    std::cout << "Each iteration, the system will run a moving average"
              << std::endl;
    
    long int nInputValue = 0;
    do
    {
        std::cout << ">>> ";
        std::cout.flush();
        
        std::cin >> nInputValue;

        *iCycles++ = nInputValue;

        long int iAccumulation = std::accumulate(
            cSlidingWindowAverage.begin(),
            cSlidingWindowAverage.end(), 0);

        // note: in the below, we need to cast to double to ensure that the
        // calculation doesn't truncate.
        double dAverage = double(iAccumulation) /
            double(cSlidingWindowAverage.size());

        std::cout << " { " << dAverage << "}" << std::endl;
        
    }while( nInputValue != 0 );
}

namespace cxx_utils_examples
{
    class RunTheMovingAverageExample
    {
    public:
        RunTheMovingAverageExample(){ moving_average_main(); }
        ~RunTheMovingAverageExample(){}
    };
    
    static RunTheMovingAverageExample s_rtmae;
}
