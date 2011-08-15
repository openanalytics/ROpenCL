library(ROpenCL)

a <- seq(11444777)/10
b <- seq(11444777)
out <- rep(0.0, length(a))

localWorkSize = 256
globalWorkSize = ceiling(length(a)/localWorkSize)*localWorkSize

platformIDs <- getPlatformIDs()
#getPlatformName(platformIDs[[1]])
deviceIDs <- getDeviceIDs(platformIDs[[1]])
#context <- createContextFromType("CL_DEVICE_TYPE_GPU") Does not work
context <- createContext(deviceIDs[[1]])
#getContextInfo(context, "CL_CONTEXT_NUM_DEVICES") Does not work
queue <- createCommandQueue(context, deviceIDs[[1]])
inputBuf1 <- createBuffer(context, "CL_MEM_READ_ONLY", globalWorkSize, a)
inputBuf2 <- createBuffer(context, "CL_MEM_READ_ONLY", globalWorkSize, b)
outputBuf1 <- createBufferFloatVector(context, "CL_MEM_WRITE_ONLY", globalWorkSize)
kernel <- "__kernel void VectorAdd(__global const float* a, __global const int* b, __global float* c, int iNumElements)
{
    // get index into global data array
    int iGID = get_global_id(0);

    // bound check (equivalent to the limit on a 'for' loop for standard/serial C code
    if (iGID >= iNumElements)
    {   
        return; 
    }
    // add the vector elements
    c[iGID] = a[iGID] + b[iGID];
}"

#program <- createProgramWithSource(context, kernel)
#getProgramInfo(program, "CL_PROGRAM_SOURCE")
#program <- buildProgram(program)
#~ kernel <- createKernel(program, "VectorAdd")
#~ kernel <- setKernelArgMem(kernel, 0, inputBuf1)
#~ kernel <- setKernelArgMem(kernel, 1, inputBuf2)
#~ kernel <- setKernelArgMem(kernel, 2, outputBuf1)
#~ kernel <- setKernelArgInt(kernel, 3, length(out))
kernel <- createProgram(context, kernel, "VectorAdd", inputBuf1, inputBuf2, outputBuf1, length(out))
enqueueWriteBuffer(queue, inputBuf1, globalWorkSize, a)
enqueueWriteBuffer(queue, inputBuf2, globalWorkSize, b)
enqueueNDRangeKernel(queue, kernel, globalWorkSize, localWorkSize)
result <- enqueueReadBuffer(queue, outputBuf1, globalWorkSize, out)
result[1:10]
