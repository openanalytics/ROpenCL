#include <CL/opencl.h>
#include <Rcpp.h>
#include "createContext.h"

RCPP_MODULE(ropencl){
	using namespace Rcpp ;
	function( "getPlatformIDs"  , &getPlatformIDs   , "getPlatformIDs help" ) ;
    function( "getPlatformName"  , &getPlatformName   , "getPlatformName help" ) ;
    function( "getDeviceIDs"  , &getDeviceIDs   , "getDeviceIDs help" ) ;
    function( "createContextFromType"  , &createContextFromType   , "createContextFromType help" ) ;
    function( "createCommandQueue"  , &createCommandQueue   , "createCommandQueue help" ) ;
    function( "createBufferFloatVector"  , &createBufferFloatVector   , "createBufferFloatVector help" ) ;
    function( "createBufferIntegerVector"  , &createBufferIntegerVector , "createBufferIntegerVector help" ) ;
    function( "createProgramWithSource"  , &createProgramWithSource   , "createProgramWithSource help" ) ;
    function( "buildProgram"  , &buildProgram   , "buildProgram help" ) ;
    function( "createKernel"  , &createKernel   , "createKernel help" ) ;
    function( "setKernelArgMem"  , &setKernelArgMem , "setKernelArgMem help" ) ;
    function( "setKernelArgInt"  , &setKernelArgInt , "setKernelArgInt help" ) ;
    function( "enqueueWriteBufferFloatVector"  , &enqueueWriteBufferFloatVector , "enqueueWriteBufferFloatVector help" ) ;
    function( "enqueueWriteBufferIntegerVector"  , &enqueueWriteBufferIntegerVector , "enqueueWriteBufferIntegerVector help" ) ;
    function( "enqueueNDRangeKernel"  , &enqueueNDRangeKernel , "enqueueNDRangeKernel help" ) ;
    function( "enqueueReadBufferFloatVector"  , &enqueueReadBufferFloatVector , "enqueueReadBufferFloatVector help" ) ;
    function( "enqueueReadBufferIntegerVector"  , &enqueueReadBufferIntegerVector , "enqueueReadBufferIntegerVector help" ) ;
    function( "getProgramInfo"  , &getProgramInfo , "getProgramInfo help" ) ;
    function( "getContextInfo"  , &getContextInfo , "getContextInfo help" ) ;
    function( "createContext"  , &createContext, "createContext help" ) ;
}                    


