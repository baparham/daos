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
 * ds_cont: Container Server Storage Layout
 *
 * This header assembles (hopefully) everything related to the persistent
 * storage layout of container metadata used by ds_cont.
 *
 * In the database of the combined pool/container service, we have this layout
 * for ds_cont:
 *
 *   Root KVS (GENERIC):
 *     Container KVS (GENERIC):
 *       Container attribute KVS (GENERIC):
 *         HCE KVS (INTEGER)
 *         LRE KVS (INTEGER)
 *         LHE KVS (INTEGER)
 *         Snapshot KVS (INTEGER)
 *       ... (more container attribute KVSs)
 *     Container handle KVS (GENERIC)
 */

#ifndef __CONTAINER_SRV_LAYOUT_H__
#define __CONTAINER_SRV_LAYOUT_H__

#include <daos_types.h>

/* Root KVS (RDB_KVS_GENERIC) */
extern daos_iov_t ds_cont_attr_conts;		/* container KVS */
extern daos_iov_t ds_cont_attr_cont_handles;	/* container handle KVS */

/*
 * Container KVS (RDB_KVS_GENERIC)
 *
 * This maps container UUIDs (uuid_t) to container attribute KVSs.
 */

/*
 * Container attribute KVS (RDB_KVS_GENERIC)
 *
 * This also stores container attributes of upper layers.
 */
extern daos_iov_t ds_cont_attr_ghce;		/* uint64_t */
extern daos_iov_t ds_cont_attr_ghpce;		/* uint64_t */
extern daos_iov_t ds_cont_attr_lres;		/* LRE KVS */
extern daos_iov_t ds_cont_attr_lhes;		/* LHE KVS */
extern daos_iov_t ds_cont_attr_snapshots;	/* snapshot KVS */

/*
 * LRE and LHE KVSs (RDB_KVS_INTEGER)
 *
 * A key is an epoch number. A value is an epoch_count. These epoch-sorted
 * KVSs enable us to quickly retrieve the minimum and maximum LREs and LHEs.
 */

/*
 * Snapshot KVS (RDB_KVS_INTEGER)
 *
 * This KVS stores an ordered list of snapshotted epochs. The values are
 * unused and empty.
 */

/*
 * Container handle KVS (RDB_KVS_GENERIC)
 *
 * A key is a container handle UUID (uuid_t). A value is a container_hdl object.
 */
struct container_hdl {
	uuid_t		ch_pool_hdl;
	uuid_t		ch_cont;
	uint64_t	ch_hce;
	uint64_t	ch_lre;
	uint64_t	ch_lhe;
	uint64_t	ch_capas;
};

#endif /* __CONTAINER_SRV_LAYOUT_H__ */
