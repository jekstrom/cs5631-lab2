"""
Constants that are used and understood by the standard MUSCLE server.
"""

__author__    = "Jeremy Friesner (jaf@meyersound.com)"
__version__   = "$Revision: 1.4 $"
__date__      = "$Date: 2005/01/01 01:03:21 $"
__copyright__ = "Copyright (c) 2007 Meyer Sound Laboratories Inc"
__license__   = "See enclosed LICENSE.TXT file"     

# Unused dummy value ('!Pc0'), marks the beginning of the reserved command array
BEGIN_PR_COMMANDS            = 558916400

# Adds/replaces the given fields in the parameters table
PR_COMMAND_SETPARAMETERS     = 558916401 

# Returns the current parameter set to the client
PR_COMMAND_GETPARAMETERS     = 558916402 

# deletes the parameters specified in PR_NAME_KEYS
PR_COMMAND_REMOVEPARAMETERS  = 558916403 

# Adds/replaces the given message in the data table
PR_COMMAND_SETDATA           = 558916404 

# Retrieves the given message(s) in the data table
PR_COMMAND_GETDATA           = 558916405 

# Removes the gives message(s) from the data table
PR_COMMAND_REMOVEDATA        = 558916406 

# Removes data from outgoing result messages
PR_COMMAND_JETTISONRESULTS   = 558916407 

# Insert nodes underneath a node, as an ordered list
PR_COMMAND_INSERTORDEREDDATA = 558916408 

# Echo this message back to the sending client
PR_COMMAND_PING              = 558916409 

# Kick matching clients off the server (Requires privilege)
PR_COMMAND_KICK              = 558916410 

# Add ban patterns to the server's ban list (Requires privilege)
PR_COMMAND_ADDBANS           = 558916411 

# Remove ban patterns from the server's ban list (Requires privilege)
PR_COMMAND_REMOVEBANS        = 558916412 

# Submessages under PR_NAME_KEYS are executed in order, as if they came separately
PR_COMMAND_BATCH             = 558916413 

# Server will ignore this message
PR_COMMAND_NOOP              = 558916414 

# Move one or more intries in a node index to a different spot in the index
PR_COMMAND_REORDERDATA       = 558916415 

# Add require patterns to the server's require list (Requires ban privilege)
PR_COMMAND_ADDREQUIRES       = 558916416 

# Remove require patterns from the server's require list (Requires ban privilege)
PR_COMMAND_REMOVEREQUIRES    = 558916417 

# Sets an entire subtree of data from a single Message (Not implemented!)
PR_COMMAND_SETDATATREES      = 558916418 

# Returns an entire subtree of data as a single Message
PR_COMMAND_GETDATATREES      = 558916419 

# Removes matching RESULT_DATATREES Messages from the outgoing queuen
PR_COMMAND_JETTISONDATATREES = 558916420 

# Reserved for future expansion
PR_COMMAND_RESERVED14        = 558916421 

# Reserved for future expansion
PR_COMMAND_RESERVED15        = 558916422 

# Reserved for future expansion
PR_COMMAND_RESERVED16        = 558916423 

# Reserved for future expansion
PR_COMMAND_RESERVED17        = 558916424 

# Reserved for future expansion
PR_COMMAND_RESERVED18        = 558916425 

# Reserved for future expansion
PR_COMMAND_RESERVED19        = 558916426 

# Reserved for future expansion
PR_COMMAND_RESERVED20        = 558916427 

# Reserved for future expansion
PR_COMMAND_RESERVED21        = 558916428 

# Reserved for future expansion
PR_COMMAND_RESERVED22        = 558916429 

# Reserved for future expansion
PR_COMMAND_RESERVED23        = 558916430 

# Reserved for future expansion
PR_COMMAND_RESERVED24        = 558916431 

# Reserved for future expansion
PR_COMMAND_RESERVED25        = 558916432 

# Dummy value to indicate the end of the reserved command range
END_PR_COMMANDS              = 558916433

# Marks beginning of range of 'what' codes that may be generated by the StorageReflectSession and sent back to the client
BEGIN_PR_RESULTS             = 558920240 

# Sent to client in response to PR_COMMAND_GETPARAMETERS
PR_RESULT_PARAMETERS         = 558920241 

# Sent to client in response to PR_COMMAND_GETDATA, or subscriptions
PR_RESULT_DATAITEMS          = 558920242 

# Sent to client to tell him that we don't know how to process his request message
PR_RESULT_ERRORUNIMPLEMENTED = 558920243 

# Notification that an entry has been inserted into an ordered index
PR_RESULT_INDEXUPDATED       = 558920244 

# Response from a PR_COMMAND_PING message
PR_RESULT_PONG               = 558920245 

# Your client isn't allowed to do something it tried to do
PR_RESULT_ERRORACCESSDENIED  = 558920246 

# Reply to a PR_COMMAND_GETDATATREES message
PR_RESULT_DATATREES          = 558920247 

# Reserved for future expansion
PR_RESULT_RESERVED5          = 558920248 

# Reserved for future expansion
PR_RESULT_RESERVED6          = 558920249 

# Reserved for future expansion
PR_RESULT_RESERVED7          = 558920250 

# Reserved for future expansion
PR_RESULT_RESERVED8          = 558920251 

# Reserved for future expansion
PR_RESULT_RESERVED9          = 558920252 

# Reserved for future expansion
PR_RESULT_RESERVED10         = 558920253 

# Reserved for future expansion
PR_RESULT_RESERVED11         = 558920254 

# Reserved for future expansion
PR_RESULT_RESERVED12         = 558920255 

# Reserved for future expansion
PR_RESULT_RESERVED13         = 558920256 

# Reserved for future expansion
PR_RESULT_RESERVED14         = 558920257 

# Reserved for future expansion
PR_RESULT_RESERVED15         = 558920258 

# Reserved for future expansion
PR_RESULT_RESERVED16         = 558920259 

# Reserved for future expansion
PR_RESULT_RESERVED17         = 558920260 

# Reserved for future expansion
PR_RESULT_RESERVED18         = 558920261 

# Reserved for future expansion
PR_RESULT_RESERVED19         = 558920262 

# Reserved for future expansion
PR_RESULT_RESERVED20         = 558920263 

# Reserved for future expansion
PR_RESULT_RESERVED21         = 558920264 

# Reserved for future expansion
PR_RESULT_RESERVED22         = 558920265 

# Reserved for future expansion
PR_RESULT_RESERVED23         = 558920266 

# Reserved for future expansion
PR_RESULT_RESERVED24         = 558920267 

# Reserved for future expansion
PR_RESULT_RESERVED25         = 558920268 

# Reserved for future expansion
END_PR_RESULTS               = 558920269 

# Privilege bit, indicates that the client is allowed to kick other clients off the MUSCLE server
PR_PRIVILEGE_KICK            = 0

# Privilege bit, indicates that the client is allowed to ban other clients from the MUSCLE server
PR_PRIVILEGE_ADDBANS         = 1

# Privilege bit, indicates that the client is allowed to unban other clients from the MUSCLE server
PR_PRIVILEGE_REMOVEBANS      = 2

# Number of defined privilege bits
PR_NUM_PRIVILEGES            = 3
  
# Op-code that indicates that an entry was inserted at the given slot index, with the given ID
INDEX_OP_ENTRYINSERTED = 'i'   

# Op-code that indicates that an entry was removed from the given slot index, had the given ID
INDEX_OP_ENTRYREMOVED  = 'r'   

# Op-code that indicates that the index was cleared
INDEX_OP_CLEARED       = 'c'   

# Field name for key-strings (often node paths or regex expressions)
PR_NAME_KEYS                      = "!SnKy" 

# Field name to contains node path strings of removed data items
PR_NAME_REMOVED_DATAITEMS         = "!SnRd" 

# Field name (any type):  If present in a PR_COMMAND_SETPARAMETERS message, disables inital-value-send from new subscriptions
PR_NAME_SUBSCRIBE_QUIETLY         = "!SnQs" 

# Field name (any type):  If present in a PR_COMMAND_SETDATA message, subscribers won't be notified about the change.
PR_NAME_SET_QUIETLY               = "!SnQ2" 

# Field name (any type):  If present in a PR_COMMAND_REMOVEDATA message, subscribers won't be notified about the change.
PR_NAME_REMOVE_QUIETLY            = "!SnQ3" 

# Field name (any type):  If set as parameter, include ourself in wildcard matches
PR_NAME_REFLECT_TO_SELF           = "!Self" 

# Field name (any type):  If set as a parameter, disable all subscription update messaging.
PR_NAME_DISABLE_SUBSCRIPTIONS     = "!Dsub" 

# Field name of int parameter sets max # of items per PR_RESULT_DATAITEMS message
PR_NAME_MAX_UPDATE_MESSAGE_ITEMS  = "!MxUp" 

# Field name of String returned in parameter set contains this session's /host/sessionID path
PR_NAME_SESSION_ROOT              = "!Root" 

# Field name for Message: In PR_RESULT_ERROR_* messages, holds the client's message that failed to execute.
PR_NAME_REJECTED_MESSAGE          = "!Rjct" 

# Field name of int32 bitchord of client's PR_PRIVILEGE_* bits.
PR_NAME_PRIVILEGE_BITS            = "!Priv" 

# Field name of int64 indicating how many more bytes are available for MUSCLE server to use
PR_NAME_SERVER_MEM_AVAILABLE      = "!Mav"

# Field name of int64 indicating how many bytes the MUSCLE server currently has allocated
PR_NAME_SERVER_MEM_USED           = "!Mus"   

# Field name of int64 indicating how the maximum number of bytes the MUSCLE server may have allocated at once.
PR_NAME_SERVER_MEM_MAX            = "!Mmx"

# Field name of String indicating version of MUSCLE that the server was compiled from
PR_NAME_SERVER_VERSION            = "!Msv"

# Field name of int64 indicating how many microseconds have elapsed since the server was started.
PR_NAME_SERVER_UPTIME             = "!Mup"

# Field name of int32 indicating how many database nodes may be uploaded by this client (total).
PR_NAME_MAX_NODES_PER_SESSION     = "!Mns"

# Field name of a string that the server will replace with the session ID string of your
# session in any outgoing client-to-client messages. 
PR_NAME_SESSION                   = "session" 

# this field name's submessage is the payload of the current node
# in the message created by StorageReflectSession::SaveNodeTreeToMessage() 
PR_NAME_NODEDATA                  = "data"  

# this field name's submessage represents the children of the current node (recursive)  
# in the message created by StorageReflectSession::SaveNodeTreeToMessage() 
PR_NAME_NODECHILDREN              = "kids"

# this field name's submessage represents the index of the current node
# in the message created by StorageReflectSession::SaveNodeTreeToMessage() 
PR_NAME_NODEINDEX                 = "index"

# This is a specialization of AbstractReflectSession that adds several
# useful capabilities to the Reflect Server.  Abilities include:
#   - Messages can specify (via wildcard path matching) which other
#     clients they should be reflected to.  If the message doesn't specify
#     a path, then the session's default path can be used.
#   - Clients can upload and store data on the server (in the server's RAM only).
#     This data will remain accessible to all sessions until the client disconnects.
#   - Clients can "subscribe" to server state information and be automatically
#     notified when the information has changed.
#
# CLIENT-TO-SERVER MESSAGE FORMAT SPECIFICATION:
#
# if 'what' is PR_COMMAND_SETPARAMETERS:
#    All fields of the message are placed into the session's parameter set.
#    Any fields in the previous parameters set with matching names are replaced.
#    Currently parsed parameter names are as follows:
#
#      "SUBSCRIBE:<path>" : Any parameter name that begins with the prefix SUBSCRIBE:
#                           is taken to represent a request to monitor the contents of
#                           all nodes whose pathnames match the path that follows.  So,
#                           for example, the parameter name
#
#                                SUBSCRIBE:/*/*/Joe
#
#                           Indicates a request to watch all nodes with paths that match
#                           the regular expression /*/*/Joe.  The value 
#                           these parameters are unimportant, and are not looked at.
#                           Thus, these parameters may be of any type.  Once a SUBSCRIBE
#                           parameter has been added, any data nodes that match the specified
#                           path will be returned immediately to the client in a PR_RESULT_DATAITEMS
#                           message.  Furthermore, any time these nodes are modified or deleted,
#                           or any time a new node is added that matches the path, another 
#                           PR_RESULT_DATAITEMS message will be sent to notify the client of
#                           the change.
#
#      PR_NAME_KEYS : If set, any non-"special" messages without a
#                     PR_NAME_KEYS field will be reflected to clients
#                     who match at least one of the set of key-paths
#                     listed here.  (Should be one or more string values)
#
#      PR_NAME_REFLECT_TO_SELF : If set, wildcard matches can match the current session.
#                                Otherwise, wildcard matches with the current session will
#                                be suppressed (on the grounds that your client already knows
#                                the value of anything that it uploaded, and doesn't need to
#                                be reminded of it).  This field may be of any type, only
#                                its existence/non-existence is relevant.
#      
#
# if 'what' is PR_COMMAND_GETPARAMETERS:
#    Causes a PR_RESULT_PARAMETERS message to be returned to the client.  The returned message
#    contains the entire current parameter set.
#
# if 'what' is PR_COMMAND_REMOVEPARAMETERS:
#    The session looks for PR_NAME_KEYS string entrys.  For each string found
#    under this entry, any matching field in the parameters message are deleted.
#    Wildcards are permitted in these strings.  (So e.g. "*" would remove ALL parameters)
#
# if 'what' is PR_COMMAND_SETDATA:
#    Scans the message for all fields of type message.  Each message field
#    should contain only one message.  The field's name is parsed as a local 
#    key-path of the data item (e.g. "myData", or "imageInfo/colors/red").  
#    Each contained message will be stored in the local session's data tree under 
#    that key path.  (Note:  fields that start with a '/' are not allowed, and
#    will be ignored!)
#
# if 'what' is PR_COMMAND_REMOVEDATA:
#    Removes all data nodes that match the path(s) in the PR_NAME_KEYS string field.
#    Paths should be specified relative to this session's root node (i.e. they should
#    not start with a slash)
#
# if 'what' is PR_COMMAND_GETDATA:
#    The session looks for one or more strings in the PR_NAME_KEYS field.  Each
#    string represents a key-path indicating which information the client is 
#    interested in retrieving.  If there is no leading slash, "/*/*/" will be
#    implicitely prepended.  Here are some valid example key-paths:
#         /*/*/color  (gets "color" from all hostnames, all session IDs)
#         /joe.blow.com/*/shape (gets "shape" from all sessions connected from joe.blow.com)
#         /joe.blow.com/19435935093/sound (gets "sound" from a single session)
#         /*/*/vehicleTypes/* (gets all vehicle types from all clients)
#         j* (equivalent to "/*/*/j*")
#         shape/* (equivalent to "/*/*/shape/*")
#    The union of all the sets of matching data nodes specified by these paths will be
#    added to a single PR_RESULT_DATAITEMS message which is then passed back to the client.  
#    Each matching message is added with its full path as a field name.  
# 
# if 'what' is PR_COMMAND_INSERTORDEREDDATA:
#    The session looks for one or more messages in the PR_NAME_KEYS field.  Each
#    string represents a wildpath, rooted at this session's node (read: no leadin
#    slash should be present) that specifies zero or more data nodes to insert ordered/
#    indexed children under.  Each node in the union of these node sets will have new
#    ordered/indexed child nodes created underneath it.  The names of these new child 
#    nodes will be chosen algorithmically by the server.  There will be one child node 
#    created for each sub-message in this message.  Sub-messages may be added under any 
#    field name if the field name happens to be the name of a currently indexed child,
#    the new message node will be be inserted *before* the specified child in the index.
#    Otherwise, it will be appended to the end of the index.  Clients who have subscribed
#    to the specified nodes will see the updates to the index clients who have subscribed
#    to the children will get updates of the actual data as well.
#
# if 'what' is PR_COMMAND_PING:
#    The session will change the message's 'what' code to PR_RESULT_PONG, and send
#    it right back to the client.  In this way, the client can find out (a) that
#    the server is still alive, (b) how long it takes the server to respond, and
#    (c) that any previously sent operations have finished.
#
# if 'what' is PR_COMMAND_KICK:
#    The server will look for one or more strings in the PR_NAME_KEYS field.  It will
#    do a search of the database for any nodes matching one or more of the node paths 
#    specified by these strings, and will kick any session with matching nodes off
#    of the server.  Of course, this will only be done if the client who sent the
#    PR_COMMAND_KICK field has PR_PRIVILEGE_KICK access.
#
# if 'what' is PR_COMMAND_ADDBANS: 
#    The server will look for one or more strings in the PR_NAME_KEYS field.  Any
#    strings that are found will be added to the server's "banned IP list", and
#    subsequent connection attempts from IP addresses matching any of these ban strings
#    will be denied.  Of course, this will only be done if the client who sent the
#    PR_COMMAND_ADDBANS field has PR_PRIVILEGE_ADDBANS access.
#
# if 'what' is PR_COMMAND_REMOVEBANS:
#    The server will look for one or more strings in the PR_NAME_KEYS field.  Any
#    strings that are found will be used a pattern-matching strings against the
#    current set of "ban" patterns.  Any "ban" patterns that are matched by any
#    of the PR_NAME_KEYS strings will be removed from the "banned IP patterns" set,
#    so that IP addresses who matched those patterns will be able to connect to
#    the server again.  Of course, this will only be done if the client who sent the
#    PR_COMMAND_REMOVEBANS field has PR_PRIVILEGE_REMOVEBANS access.
#
# if 'what' is PR_COMMAND_RESERVED_*:
#    The server will change the 'what' code of your message to PR_RESULT_UNIMPLEMENTED,
#    and send it back to your client.
#
# if 'what' is PR_RESULT_*:
#    The message will be silently dropped.  You are not allowed to send PR_RESULT_(*)
#    messages to the server, and should be very ashamed of yourself for even thinking 
#    about it.
#
# All other 'what' codes
#    Messages with other 'what' codes are simply reflected to other clients verbatim.
#    If a PR_NAME_KEYS string field is found in the message, then it will be parsed as a
#    set of key-paths, and only other clients who can match at least one of these key-paths
#    will receive the message.  If no PR_NAME_KEYS field is found, then the parameter
#    named PR_NAME_KEYS will be used as a default value.  If that parameter isn't
#    found, then the message will be reflected to all clients (except this one).
#
# SERVER-TO-CLIENT MESSAGE FORMAT SPECIFICATION:
#
# if 'what' is PR_RESULT_PARAMETERS:
#    The message contains the complete parameter set that is currently associated with
#    this client on the server.  Parameters may have any field name, and be of any type.
#    Certain parameter names are recognized by the StorageReflectSession as having special
#    meaning see the documentation on PR_COMMAND_SETPARAMETERS for information about these.
#
# if 'what' is PR_RESULT_DATAITEMS:
#    The message contains information about data that is stored on the server.  All stored
#    data is stored in the form of Messages.  Thus, all data in this message will be
#    in the form of a message field, with the field's name being the fully qualified path
#    of the node it represents (e.g. "/my.computer.com/5/MyNodeName") and the value bein
#    the stored data itself.  Occasionally it is necessary to inform the client that a data
#    node has been deleted this is done by adding the deceased node's path name as a string 
#    to the PR_NAME_REMOVED_DATAITEM field.  If multiple nodes were removed, there may be
#    more than one string present in the PR_NAME_REMOVED_DATAITEM field.
#
# if 'what' is PR_RESULT_INDEXUPDATED:
#    The message contains information about index entries that were added (via PR_COMMAND_INSERTORDERREDDATA)
#    to a node that the client is subscribed to.  Each entry's field name is the fully qualified
#    path of a subscribed node, and the value(s) are strings of this format:  "%c%lu:%s", %c is
#    a single character that is one of the INDEX_OP_* values, the %lu is an index the item was added to
#    or removed from, and %s is the key string for the child in question.
#    Note that there may be more than one value per field!
#
# if 'what' is PR_RESULT_ERRORUNIMPLEMENTED:
#    Your message is being returned to you because it tried to use functionality that
#    hasn't been implemented on the server side.  This usually happens if you are trying
#    to use a new MUSCLE feature with an old MUSCLE server.
#
# if 'what' is PR_RESULT_PONG:
#    Your PR_COMMAND_PING message has been returned to you as a PR_RESULT_PONG message.
#
# if 'what' is PR_RESULT_ERRORACCESSDENIED:
#    You tried to do something that you don't have permission to do (such as kick, ban,
#    or unban another user).
#
# if 'what' is anything else:
#    This message was reflected to your client by a neighboring client session.  The content
#    of the message is not specified by the StorageReflectSession it just passes any message
#    on verbatim.
