
#include "xyzlabs/periodictask.hpp"
#include "xyzlabs/taskmanager.hpp"
#include "xyzlabs/globals.hpp"

PeriodicTaskInterface::PeriodicTaskInterface(uint64_t timeout): 
    timer_(task_manager().io_ctx()),
    milisecondsTimeout_(timeout) {}
