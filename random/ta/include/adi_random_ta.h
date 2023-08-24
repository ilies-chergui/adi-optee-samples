// Copyright 2023, Analog Devices, Inc.
//
// SPDX-License-Identifier: BSD-3-Clause

#ifndef __ADI_RANDOM_TA_H__
#define __ADI_RANDOM_TA_H__

/*
 * Each trusted app UUID should have a unique UUID that is
 * generated from a UUID generator such as
 * https://www.uuidgenerator.net/
 *
 * UUID : {520618fe-c7b2-4e8e-b340-56ef6cbc1878}
 */
#define ADI_TA_RANDOM_UUID \
	{ 0x520618fe, 0xc7b2, 0x4e8e, \
		{ 0xb3, 0x40, 0x56, 0xef, 0x6c, 0xbc, 0x18, 0x78} }

/*
 * TA_RANDOM_CMD_GENERATE - Get random bytes from TRNG
 * param[0] out (memref) RNG data, RNG size
 * param[1] unused
 * param[2] unused
 * param[3] unused
 */
#define TA_RANDOM_CMD_GENERATE		0

#endif /* __ADI_RANDOM_TA_H__ */
