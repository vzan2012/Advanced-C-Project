/*** status.h source code. By Sitharaman Deepak Guptha. November 2018 ***/

#ifndef STATUS_H_INCLUDED
#define STATUS_H_INCLUDED

/**
 * User defined data types for the error messages
 */
typedef enum {
	OK,
	ERROPEN,
	ERRACCESS,
	ERRCLOSE,
	ERRALLOC,
	ERRDATE,
	ERRFULL,
	ERREMPTY,
	ERRABSENT,
	ERREXIST,
	ERRINDEX,
	ERRUNABLE,

	ERRUNKNOWN,
} status;

/**
 * Get message associated with the given status value (O(1)).
 * @param s the status value
 * @return the associated message
 */
extern char* message(status s);

#endif // STATUS_H_INCLUDED
