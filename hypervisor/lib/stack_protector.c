/*
 * Copyright (C) 2018-2022 Intel Corporation.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <logmsg.h>

/**
 * @addtogroup lib_util
 *
 * @{
 */

/**
 * @file
 *
 * @brief This file implements __stack_chk_fail API that shall be provided by the lib.util.
 *
 * External APIs:
 * - __stack_chk_fail: Assert for stack check failure.
 */

void __stack_chk_fail(void);

/**
 * @brief Assert for stack check failure.
 *
 * @return N/A
 *
 * @pre N/A
 *
 * @post N/A
 */
void __stack_chk_fail(void)
{
	ASSERT(false, "stack check fails in HV\n");
}

/**
 * @}
 */
