#include "utils.h"

namespace JINFENG{

pid_t getTid()
{
	return syscall(SYS_gettid);
}


};
