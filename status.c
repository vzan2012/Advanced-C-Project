/*** status.c source code. By Sitharaman Deepak Guptha. November 2018 ***/

#include "status.h"

/*** Default error message for unknown errors ***/
static char* unknownError = "Unknown error";

/*** Defined error messages ***/
static char* msg[] = {
  "",
  "File open failed",
  "Access refused",
  "File close failed",
  "Memory allocation failed",
  "Wrong date",
  "Full structure",
  "Empty structure",
  "Value not found",
  "Value already exists",
  "index out of bounds",
  "unable to perform operation",

  "unknown error"
};

/***
 * Get message associated with the given status value (O(1)).
 * @param s the status value
 * @return the associated message
 */
char* message(status s) {
  return (s<0 || s >= ERRUNKNOWN) ? msg[ERRUNKNOWN] : msg[s];
}
