/*    (c) Copyright Hewlett-Packard Company, 1998.  All rights are reserved.
      Copying or other  reproduction  of this  program  except for  archival
      purposes  is  prohibited   without  the  prior   written   consent  of
      Hewlett-Packard Company.


                             RESTRICTED RIGHTS LEGEND

      Use,  duplication, or disclosure by the U.S.  Government is subject to
      restrictions as set forth in subdivision (b) (3) (ii) of the Rights in
      Technical Data and Computer Software clause at 52.227-7013.

                  HEWLETT-PACKARD COMPANY Fort Collins, Colorado
*/


/***************************************************************************************
 ***************************************************************************************

      THIS HEADER FILE AND THE TYPES DEFINED WITHIN ARE FOR HP INTERNAL USE ONLY.
          THIS HEADER FILE SHOULD *NOT* BE ADDED TO THE INSTALL DATABASE.  

   ************************************************************************************* 
   *************************************************************************************/


#if !defined(GLHOOKS_H)
#define GLHOOKS_H

#include <GL/gl.h>
#include <GL/glx.h>

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************************************************************
   PROTOTYPES FOR HOOKS CLIENT PROVIDED FUNCTIONS.
   *************************************************************************************
   DO _NOT_ ATTEMPT TO CALL THESE FUNCTIONS!  THESE PROTOTYPES AND COMMENTS ARE FOR
   DOCUMENTATION PURPOSES ONLY, AND DO NOT REPRESENT CALLABLE FUNCTION SYMBOLS.
   *************************************************************************************
   These three functions are required to be implemented in and exported from any
   client shared library that wishes to attach to the OpenGL Hooks interface.  The
   client library is specified by setting the environment variable HPOGL_GLHKS_LIBRARY
   to the library:
  
   export HPOGL_GLHKS_LIBRARY=/home/me/libHooksClientLib.sl

   The three functions are dynamically loaded from the client library, and called by
   the Hooks interface at the appropriate times.  They are not callable from any
   other code. */

void glhksClientInitialize(void);
/* This function is called when the OpenGL library is initialized (at application startup
   time).  Its purpose is to allow the client library to initialize itself before any API
   calls have been made */

void glhksClientPing(void);
/* This function is called periodically while OpenGL is executing, usually on a frame
   boundary, or some other "safe" and non-performance critical time.  Its purpose is to
   give the client library a periodic opportunity to enable or disable the Hooks
   callbacks, or to do anything it needs to do periodically during a non-performance
   sensitive time. 

   It should not be assumed that glhksClientPing will be called as part
   of or as a result of any particular or specific OpenGL entry point. However,
   currently, Ping is called as a result of glClear and glXSwapBuffers. The work
   done in or as a result of Ping may skew the results if individual function
   execution is being time.
   */

/***************************************************************************************
   THE REST OF THIS HEADER FILE REPRESENTS THE CALLABLE HOOKS INTERFACE.
 ***************************************************************************************/
    
/***************************************************************************************
   MAIN HOOKS ENABLE/DISABLE FUNCTIONS
   *************************************************************************************
   These functions enable or disable the Hooks capability of the OpenGL library.  If Hooks
   are disabled, no callback functions will be called, but the external Hooks tool
   library's "glhksClientPing" function will occasionally be called to give the tool the
   ability to re-enable the Hooks functionality.  Enabling the Hooks functionality will
   extract a significant performance penalty from all calls to OpenGL, so it's a good idea
   to disable Hooks when they're not being used. */

void glhksEnableHooks(void);
void glhksDisableHooks(void);

/***************************************************************************************
   ENABLE/DISABLE CAPTURE RECURSION
   *************************************************************************************
   In HP OGL, some of the OGL functions call other OGL functions using the externally
   visable entry points.  Capturing these calls may or may not be desirable.  The
   following functions enable/disable the capture of OGL functions call from within
   other functions. */

void glhksEnableCaptureRecursion(void);
void glhksDisableCaptureRecursion(void);


/***************************************************************************************
   CALLBACK MANAGEMENT FUNCTIONS
   *************************************************************************************

   Calling the "glhksSet[*]Callback" functions will cause the passed callback functions to
   be called at appropriate intervals.  Passing a value of FNULL in cbk will disable any
   previously set callback function.  "context" is a pointer to storage/data determined by
   the client library.  The value passed in "context" is returned unmodified
   as the last parameter to the callback function provided in "cbk". */

/***********************************************************
   Types associated with glhksSetEventCallback.
*/
typedef enum 
{
    eAPIFunctionCall = 0x0001,
    /*   For this event, "eventData" points to a "GLHksFunctionCall" structure.
         
       This event is generated when a call to an API function has been encountered by the
       OpenGL library.  If the event callback returns "false" (the event wasn't handled)
       then the OpenGL library will immediately call the "real" OpenGL function, and
       generate an "eAPIFunctionReturn" event when it returns.  If the callback has
       handled the event, and doesn't wish for the OpenGL library to call the "real"
       function, it should return "true" to signify that it has completely handled the
       event.  In this case, no other events will be generated, or processing done
       pertaining to the particular API function call. */
    
    eAPIFunctionReturn = 0x0002,
    /*   For this event, "eventData" points to a "GLHksFunctionCall" structure.
         
       This event signals that an API function has finished executing and returned.  This
       event, with the "eAPIFunctionCall" event, can be used to collect timing data. */

    ePipelineStateRecook = 0x0004,
    /*   For this event, "eventData" points to a "GLHksPipelineState" struct.
         
       This event is generated whenever the current state conditions have changed, and an
       API call forces the OpenGL library to update and "re-cook" that state. */
    
    eSlowPathScheduled = 0x0008,
    /*   For this event, "eventData" points to a "GLHksSlowPathReason" bit-vector.
         
       This event is generated whenever the current state conditions change, and force a
       sub-optimal rendering path to be scheduled for execution. */
    
    eFastPathScheduled = 0x0010,
    /*   For this event, "eventData" is a NULL pointer.
         
       This event is generated whenever the current state conditions change, and allow the
       most optimal available rendering path to be scheduled for execution. */

    eAPIError = 0x0020,
    /*   For this event, "eventData" is a single enum field containing the enumeration
         value of the GL error that has occured.
      
        This event is generated whenever a GL error occurs during the execution
        of a API function call. */
         
    eAllEventsMask = 0xFFFF,
    eNoEventsMask = 0x0000
    
} GLHksEvent;
    
typedef const char *GLHksParmsFormat;
/* This is a string that describes the format of a function's formal parameters.  The
   first described parameter represents the return value of the function, then the formal
   parameters follow in order from left to right.  Each character in the string describes
   a single parameter (or how to interpret the next character) as described below:

   Character        Meaning
   ---------        -------
   "*"              The following character should be interpreted as a pointer to the
                    associated type (this is recursive, ** means pointer to pointer).
   "+\x0n+"         This is a string of three characters.  It means that the previous
                    pointer "*" points to a size-\x0n array of the following associated
                    type. The '+' on the left-hand is signal that the next character can 
                    be converted directly to int to get the size of the array.  The right-hand
                    '+' is to delimit the hex value because in C hex numbers can be represented
                    by any number of hex digits.
   "e"              GLenum
   "o"              GLboolean (or Bool)
   "t"              GLbitfield
   "b"              GLbyte (or char)
   "B"              GLubyte (or unsigned char)
   "s"              GLshort (or short)
   "S"              GLushort (or unsigned short)
   "i"              GLint (or int)
   "I"              GLuint (or unsigned int)
   "z"              GLsizei (or sizei)
   "f"              GLfloat (or float)
   "d"              GLdouble (or double)
   "c"              GLclampf
   "C"              GLclampd
   "v"              GLvoid (or void)
   "l"              long
   "L"              unsigned long
   "O"              GLXContext
   "D"              GLXDrawable
   "P"              GLXPixmap
   "p"              Pixmap
   "n"              Font
   "V"              XVisualInfo
   "a"              Display
*/

#if defined(__cplusplus) || defined(BOOL_DEFINED)
typedef bool GLHksBool;
#else
typedef enum {false, true} GLHksBool;
#endif
/* Just a simple boolean true or false value. */

typedef GLuint GLHksFuncID;
#define GLHKS_BOGUS_FUNC_ID ((GLHksFuncID)0xFFFFFFFF)
/* This is an opaque id type for a particular function.  You must pass one of these ids to
   the Hooks interface to obtain any further information about the associated API function.

   Well, OK, it's not _that_ opaque.  The ID for a function is actually an unsigned
   integer value between 0 and glhksGetMaxFuncID().  So you can go ahead and use it as an
   index into an array, or whatever..
*/

typedef void (*GLHksFuncPtr)(void);
/* This is a simple generic function pointer used by the function call event to pass the
   function pointer of the "real" API entrypoint. */

/* Define a special function version of NULL. */
#if defined(__cplusplus)
/* *%&^^&@# new C++ pointer rules! */
#if defined(_HPUX_SOURCE)
const GLHksFuncPtr FNULL = reinterpret_cast<GLHksFuncPtr>(NULL);
#else
const GLHksFuncPtr FNULL = (GLHksFuncPtr)(NULL);
#endif
#else
#define FNULL NULL
#endif

/* Type of the function pointer passed to "glhksSetEventCallback" to handle events. */
typedef GLHksBool (*GLHksEventCallback)(GLHksEvent event, void *eventData, void *context);

/************************************************************/
void glhksSetEventCallback(GLHksEventCallback cbk, GLHksEvent mask,  void *context);
/* This callback will be called to report an event generated by the OpenGL library.
   Possible events are described above in the "GLHksEvent" type definition.  "mask" is a
   bitfield that consists of all events that the callback is interested in receiving,
   bitwise or'ed together.  Note that only a single event callback can be active at any
   given time.  Although that's somewhat restrictive, it's limited in order to reduce the
   performance penalty and complexity inherent in managing multiple event handlers.

   ****************************************************************************
   FAST PROCEDURE CALLING CONVENTIONS ###### IMPORTANT!!! ######
   ****************************************************************************
   Both GLHksEventCallback and GLHksAPIInterceptCallbacks must be called using the
   fast procedure calling mechanism.  This is indicated by using the following two
   pragmas:

   #pragma HP_LONG_RETURN callbackProcedureName
   #pragma HP_NO_RELOCATION callbackProcedureName
 
   These pragmas must be placed in your source code _after_ the callback
   prototype definition and _before_ the callback is defined.

   The callback routine should return a GLHksBool value (true or false) that indicates
   whether or not it "handled" the event.  If the callback desires for the OpenGL library
   to finish processing the event (i.e. calling the "real" API function, and possibly
   generating other function call related events in the case of an "eAPIFunctionCall"
   event, for example), it must return a value of "false".  If it returns a value of
   "true", the OpenGL library will consider the event "handled", and will do no further
   processing related to that event. */
   

typedef struct GLHksFunctionCallStruct GLHksFunctionCall;
typedef void (*GLHksAPIInterceptCallback)(GLHksFunctionCall *data, void *context);

/************************************************************/
void glhksSetAPIInterceptCallback(GLHksFuncID id, GLHksAPIInterceptCallback cbk, void *context);
/* This callback is unique in that if it is enabled, the OpenGL library will never execute
   the "real" API function, nor will it generate any GLHksEvent events or call any
   other callbacks associated with the indicated function.  Instead, it is up to the intercept
   callback function "cbk" to call the API function if desired.  Another unique aspect of
   this callback is that it is enabled/disabled on a function-by-function basis, rather
   than globally for all API functions, like the event callback is.  This is useful, for
   example, if you want to set up an event handler that can handle most API function
   calls, then for the few API calls that can't be handled by this code, provide specific
   handler functions one-by-one.

   ****************************************************************************
   FAST PROCEDURE CALLING CONVENTIONS ###### IMPORTANT!!! ######
   ****************************************************************************
   Both GLHksEventCallback and GLHksAPIInterceptCallbacks must be called using the
   faster procedure calling mechanism.  This is indicated by using the following two
   pragmas:

   #pragma HP_LONG_RETURN callbackProcedureName
   #pragma HP_NO_RELOCATION callbackProcedureName
 
   The pragmas must come after the callback prototype definition and before the 
   callback is defined.

   Furthermore, if the "real" API function is called from within the intercept
   callback the following pragma must be defined to insure that the correct calling
   mechanism is used:

   #pragma _HP_OPT_FUNC_PTR realAPIFunction
*/


/***************************************************************************************
   UTILITY FUNCTIONS
   *********************************************************/
void glhksGetPipelineState(void);
/* Calling this routine will initiate a stream of events that represent the
   current OpenGL pipeline control state (things like the current render path,
   lighting space, etc...). */

GLHksFuncID glhksGetMaxFuncID(void);
/* Returns the highest valid GLHksFuncID index.  This might be useful if you wanted to
   build a table with entries for each function, the use the function's ID to index into
   the table.  Or it might not be useful at all, who knows? */

GLHksFuncID glhksGetFuncID(char *funcName);
/* Returns the GLHksFuncID associated with the function name.  If no such function can be
   found, returns GLHKS_BOGUS_FUNC_ID. */

const char *glhksGetFuncName(GLHksFuncID id);
/* Returns the name of the OpenGL API function associated with "id".  If "id" is not a
   valid GLHksFuncID, returns NULL. */

const GLHksParmsFormat glhksGetFuncArgsDescriptor(GLHksFuncID id);
/* Returns a string that describes the formal parameters for the function associated with
   "id".  If "id" is not a valid GLHksFuncID, returns NULL.*/

const GLHksFuncPtr glhksGetFuncPtr(GLHksFuncID id);
/* Returns a pointer to the API function represented by "id".  Calling this function will
   not generate any events by itself, but it may generate events as a side effect (calling
   glCallList for example, may generate events from the contents of the list. Returns
   FNULL if "id "is not a valid GLHksFuncID. */

#if defined(__cplusplus)
}
#endif

/***************************************************************************************
  Following are type definitions for the data associated with each type of event.  The
  "eventData" pointer that is passed to the event handler callback is a pointer to one of
  these types, depending on which event is generated.  The function call data type
  described below is also used by the intercept callback (more info below). */
  
/* "ePipelineStateRecook" type:
   
   This struct holds the pieces of "cooked" OpenGL state that are interesting to the
   client ("interesting" is defined rather arbitrarily here ;oD). */
typedef struct
{
    enum {
        eInvalidLightingSpace,  /* Lighting is not enabled, or no lighting operator has
                                   been scheduled yet. */
        
        eObjectLightingSpace,   /* Lighting calculations will be performed in object
                                   space, previous to the eye space transformation. */
        
        eEyeLightingSpace       /* Lighting calculations will be performed after the
                                   transformation to eye space. */
    } lightingSpace;
    
} GLHksPipelineState;


/* "eSlowPathScheduled" type:
   
   Enumerated bit masks for why a sub-optimal rendering path was scheduled. */
typedef enum
{
    eNoGeometryHardwareMask = 0x0001,
    /* This bit indicates that there is no geometry accelerated hardware available (or
       that an environment variable has been set to indicate that geometry accelerated
       hardware should not be used).  General Path is used when there is no 
       geometry hardware and is scheduled by default.  Because GP is scheduled by default,
       this event may never be reported. */
    
    eNoTextureHardwareMask = 0x0002,
    /* This bit indicates that the application is using OpenGL texture mapping, but
       that no texture mapping hardware is available (or that an environment variable
       has been set to indicate that texture hardware should not be used). */

    eFeedbackModeEnabledMask = 0x0004,
    /* This bit indicates that feedback mode is enabled (and that feedback isn't
       accelerated by the graphics hardware). */
    
    eSelectionModeEnabledMask = 0x0008,
    /* This bit indicates that selection mode is enabled (and that selection isn't
       accelerated by the graphics hardware). */

    eIndexedLightingEnabledMask = 0x0010,
    /* This bit indicates that the application is using lighting with an indexed
       (pseudo-color) visual, and that this combination is not supported by the
       graphics hardware. */

    eIndexedFogEnabledMask = 0x0020,
    /* This bit indicates that the application is using fog with an indexed
       (pseudo-color) visual, and that this combination is not supported by the
       graphics hardware. */

    eNoReasonsMask = 0x0000,
    /* If no bits are set, it indicates that the general path is simply scheduled by
       default.  In other words, GP is the default path when no event has occurred to
       cause the rendering path to be rescheduled. */
    
    eAllReasonsMask = 0xFFFF
    /* This is just for convenience. */

} GLHksSlowPathReason;


/* This struct contains information that is generally useful when a function call is
   encountered.  Note that this type is not only passed to the "GLHksAPIInterceptCallback"
   function, but is also passed to the "GLHksEventCallback" when the event is of type
   "eAPIFunctionCall" or "eAPIFunctionReturn". */
struct GLHksFunctionCallStruct
{
    GLHksFuncID id;     /* ID of the function that was called. */
    GLHksFuncPtr fptr;  /* Pointer to associated "real" API function. */
    void *parms[16];    /* An array of pointers which point to parameter storage memory
                           for the given function call.  If the function has more than 15
                           formal parameters, the last (14th) element will be a pointer to
                           another array which contains pointers to the remaining (14th ->
                           nth) parameters (currently <1998>, no OpenGL functions have
                           more than 11 parameters).  The return value storage (if any) is
                           always pointed to by the first (0th) element of this array.  If
                           the function has a void return type, the 0th element will
                           contain a NULL pointer. */
};

#endif /* !defined(GLHOOKS_H) */
