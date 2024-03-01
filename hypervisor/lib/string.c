/*
 * Copyright (C) 2018-2022 Intel Corporation.
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <types.h>
#include <rtl.h>
#include <logmsg.h>

/**
 * @defgroup lib_util lib.util
 *
 * @ingroup lib
 *
 * @brief Implementation of lib APIs to do string operation, memory operation, bitmap operation and lock operation.
 *
 * This module is decomposed into the following sub-component:
 *
 * - lib.util: Implementation of utility APIs to do string operating and memory operation.
 */

/**
 * @addtogroup lib_util
 *
 * @{
 */

/**
 * @file
 *
 * @brief This file implements string operation APIs that shall be provided by the lib.util module.
 *
 * External APIs:
 * - strtoul_hex: Convert a string to an uint64_t integer, hexadecimal support only.
 * - strchr:      Locate character in the input string.
 * - strncpy_s:   Copies one string including the terminating null byte ('\0') to another buffer, at most
 *                a given number of bytes to be copied.
 * - strnlen_s:   Calculate the length of the input string, excluding the terminating null byte.
 * - strncmp:     Compare at most given number of characters between two string operands.
 * - strstr_s:    Finds the first occurrence of the substring in the target string. The terminating null bytes ('\0')
 *                are not compared.
 * - strncat_s:   Concatenate two strings by at most a given number of bytes.
 */

static inline char hex_digit_value(char ch)
{
	char c;
	if (('0' <= ch) && (ch <= '9')) {
		c = ch - '0';
	} else if (('a' <= ch) && (ch <= 'f')) {
		c = ch - 'a' + 10;
	} else if (('A' <= ch) && (ch <= 'F')) {
		c = ch - 'A' + 10;
	} else {
		c = -1;
	}
	return c;
}

uint64_t strtoul_hex(const char *nptr)
{
	const char *s = nptr;
	char c, digit;
	uint64_t acc, cutoff, cutlim;
	uint64_t base = 16UL;
	int32_t any;

	/** Proceed input string, set c to first non-space character and s points to the next character. */
	do {
		c = *s;
		s++;
	} while (is_space(c));

	/**
	 * After omitting the initial spaces, if the string starts with current hexadecimal mark "0x" or "0X", set
	 * c to first character after "0x|0X", and set s points to second character after "0x|0X".
	 */
	if ((c == '0') && ((*s == 'x') || (*s == 'X'))) {
		c = s[1];
		s += 2;
	}

	cutoff = ULONG_MAX / base;
	cutlim = ULONG_MAX % base;
	acc = 0UL;
	any = 0;
	/**
	 * Convert single digit hexadecimal c to decimal value and save to digit.
	 * If not in the correct hexadecimal range, set digit to -1.
	 */
	digit = hex_digit_value(c);
	/**
	 * Loop proceeding the string:
	 * - Accumulate each digit to acc.
	 * - If acc equals or is greater than ULONG_MAX / 16, set any to -1 and stop loop.
	 */
	while (digit >= 0) {
		if ((acc > cutoff) || ((acc == cutoff) && ((uint64_t)digit > cutlim))) {
			any = -1;
			break;
		} else {
			acc *= base;
			acc += (uint64_t)digit;
		}

		c = *s;
		s++;
		digit = hex_digit_value(c);
	}

	/** If acc equals or is greater than ULONG_MAX / 16, set acc to ULONG_MAX. */
	if (any < 0) {
		acc = ULONG_MAX;
	}
	return acc;
}

char *strchr(char *s_arg, char ch)
{
	char *s = s_arg;

	/** Pointer s proceeds input string by character if it's not '\0' and not the target character. */
	while ((*s != '\0') && (*s != ch)) {
		++s;
	}

	/** Return s if it doesn't point to '\0', otherwise return NULL. */
	return ((*s) != '\0') ? s : NULL;
}

int32_t strncpy_s(char *d, size_t dmax, const char *s, size_t slen)
{
	char *dest = d;
	int32_t ret = -1;
	size_t len = strnlen_s(s, dmax);

	/** If destination buffer size is greater than given input size, call memcpy_s() to copy. */
	if ((slen < dmax) || (dmax > len)) {
		ret = memcpy_s(d, dmax, s, len);
	}

	/**
	 * Padding '\0' after the copied characters in destination buffer if copy is successful.
	 * Or set the first byte in \a d to '\0' if copy is not succeful and \a d is not NULL.
	 */
	if (ret == 0) {
		*(dest + len) = '\0';
	} else {
		if ((d != NULL) && (dmax > 0U)) {
			*dest = '\0';
		}
	}

	return ret;
}

size_t strnlen_s(const char *str_arg, size_t maxlen_arg)
{
	const char *str = str_arg;
	size_t count = 0U;

	/** Proceed only when input str pointer is not NULL. */
	if (str != NULL) {
		/**
		 * While *str is not a string terminator, loop *str by ASCII character, increment variable count by 1
		 * and decrement maxlen by 1 for each character.
		 * Terminate the loop when variable maxlen equals 0 at the beginning of each loop.
		 */
		size_t maxlen = maxlen_arg;
		while ((*str) != '\0') {
			if (maxlen == 0U) {
				break;
			}

			count++;
			maxlen--;
			str++;
		}
	}

	/** Return the count of ASCII characters encountered */
	return count;
}

int32_t strcmp(const char *s1_arg, const char *s2_arg)
{
	const char *str1 = s1_arg;
	const char *str2 = s2_arg;

	/**
	 * While *str1 is not a string terminator and
	 * *str2 is not a string terminator and
	 * the ASCII character resident in str1 is same with the ASCII character resident in str2
	 */
	while (((*str1) != '\0') && ((*str2) != '\0') && ((*str1) == (*str2))) {
		str1++;
		str2++;
	}

	return *str1 - *str2;
}

int32_t strncmp(const char *s1_arg, const char *s2_arg, size_t n_arg)
{
	const char *str1 = s1_arg;
	const char *str2 = s2_arg;
	size_t n = n_arg;
	int32_t ret = 0;

	/** Process only when number of characters to be compared is greater than 0. */
	if (n > 0U) {
		/**
		 * While n is not equal to 1 and
		 * *str1 is not a string terminator and
		 * *str2 is not a string terminator and
		 * the ASCII character resident in str1 is same with the ASCII character resident in str2
		 */
		while (((n - 1) != 0U) && ((*str1) != '\0') && ((*str2) != '\0') && ((*str1) == (*str2))) {
			str1++;
			str2++;
			n--;
		}
		/** Set ret to *str1 - *str2 */
		ret = (int32_t) (*str1 - *str2);
	}

	/** Return the result of string comparison operation */
	return ret;
}

char *strstr_s(const char *str1, size_t maxlen1, const char *str2, size_t maxlen2)
{
	size_t len1, len2;
	size_t i;
	const char *pstr, *pret;

	/** Input variables NULL pointer check and size non-zero check. If failure set pret to NULL */
	if ((str1 == NULL) || (str2 == NULL)) {
		pret = NULL;
	} else if ((maxlen1 == 0U) || (maxlen2 == 0U)) {
		pret = NULL;
	} else {
		len1 = strnlen_s(str1, maxlen1);
		len2 = strnlen_s(str2, maxlen2);

		if (len1 < len2) {
			pret = NULL;
		} else if ((str1 == str2) || (len2 == 0U)) {
			/** Return str1 if str2 equals to str1 or str2 points to a string with zero length. */
			pret = str1;
		} else {
			/**
			 * Set pstr to \a str1.
			 * Compare pstr and \a str2 for len2 characters to check if the two strings match.
			 * - If matching, set pret to the current pstr position.
			 * - If not matching, advance pstr by one char in \a str1 and compare again, unless the left
			 *   \a str1 characters length is less than len2.
			 */
			pret = NULL;
			pstr = str1;
			while (len1 >= len2) {
				for (i = 0U; i < len2; i++) {
					if (pstr[i] != str2[i]) {
						break;
					}
				}
				if (i == len2) {
					pret = pstr;
					break;
				}
				pstr++;
				len1--;
			}
		}
	}

	return (char *)pret;
}

int32_t strncat_s(char *dest, size_t dmax, const char *src, size_t slen)
{
	int32_t ret = -1;
	size_t len_d, len_s;
	char *d = dest, *start;

	/**
	 * Using strnlen_s() to set len_d and len_s to safe regions of \a dest and \a src.
	 */
	len_d = strnlen_s(dest, dmax);
	len_s = strnlen_s(src, slen);
	start = dest + len_d;

	/** Catenates \a src into a string at start and pads '\0' after the copied characters. */
	if ((dest != NULL) && (src != NULL) && (dmax > (len_d + len_s))
			&& ((dest > (src + len_s)) || (src > (dest + len_d)))) {
		(void)memcpy_s(start, (dmax - len_d), src, len_s);
		*(start + len_s) = '\0';
		ret = 0;
	} else {
		/** Set dest[0] to NULL char on runtime-constraint violation */
		if (dest != NULL) {
			*d = '\0';
		}
	}
	return ret;
}

/**
 * @}
 */
