/**
 * (C) Copyright 2016 Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * GOVERNMENT LICENSE RIGHTS-OPEN SOURCE SOFTWARE
 * The Government's rights to use, modify, reproduce, release, perform, display,
 * or disclose this software are subject to the terms of the Apache License as
 * provided in Contract No. B609815.
 * Any reproduction of computer software, computer software documentation, or
 * portions thereof marked with this legend must also reproduce the markings.
 */
/**
 * dc_pool: Pool Client API
 *
 * This consists of dc_pool methods that do not belong to DAOS API.
 */

#ifndef __DAOS_POOL_H__
#define __DAOS_POOL_H__

#include <daos_types.h>
#include <daos/common.h>
#include <daos/hash.h>
#include <daos/pool_map.h>
#include <daos/rsvc.h>
#include <daos/tse.h>

int dc_pool_init(void);
void dc_pool_fini(void);

/* Client pool handle */
struct dc_pool {
	/* container list of the pool */
	daos_list_t		dp_co_list;
	/* lock for the container list */
	pthread_rwlock_t	dp_co_list_lock;
	/* pool uuid */
	uuid_t			dp_pool;
	crt_group_t	       *dp_group;
	pthread_mutex_t		dp_client_lock;
	struct rsvc_client	dp_client;
	uuid_t			dp_pool_hdl;
	uint64_t		dp_capas;
	pthread_rwlock_t	dp_map_lock;
	struct pool_map	       *dp_map;
	uint32_t		dp_ver;
	uint32_t		dp_ref;
	uint32_t		dp_disconnecting:1,
				dp_slave:1; /* generated via g2l */
};

struct dc_pool *dc_hdl2pool(daos_handle_t hdl);
void dc_pool_get(struct dc_pool *pool);
void dc_pool_put(struct dc_pool *pool);

int dc_pool_local2global(daos_handle_t poh, daos_iov_t *glob);
int dc_pool_global2local(daos_iov_t glob, daos_handle_t *poh);
int dc_pool_connect(tse_task_t *task);
int dc_pool_disconnect(tse_task_t *task);
int dc_pool_query(tse_task_t *task);
int dc_pool_target_query(tse_task_t *task);
int dc_pool_exclude(tse_task_t *task);
int dc_pool_exclude_out(tse_task_t *task);
int dc_pool_add(tse_task_t *task);
int dc_pool_evict(tse_task_t *task);
int dc_pool_svc_stop(tse_task_t *task);

int
dc_pool_map_version_get(daos_handle_t ph, unsigned int *map_ver);

int
dc_pool_local_open(uuid_t pool_uuid, uuid_t pool_hdl_uuid,
		   unsigned int flags, const char *grp,
		   struct pool_map *map, d_rank_list_t *svc_list,
		   daos_handle_t *ph);
int dc_pool_local_close(daos_handle_t ph);
int dc_pool_update_map(daos_handle_t ph, struct pool_map *map);

#endif /* __DAOS_POOL_H__ */
