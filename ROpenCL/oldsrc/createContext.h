#ifndef _ROpenCL_CREATECONTEXT_H
#define _ROpenCL_CREATECONTEXT_H

#include <CL/opencl.h>
#include <Rcpp.h>

/*
 * note : RcppExport is an alias to `extern "C"` defined by Rcpp.
 *
 * It gives C calling convention to the rcpp_hello_world function so that 
 * it can be called from .Call in R. Otherwise, the C++ compiler mangles the 
 * name of the function and .Call can't find it.
 *
 * It is only useful to use RcppExport when the function is intended to be called
 * by .Call. See the thread http://thread.gmane.org/gmane.comp.lang.r.rcpp/649/focus=672
 * on Rcpp-devel for a misuse of RcppExport
 */
RcppExport SEXP getPlatformIDs();
RcppExport SEXP getDeviceIDs(SEXP sPlatformID);
RcppExport SEXP createContextFromType();
RcppExport SEXP createCommandQueue(SEXP sContext);
RcppExport SEXP getPlatformName(SEXP sPlatformID);
RcppExport SEXP getPlatformIDs2();

static void cl_platform_idObjFinalizer(SEXP ref);
SEXP cl_platform_id2EXP(cl_platform_id *o);
cl_platform_id *SEXP2cl_platform_id(SEXP o);

static void cl_device_idObjFinalizer(SEXP ref);
SEXP cl_device_id2EXP(cl_device_id *o);
cl_device_id *SEXP2cl_device_id(SEXP o);

static void cl_contextObjFinalizer(SEXP ref);
SEXP cl_context2EXP(cl_context *o);
cl_context *SEXP2cl_context(SEXP o);

static void cl_command_queueObjFinalizer(SEXP ref);
SEXP cl_command_queue2EXP(cl_command_queue *o);
cl_command_queue *SEXP2cl_command_queue(SEXP o);

#endif
