/*
 * Copyright (C) 2018-2022 Intel Corporation.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RTL_H
#define RTL_H

#include <types.h>

/**
 * @addtogroup lib_util
 *
 * @{
 */

/**
 * @file
 *
 * @brief This file declares the external data structures, macros and string APIs provided by lib.utils module.
 */

/**
 * @brief Union to present quad word.
 *
 * @consistency N/A
 *
 * @alignment N/A
 */
union u_qword {
	struct {
		uint32_t low;
		uint32_t high;
	} dwords;

	uint64_t qword;

};

/** MACRO related to string */
#define ULONG_MAX	((uint64_t)(~0UL))	/* 0xFFFFFFFF */
#define LONG_MAX	(ULONG_MAX >> 1U)	/* 0x7FFFFFFF */
#define LONG_MIN	(~LONG_MAX)		/* 0x80000000 */

/**
 * @brief Check whether a character is white space or tab.
 *
 * @param[in] c Input char.
 *
 * @return Boolean whether the input character is white space or tab.
 */
static inline bool is_space(char c)
{
	return ((c == ' ') || (c == '\t'));
}

/**
 * @brief Check whether a character is EOL.
 *
 * ASCII CR, LF, NUL all will be treated as EOL.
 *
 * @param[in] c Input char.
 *
 * @return Boolean whether the input character is EOL.
 */
static inline bool is_eol(char c)
{
	return ((c == 0x0d) || (c == 0x0a) || (c == '\0'));
}

/**
 * @brief Compare the two string operands.
 *
 * @param[in] s1_arg Pointer to the first string.
 * @param[in] s2_arg Pointer to the second string.
 *
 * @return An integer less than, equal to, or greater than zero if
 * \a s1_arg is found, respectively, to be less than, to match, or be greater
 * than \a s2.
 *
 * @pre s1_arg != NULL
 * @pre s2_arg != NULL
 * @pre s1_arg maps to memory with read privilege.
 * @pre s2_arg maps to memory with read privilege.
 */
int32_t strcmp(const char *s1_arg, const char *s2_arg);

/**
 * @brief Compare at most n_arg characters between two string operands.
 *
 * The string are compared in ASCII sort order.
 *
 * @param[in]    s1_arg Pointer to the first string.
 * @param[in]    s2_arg Pointer to the second string.
 * @param[in]    n_arg The maximum number of characters to be compared
 *
 * @return An integer less than, equal to, or greater than zero if
 * \a s1_arg is found, respectively, to be less than, to match, or be greater
 * than \a s2.
 *
 * @pre s1_arg != NULL
 * @pre s2_arg != NULL
 * @pre n_arg > 0
 * @pre The host logical address range [s1_arg , s1_arg + n_arg ) maps to memory with read privilege.
 * @pre The host logical address range [s2_arg , s2_arg + n_arg ) maps to memory with read privilege.
 */
int32_t strncmp(const char *s1_arg, const char *s2_arg, size_t n_arg);

/**
 * @brief This function copies maximum 'slen'characters from string pointed by s to a buffer pointed by d.
 *    If slen is not less than dmax, then dmax shall be more than strnlen_s(s, dmax).
 *    d[0] shall be set to '\0' if there is a runtime-constraint violation.
 *
 * @param[in] d Pointer to destination buffer.
 * @param[in] dmax Maximum length of destination buffer.
 * @param[in] s Pointer to the source string.
 * @param[in] slen The Maximum number of characters to copy from source string.
 *
 * @return Return 0 if source string is copied successfully, or return -1 if there is a runtime-constraint violation.
 *
 * @pre dmax != 0
 * @pre d != NULL
 * @pre s != NULL
 * @pre d maps to memory with read privilege.
 * @pre s maps to memory with read privilege.
 * @pre Copying shall not take place between objects that overlap.
 */
int32_t strncpy_s(char *d, size_t dmax, const char *s, size_t slen);

/**
 * @brief Locate a character in the string.
 *
 * @param[in] s_arg Pointer to the string to be checked.
 * @param[in] ch Character to be located.
 *
 * @return pointer to first occurrence of character ch in the string s_arg, or NULL if not found in string s_arg.
 *
 * @pre s_arg != NULL
 * @pre s_arg maps to memory with read privilege.
 */
char *strchr(char *s_arg, char ch);

/**
 * @brief Calculate the length of the string str_arg, excluding the terminating null byte.
 *
 * @param[in] str_arg Pointer to the string whose length is calculated
 * @param[in] maxlen_arg The maximum number of characters to examine
 *
 * @return String length, excluding the NULL character. 0 if str_arg is NULL.
 *
 * @pre [str_arg , str_arg + maxlen_arg ) maps to memory with read privilege.
 */
size_t strnlen_s(const char *str_arg, size_t maxlen_arg);

void *memset(void *base, uint8_t v, size_t n);
int32_t memcpy_s(void *d, size_t dmax, const void *s, size_t slen);
void memcpy_erms(void *d, const void *s, size_t slen);
void memcpy_erms_backwards(void *d, const void *s, size_t slen);

/**
 * @brief Convert the input string to an int64_t integer, decimal support only.
 *
 * @param[in] nptr Pointer to the string to be converted. The string spaces consists of decimal characters and
 *                 leading +/- sign if any, leading white spaces are allowed.
 *
 * @return Integer of type int64_t converted from the input string, unless:
 *         - LONG_MAX if positive overflow;
 *         - LONG_MIN if negative overflow;
 *
 * @pre nptr != NULL
 * @pre nptr maps to memory with read privilege.
 *
 * @post N/A
 */
int64_t strtol_deci(const char *nptr);

/**
 * @brief Convert the input string to an uint64_t integer, hexadecimal support only.
 *
 * @param[in] nptr Pointer to the string to be converted. The string consists of hexadecimal characters starting
 *                 with "0x" or "0X", leading white spaces are allowed.
 *
 * @return Integer of type uint64_t converted from the input string, or ULONG_MAX if overflow.
 *
 * @pre nptr != NULL
 * @pre nptr follows the parameter format description, trailing invalid characters will be ignored.
 * @pre nptr maps to memory with read privilege.
 *
 * @post N/A
 */
uint64_t strtoul_hex(const char *nptr);

/**
 * @brief Locate substring in given string.
 *
 * @param[in] str1 Pointer to the string to be searched to find the substring.
 * @param[in] maxlen1 Maximum length of str1.
 * @param[in] str2 Pointer to the substring.
 * @param[in] maxlen2 Maximum length of str2.
 *
 * @return Pointer to the first occurrence of str2 in str1 if str2 is found.
 * Return NULL if str2 is not found, or if maxlen1 or maxlen2 is 0.
 *
 * @pre The address range [str1, str1 + maxlen1 ) maps to memory with read privilege.
 * @pre The address range [str2, str2 + maxlen2 ) maps to memory with read privilege.
 */
char *strstr_s(const char *str1, size_t maxlen1, const char *str2, size_t maxlen2);

/**
 * @brief Append source string to the end of destination string, both souce and destination strings have size limits.
 *
 * @param[in] dest Pointer to the string to be appended.
 * @param[in] dmax Maximum length of the destination buffer including the terminating null byte.
 * @param[in] src Pointer to the string to be concatenated to string dest.
 * @param[in] slen Maximum characters to be appended.
 *
 * @return 0 for success, -1 for failure. When failure, also set dest[0] to '\0' if dest is not NULL.
 *
 * @pre The address range [dest, dest + dmax ) maps to memory with read privilege.
 * @pre The address range [src, src + slen ) maps to memory with read privilege.
 */
int32_t strncat_s(char *dest, size_t dmax, const char *src, size_t slen);

/**
 * @}
 */

#endif /* RTL_H */
