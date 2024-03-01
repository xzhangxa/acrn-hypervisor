/*
 * Copyright (C) 2020-2022 Intel Corporation.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef HASH_H
#define HASH_H

#include <types.h>

/**
 * @addtogroup lib_util
 *
 * @{
 */

/**
 * @file
 *
 * @brief This file declares the hash macros and inline functions provided by lib.utils module.
 */

/**
 * Hash factor is selected following below formula:
 *  - factor64 = 2^64 * ((sqrt(5) - 1)/2)
 * Here, ((sqrt(5) - 1)/2) is golden ratio.
 */
#define HASH_FACTOR64 0x9E3779B9486E555EUL

/**
 * @brief Hash function multiplies 64bit key by 64bit hash factor and returns high bits.
 *
 * @param[in] key 64-bit key.
 * @param[in] bits High bits number to be returned.
 *
 * @return High bits of the hashed value.
 *
 * @pre (bits < 64)
 */
static inline uint64_t hash64(uint64_t key, uint32_t bits)
{
	return (key * HASH_FACTOR64) >> (64U - bits);
}

/**
 * @}
 */

#endif /* HASH_H */
