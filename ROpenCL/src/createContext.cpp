#include "createContext.h"
#include <iostream>
#include <CL/opencl.h>
#include <Rcpp.h>

SEXP getPlatformIDs(){
    //returns a list of platform ids
    cl_uint num_platforms = 0;
    clGetPlatformIDs(0, 0, &num_platforms);
    std::vector<cl_platform_id> platforms(num_platforms);
    clGetPlatformIDs(num_platforms, platforms.empty() ? NULL : &platforms.front(), &num_platforms);
    //for each platform in platforms add its pointer to the return list
    Rcpp::List result(platforms.size());
    for (int i=0; i<platforms.size(); i++){
        cl_platform_id *tempPlatformID = new cl_platform_id;
        *tempPlatformID = platforms[i];
         Rcpp::XPtr<cl_platform_id> tempXPtr(tempPlatformID);
        result[i] = tempXPtr;
    }
    return result;
}

SEXP getPlatformName(SEXP sPlatformID){
    char cBuffer[1024];
    Rcpp::XPtr<cl_platform_id> platformID(sPlatformID);
    clGetPlatformInfo (*platformID, CL_PLATFORM_NAME, sizeof(cBuffer), cBuffer, NULL);
    std::string retVal = cBuffer;
    return Rcpp::wrap(retVal);
}

SEXP getDeviceIDs(SEXP sPlatformID){
    Rcpp::XPtr<cl_platform_id> platformID(sPlatformID);
    cl_uint num_devices = 0;
    clGetDeviceIDs(*platformID, CL_DEVICE_TYPE_GPU, 0, 0, &num_devices);
    std::vector<cl_device_id> devices(num_devices);
    clGetDeviceIDs(*platformID, CL_DEVICE_TYPE_GPU, num_devices, devices.empty() ? NULL : &devices.front(), &num_devices);
    //for each platform in platforms add its pointer to the return list
    Rcpp::List result(devices.size());
    for (int i=0; i<devices.size(); i++){
        cl_device_id *tempDeviceID = new cl_device_id;
        *tempDeviceID = devices[i];
        Rcpp::XPtr<cl_device_id> tempXPtr(tempDeviceID);
        result[i] = tempXPtr;
    }
    return result;
}

SEXP createContext(SEXP sDeviceID){
    Rcpp::XPtr<cl_device_id> deviceID(sDeviceID);
    
    cl_context *context = new cl_context;
    *context = clCreateContext(0, 1, deviceID, NULL, NULL, NULL);
    Rcpp::XPtr<cl_context> tempXPtr(context);
    return tempXPtr;
}

SEXP createContextFromType(SEXP deviceType){
    //We need to look at this, because this does not seem to work...
    std::string deviceString = Rcpp::as<std::string>(deviceType);
    cl_context *context = new cl_context;
    if(deviceString == "CL_DEVICE_TYPE_GPU"){
        *context = clCreateContextFromType(NULL, CL_DEVICE_TYPE_GPU, NULL, NULL, NULL);
    }
    Rcpp::XPtr<cl_context> tempXPtr(context);
    return tempXPtr;
}

void getContextInfo(SEXP sContext, SEXP sContextInfo){
    //We need to look at this, because this does not seem to work...
    Rcpp::XPtr<cl_context> context(sContext);
    std::string contextInfo = Rcpp::as<std::string>(sContextInfo);
    
    if(contextInfo == "CL_CONTEXT_NUM_DEVICES"){
        cl_uint *nrDevices = new cl_uint;
        std::cout << "get Context number of devices\n";
        cl_int ciErr1 = clGetContextInfo(*context, CL_CONTEXT_NUM_DEVICES, sizeof(nrDevices), nrDevices, NULL);
        if (ciErr1 == CL_INVALID_CONTEXT){std::cout << "invalid context";};
        if (ciErr1 == CL_INVALID_VALUE){std::cout << "invalid value";};
        if (ciErr1 == CL_OUT_OF_RESOURCES){std::cout << "OOR";};
        if (ciErr1 == CL_OUT_OF_HOST_MEMORY){std::cout << "OOHM";};
        std::cout << *nrDevices;
        delete nrDevices;
    }
}

SEXP createCommandQueue(SEXP sContext, SEXP sDeviceID){
    Rcpp::XPtr<cl_context> context(sContext);
    Rcpp::XPtr<cl_device_id> deviceID(sDeviceID);
    
    cl_command_queue *queue = new cl_command_queue;
    *queue = clCreateCommandQueue(*context, *deviceID, 0, NULL);
    Rcpp::XPtr<cl_command_queue> tempXPtr(queue);
    return tempXPtr;
}

SEXP createBufferFloatVector(SEXP sContext, SEXP sMemFlag, SEXP sGlobalWorkSize){
    Rcpp::XPtr<cl_context> context(sContext);
    std::string memFlag = Rcpp::as<std::string>(sMemFlag);
    int globalWorkSize = Rcpp::as<int>(sGlobalWorkSize);
    
    cl_mem *memBuffer = new cl_mem;
    cl_int ciErr1;
    if(memFlag == "CL_MEM_READ_ONLY"){
        //clCreateBuffer(cxGPUContext, CL_MEM_READ_ONLY, sizeof(cl_float) * szGlobalWorkSize, NULL, &ciErr2);
        *memBuffer = clCreateBuffer(*context, CL_MEM_READ_ONLY, sizeof(cl_float) * globalWorkSize, NULL, &ciErr1);
        if (ciErr1 != CL_SUCCESS){ std::cout << "error creating buffer float vector\n"; };
        if (ciErr1 == CL_INVALID_CONTEXT){std::cout << "invalid context";};
        if (ciErr1 == CL_INVALID_VALUE){std::cout << "invalid value";};
        if (ciErr1 == CL_INVALID_BUFFER_SIZE){std::cout << "invalid buffer size";};
        if (ciErr1 == CL_INVALID_HOST_PTR){std::cout << "invalid host ptr";};
        if (ciErr1 == CL_MEM_OBJECT_ALLOCATION_FAILURE){std::cout << "mem object allocation fail";};
        if (ciErr1 == CL_OUT_OF_RESOURCES){std::cout << "OOR";};
        if (ciErr1 == CL_OUT_OF_HOST_MEMORY){std::cout << "OOHM";};
    }
    if(memFlag == "CL_MEM_WRITE_ONLY"){
        *memBuffer = clCreateBuffer(*context, CL_MEM_WRITE_ONLY, sizeof(cl_float) * globalWorkSize, NULL, NULL);
        if (ciErr1 != CL_SUCCESS){ std::cout << "error creating buffer float vector\n"; };
        if (ciErr1 == CL_INVALID_CONTEXT){std::cout << "invalid context";};
        if (ciErr1 == CL_INVALID_VALUE){std::cout << "invalid value";};
        if (ciErr1 == CL_INVALID_BUFFER_SIZE){std::cout << "invalid buffer size";};
        if (ciErr1 == CL_INVALID_HOST_PTR){std::cout << "invalid host ptr";};
        if (ciErr1 == CL_MEM_OBJECT_ALLOCATION_FAILURE){std::cout << "mem object allocation fail";};
        if (ciErr1 == CL_OUT_OF_RESOURCES){std::cout << "OOR";};
        if (ciErr1 == CL_OUT_OF_HOST_MEMORY){std::cout << "OOHM";};
    }
    Rcpp::XPtr<cl_mem> tempXPtr(memBuffer);
    return tempXPtr;
}

SEXP createBufferIntegerVector(SEXP sContext, SEXP sMemFlag, SEXP sGlobalWorkSize){
    Rcpp::XPtr<cl_context> context(sContext);
    std::string memFlag = Rcpp::as<std::string>(sMemFlag);
    int globalWorkSize = Rcpp::as<int>(sGlobalWorkSize);
    
    cl_mem *memBuffer = new cl_mem;
    cl_int ciErr1;
    if(memFlag == "CL_MEM_READ_ONLY"){
        //clCreateBuffer(cxGPUContext, CL_MEM_READ_ONLY, sizeof(cl_float) * szGlobalWorkSize, NULL, &ciErr2);
        *memBuffer = clCreateBuffer(*context, CL_MEM_READ_ONLY, sizeof(cl_int) * globalWorkSize, NULL, &ciErr1);
        if (ciErr1 != CL_SUCCESS){ std::cout << "error creating buffer float vector\n"; };
        if (ciErr1 == CL_INVALID_CONTEXT){std::cout << "invalid context";};
        if (ciErr1 == CL_INVALID_VALUE){std::cout << "invalid value";};
        if (ciErr1 == CL_INVALID_BUFFER_SIZE){std::cout << "invalid buffer size";};
        if (ciErr1 == CL_INVALID_HOST_PTR){std::cout << "invalid host ptr";};
        if (ciErr1 == CL_MEM_OBJECT_ALLOCATION_FAILURE){std::cout << "mem object allocation fail";};
        if (ciErr1 == CL_OUT_OF_RESOURCES){std::cout << "OOR";};
        if (ciErr1 == CL_OUT_OF_HOST_MEMORY){std::cout << "OOHM";};
    }
    if(memFlag == "CL_MEM_WRITE_ONLY"){
        *memBuffer = clCreateBuffer(*context, CL_MEM_WRITE_ONLY, sizeof(cl_int) * globalWorkSize, NULL, NULL);
        if (ciErr1 != CL_SUCCESS){ std::cout << "error creating buffer float vector\n"; };
        if (ciErr1 == CL_INVALID_CONTEXT){std::cout << "invalid context";};
        if (ciErr1 == CL_INVALID_VALUE){std::cout << "invalid value";};
        if (ciErr1 == CL_INVALID_BUFFER_SIZE){std::cout << "invalid buffer size";};
        if (ciErr1 == CL_INVALID_HOST_PTR){std::cout << "invalid host ptr";};
        if (ciErr1 == CL_MEM_OBJECT_ALLOCATION_FAILURE){std::cout << "mem object allocation fail";};
        if (ciErr1 == CL_OUT_OF_RESOURCES){std::cout << "OOR";};
        if (ciErr1 == CL_OUT_OF_HOST_MEMORY){std::cout << "OOHM";};
    }
    Rcpp::XPtr<cl_mem> tempXPtr(memBuffer);
    return tempXPtr;
}

SEXP createProgramWithSource(SEXP sContext, SEXP sKernelSrc){
    Rcpp::XPtr<cl_context> context(sContext);
    std::string kernelSrc = Rcpp::as<std::string>(sKernelSrc);
    const char* tmpKernelSrc = kernelSrc.data();
    cl_program *program = new cl_program;
    *program = clCreateProgramWithSource(*context, 1, &tmpKernelSrc, NULL, NULL);
    Rcpp::XPtr<cl_program> tempXPtr(program);
    return tempXPtr;
}

SEXP getProgramInfo(SEXP sProgram, SEXP sProgramInfo){
    Rcpp::XPtr<cl_program> program(sProgram);
    std::string programInfo = Rcpp::as<std::string>(sProgramInfo);
    char cBuffer[1024];
    
    if(programInfo == "CL_PROGRAM_SOURCE"){
        std::cout << "get Program source\n";
        clGetProgramInfo(*program, CL_PROGRAM_SOURCE,	sizeof(cBuffer), cBuffer, NULL);
    }
    std::string retVal = cBuffer;
    return Rcpp::wrap(retVal);
}

SEXP buildProgram(SEXP sProgram){
    Rcpp::XPtr<cl_program> program(sProgram);
    cl_int ciErr1 = clBuildProgram(*program, 0, NULL, NULL, NULL, NULL);
    if (ciErr1 != CL_SUCCESS)
    {
        std::cout << "error building program";
    }
    if (ciErr1 == CL_INVALID_PROGRAM){std::cout << "invalid program";};
    if (ciErr1 == CL_INVALID_VALUE){std::cout << "invalid value";};
    if (ciErr1 == CL_INVALID_DEVICE){std::cout << "invalid device";};
    if (ciErr1 == CL_INVALID_BUILD_OPTIONS){std::cout << "invalid build options";};
    if (ciErr1 == CL_INVALID_OPERATION){std::cout << "invalid operation";};
    if (ciErr1 == CL_COMPILER_NOT_AVAILABLE){std::cout << "compiler NA";};
    if (ciErr1 == CL_BUILD_PROGRAM_FAILURE){std::cout << "build program fail";};
    if (ciErr1 == CL_OUT_OF_RESOURCES){std::cout << "OOR";};
    if (ciErr1 == CL_OUT_OF_HOST_MEMORY){std::cout << "OOHM";};
    
    Rcpp::XPtr<cl_program> tempXPtr(program);
    return tempXPtr;
}

SEXP createKernel(SEXP sProgram, SEXP sKernelName){
    Rcpp::XPtr<cl_program> program(sProgram);
    std::string kernelName = Rcpp::as<std::string>(sKernelName);
    
    cl_kernel *kernel = new cl_kernel;
    *kernel = clCreateKernel(*program, kernelName.data(), NULL);
    Rcpp::XPtr<cl_kernel> tempXPtr(kernel);
    return tempXPtr;
}

SEXP setKernelArgMem(SEXP sKernel, SEXP sIndex, SEXP sBuffer){
    Rcpp::XPtr<cl_kernel> kernel(sKernel);
    int argNr = Rcpp::as<int>(sIndex);
    Rcpp::XPtr<cl_mem> memObject(sBuffer);
    
    cl_int ciErr1 = clSetKernelArg(*kernel, argNr, sizeof(cl_mem), memObject);
    if (ciErr1 != CL_SUCCESS){ std::cout << "error setting kernel arguments\n"; };
    if (ciErr1 == CL_INVALID_KERNEL){std::cout << "invalid kernel";};
    if (ciErr1 == CL_INVALID_ARG_INDEX){std::cout << "invalid arg index";};
    if (ciErr1 == CL_INVALID_ARG_VALUE){std::cout << "invalid arg value";};
    if (ciErr1 == CL_INVALID_MEM_OBJECT){std::cout << "invalid mem object";};
    if (ciErr1 == CL_INVALID_SAMPLER){std::cout << "invalid sampler";};
    if (ciErr1 == CL_INVALID_ARG_SIZE){std::cout << "invalid arg size";};
    if (ciErr1 == CL_OUT_OF_RESOURCES){std::cout << "OOR";};
    if (ciErr1 == CL_OUT_OF_HOST_MEMORY){std::cout << "OOHM";};
    Rcpp::XPtr<cl_kernel> tempXPtr(kernel);
    return tempXPtr;
}

SEXP setKernelArgInt(SEXP sKernel, SEXP sIndex, SEXP sIntegerValue){
    Rcpp::XPtr<cl_kernel> kernel(sKernel);
    int argNr = Rcpp::as<int>(sIndex);
    int integerValue = Rcpp::as<int>(sIntegerValue);
    
    cl_int ciErr1 = clSetKernelArg(*kernel, argNr, sizeof(cl_int), (void*)&integerValue);
    if (ciErr1 != CL_SUCCESS){ std::cout << "error setting kernel arguments\n"; };
    Rcpp::XPtr<cl_kernel> tempXPtr(kernel);
    return tempXPtr;
}

void enqueueWriteBufferFloatVector(SEXP sQueue, SEXP sMemBuffer, SEXP sGlobalWorkSize, SEXP sObject){
    Rcpp::XPtr<cl_command_queue> queue(sQueue);
    Rcpp::XPtr<cl_mem> clMemBuffer(sMemBuffer);
    int globalWorkSize = Rcpp::as<int>(sGlobalWorkSize);
    Rcpp::NumericVector vec(sObject);
    
    float *object = new float[globalWorkSize];
    for (int i=0; i<vec.size(); i++) {
        object[i] = vec[i];
    }
    
    //Can we delete this here?
    cl_int ciErr1 = clEnqueueWriteBuffer(*queue, *clMemBuffer, CL_FALSE, 0, sizeof(cl_float) * globalWorkSize, object, 0, NULL, NULL);
    if (ciErr1 != CL_SUCCESS){ std::cout << "error writing buffer\n"; };
    if (ciErr1 == CL_INVALID_COMMAND_QUEUE){std::cout << "invalid queue";};
    if (ciErr1 == CL_INVALID_CONTEXT){std::cout << "invalid context";};
    if (ciErr1 == CL_INVALID_MEM_OBJECT){std::cout << "invalid mem object";};
    if (ciErr1 == CL_INVALID_VALUE){std::cout << "invalid value";};
    if (ciErr1 == CL_INVALID_EVENT_WAIT_LIST){std::cout << "invalid event wait list";};
    if (ciErr1 == CL_MISALIGNED_SUB_BUFFER_OFFSET){std::cout << "misaligned sub buffer offset";};
    if (ciErr1 == CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST){std::cout << "Exec status error for events in wait list";};
    if (ciErr1 == CL_MEM_OBJECT_ALLOCATION_FAILURE){std::cout << "mem object allocation fail";};
    if (ciErr1 == CL_OUT_OF_RESOURCES){std::cout << "OOR";};
    if (ciErr1 == CL_OUT_OF_HOST_MEMORY){std::cout << "OOHM";};
}

void enqueueWriteBufferIntegerVector(SEXP sQueue, SEXP sMemBuffer, SEXP sGlobalWorkSize, SEXP sObject){
    Rcpp::XPtr<cl_command_queue> queue(sQueue);
    Rcpp::XPtr<cl_mem> clMemBuffer(sMemBuffer);
    int globalWorkSize = Rcpp::as<int>(sGlobalWorkSize);
    Rcpp::NumericVector vec(sObject);
    
    int *object = new int[globalWorkSize];
    for (int i=0; i<vec.size(); i++) {
        object[i] = vec[i];
    }
    
    //Can we delete this here?
    cl_int ciErr1 = clEnqueueWriteBuffer(*queue, *clMemBuffer, CL_FALSE, 0, sizeof(cl_int) * globalWorkSize, object, 0, NULL, NULL);
    if (ciErr1 != CL_SUCCESS){ std::cout << "error writing buffer\n"; };
    if (ciErr1 == CL_INVALID_COMMAND_QUEUE){std::cout << "invalid queue";};
    if (ciErr1 == CL_INVALID_CONTEXT){std::cout << "invalid context";};
    if (ciErr1 == CL_INVALID_MEM_OBJECT){std::cout << "invalid mem object";};
    if (ciErr1 == CL_INVALID_VALUE){std::cout << "invalid value";};
    if (ciErr1 == CL_INVALID_EVENT_WAIT_LIST){std::cout << "invalid event wait list";};
    if (ciErr1 == CL_MISALIGNED_SUB_BUFFER_OFFSET){std::cout << "misaligned sub buffer offset";};
    if (ciErr1 == CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST){std::cout << "Exec status error for events in wait list";};
    if (ciErr1 == CL_MEM_OBJECT_ALLOCATION_FAILURE){std::cout << "mem object allocation fail";};
    if (ciErr1 == CL_OUT_OF_RESOURCES){std::cout << "OOR";};
    if (ciErr1 == CL_OUT_OF_HOST_MEMORY){std::cout << "OOHM";};
}

void enqueueNDRangeKernel(SEXP sQueue, SEXP sKernel, SEXP sGlobalWorkSize, SEXP sLocalWorkSize){
    Rcpp::XPtr<cl_command_queue> queue(sQueue);
    Rcpp::XPtr<cl_kernel> kernel(sKernel);
    size_t globalWorkSize = Rcpp::as<size_t>(sGlobalWorkSize);
    size_t localWorkSize = Rcpp::as<size_t>(sLocalWorkSize);
    
    //globalWorkSize = 11444992;
    //localWorkSize = 256;
    
    //ciErr1 = clEnqueueNDRangeKernel(cqCommandQueue, ckKernel, 1, NULL, &szGlobalWorkSize, &szLocalWorkSize, 0, NULL, NULL);
    cl_int ciErr1 = clEnqueueNDRangeKernel(*queue, *kernel, 1, NULL, &globalWorkSize, &localWorkSize, 0, NULL, NULL);
    if (ciErr1 != CL_SUCCESS){ std::cout << "error enqueueing kernel\n"; };
    if (ciErr1 == CL_INVALID_PROGRAM_EXECUTABLE){std::cout << "invalid program executable";};
    if (ciErr1 == CL_INVALID_COMMAND_QUEUE){std::cout << "invalid queue";};
    if (ciErr1 == CL_INVALID_KERNEL){std::cout << "invalid kernel";};
    if (ciErr1 == CL_INVALID_CONTEXT){std::cout << "invalid context";};
    if (ciErr1 == CL_INVALID_KERNEL_ARGS){std::cout << "invalid kernel args";};
    if (ciErr1 == CL_INVALID_WORK_DIMENSION){std::cout << "invalid work dimension";};
    if (ciErr1 == CL_INVALID_GLOBAL_WORK_SIZE){std::cout << "invalid global work size";};
    if (ciErr1 == CL_INVALID_GLOBAL_OFFSET){std::cout << "invalid global offset";};
    if (ciErr1 == CL_INVALID_WORK_GROUP_SIZE){std::cout << "invalid work group size";};
    if (ciErr1 == CL_INVALID_WORK_ITEM_SIZE){std::cout << "invalid work item size";};
    if (ciErr1 == CL_MISALIGNED_SUB_BUFFER_OFFSET){std::cout << "misaligned sub buffer offset";};
    if (ciErr1 == CL_INVALID_IMAGE_SIZE){std::cout << "invalid image size";};
    if (ciErr1 == CL_OUT_OF_RESOURCES){std::cout << "OOR";};
    if (ciErr1 == CL_MEM_OBJECT_ALLOCATION_FAILURE){std::cout << "mem object allocation fail";};
    if (ciErr1 == CL_INVALID_EVENT_WAIT_LIST){std::cout << "invalid event wait list";};
    if (ciErr1 == CL_OUT_OF_HOST_MEMORY){std::cout << "OOHM";};
}

SEXP enqueueReadBufferFloatVector(SEXP sQueue, SEXP sMemBuffer, SEXP sGlobalWorkSize, SEXP sObject){
    Rcpp::XPtr<cl_command_queue> queue(sQueue);
    Rcpp::XPtr<cl_mem> clMemBuffer(sMemBuffer);
    int globalWorkSize = Rcpp::as<int>(sGlobalWorkSize);
    Rcpp::NumericVector vec(sObject);
    
    float *object = new float[globalWorkSize];
    cl_int ciErr1 = clEnqueueReadBuffer(*queue, *clMemBuffer, CL_TRUE, 0, sizeof(cl_float) * globalWorkSize, object, 0, NULL, NULL);
    if (ciErr1 != CL_SUCCESS){ std::cout << "error reading buffer\n"; };
    for (int i=0; i<vec.size(); i++) {
        vec[i] = object[i];
    }
    return Rcpp::wrap(vec);
}

SEXP enqueueReadBufferIntegerVector(SEXP sQueue, SEXP sMemBuffer, SEXP sGlobalWorkSize, SEXP sObject){
    Rcpp::XPtr<cl_command_queue> queue(sQueue);
    Rcpp::XPtr<cl_mem> clMemBuffer(sMemBuffer);
    int globalWorkSize = Rcpp::as<int>(sGlobalWorkSize);
    Rcpp::NumericVector vec(sObject);
    
    int *object = new int[globalWorkSize];
    cl_int ciErr1 = clEnqueueReadBuffer(*queue, *clMemBuffer, CL_TRUE, 0, sizeof(cl_int) * globalWorkSize, object, 0, NULL, NULL);
    if (ciErr1 != CL_SUCCESS){ std::cout << "error reading buffer\n"; };
    for (int i=0; i<vec.size(); i++) {
        vec[i] = object[i];
    }
    return Rcpp::wrap(vec);
}

/* // Stuff to expose the int to R
static void intObjFinalizer(SEXP ref){
       if(TYPEOF(ref) == EXTPTRSXP){
               int *o = static_cast<int*> (R_ExternalPtrAddr(ref));
               if (o) delete o;
       }
}

SEXP int2EXP(int *o){
       SEXP xp = R_MakeExternalPtr(o, R_NilValue, R_NilValue);
       R_RegisterCFinalizerEx(xp, intObjFinalizer, TRUE);
       return xp;
}

int *SEXP2int(SEXP o){
       if(TYPEOF(o) != EXTPTRSXP)
               Rf_error("invalid object");
       return (int*) R_ExternalPtrAddr(o);
}

SEXP getIntPointer(){
    int *test = new int;
    *test = 6;
    SEXP retVal = int2EXP(test);
    int test2 = *SEXP2int(retVal);
    return retVal;
}

SEXP doubleIntPointer(SEXP test){
    int test2 = *SEXP2int(test);
    return Rcpp::wrap(test2*2);
}*/