#' Function to create a context
#' @return pointer to the context
#' @author Willem Ligtenberg
#' @export
createContext <- function(){
	.Call( "createContext", PACKAGE = "ROpenCL" )
}

#' Create a buffer
#' @param sContext context
#' @param sMemFlag memflag
#' @param sGlobalWorkSize globalworksize
#' @param sObject object
#' @return pointer to the buffer
#' @author Willem Ligtenberg
#' @export
createBuffer <- function(sContext, sMemFlag, sGlobalWorkSize, sObject){
    type <- class(sObject)
    if (type == "integer") {
        buffer <- createBufferIntegerVector(sContext, sMemFlag, sGlobalWorkSize)
    }
    if (type == "numeric") {
        buffer <- createBufferFloatVector(sContext, sMemFlag, sGlobalWorkSize)
    }
    if (!(type %in% c("integer", "numeric"))){
      stop(paste("Objects of class ", type, " are not supported yet", sep = ""))
    }
    return(buffer)
}

#' Create a program
#' @param context context in which to execute the kernel
#' @param kernelSrc source code of the kernel function as a string
#' @param kernelName name of the kernel
#' @param ... further arguments
#' @return pointer to the kernel
#' @author Willem Ligtenberg
#' @export
createProgram <- function(context, kernelSrc, kernelName, ...){
    program <- createProgramWithSource(context, kernelSrc)
    program <- buildProgram(program)
    kernel <- createKernel(program, kernelName)
    
    dotList <- list(...)
    index <- 0
    for (item in dotList){
        if(class(item) == "externalptr"){
            kernel <- setKernelArgMem(kernel, index, item)
        }
        if(class(item) == "integer"){
            kernel <- setKernelArgInt(kernel, index, item)
        }
        index <- index + 1
    }
    return(kernel)
}

#' Enqueue a write buffer
#' @param sQueue queue
#' @param sMemBuffer membufffer
#' @param sGlobalWorkSize globalworksize
#' @param sObject object
#' @return pointer
#' @author Willem Ligtenberg
#' @export
enqueueWriteBuffer <- function(sQueue, sMemBuffer, sGlobalWorkSize, sObject){
    type <- class(sObject)
    if(type == "integer") {
        enqueueWriteBufferIntegerVector(sQueue, sMemBuffer, sGlobalWorkSize, sObject)
    }
    if(type == "numeric") {
        enqueueWriteBufferFloatVector(sQueue, sMemBuffer, sGlobalWorkSize, sObject)
    }
    if(!(type %in% c("integer", "numeric"))){
      stop(paste("Objects of class ", type, " are not supported yet", sep = ""))
    }
}

#' Enqueue the read buffer
#' @param sQueue queue
#' @param sMemBuffer membuffer
#' @param sGlobalWorkSize global worksize
#' @param sObject object
#' @return pointer
#' @author Willem Ligtenberg
#' @export
enqueueReadBuffer <- function(sQueue, sMemBuffer, sGlobalWorkSize, sObject){
    type <- class(sObject)
    if(type == "integer") {
        buffer <- enqueueReadBufferIntegerVector(sQueue, sMemBuffer, sGlobalWorkSize, sObject)
    }
    if(type == "numeric") {
        buffer <- enqueueReadBufferFloatVector(sQueue, sMemBuffer, sGlobalWorkSize, sObject)
    }
    if(!(type %in% c("integer", "numeric"))){
        stop(paste("Objects of class ", type, " are not supported yet", sep = ""))
    }
    return(buffer)
}
