/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 */

#ifndef _TUNABLES_H_
#define _TUNABLES_H_

/* RAM system-defined tunables */

/*
 * The easy way to use a tuned value is like this:
 *
 *    value_to_use = SYS_TUNE_OR_DFL(SYS_TUNE_XYZ, default_value);
 * When this is executed, value_to_use is assigned either the
 * default value for this tunable parameter OR a value that was
 * provided by via an adminstrarive interface for this parameter.
 *
 *    my_type_t my_array[SYS_TUNE_OR_DFL(SYS_TUNE_MY_SZ, MY_SZ_DEFAULT)];
 * This declares an array with MY_SZ_DEFAULT elements unless
 * the size of the array has been administratively tuned.
 *
 * The "tunable" tool, used by the "qonstruct" tool provides
 * a simple way for a user to alter tunable values.
 *
 * All tunables are 32-bit unsigned ints; cast as required.
 */


#define SYS_TUNABLES_MAX 160 /* Increase as needed, 32 at a time */
extern unsigned int sys_tunables_max;

extern A_UINT32 *SYS_tunables;
extern A_UINT32 *SYS_tunables_valid; /* 1 VALID bit for each tunable word */

/* Startup/Inittime system tunables
 * These offsets are known to tunable_tool; keep them in sync.
 * TBD: provide a more robust mechanism to keep these values
 * and the values in tunable_tool in sync automatically.
 */
#define SYS_TUNE_SNTPCLNT_ENABLE              0 /* 0x00 */
#define SYS_TUNE_DNSCLNT_ENABLE               1 /* 0x04 */
#define SYS_TUNE_DNSSERV_ENABLE               2 /* 0x08 */
#define SYS_TUNE_BRIDGEMODE                   3 /* 0x0c */
#define SYS_TUNE_COUNTRY_CODE                 4 /* 0x10 */

#define SYS_TUNE_GPIO0_ACTIVE_CONFIG          5 /* 0x14 */
#define SYS_TUNE_GPIO1_ACTIVE_CONFIG          6 /* 0x18 */
#define SYS_TUNE_GPIO2_ACTIVE_CONFIG          7 /* 0x1c */
#define SYS_TUNE_GPIO3_ACTIVE_CONFIG          8 /* 0x20 */
#define SYS_TUNE_GPIO4_ACTIVE_CONFIG          9 /* 0x24 */
#define SYS_TUNE_GPIO5_ACTIVE_CONFIG         10 /* 0x28 */
#define SYS_TUNE_GPIO6_ACTIVE_CONFIG         11 /* 0x2c */
#define SYS_TUNE_GPIO7_ACTIVE_CONFIG         12 /* 0x30 */
#define SYS_TUNE_GPIO8_ACTIVE_CONFIG         13 /* 0x34 */
#define SYS_TUNE_GPIO9_ACTIVE_CONFIG         14 /* 0x38 */
#define SYS_TUNE_GPIO10_ACTIVE_CONFIG        15 /* 0x3c */
#define SYS_TUNE_GPIO11_ACTIVE_CONFIG        16 /* 0x40 */
#define SYS_TUNE_GPIO12_ACTIVE_CONFIG        17 /* 0x44 */
#define SYS_TUNE_GPIO13_ACTIVE_CONFIG        18 /* 0x48 */
#define SYS_TUNE_GPIO14_ACTIVE_CONFIG        19 /* 0x4c */
#define SYS_TUNE_GPIO15_ACTIVE_CONFIG        20 /* 0x50 */
#define SYS_TUNE_GPIO16_ACTIVE_CONFIG        21 /* 0x54 */
#define SYS_TUNE_GPIO17_ACTIVE_CONFIG        22 /* 0x58 */
#define SYS_TUNE_GPIO18_ACTIVE_CONFIG        23 /* 0x5c */
#define SYS_TUNE_GPIO19_ACTIVE_CONFIG        24 /* 0x60 */
#define SYS_TUNE_GPIO20_ACTIVE_CONFIG        25 /* 0x64 */
#define SYS_TUNE_GPIO21_ACTIVE_CONFIG        26 /* 0x68 */
#define SYS_TUNE_GPIO22_ACTIVE_CONFIG        27 /* 0x6c */
#define SYS_TUNE_GPIO23_ACTIVE_CONFIG        28 /* 0x70 */
#define SYS_TUNE_GPIO24_ACTIVE_CONFIG        29 /* 0x74 */
#define SYS_TUNE_GPIO25_ACTIVE_CONFIG        30 /* 0x78 */
#define SYS_TUNE_GPIO26_ACTIVE_CONFIG        31 /* 0x7c */
#define SYS_TUNE_GPIO27_ACTIVE_CONFIG        32 /* 0x80 */
#define SYS_TUNE_GPIO28_ACTIVE_CONFIG        33 /* 0x84 */
#define SYS_TUNE_GPIO29_ACTIVE_CONFIG        34 /* 0x88 */
#define SYS_TUNE_GPIO30_ACTIVE_CONFIG        35 /* 0x8c */
#define SYS_TUNE_GPIO31_ACTIVE_CONFIG        36 /* 0x90 */
#define SYS_TUNE_GPIO32_ACTIVE_CONFIG        37 /* 0x94 */
#define SYS_TUNE_GPIO33_ACTIVE_CONFIG        38 /* 0x98 */
#define SYS_TUNE_GPIO34_ACTIVE_CONFIG        39 /* 0x9c */
#define SYS_TUNE_GPIO35_ACTIVE_CONFIG        40 /* 0xa0 */
#define SYS_TUNE_GPIO36_ACTIVE_CONFIG        41 /* 0xa4 */
#define SYS_TUNE_GPIO37_ACTIVE_CONFIG        42 /* 0xa8 */
#define SYS_TUNE_GPIO38_ACTIVE_CONFIG        43 /* 0xac */
#define SYS_TUNE_GPIO39_ACTIVE_CONFIG        44 /* 0xb0 */
#define SYS_TUNE_GPIO40_ACTIVE_CONFIG        45 /* 0xb4 */
#define SYS_TUNE_GPIO_ACTIVE_CONFIG(n) (SYS_TUNE_GPIO0_ACTIVE_CONFIG+(n))

#define SYS_TUNE_GPIO0_INACTIVE_CONFIG       46 /* 0xb8 **/
#define SYS_TUNE_GPIO1_INACTIVE_CONFIG       47 /* 0xbc **/
#define SYS_TUNE_GPIO2_INACTIVE_CONFIG       48 /* 0xc0 **/
#define SYS_TUNE_GPIO3_INACTIVE_CONFIG       49 /* 0xc4 **/
#define SYS_TUNE_GPIO4_INACTIVE_CONFIG       50 /* 0xc8 **/
#define SYS_TUNE_GPIO5_INACTIVE_CONFIG       51 /* 0xcc **/
#define SYS_TUNE_GPIO6_INACTIVE_CONFIG       52 /* 0xd0 **/
#define SYS_TUNE_GPIO7_INACTIVE_CONFIG       53 /* 0xd4 **/
#define SYS_TUNE_GPIO8_INACTIVE_CONFIG       54 /* 0xd8 **/
#define SYS_TUNE_GPIO9_INACTIVE_CONFIG       55 /* 0xdc **/
#define SYS_TUNE_GPIO10_INACTIVE_CONFIG      56 /* 0xe0 **/
#define SYS_TUNE_GPIO11_INACTIVE_CONFIG      57 /* 0xe4 **/
#define SYS_TUNE_GPIO12_INACTIVE_CONFIG      58 /* 0xe8 **/
#define SYS_TUNE_GPIO13_INACTIVE_CONFIG      59 /* 0xec **/
#define SYS_TUNE_GPIO14_INACTIVE_CONFIG      60 /* 0xf0 **/
#define SYS_TUNE_GPIO15_INACTIVE_CONFIG      61 /* 0xf4 **/
#define SYS_TUNE_GPIO16_INACTIVE_CONFIG      62 /* 0xf8 **/
#define SYS_TUNE_GPIO17_INACTIVE_CONFIG      63 /* 0xfc **/
#define SYS_TUNE_GPIO18_INACTIVE_CONFIG      64 /* 0x100 */
#define SYS_TUNE_GPIO19_INACTIVE_CONFIG      65 /* 0x104 */
#define SYS_TUNE_GPIO20_INACTIVE_CONFIG      66 /* 0x108 */
#define SYS_TUNE_GPIO21_INACTIVE_CONFIG      67 /* 0x10c */
#define SYS_TUNE_GPIO22_INACTIVE_CONFIG      68 /* 0x110 */
#define SYS_TUNE_GPIO23_INACTIVE_CONFIG      69 /* 0x114 */
#define SYS_TUNE_GPIO24_INACTIVE_CONFIG      70 /* 0x118 */
#define SYS_TUNE_GPIO25_INACTIVE_CONFIG      71 /* 0x11c */
#define SYS_TUNE_GPIO26_INACTIVE_CONFIG      72 /* 0x120 */
#define SYS_TUNE_GPIO27_INACTIVE_CONFIG      73 /* 0x124 */
#define SYS_TUNE_GPIO28_INACTIVE_CONFIG      74 /* 0x128 */
#define SYS_TUNE_GPIO29_INACTIVE_CONFIG      75 /* 0x12c */
#define SYS_TUNE_GPIO30_INACTIVE_CONFIG      76 /* 0x130 */
#define SYS_TUNE_GPIO31_INACTIVE_CONFIG      77 /* 0x134 */
#define SYS_TUNE_GPIO32_INACTIVE_CONFIG      78 /* 0x138 */
#define SYS_TUNE_GPIO33_INACTIVE_CONFIG      79 /* 0x13c */
#define SYS_TUNE_GPIO34_INACTIVE_CONFIG      80 /* 0x140 */
#define SYS_TUNE_GPIO35_INACTIVE_CONFIG      81 /* 0x144 */
#define SYS_TUNE_GPIO36_INACTIVE_CONFIG      82 /* 0x148 */
#define SYS_TUNE_GPIO37_INACTIVE_CONFIG      83 /* 0x14c */
#define SYS_TUNE_GPIO38_INACTIVE_CONFIG      84 /* 0x150 */
#define SYS_TUNE_GPIO39_INACTIVE_CONFIG      85 /* 0x154 */
#define SYS_TUNE_GPIO40_INACTIVE_CONFIG      86 /* 0x158 */
#define SYS_TUNE_GPIO_INACTIVE_CONFIG(n) (SYS_TUNE_GPIO0_INACTIVE_CONFIG+(n))

#define SYS_TUNE_GPIO0_PERIPHERAL_CONFIG       87 /* 0x15c */
#define SYS_TUNE_GPIO1_PERIPHERAL_CONFIG       88 /* 0x160 */
#define SYS_TUNE_GPIO2_PERIPHERAL_CONFIG       89 /* 0x164 */
#define SYS_TUNE_GPIO3_PERIPHERAL_CONFIG       90 /* 0x168 */
#define SYS_TUNE_GPIO4_PERIPHERAL_CONFIG       91 /* 0x16c */
#define SYS_TUNE_GPIO5_PERIPHERAL_CONFIG       92 /* 0x170 */
#define SYS_TUNE_GPIO6_PERIPHERAL_CONFIG       93 /* 0x174 */
#define SYS_TUNE_GPIO7_PERIPHERAL_CONFIG       94 /* 0x178 */
#define SYS_TUNE_GPIO8_PERIPHERAL_CONFIG       95 /* 0x17c */
#define SYS_TUNE_GPIO9_PERIPHERAL_CONFIG       96 /* 0x180 */
#define SYS_TUNE_GPIO10_PERIPHERAL_CONFIG      97 /* 0x184 */
#define SYS_TUNE_GPIO11_PERIPHERAL_CONFIG      98 /* 0x188 */
#define SYS_TUNE_GPIO12_PERIPHERAL_CONFIG      99 /* 0x18c */
#define SYS_TUNE_GPIO13_PERIPHERAL_CONFIG     100 /* 0x190 */
#define SYS_TUNE_GPIO14_PERIPHERAL_CONFIG     101 /* 0x194 */
#define SYS_TUNE_GPIO15_PERIPHERAL_CONFIG     102 /* 0x198 */
#define SYS_TUNE_GPIO16_PERIPHERAL_CONFIG     103 /* 0x19c */
#define SYS_TUNE_GPIO17_PERIPHERAL_CONFIG     103 /* 0x1a0 */
#define SYS_TUNE_GPIO18_PERIPHERAL_CONFIG     104 /* 0x1a4 */
#define SYS_TUNE_GPIO19_PERIPHERAL_CONFIG     105 /* 0x1a8 */
#define SYS_TUNE_GPIO20_PERIPHERAL_CONFIG     106 /* 0x1ac */
#define SYS_TUNE_GPIO21_PERIPHERAL_CONFIG     107 /* 0x1b0 */
#define SYS_TUNE_GPIO22_PERIPHERAL_CONFIG     108 /* 0x1b4 */
#define SYS_TUNE_GPIO23_PERIPHERAL_CONFIG     109 /* 0x1b8 */
#define SYS_TUNE_GPIO24_PERIPHERAL_CONFIG     120 /* 0x1bc */
#define SYS_TUNE_GPIO25_PERIPHERAL_CONFIG     121 /* 0x1c0 */
#define SYS_TUNE_GPIO26_PERIPHERAL_CONFIG     122 /* 0x1c4 */
#define SYS_TUNE_GPIO27_PERIPHERAL_CONFIG     123 /* 0x1c8 */
#define SYS_TUNE_GPIO28_PERIPHERAL_CONFIG     124 /* 0x1cc */
#define SYS_TUNE_GPIO29_PERIPHERAL_CONFIG     125 /* 0x1d0 */
#define SYS_TUNE_GPIO30_PERIPHERAL_CONFIG     126 /* 0x1d4 */
#define SYS_TUNE_GPIO31_PERIPHERAL_CONFIG     127 /* 0x1d8 */
#define SYS_TUNE_GPIO32_PERIPHERAL_CONFIG     128 /* 0x1dc */
#define SYS_TUNE_GPIO33_PERIPHERAL_CONFIG     129 /* 0x1e0 */
#define SYS_TUNE_GPIO34_PERIPHERAL_CONFIG     130 /* 0x1e4 */
#define SYS_TUNE_GPIO35_PERIPHERAL_CONFIG     131 /* 0x1e8 */
#define SYS_TUNE_GPIO36_PERIPHERAL_CONFIG     132 /* 0x1ec */
#define SYS_TUNE_GPIO37_PERIPHERAL_CONFIG     133 /* 0x1f0 */
#define SYS_TUNE_GPIO38_PERIPHERAL_CONFIG     134 /* 0x1f4 */
#define SYS_TUNE_GPIO39_PERIPHERAL_CONFIG     135 /* 0x1f8 */
#define SYS_TUNE_GPIO40_PERIPHERAL_CONFIG     136 /* 0x1fc */
#define SYS_TUNE_GPIO_PERIPHERAL_ID(n) (SYS_TUNE_GPIO0_PERIPHERAL_CONFIG+(n))
#define SYS_TUNE_NUM_SOFTAP_CLIENTS           137 /* 0x200 */
#define SYS_TUNE_QCOM_MEMPOOL_CHUNKSIZE       138 /* 0x204 */
#define SYS_TUNE_STACKGHOST_ATHOS_KEY_INTERVAL 139/* 0x208 */
#define SYS_TUNE_STACKGHOST_COOKIE_FIXED 	  140/* 0x20c */
/* Add new system tunables here, at the end */



/* Dynamic/Runtime system parameters
* 
*/

/*QCOM_PARAM_GROUP_SYSTEM Parameters*/
#define PARAM_GROUP_SYSTEM_SLEEP_MAINT_TIMEOUT_MSECS            1
#define PARAM_GROUP_SYSTEM_SLEEP_WATCHDOG_TIMEOUT_SECS          2
/* Add new system tunables here, at the end */


/* Tunable API */
#define QCOM_TUNE_VALID(which) (SYS_tunables_valid[(which)>>5] & (1<<((which) & 0x1f)))
#define QCOM_TUNE_VALUE(which) (SYS_tunables[(which)])
#define QCOM_TUNE_VAL_OR_DFL(which, default) (QCOM_TUNE_VALID(which) ? QCOM_TUNE_VALUE(which) : (default))

#endif /*_TUNABLES_H_ */
