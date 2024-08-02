/*
 * Copyright (C) 2024 Intel Corporation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __BACKENDS_SHMEM_H__
#define __BACKENDS_SHMEM_H__

#include <stddef.h>
#include "types.h"
#include "vmmapi.h"

#define min(a, b)  (((a) < (b)) ? (a) : (b))

#define IVSHM_ADD_LISTENER	_IOW('u', 100, struct ivshm_listener_data)
#define IVSHM_GET_MMIO_SZ	_IOR('u', 101, unsigned long long)

struct shmem_info;

struct shmem_ops {
	const char *name;

	int (*open)(const char *devpath, struct shmem_info *info, int evt_fds[], int nr_ent_fds);
	void (*close)(struct shmem_info *info);
	void (*notify_peer)(struct shmem_info *info, int vector);
};

struct shmem_info {
	struct shmem_ops *ops;

	void *mmio_base;

	int mem_fd;
	char *mem_base;
	size_t mem_size;

	int this_id;
	int peer_id;

	int nr_vecs;

	void *private;
	void *be_info;
};

extern struct shmem_ops uio_shmem_ops;
extern struct shmem_ops ivshm_ivshmem_ops;
extern struct shmem_ops ivshm_guest_shm_ops;
extern struct shmem_ops sock_ivshmem_ops;

static inline uint32_t mmio_read32(void *address)
{
	return *(volatile uint32_t *)address;
}

static inline void mmio_write32(void *address, uint32_t value)
{
	*(volatile uint32_t *)address = value;
}

#endif  /* __BACKENDS_SHMEM_H__ */
