/**
 * @file devinfoservice.h
 * @brief Device Information Service.
 * @author Thomas Reidemeister <treideme@gmail.com>, WCH
 * @copyright 2023 Thomas Reidemeister
 * @license Apache-2.0
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * @brief This file is a modified version of the original WCH file.
 */

#ifndef DEVINFOSERVICE_H
#define DEVINFOSERVICE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */

// Device Information Service Parameters
#define DEVINFO_SYSTEM_ID              0
#define DEVINFO_MODEL_NUMBER           1
#define DEVINFO_SERIAL_NUMBER          2
#define DEVINFO_FIRMWARE_REV           3
#define DEVINFO_HARDWARE_REV           4
#define DEVINFO_SOFTWARE_REV           5
#define DEVINFO_MANUFACTURER_NAME      6
#define DEVINFO_11073_CERT_DATA        7
#define DEVINFO_PNP_ID                 8

// IEEE 11073 authoritative body values
#define DEVINFO_11073_BODY_EXP         254

// System ID length
#define DEVINFO_SYSTEM_ID_LEN          8

// PnP ID length
#define DEVINFO_PNP_ID_LEN             7

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * Profile Callbacks
 */

/*********************************************************************
 * API FUNCTIONS
 */

/*
 * DevInfo_AddService- Initializes the Device Information service by registering
 *          GATT attributes with the GATT server.
 *
 */

extern bStatus_t DevInfo_AddService(void);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* DEVINFOSERVICE_H */
