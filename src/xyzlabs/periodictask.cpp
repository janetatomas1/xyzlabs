
#include "xyzlabs/periodictask.hpp"
#include "xyzlabs/xyzlabs.hpp"

PeriodicTaskInterface::PeriodicTaskInterface(): timer_(XYZLabs::instance().task_manager().io_ctx()) {
};
