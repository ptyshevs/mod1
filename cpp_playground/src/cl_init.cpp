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

void	cl_host_part(CLCore &cl_core, bool wGLInterop)
{
	int err;
	// Get platform and device to use
	cl_platform_id platforms_available[10];
	unsigned int num_platforms = 0;
	err = clGetPlatformIDs(10, platforms_available, &num_platforms);
	if (err == CL_SUCCESS) {
		std::cout << "Platforms read" << std::endl;
	} else if (err == CL_INVALID_VALUE) {
		std::cout << "Invalid value" << std::endl;
	} else if (err == CL_OUT_OF_HOST_MEMORY) {
		std::cout << "Out of host memory" << std::endl;
	} else if (err == CL_PLATFORM_NOT_FOUND_KHR) {
		std::cout << "Platform not found" << std::endl;
	}
	else {
		std::cout << "Unknown error:" << err << std::endl;
	}
	std::cout << "# of platforms= " << num_platforms << std::endl;

	err = clGetDeviceIDs(platforms_available[0], CL_DEVICE_TYPE_CPU, 1, &(cl_core.device), NULL);
	if (err != CL_SUCCESS)
	{
		if (err == CL_INVALID_PLATFORM) {
			std::cout << "Invalid platform";
		} else if (err == CL_INVALID_DEVICE_TYPE) {
			std::cout << "Invalid device type";
		} else if (err == CL_INVALID_VALUE) {
			std::cout << "Invalid value";
		} else if (err == CL_DEVICE_NOT_FOUND) {
			std::cout << "Device not found";
		} else if (err == CL_OUT_OF_RESOURCES) {
			std::cout << "Out of resources";
		} else if (err == CL_OUT_OF_HOST_MEMORY) {
			std::cout << "Out of host memory";
		}
		std::cout << std::endl;
		std::cout << "Error: line " << __LINE__ << ": no valid GPU's found.\n";
		exit(1);
	}

	// Shared context for OpenGl-OpenCL interop
	// CGLContextObj glContext = CGLGetCurrentContext();
	// CGLShareGroupObj shareGroup = CGLGetShareGroup(glContext);

	cl_context_properties properties[] = {
		 CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
		 CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
		 CL_CONTEXT_PLATFORM, (cl_context_properties)platforms_available[0],
		0
	};

	// Create context
	if (wGLInterop)
	{
	  cl_core.context = clCreateContext(properties, 1, &cl_core.device, NULL, NULL, &err);

	  if (err == CL_SUCCESS) {
			std::cout << "Context created" << std::endl;
		} else if (err == CL_INVALID_PLATFORM) {
			std::cout << "Invalid platform" << std::endl;
		} else if (err == CL_INVALID_VALUE) {
			std::cout << "Invalid value" << std::endl;
		} else if (err == CL_INVALID_DEVICE) {
			std::cout << "Invalid device" << std::endl;
		} else if (err == CL_DEVICE_NOT_AVAILABLE) {
			std::cout << "Device not available" << std::endl;
		} else if (err == CL_OUT_OF_HOST_MEMORY) {
			std::cout << "Out of host memory" << std::endl;
		} else {
			std::cout << "Unknown error:" << err << std::endl;
		}
	}
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

void	cl_compile_kernel(CLCore &cl, const char *filepath, const char *program_name)
{
	int err;
	char	*kernel = reader(filepath);

	// Create obj
	cl.program = clCreateProgramWithSource(cl.context, 1, (const char **) & kernel, NULL, &err);
	if (!cl.program || err != CL_SUCCESS)
	{
		printf("Error: Failed to create compute program!\n");
		return ;
	}
	free(kernel);
	// Compile
	err = clBuildProgram(cl.program, 1, &(cl.device), "-I ../cpp_playground/inc -cl-fast-relaxed-math", NULL, NULL);
	if (err != CL_SUCCESS)
	{
		size_t len;
		char buffer[2048];
		// first get length of the log
		clGetProgramBuildInfo(cl.program, cl.device, CL_PROGRAM_BUILD_LOG, 0, NULL, &len);
		printf("Error: Failed to build program executable! %d \n", err);
		// get the log itself
		clGetProgramBuildInfo(cl.program, cl.device, CL_PROGRAM_BUILD_LOG, len, buffer, NULL);
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

void	cl_compile_water_kernel(CLWaterCore &cl, cl_program &program_field, cl_kernel &kernel_field, const char *filepath, const char *program_name)
{
	int err;
	char	*kernel = reader(filepath);

	// Create obj
	program_field = clCreateProgramWithSource(cl.context, 1, (const char **) & kernel, NULL, &err);
	if (!program_field || err != CL_SUCCESS)
	{
		printf("Error: Failed to create compute program!\n");
		return ;
	}
	free(kernel);
	// Compile
	err = clBuildProgram(program_field, 1, &(cl.device), "-I../cpp_playground/inc -cl-fast-relaxed-math", NULL, NULL);
	if (err != CL_SUCCESS)
	{
		size_t len;
		char buffer[2048];
		// first get length of the log
		clGetProgramBuildInfo(program_field, cl.device, CL_PROGRAM_BUILD_LOG, 0, NULL, &len);
		printf("Error: Failed to build program executable! %d \n", err);
		// get the log itself
		clGetProgramBuildInfo(program_field, cl.device, CL_PROGRAM_BUILD_LOG, len, buffer, NULL);
		printf("%s\n", buffer);
		exit(1);
	}
	// Link
	kernel_field = clCreateKernel(program_field, program_name, &err);
	if (!kernel_field || err != CL_SUCCESS)
	{
		printf("Error: Failed to create compute kernel!\n");
		exit(1);
	}
}