#include "createContext.h"
#include <iostream>
#include <string>
#include <CL/opencl.h>
#include <Rcpp.h>

// Stuff to expose the cl_platform_id to R
static void cl_platform_idObjFinalizer(SEXP ref){
	if(TYPEOF(ref) == EXTPTRSXP){
		cl_platform_id *o = static_cast<cl_platform_id*> (R_ExternalPtrAddr(ref));
		if (o) delete o;
	}
}

SEXP cl_platform_id2SEXP(cl_platform_id *o) {
	SEXP xp = R_MakeExternalPtr(o, R_NilValue, R_NilValue);
	R_RegisterCFinalizerEx(xp, cl_platform_idObjFinalizer, TRUE);
	return xp;
}

cl_platform_id *SEXP2cl_platform_id(SEXP o){
	if(TYPEOF(o) != EXTPTRSXP)
		Rf_error("invalid object");
	return (cl_platform_id*) R_ExternalPtrAddr(o);
}

// Stuff to expose the cl_device_id to R
static void cl_device_idObjFinalizer(SEXP ref){
	if(TYPEOF(ref) == EXTPTRSXP){
		cl_device_id *o = static_cast<cl_device_id*> (R_ExternalPtrAddr(ref));
		if (o) delete o;
	}
}

SEXP cl_device_id2EXP(cl_device_id *o){
	SEXP xp = R_MakeExternalPtr(o, R_NilValue, R_NilValue);
	R_RegisterCFinalizerEx(xp, cl_device_idObjFinalizer, TRUE);
	return xp;
}

cl_device_id *SEXP2cl_device_id(SEXP o){
	if(TYPEOF(o) != EXTPTRSXP)
		Rf_error("invalid object");
	return (cl_device_id*) R_ExternalPtrAddr(o);
}

// Stuff to expose the cl_context to R
static void cl_contextObjFinalizer(SEXP ref){
	if(TYPEOF(ref) == EXTPTRSXP){
		cl_context *o = static_cast<cl_context*> (R_ExternalPtrAddr(ref));
		if (o) delete o;
	}
}

SEXP cl_context2EXP(cl_context *o){
	SEXP xp = R_MakeExternalPtr(o, R_NilValue, R_NilValue);
	R_RegisterCFinalizerEx(xp, cl_contextObjFinalizer, TRUE);
	return xp;
}

cl_context *SEXP2cl_context(SEXP o){
    if(TYPEOF(o) != EXTPTRSXP)
		Rf_error("invalid object");
	return (cl_context*) R_ExternalPtrAddr(o);
}

// Stuff to expose the cl_command_queue to R
static void cl_command_queueObjFinalizer(SEXP ref){
	if(TYPEOF(ref) == EXTPTRSXP){
		cl_command_queue *o = static_cast<cl_command_queue*> (R_ExternalPtrAddr(ref));
		if (o) delete o;
	}
}

SEXP cl_command_queue2EXP(cl_command_queue *o){
	SEXP xp = R_MakeExternalPtr(o, R_NilValue, R_NilValue);
	R_RegisterCFinalizerEx(xp, cl_command_queueObjFinalizer, TRUE);
	return xp;
}

cl_command_queue *SEXP2cl_command_queue(SEXP o){
    if(TYPEOF(o) != EXTPTRSXP)
		Rf_error("invalid object");
	return (cl_command_queue*) R_ExternalPtrAddr(o);
}

SEXP getPlatformIDs(){
    //returns a list of platform ids
    cl_uint num_platforms = 0;
    clGetPlatformIDs(0, 0, &num_platforms);
    cl_platform_id *platforms = new cl_platform_id[ num_platforms ];
    clGetPlatformIDs(num_platforms, &platforms[0], &num_platforms);
    //for each platform in platforms add its pointer to the return list
    Rcpp::List result(num_platforms);
    for (int i=0; i<num_platforms; i++){
        //~ std::cout << platforms[i];
        cl_platform_id tempPlatformID = platforms[i];
        result[i] = cl_platform_id2EXP(&tempPlatformID);
        //~ result[i] = cl_platform_id2EXP(&platforms[i]);
        char cBuffer[1024];
        clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(cBuffer), cBuffer, NULL);
        //~ clGetPlatformInfo(*SEXP2cl_platform_id(result[i]), CL_PLATFORM_NAME, sizeof(cBuffer), cBuffer, NULL);
        std::cout << cBuffer;
    }
    return result;
}
/*
SEXP getPlatformIDs(){
	//returns a list of platform ids
	cl_uint num_platforms = 0;
	clGetPlatformIDs(0, 0, &num_platforms);
	std::vector<cl_platform_id> platforms(num_platforms);
	clGetPlatformIDs(num_platforms, platforms.empty() ? NULL : &platforms.front(), &num_platforms);
	//for each platform in platforms add its pointer to the return list
	Rcpp::List result(platforms.size());
	for (int i=0; i<platforms.size(); i++){
        //~ cl_platform_id *tempPlatformID = new cl_platform_id;
        //~ tempPlatformID = &platforms[i];
        std::cout << platforms[i];
		result[i] = cl_platform_id2EXP(platforms[i]);
	}
	return result;
}*/

SEXP getPlatformIDs2(){
	//returns a list of platform ids
	cl_uint num_platforms = 0;
	clGetPlatformIDs(0, 0, &num_platforms);
	std::vector<cl_platform_id> platforms(num_platforms);
	clGetPlatformIDs(num_platforms, platforms.empty() ? NULL : &platforms.front(), &num_platforms);
	char cBuffer[1024];
    clGetPlatformInfo (platforms[0], CL_PLATFORM_NAME, sizeof(cBuffer), cBuffer, NULL);
    //~ Rcpp::CharacterVector ab(1);
    std:string retStr = cBuffer;
    //~ ab[0] = cBuffer;
    return wrap(retStr);
}

SEXP getPlatformName(SEXP sPlatformID){
    char cBuffer[1024];
    cl_platform_id platformID = *SEXP2cl_platform_id(sPlatformID);
    std::cout << platformID;
    clGetPlatformInfo (platformID, CL_PLATFORM_NAME, sizeof(cBuffer), cBuffer, NULL);
    std::cout << "blah";
    Rcpp::CharacterVector ab(1);
    std::cout << "blah1";
    ab[0] = cBuffer;
    std::cout << "blah2";
    return ab;
}

SEXP getDeviceIDs(SEXP sPlatformID){
    //returns a list of device ids
	cl_uint num_devices = 0;
	std::vector<cl_device_id> devices(num_devices);
	clGetDeviceIDs(*SEXP2cl_platform_id(sPlatformID), 	CL_DEVICE_TYPE_GPU, num_devices, devices.empty() ? NULL : &devices.front(), &num_devices);
	//for each device in devices add its pointer to the return list
	Rcpp::List result(devices.size());
	std::vector<cl_device_id>::iterator itr;
	for(itr = devices.begin(); itr != devices.end(); ++itr){
		result.push_back(cl_device_id2EXP(&*itr));
	}
	return result;
}

/*
SEXP createContext(){
    cl_context clCreateContext(NULL, 	cl_uint num_devices, 	const cl_device_id *devices, 	(voidCL_CALLBACK  *pfn_notify) (const char *errinfo, const void *private_info, size_t cb, void *user_data), void *user_data, cl_int *errcode_ret)
}
*/

SEXP createContextFromType(){
    //returns an OpenCL context
    cl_context context = clCreateContextFromType(NULL, CL_DEVICE_TYPE_GPU, NULL, NULL, NULL);
    SEXP sContext = cl_context2EXP(&context);
	return sContext;
}

SEXP createCommandQueue(SEXP sContext){
    //returns an OpenCL command queue
    cl_command_queue queue = clCreateCommandQueue(*SEXP2cl_context(sContext), NULL, 0, NULL);
    SEXP sQueue = cl_command_queue2EXP(&queue);
	return sQueue;
}