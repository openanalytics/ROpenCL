createContext <- function(){
	.Call( "createContext", PACKAGE = "ROpenCL" )
}

createBuffer <- function(sContext, sMemFlag, sGlobalWorkSize, sObject){
    type <- class(sObject)
    if(type == "integer") {
        buffer <- createBufferIntegerVector(sContext, sMemFlag, sGlobalWorkSize)
    }
    if(type == "numeric") {
        buffer <- createBufferFloatVector(sContext, sMemFlag, sGlobalWorkSize)
    }
    if((class(b) %in% c("integer", "numeric")) == FALSE){
        print("Not supported yet")
    }
    return(buffer)
}

createProgram <- function(context, kernelSrc, kernelName, ...){
    program <- createProgramWithSource(context, kernelSrc)
    program <- buildProgram(program)
    kernel <- createKernel(program, kernelName)
    
    dotList <- list(...)
    index <- 0
    for(item in dotList){
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

enqueueWriteBuffer <- function(sQueue, sMemBuffer, sGlobalWorkSize, sObject){
    type <- class(sObject)
    if(type == "integer") {
        enqueueWriteBufferIntegerVector(sQueue, sMemBuffer, sGlobalWorkSize, sObject)
    }
    if(type == "numeric") {
        enqueueWriteBufferFloatVector(sQueue, sMemBuffer, sGlobalWorkSize, sObject)
    }
    if((class(b) %in% c("integer", "numeric")) == FALSE){
        print("Not supported yet")
    }
}

enqueueReadBuffer <- function(sQueue, sMemBuffer, sGlobalWorkSize, sObject){
    type <- class(sObject)
    if(type == "integer") {
        buffer <- enqueueReadBufferIntegerVector(sQueue, sMemBuffer, sGlobalWorkSize, sObject)
    }
    if(type == "numeric") {
        buffer <- enqueueReadBufferFloatVector(sQueue, sMemBuffer, sGlobalWorkSize, sObject)
    }
    if((class(b) %in% c("integer", "numeric")) == FALSE){
        print("Not supported yet")
    }
    return(buffer)
}