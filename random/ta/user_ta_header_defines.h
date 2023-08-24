// Copyright 2023, Analog Devices, Inc.
//
// SPDX-License-Identifier: BSD-3-Clause

/*
 * The name of this file must not be modified
 */

#ifndef USER_TA_HEADER_DEFINES_H
#define USER_TA_HEADER_DEFINES_H

#include <adi_random_ta.h>

#define TA_UUID				ADI_TA_RANDOM_UUID
#define TA_FLAGS			TA_FLAG_SINGLE_INSTANCE
#define TA_STACK_SIZE			(2 * 1024)
#define TA_DATA_SIZE			(32 * 1024)

#endif /* USER_TA_HEADER_DEFINES_H */
