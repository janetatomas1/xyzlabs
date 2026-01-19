
#include "xyzlabs/task/periodictask.hpp"
#include "xyzlabs/task/taskmanager.hpp"

namespace xyzlabs {

PeriodicTaskInterface::PeriodicTaskInterface(TaskManager &taskManager, uint64_t timeout):
    timer_(taskManager.io_ctx()),
    milisecondsTimeout_(timeout) {}

}
