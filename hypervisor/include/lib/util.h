/*
 * Copyright (C) 2018-2022 Intel Corporation.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef UTIL_H
#define UTIL_H
#include <types.h>

/**
 * @addtogroup lib_util
 *
 * @{
 */

/**
 * @file
 *
 * @brief This file declares the miscellaneous macros and inline functions provided by lib.utils module.
 */

#define offsetof(st, m) __builtin_offsetof(st, m)
#define va_start	__builtin_va_start
#define va_end		__builtin_va_end

/** Roundup (x/y) to ( x/y + (x%y) ? 1 : 0). */
#define INT_DIV_ROUNDUP(x, y)	((((x)+(y))-1)/(y))

/** Roundup (x) to (y) aligned. */
#define roundup(x, y)  (((x) + ((y) - 1UL)) & (~((y) - 1UL)))

#define min(x, y)	(((x) < (y)) ? (x) : (y))

#define max(x, y)	(((x) < (y)) ? (y) : (x))

/** Return a value of v clamped to the range [l, h]. */
#define clamp(v, l, h)	(max(min((v), (h)), (l)))

/** Replaces 'x' by the string "x". */
#define STRINGIFY(x) #x

/**
 * @brief Check if a value is aligned to the required boundary.
 *
 * @param[in] value Input value of type uint64_t.
 * @param[in] req_align Required alignment, must be a power of 2 (2, 4, 8, 16, 32, etc.).
 *
 * @return TRUE if aligned; False if not aligned.
 */
static inline bool mem_aligned_check(uint64_t value, uint64_t req_align)
{
	return ((value & (req_align - 1UL)) == 0UL);
}

/**
 * @brief Calculate the sum of a buffer of uint8_t numbers.
 *
 * @param[in] buf The input buffer, each byte will be treated as an uint8_t number.
 * @param[in] length The buffer length of bytes.
 *
 * @return The sum of the uint8_t numbers in the input buffer.
 *
 * @pre buf != NULL
 */
static inline uint8_t calculate_sum8(const void *buf, uint32_t length)
{
	uint32_t i;
	uint8_t sum = 0U;

	for (i = 0U; i < length; i++) {
		sum += *((const uint8_t *)buf + i);
	}

	return sum;
}

/**
 * @brief Calculate the checksum of the input buffer.
 *
 * @param[in] buf The input buffer.
 * @param[in] len The buffer length of bytes.
 *
 * @return The checksum of the input buffer.
 *
 * @pre buf != NULL
 */
static inline uint8_t calculate_checksum8(const void *buf, uint32_t len)
{
	return (uint8_t)(0x100U - calculate_sum8(buf, len));
}

/**
 * @brief Compare two UUIDs.
 *
 * @param[in] uuid1 The pointer to first 128 bit UUID.
 * @param[in] uuid2 The pointer to second 128 bit UUID.
 *
 * @return TRUE if the two UUIDs are equal, FALSE otherwise.
 *
 * @pre (uuid1 != NULL)
 * @pre (uuid2 != NULL)
 * @pre uuid1 maps to 16 byte memory with read privilege.
 * @pre uuid2 maps to 16 byte memory with read privilege.
 */
static inline bool uuid_is_equal(const uint8_t *uuid1, const uint8_t *uuid2)
{
	uint64_t uuid1_h = *(const uint64_t *)uuid1;
	uint64_t uuid1_l = *(const uint64_t *)(uuid1 + 8);
	uint64_t uuid2_h = *(const uint64_t *)uuid2;
	uint64_t uuid2_l = *(const uint64_t *)(uuid2 + 8);

	return ((uuid1_h == uuid2_h) && (uuid1_l == uuid2_l));
}

/**
 * @}
 */

#endif /* UTIL_H */
