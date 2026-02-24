/*
 * wrapper_fns.h
 *
 *  Created on: Feb 23, 2026
 *      Author: vlsiuser
 */

#ifndef SRC_CORE_LOGIC_WRAPPER_FNS_H_
#define SRC_CORE_LOGIC_WRAPPER_FNS_H_

#include "../api_types.h"

void genWrapperFn (funcSpec spec, void **args, buffer_t *buf);
void readWrapperFn(funcSpec spec, void **args, buffer_t *buf);

#endif /* SRC_CORE_LOGIC_WRAPPER_FNS_H_ */
