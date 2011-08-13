#include <CL/opencl.h>
#include <Rcpp.h>
#include "createContext.h"

RCPP_MODULE(ropencl){
	using namespace Rcpp ;
	function( "getPlatformIDs"  , &getPlatformIDs   , "getPlatformIDs" ) ;
    function( "getDeviceIDs"  , &getDeviceIDs   , "getDeviceIDs" ) ;
    function( "getPlatformName"  , &getPlatformName   , "getPlatformName" ) ;
    function( "createContextFromType"  , &createContextFromType   , "createContextFromType" ) ;
    function( "createCommandQueue"  , &createCommandQueue   , "createCommandQueue" ) ;
    function( "getPlatformIDs2"  , &getPlatformIDs2 , "getPlatformIDs2" ) ;
}                     


