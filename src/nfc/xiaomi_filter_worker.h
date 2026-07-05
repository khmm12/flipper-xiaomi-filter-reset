// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file xiaomi_filter_worker.h
 * @brief NFC worker that authenticates a Xiaomi filter tag and clears its counter.
 *
 * The worker drives a custom MfUltralight (NTAG213) poller state machine in the
 * NFC worker thread. On each detected tag it:
 *   1. reads the UID (pages 0-1, unprotected),
 *   2. derives the tag password from the UID,
 *   3. authenticates with PWD_AUTH,
 *   4. reads the product id and current usage counter (for reporting),
 *   5. writes zeros to the counter page (page 8) to reset filter life to 100%,
 *   6. reads the page back and verifies it is zero.
 *
 * A single successful authentication is the definitive signal that the presented
 * tag is a genuine Xiaomi filter, because the password is a deterministic function
 * of that specific tag's UID. The worker never writes unless authentication
 * succeeds, so a foreign tag is left untouched.
 */
#pragma once

#include <nfc/nfc.h>

#include "../core/xiaomi_filter.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct XiaomiFilterWorker XiaomiFilterWorker;

/** @brief Outcome of a reset attempt. */
typedef enum {
    XiaomiFilterWorkerResultSuccess, /**< Counter cleared and verified. */
    XiaomiFilterWorkerResultNotDetected, /**< No tag was activated. */
    XiaomiFilterWorkerResultReadFailed, /**< Could not read the UID pages. */
    XiaomiFilterWorkerResultAuthFailed, /**< Wrong password: not a Xiaomi filter. */
    XiaomiFilterWorkerResultWriteFailed, /**< Counter write was rejected. */
    XiaomiFilterWorkerResultVerifyFailed, /**< Write did not stick on read-back. */
} XiaomiFilterWorkerResult;

/** @brief Invoked (from the worker thread) once a reset attempt finishes. */
typedef void (*XiaomiFilterWorkerCallback)(void* context);

/** @brief Allocate a worker instance. */
XiaomiFilterWorker* xiaomi_filter_worker_alloc(void);

/** @brief Free a worker instance. Must be stopped first. */
void xiaomi_filter_worker_free(XiaomiFilterWorker* worker);

/**
 * @brief Start polling for a filter tag and reset it.
 *
 * @param[in,out] worker   worker instance
 * @param[in]     nfc      borrowed Nfc instance (owned by the caller)
 * @param[in]     callback invoked from the worker thread when a tag is processed
 * @param[in]     context  passed back to @p callback
 */
void xiaomi_filter_worker_start(
    XiaomiFilterWorker* worker,
    Nfc* nfc,
    XiaomiFilterWorkerCallback callback,
    void* context);

/** @brief Stop polling and release the poller. Safe to call if not started. */
void xiaomi_filter_worker_stop(XiaomiFilterWorker* worker);

/** @brief Result of the most recent attempt. */
XiaomiFilterWorkerResult xiaomi_filter_worker_get_result(const XiaomiFilterWorker* worker);

/** @brief Usage counter value read before the reset (arbitrary vendor units). */
uint32_t xiaomi_filter_worker_get_old_counter(const XiaomiFilterWorker* worker);

/**
 * @brief Copy the decoded product code (e.g. "AP11") of the last processed tag.
 *
 * @param[in]  worker   worker instance
 * @param[out] out_code NUL-terminated buffer of XIAOMI_FILTER_PRODUCT_CODE_SIZE bytes
 */
void xiaomi_filter_worker_get_product_code(
    const XiaomiFilterWorker* worker,
    char out_code[XIAOMI_FILTER_PRODUCT_CODE_SIZE]);

#ifdef __cplusplus
}
#endif
