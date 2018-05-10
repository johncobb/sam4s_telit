/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.4.0-dev at Tue May  8 12:36:28 2018. */

#include "geodesity.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t Event_fields[8] = {
    PB_FIELD(  1, STRING  , SINGULAR, STATIC  , FIRST, Event, id, id, 0),
    PB_FIELD(  2, UENUM   , SINGULAR, STATIC  , OTHER, Event, type, id, 0),
    PB_FIELD(  4, UINT64  , SINGULAR, STATIC  , OTHER, Event, utc_millis, type, 0),
    PB_ONEOF_FIELD(Details, 100, MESSAGE , ONEOF, STATIC  , OTHER, Event, heartbeat, utc_millis, &Heartbeat_fields),
    PB_ONEOF_FIELD(Details, 101, MESSAGE , ONEOF, STATIC  , UNION, Event, position, utc_millis, &Position_fields),
    PB_ONEOF_FIELD(Details, 102, MESSAGE , ONEOF, STATIC  , UNION, Event, idle_notify, utc_millis, &IdleNotify_fields),
    PB_ONEOF_FIELD(Details, 103, MESSAGE , ONEOF, STATIC  , UNION, Event, park_notify, utc_millis, &ParkNotify_fields),
    PB_LAST_FIELD
};

const pb_field_t Heartbeat_fields[2] = {
    PB_FIELD(300, UINT32  , SINGULAR, STATIC  , FIRST, Heartbeat, sequence, sequence, 0),
    PB_LAST_FIELD
};

const pb_field_t Position_fields[7] = {
    PB_FIELD(301, FLOAT   , SINGULAR, STATIC  , FIRST, Position, longitude, longitude, 0),
    PB_FIELD(302, FLOAT   , SINGULAR, STATIC  , OTHER, Position, latitude, longitude, 0),
    PB_FIELD(303, FLOAT   , SINGULAR, STATIC  , OTHER, Position, altitude, latitude, 0),
    PB_FIELD(304, FLOAT   , SINGULAR, STATIC  , OTHER, Position, course, altitude, 0),
    PB_FIELD(305, FLOAT   , SINGULAR, STATIC  , OTHER, Position, speed, course, 0),
    PB_FIELD(306, FLOAT   , SINGULAR, STATIC  , OTHER, Position, odometer, speed, 0),
    PB_LAST_FIELD
};

const pb_field_t IdleNotify_fields[4] = {
    PB_FIELD(307, MESSAGE , SINGULAR, STATIC  , FIRST, IdleNotify, position_timed, position_timed, &Position_fields),
    PB_FIELD(308, UENUM   , SINGULAR, STATIC  , OTHER, IdleNotify, type, position_timed, 0),
    PB_FIELD(309, UINT32  , SINGULAR, STATIC  , OTHER, IdleNotify, seconds, type, 0),
    PB_LAST_FIELD
};

const pb_field_t ParkNotify_fields[4] = {
    PB_FIELD(310, MESSAGE , SINGULAR, STATIC  , FIRST, ParkNotify, position_timed, position_timed, &Position_fields),
    PB_FIELD(311, UENUM   , SINGULAR, STATIC  , OTHER, ParkNotify, type, position_timed, 0),
    PB_FIELD(312, UINT32  , SINGULAR, STATIC  , OTHER, ParkNotify, seconds, type, 0),
    PB_LAST_FIELD
};




/* Check that field information fits in pb_field_t */
#if !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_32BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in 8 or 16 bit
 * field descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(Event, Details.heartbeat) < 65536 && pb_membersize(Event, Details.position) < 65536 && pb_membersize(Event, Details.idle_notify) < 65536 && pb_membersize(Event, Details.park_notify) < 65536 && pb_membersize(IdleNotify, position_timed) < 65536 && pb_membersize(ParkNotify, position_timed) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_Event_Heartbeat_Position_IdleNotify_ParkNotify)
#endif

#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
#error Field descriptor for ParkNotify.seconds is too large. Define PB_FIELD_16BIT to fix this.
#endif


/* @@protoc_insertion_point(eof) */