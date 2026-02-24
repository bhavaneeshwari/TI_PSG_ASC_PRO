/*
 * api.h
 *
 *  Created on: Feb 23, 2026
 *      Author: vlsiuser
 */

#ifndef API_H_
#define API_H_

#include "api_types.h"

uint32_t afeSpiRawWrite(void **args);
uint32_t afeSpiRawRead(void **args);
uint32_t afeSpiBurstWrite(void **args);
uint32_t afeSpiBurstRead(void **args);
uint32_t afeSpiRawWriteMulti(void **args);
uint32_t afeSpiRawReadMulti(void **args);
uint32_t afeSpiBurstWriteMulti(void **args);

extern funcSpec funcTable[FUNC_ID_COUNT];


#endif
