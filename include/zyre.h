/*  =========================================================================
    zyre_api.h - Zyre public API

    -------------------------------------------------------------------------
    Copyright (c) the Contributors as noted in the AUTHORS file.

    This file is part of Zyre, an open-source framework for proximity-based
    peer-to-peer applications -- See http://zyre.org.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

#ifndef __ZYRE_API_H_INCLUDED__
#define __ZYRE_API_H_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#include "zyre_library.h"

//  @warning THE FOLLOWING @INTERFACE BLOCK IS AUTO-GENERATED BY ZPROJECT
//  @warning Please edit the model at "api/zyre.api" to make changes.
//  @interface
//  This is a stable class, and may not change except for emergencies. It
//  is provided in stable builds.
//  This class has draft methods, which may change over time. They are not
//  in stable releases, by default. Use --enable-drafts to enable.
//  Constructor, creates a new Zyre node. Note that until you start the
//  node it is silent and invisible to other nodes on the network.
//  The node name is provided to other nodes during discovery. If you
//  specify NULL, Zyre generates a randomized node name from the UUID.
ZYRE_EXPORT zyre_t *
    zyre_new (const char *name);

//  Destructor, destroys a Zyre node. When you destroy a node, any
//  messages it is sending or receiving will be discarded.
ZYRE_EXPORT void
    zyre_destroy (zyre_t **self_p);

//  Return our node UUID string, after successful initialization
ZYRE_EXPORT const char *
    zyre_uuid (zyre_t *self);

//  Return our node name, after successful initialization. First 6
//  characters of UUID by default.
ZYRE_EXPORT const char *
    zyre_name (zyre_t *self);

//  Set the public name of this node overriding the default. The name is
//  provide during discovery and come in each ENTER message.
ZYRE_EXPORT void
    zyre_set_name (zyre_t *self, const char *name);

//  Set node header; these are provided to other nodes during discovery
//  and come in each ENTER message.
ZYRE_EXPORT void
    zyre_set_header (zyre_t *self, const char *name, const char *format, ...) CHECK_PRINTF (3);

//  Set verbose mode; this tells the node to log all traffic as well as
//  all major events.
ZYRE_EXPORT void
    zyre_set_verbose (zyre_t *self);

//  Set UDP beacon discovery port; defaults to 5670, this call overrides
//  that so you can create independent clusters on the same network, for
//  e.g. development vs. production. Has no effect after zyre_start().
ZYRE_EXPORT void
    zyre_set_port (zyre_t *self, int port_nbr);

//  Set the peer evasiveness timeout, in milliseconds. Default is 5000.
//  This can be tuned in order to deal with expected network conditions
//  and the response time expected by the application. This is tied to
//  the beacon interval and rate of messages received.
ZYRE_EXPORT void
    zyre_set_evasive_timeout (zyre_t *self, int interval);

//  Set the peer expiration timeout, in milliseconds. Default is 30000.
//  This can be tuned in order to deal with expected network conditions
//  and the response time expected by the application. This is tied to
//  the beacon interval and rate of messages received.
ZYRE_EXPORT void
    zyre_set_expired_timeout (zyre_t *self, int interval);

//  Set UDP beacon discovery interval, in milliseconds. Default is instant
//  beacon exploration followed by pinging every 1,000 msecs.
ZYRE_EXPORT void
    zyre_set_interval (zyre_t *self, size_t interval);

//  Set network interface for UDP beacons. If you do not set this, CZMQ will
//  choose an interface for you. On boxes with several interfaces you should
//  specify which one you want to use, or strange things can happen.
ZYRE_EXPORT void
    zyre_set_interface (zyre_t *self, const char *value);

//  By default, Zyre binds to an ephemeral TCP port and broadcasts the local
//  host name using UDP beaconing. When you call this method, Zyre will use
//  gossip discovery instead of UDP beaconing. You MUST set-up the gossip
//  service separately using zyre_gossip_bind() and _connect(). Note that the
//  endpoint MUST be valid for both bind and connect operations. You can use
//  inproc://, ipc://, or tcp:// transports (for tcp://, use an IP address
//  that is meaningful to remote as well as local nodes). Returns 0 if
//  the bind was successful, else -1.
ZYRE_EXPORT int
    zyre_set_endpoint (zyre_t *self, const char *format, ...) CHECK_PRINTF (2);

//  Set-up gossip discovery of other nodes. At least one node in the cluster
//  must bind to a well-known gossip endpoint, so other nodes can connect to
//  it. Note that gossip endpoints are completely distinct from Zyre node
//  endpoints, and should not overlap (they can use the same transport).
ZYRE_EXPORT void
    zyre_gossip_bind (zyre_t *self, const char *format, ...) CHECK_PRINTF (2);

//  Set-up gossip discovery of other nodes. A node may connect to multiple
//  other nodes, for redundancy paths. For details of the gossip network
//  design, see the CZMQ zgossip class.
ZYRE_EXPORT void
    zyre_gossip_connect (zyre_t *self, const char *format, ...) CHECK_PRINTF (2);

//  Start node, after setting header values. When you start a node it
//  begins discovery and connection. Returns 0 if OK, -1 if it wasn't
//  possible to start the node.
ZYRE_EXPORT int
    zyre_start (zyre_t *self);

//  Stop node; this signals to other peers that this node will go away.
//  This is polite; however you can also just destroy the node without
//  stopping it.
ZYRE_EXPORT void
    zyre_stop (zyre_t *self);

//  Join a named group; after joining a group you can send messages to
//  the group and all Zyre nodes in that group will receive them.
ZYRE_EXPORT int
    zyre_join (zyre_t *self, const char *group);

//  Leave a group
ZYRE_EXPORT int
    zyre_leave (zyre_t *self, const char *group);

//  Receive next message from network; the message may be a control
//  message (ENTER, EXIT, JOIN, LEAVE) or data (WHISPER, SHOUT).
//  Returns zmsg_t object, or NULL if interrupted
//  Caller owns return value and must destroy it when done.
ZYRE_EXPORT zmsg_t *
    zyre_recv (zyre_t *self);

//  Send message to single peer, specified as a UUID string
//  Destroys message after sending
ZYRE_EXPORT int
    zyre_whisper (zyre_t *self, const char *peer, zmsg_t **msg_p);

//  Send message to a named group
//  Destroys message after sending
ZYRE_EXPORT int
    zyre_shout (zyre_t *self, const char *group, zmsg_t **msg_p);

//  Send formatted string to a single peer specified as UUID string
ZYRE_EXPORT int
    zyre_whispers (zyre_t *self, const char *peer, const char *format, ...) CHECK_PRINTF (3);

//  Send formatted string to a named group
ZYRE_EXPORT int
    zyre_shouts (zyre_t *self, const char *group, const char *format, ...) CHECK_PRINTF (3);

//  Return zlist of current peer ids.
//  Caller owns return value and must destroy it when done.
ZYRE_EXPORT zlist_t *
    zyre_peers (zyre_t *self);

//  Return zlist of current peers of this group.
//  Caller owns return value and must destroy it when done.
ZYRE_EXPORT zlist_t *
    zyre_peers_by_group (zyre_t *self, const char *name);

//  Return zlist of currently joined groups.
//  Caller owns return value and must destroy it when done.
ZYRE_EXPORT zlist_t *
    zyre_own_groups (zyre_t *self);

//  Return zlist of groups known through connected peers.
//  Caller owns return value and must destroy it when done.
ZYRE_EXPORT zlist_t *
    zyre_peer_groups (zyre_t *self);

//  Return the endpoint of a connected peer.
//  Caller owns return value and must destroy it when done.
ZYRE_EXPORT char *
    zyre_peer_address (zyre_t *self, const char *peer);

//  Return the value of a header of a conected peer.
//  Returns null if peer or key doesn't exits.
//  Caller owns return value and must destroy it when done.
ZYRE_EXPORT char *
    zyre_peer_header_value (zyre_t *self, const char *peer, const char *name);

//  Return socket for talking to the Zyre node, for polling
ZYRE_EXPORT zsock_t *
    zyre_socket (zyre_t *self);

//  Print zyre node information to stdout
ZYRE_EXPORT void
    zyre_print (zyre_t *self);

//  Return the Zyre version for run-time API detection; returns
//  major * 10000 + minor * 100 + patch, as a single integer.
ZYRE_EXPORT uint64_t
    zyre_version (void);

//  Self test of this class.
ZYRE_EXPORT void
    zyre_test (bool verbose);

#ifdef ZYRE_BUILD_DRAFT_API
//  *** Draft method, for development use, may change without warning ***
//  Apply a azcert to a Zyre node.
ZYRE_EXPORT void
    zyre_set_zcert (zyre_t *self, zcert_t *zcert);

//  *** Draft method, for development use, may change without warning ***
//  Set the beacon version. Useful when working with ZYREv3 with secure beacons.
ZYRE_EXPORT void
    zyre_beacon_set_version (zyre_t *self, const char *version);

//  *** Draft method, for development use, may change without warning ***
//  Set-up gossip discovery with CURVE enabled.
ZYRE_EXPORT void
    zyre_gossip_connect_curve (zyre_t *self, const char *public_key, const char *format, ...) CHECK_PRINTF (3);

#endif // ZYRE_BUILD_DRAFT_API
//  @end

#define zyre_dump(z) zyre_print((z))

#ifdef __cplusplus
}
#endif

#endif

