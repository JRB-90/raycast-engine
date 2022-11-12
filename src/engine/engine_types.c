#include "engine/engine_types.h"

test_t create_test_t(int value)
{
	return 
		(test_t) {
			.value = value
		};
}
