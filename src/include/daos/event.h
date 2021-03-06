/**
 * (C) Copyright 2015, 2016 Intel Corporation.
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
 * Internal event API.
 */

#ifndef __DAOS_EVENTX_H__
#define __DAOS_EVENTX_H__

#include <daos_types.h>
#include <daos_errno.h>
#include <daos/list.h>
#include <daos/hash.h>
#include <daos_task.h>

enum daos_ev_flags {
	/**
	 * An event will be queued in EQ on completion and wait for polling
	 * by default. With this flag, the completed event will not be queued
	 * in the EQ anymore, upper level stack is supposed to be notified by
	 * callback.
	 */
	DAOS_EVF_NO_POLL	= (1 << 0),
	/**
	 * Only useful for parent event:
	 * without this flag, a parent event will be automatially launched
	 * if any child event is launched. With this flag, a parent event
	 * always needs to be explicitly launched.
	 */
	DAOS_EVF_NEED_LAUNCH	= (1 << 1),
};

struct tse_task_t;

typedef int (*daos_event_comp_cb_t)(void *, daos_event_t *, int);

/**
 * Finish event queue library
 */
int
daos_eq_lib_fini(void);

/**
 * Initialize event queue library
 */
int
daos_eq_lib_init();

crt_context_t *
daos_task2ctx(tse_task_t *task);

/**
 * Initialize a new event for \a eqh
 *
 * \param ev [IN]	event to initialize
 * \param flags [IN]	see daos_ev_flags
 * \param eqh [IN]	where the event to be queued on, it's ignored if
 *			\a parent is specified
 * \param parent [IN]	"parent" event, it can be NULL if no parent event.
 *			If it's not NULL, caller will never see completion
 *			of this event, instead he will only see completion
 *			of \a parent when all children of \a parent are
 *			completed.
 *
 * \return		zero on success, negative value if error
 */
int daos_event_init_adv(struct daos_event *ev, enum daos_ev_flags flags,
			daos_handle_t eqh, struct daos_event *parent);

/**
 * Mark the event completed, i.e. move this event
 * to completion list.
 *
 * \param ev [IN]	event to complete.
 * \param rc [IN]	operation return code
 */
void
daos_event_complete(daos_event_t *ev, int rc);

/**
 * Mark the event launched, i.e. move this event to running list.
 *
 * \param ev		[IN]	event to launch.
 */
int
daos_event_launch(struct daos_event *ev);

/**
 * Return transport context associated with a particular event
 *
 * \param ev [IN]	event to retrieve context.
 */
crt_context_t
daos_ev2ctx(struct daos_event *ev);

tse_sched_t *
daos_ev2sched(struct daos_event *ev);

/**
 * Return the EQ handle of the specified event.
 *
 * \param ev [IN]	event to retrive handle.
 */
daos_handle_t
daos_ev2eqh(struct daos_event *ev);

int
daos_event_destroy(struct daos_event *ev, bool force);

int
daos_event_destroy_children(struct daos_event *ev, bool force);

int
daos_event_register_comp_cb(struct daos_event *ev,
			    daos_event_comp_cb_t cb, void *arg);

/* TODO: these task functions should be moved to a different header */
int
daos_client_task_prep(void *arg, int arg_size, tse_task_t **taskp,
		      daos_event_t **evp);

int
dc_task_create(daos_opc_t opc, void *arg, int arg_size,
	       tse_task_t **taskp, daos_event_t **evp);

int
dc_task_new(daos_opc_t opc, daos_event_t *ev, tse_task_t **taskp);

int
dc_task_schedule(tse_task_t *task);

/**
 * Wait for completion of the private event
 * This function is deprecated, use dc_task_new() and dc_task_schedule()
 * instead of it.
 */
int
daos_event_priv_wait();

/**
 * Check whether \a ev is the private per-thread event
 *
 * \param ev [IN]	input event to compare with the private event.
 */
bool
daos_event_is_priv(daos_event_t *ev);

/**
 * Wait for completion if blocking mode. We always return 0 for asynchronous
 * mode because the application will get the result from event in this case,
 * besides certain failure might be reset anyway see daos_obj_comp_cb().
 */
static inline int
daos_client_result_wait(daos_event_t *ev)
{
	D__ASSERT(ev != NULL);
	if (daos_event_is_priv(ev)) /* blocking mode */
		return daos_event_priv_wait();

	return 0;
}
#endif /*  __DAOS_EV_INTERNAL_H__ */
