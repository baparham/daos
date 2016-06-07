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

#include <daos_sr.h>
#include <daos_m.h>

int
dsr_oclass_register(daos_handle_t coh, daos_oclass_id_t cid,
		    daos_oclass_attr_t *cattr, daos_event_t *ev)
{
	return -DER_NOSYS;
}

int
dsr_oclass_query(daos_handle_t coh, daos_oclass_id_t cid,
		 daos_oclass_attr_t *cattr, daos_event_t *ev)
{
	return -DER_NOSYS;
}

int
dsr_oclass_list(daos_handle_t coh, daos_oclass_list_t *clist,
		daos_hash_out_t *anchor, daos_event_t *ev)
{
	return -DER_NOSYS;
}

int
dsr_obj_declare(daos_handle_t coh, daos_obj_id_t oid, daos_epoch_t epoch,
		dsr_obj_attr_t *oa, daos_event_t *ev)
{
	return 0;
}

int
dsr_obj_open(daos_handle_t coh, daos_obj_id_t oid, daos_epoch_t epoch,
	     unsigned int mode, daos_handle_t *oh, daos_event_t *ev)
{
	/** XXX: just a temporary workaround , always use tgt 0 */
	daos_unit_oid_t oid_dsm = {
		.id_pub = oid,
		.id_shard = 0,
		.id_pad_32 = 0,
	};

	return dsm_obj_open(coh, 0, oid_dsm, mode, oh, ev);
}

int
dsr_obj_close(daos_handle_t oh, daos_event_t *ev)
{
	return dsm_obj_close(oh, ev);
}

int
dsr_obj_punch(daos_handle_t oh, daos_epoch_t epoch, daos_event_t *ev)
{
	return -DER_NOSYS;
}

int
dsr_obj_query(daos_handle_t oh, daos_epoch_t epoch, dsr_obj_attr_t *oa,
	      daos_rank_list_t *ranks, daos_event_t *ev)
{
	return -DER_NOSYS;
}


int
dsr_obj_fetch(daos_handle_t oh, daos_epoch_t epoch, daos_dkey_t *dkey,
	      unsigned int nr, daos_vec_iod_t *iods, daos_sg_list_t *sgls,
	      daos_vec_map_t *maps, daos_event_t *ev)
{
	return dsm_obj_fetch(oh, epoch, dkey, nr, iods, sgls, maps, ev);
}

int
dsr_obj_update(daos_handle_t oh, daos_epoch_t epoch, daos_dkey_t *dkey,
	       unsigned int nr, daos_vec_iod_t *iods, daos_sg_list_t *sgls,
	       daos_event_t *ev)
{
	return dsr_obj_update(oh, epoch, dkey, nr, iods, sgls, ev);
}

int
dsr_obj_list_dkey(daos_handle_t oh, daos_epoch_t epoch, uint32_t *nr,
		  daos_key_desc_t *kds, daos_sg_list_t *sgl,
		  daos_hash_out_t *anchor, daos_event_t *ev)
{
	return dsr_obj_list_dkey(oh, epoch, nr, kds, sgl, anchor, ev);
}

int
dsr_obj_list_akey(daos_handle_t oh, daos_epoch_t epoch, daos_dkey_t *dkey,
		  uint32_t *nr, daos_key_desc_t *kds, daos_sg_list_t *sgl,
		  daos_hash_out_t *anchor, daos_event_t *ev)
{
	return -DER_NOSYS;
}