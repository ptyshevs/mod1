/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mapgen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpopovyc <vpopovyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 16:40:21 by vpopovyc          #+#    #+#             */
/*   Updated: 2018/12/17 19:25:56 by vpopovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core.hpp>

/*
** Determine GPU device, create context object with it and create command 
** queue with selected context to specific device
*/

void     cl_host_part(CLCore &cl_core, bool wGLInterop)
{
    int err;  
    // Get platform and device to use
    err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &(cl_core.device), NULL);
    if (err != CL_SUCCESS)
    {
        std::cout << "Error: line " << __LINE__ << ": no valid GPU's found.\n";
        exit(1);
    }

    // Shared context for OpenGl-OpenCL interop
    CGLContextObj glContext = CGLGetCurrentContext();
    CGLShareGroupObj shareGroup = CGLGetShareGroup(glContext);

    cl_context_properties properties[] = {
        CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
        (cl_context_properties)shareGroup,
        0
    };

    // Create context
    if (wGLInterop)
        cl_core.context = clCreateContext(properties, 1, &cl_core.device, NULL, NULL, &err);
    else
        cl_core.context = clCreateContext(0, 1, &cl_core.device, NULL, NULL, &err);
    if (!cl_core.context || err != CL_SUCCESS)
    {
        std::cout << "Error: line " << __LINE__ << ": failed to create context.\n";
        exit(1);
    }
    // Create command queue
    cl_core.queue = clCreateCommandQueue(cl_core.context, cl_core.device, 0, &err);
    if (!cl_core.queue || err != CL_SUCCESS) {
        std::cout << "Error: line " << __LINE__ << ": failed to create command queue.\n";
        exit(1);
    }
}

void    cl_compile_kernel(CLCore &cl, const char *filepath, const char *program_name)
{
    int err;
    char    *kernel = reader(filepath);

    // Create obj
    cl.program = clCreateProgramWithSource(cl.context, 1, (const char **) & kernel, NULL, &err);
    if (!cl.program || err != CL_SUCCESS)
    {
        printf("Error: Failed to create compute program!\n");
        return ;
    }
    free(kernel);
    // Compile
    err = clBuildProgram(cl.program, 1, &(cl.device), NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];

        printf("Error: Failed to build program executable! %d \n", err);
        clGetProgramBuildInfo(cl.program, cl.device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        exit(1);
    }
    // Link
    cl.kernel = clCreateKernel(cl.program, program_name, &err);
    if (!cl.kernel || err != CL_SUCCESS)
    {
        printf("Error: Failed to create compute kernel!\n");
        exit(1);
    }
}