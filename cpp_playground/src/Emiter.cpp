
#include <core.hpp>
#include <stdlib.h>
#include <time.h> 

void Emiter::prepare_emit(CLCore &core) {
    device = core.device;
    context = core.context;
    queue = core.queue;

    cl_compile_kernel(*this, "src/kernels/emit_kernel.cl", "emit_kernel");

    int err = 0;
    cl_emiter = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(t_emiter), NULL, &err);
    if (err != CL_SUCCESS) {
        std::cout << "Error: " << __FILE__ << " " << __LINE__ << "code: " << err << ".\n";
        exit(1);
    }
    
    pps = 10; // Once in a sec
    type = EMITER_UNDERGROUND;

    srand(time(NULL));
}

static const size_t  global_work_size = 1;

void Emiter::emit(cl_mem &cl_vbo)
{
    int err = 0;

    t_emiter emiter = (t_emiter){ (cl_int)type, (cl_uint)pps, (cl_uint)rand() };
    err = clEnqueueWriteBuffer(queue, cl_emiter, CL_TRUE, 0, sizeof(t_emiter), &emiter, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        std::cout << "Error: " << __FILE__ << " " << __LINE__ << "code: " << err << ".\n";
        exit(1);
    }

	err = clSetKernelArg(kernel, 0, sizeof(cl_vbo), &cl_vbo);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_emiter), &cl_emiter);
	if (err != CL_SUCCESS) {
        std::cout << "Error: " << __FILE__ << " " << __LINE__ << "code: " << err << ".\n";
        exit(1);
    }

    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        std::cout << "Error: " << __LINE__ << "code: " << err << ".\n";
        exit(1);
    }
}

