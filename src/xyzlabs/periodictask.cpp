
#include "xyzlabs/periodictask.hpp"
#include "xyzlabs/xyzlabs.hpp"

PeriodicTaskInterface::PeriodicTaskInterface(uint64_t timeout): 
    timer_(XYZLabs::instance().task_manager().io_ctx()),
    milisecondsTimeout_(timeout) {}
