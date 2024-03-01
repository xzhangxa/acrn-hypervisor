/*
 * Copyright (C) 2018-2022 Intel Corporation.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SPRINTF_H
#define SPRINTF_H

/**
 * @addtogroup lib_util
 *
 * @{
 */

/**
 * @file
 *
 * @brief This file declares the sprintf family APIs provided by lib.utils module.
 */

/** Command for the emit function: copy string to output. */
#define PRINT_CMD_COPY			0x00000000U

/** Command for the emit function: fill output with first character. */
#define PRINT_CMD_FILL			0x00000001U

/**
 * @brief Structure used to call back emit lived in print_param.
 *
 * @consistency N/A
 *
 * @alignment N/A
 */
struct snprint_param {
	/** The destination buffer. */
	char *dst;
	/** The size of the destination buffer. */
	uint32_t sz;
	/** Counter for written chars. */
	uint32_t wrtn;
};

/**
 * @brief Structure used to parse parameters and variables to subroutines.
 *
 * @consistency N/A
 *
 * @alignment N/A
 */
struct print_param {
	/** A pointer to the function that is used to emit characters. */
	void (*emit)(size_t, const char *, uint32_t, struct snprint_param *);
	/** An opaque pointer that is passed as forth argument to the emit function. */
        struct snprint_param *data;
	/** Contains variables which are recalculated for each argument. */
	struct {
		/** A bitfield with the parsed format flags. */
		uint32_t flags;
		/** The parsed format width. */
		uint32_t width;
		/** The parsed format precision. */
		uint32_t precision;
		/** The bitmask for unsigned values. */
		uint64_t mask;
		/** A pointer to the preformated value. */
		const char *value;
		/** The number of characters in the preformated value buffer. */
		uint32_t valuelen;
		/** A pointer to the values prefix. */
		const char *prefix;
		/** The number of characters in the prefix buffer. */
		uint32_t prefixlen;
	} vars;
};

/**
 * @brief Write bytes to output string according to given format using print_param and va_list.
 *
 * @param[out] fmt_arg Pointer to destination char buffer.
 * @param[in] param String format description.
 * @param[in] args String arguments saved in va_list format.
 *
 * @return Number of characters written to the destination buffer.
 *
 * @pre fmt_arg maps to memory with read privilege.
 *
 * @post fmt_arg buffer was changed.
 */
void do_print(const char *fmt_arg, struct print_param *param,
		__builtin_va_list args);

/**
 * @brief Write at most given number of bytes to output string according to given format, using va_list as arguments.
 *
 * @param[out] dst_arg Pointer to destination char buffer.
 * @param[in] sz_arg Buffer length of the destination buffer.
 * @param[in] fmt String format description.
 * @param[in] args String arguments saved in va_list format.
 *
 * @return Number of characters written to the destination buffer.
 *
 * @pre dst_arg maps to memory with read privilege.
 *
 * @post dst_arg buffer was changed.
 */
size_t vsnprintf(char *dst_arg, size_t sz_arg, const char *fmt, va_list args);

/**
 * @brief Write at most given number of bytes to output string according to given format.
 *
 * @param[out] dest Pointer to destination char buffer.
 * @param[in] sz Buffer length of the destination buffer.
 * @param[in] fmt Pointer to the NULL terminated format string.
 * @param[in] ... Arguments of Variable number matching format string.
 *
 * @return Number of characters written to the destination buffer.
 *
 * @pre dest maps to memory with read privilege.
 *
 * @post dest buffer was changed.
 */
size_t snprintf(char *dest, size_t sz, const char *fmt, ...);

/**
 * @}
 */

#endif /* SPRINTF_H */
