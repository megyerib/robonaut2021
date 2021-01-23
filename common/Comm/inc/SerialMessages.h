#pragma once
#include <stdint.h>

// SM == Serial Message
//
// This file shall be C compatible!

// Serial message types ------------------------------------------------------------------------------------------------

typedef uint8_t  smID;
typedef uint8_t  tmID;
typedef uint16_t cfgID;

typedef enum
{
    smText = 0,
    smDummy,
	smTelemetry,
	smCfg,

    smNum
}
SM_ID;

typedef enum
{
    tmSpeed = 0,
    tmRemote,
    tmDistanceFront,
    tmSpeedCtrlProc,
    tmSpeedCtrlPID,
    tmSpeedCtrlDetail
}
TM_ID;

typedef enum
{
    cfgUiNumber = 1,
    cfgSpeedCtrlR,
    cfgSpeedCtrlP,
    cfgSpeedCtrlI,
    cfgSpeedCtrlD,
    cfgSpeedCtrlILim
}
CFG_ID;

// Message structs (fixed only) ----------------------------------------------------------------------------------------

#pragma pack(push,1)

// 0x00 Text
typedef struct
{
    smID id = smText;
    char text[];
}
SM_TEXT;

// 0x01 Dummy
typedef struct
{
    smID     id = smDummy;
    uint32_t timestamp; // [us]
    int32_t  value;
}
SM_DUMMY;

// 0x02 Telemetry
typedef struct
{
	smID     id = smTelemetry;
	uint8_t  type = tmSpeed;
	uint32_t timestamp;  // [us]
	uint32_t speed;      // [mm/s]
	uint32_t distance;   // [mm]
}
SM_TM_SPEED;

typedef struct
{
	smID     id = smTelemetry;
	uint8_t  type = tmRemote;
	uint32_t timestamp;  // [us]
	int8_t   channel[3];
}
SM_TM_REMOTE;

typedef struct
{
    smID    id = smCfg;
    cfgID   type = cfgUiNumber;
    uint8_t number;
}
SM_CFG_UINUMBER;

typedef struct
{
    smID     id = smTelemetry;
    tmID     type = tmDistanceFront;
    uint32_t timestamp;  // [us]
    uint16_t dist;       // mm
}
SM_TM_FORNTDISTANCE;

typedef struct
{
    smID     id = smTelemetry;
    tmID     type = tmSpeedCtrlProc;
    uint32_t timestamp;  // [us]
    int32_t  setpoint_r;
    int32_t  controlvalue_u;
    int32_t  processvalue_y;
}
SM_TM_SPEEDCTRLPROC;

typedef struct
{
    smID     id = smTelemetry;
    tmID     type = tmSpeedCtrlPID;
    uint32_t timestamp;  // [us]
    int32_t  P;
    int32_t  I;
    int32_t  D;
}
SM_TM_SPEEDCTRLPID;

typedef struct
{
    smID     id = smTelemetry;
    tmID     type = tmSpeedCtrlDetail;
    uint32_t timestamp;  // [us]
    int32_t  intlimit;
    int32_t  integrate;
    int32_t  derivative;
}
SM_TM_SPEEDCTRLDETAIL;

typedef struct
{
    smID    id = smCfg;
    cfgID   type = cfgSpeedCtrlR;
    int32_t setpoint_r;
}
SM_CFG_SPEEDCTRLR;

typedef struct
{
    smID    id = smCfg;
    cfgID   type = cfgSpeedCtrlP;
    int32_t P;
}
SM_CFG_SPEEDCTRLP;

typedef struct
{
    smID    id = smCfg;
    cfgID   type = cfgSpeedCtrlI;
    int32_t I;
}
SM_CFG_SPEEDCTRLI;

typedef struct
{
    smID    id = smCfg;
    cfgID   type = cfgSpeedCtrlD;
    int32_t D;
}
SM_CFG_SPEEDCTRLD;

typedef struct
{
    smID    id = smCfg;
    cfgID   type = cfgSpeedCtrlILim;
    int32_t intlim;
}
SM_CFG_SPEEDCTRLILIM;

#pragma pack(pop)

#define TRACE_DUMMY(x) SM_DUMMY msg_dummy; msg_dummy.timestamp = UPTIME_us(); msg_dummy.value = (x); TRACE_BIN(&msg_dummy, sizeof(msg_dummy))

#define TRACE_TM_SPEED(v, d) SM_TM_SPEED msg_tm_speed; msg_tm_speed.timestamp = UPTIME_us(); msg_tm_speed.speed = (v); msg_tm_speed.distance = (d); TRACE_BIN(&msg_tm_speed, sizeof(msg_tm_speed))

#define TRACE_TM_REMOTE(ch1, ch2, ch3) SM_TM_REMOTE msg_tm_remote; msg_tm_remote.timestamp = UPTIME_us(); msg_tm_remote.channel[0] = (ch1); msg_tm_remote.channel[1] = (ch2); msg_tm_remote.channel[2] = (ch3); TRACE_BIN(&msg_tm_remote, sizeof(msg_tm_remote))

#define TRACE_CFG_UINUMBER(n) SM_CFG_UINUMBER msg_cfg_uinum; msg_cfg_uinum.number = (n); TRACE_BIN(&msg_cfg_uinum, sizeof(msg_cfg_uinum))

#define TRACE_TM_FORNTDISTANCE(d) SM_TM_FORNTDISTANCE msg_tm_distf; msg_tm_distf.timestamp = UPTIME_us(); msg_tm_distf.dist = (d); TRACE_BIN(&msg_tm_distf, sizeof(msg_tm_distf))

#define TRACE_TM_SPEEDCTRLPROC(r, u, y) SM_TM_SPEEDCTRLPROC msg_tm_spctrlproc; msg_tm_spctrlproc.timestamp = UPTIME_us(); msg_tm_spctrlproc.setpoint_r = (r); msg_tm_spctrlproc.controlvalue_u = (u); msg_tm_spctrlproc.processvalue_y = (y); TRACE_BIN(&msg_tm_spctrlproc, sizeof(msg_tm_spctrlproc))

#define TRACE_TM_SPEEDCTRLPID(p, i, d) SM_TM_SPEEDCTRLPID msg_tm_spctrlpid; msg_tm_spctrlpid.timestamp = UPTIME_us(); msg_tm_spctrlpid.P = (p); msg_tm_spctrlpid.I = (i); msg_tm_spctrlpid.D = (d); TRACE_BIN(&msg_tm_spctrlpid, sizeof(msg_tm_spctrlpid))

#define TRACE_TM_SPEEDCTRLDETAIL(ilim, i, d) SM_TM_SPEEDCTRLDETAIL msg_tm_spctrldetail; msg_tm_spctrldetail.timestamp = UPTIME_us(); msg_tm_spctrldetail.intlimit = (ilim); msg_tm_spctrldetail.integrate = (i); msg_tm_spctrldetail.derivative = (d); TRACE_BIN(&msg_tm_spctrldetail, sizeof(msg_tm_spctrldetail))
