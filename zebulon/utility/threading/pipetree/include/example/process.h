///
///
///

#ifndef __EXAMPLE_PROCESS_
#define __EXAMPLE_PROCESS_

#include <pipetree/branch.hpp>

class Process : public PipeTree::Branch//<double, double>
{
public:

    Process(int id)
        //: PipeTree::Branch(id)
    {
    }

    virtual boost::shared_ptr<void> ExecuteJob()
    {
        //std::cout << "Input: " << ;
        //*((int*)mInput) = (*((int*)mInput)) * 2;
        //std::cout << " Out: " << *((int*)mInput) << std::endl;

        //this->Fork(mInput);
    }
};

#endif // __EXAMPLE_PROCESS_
/* End of file */
