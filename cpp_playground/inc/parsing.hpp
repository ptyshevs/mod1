#ifndef MOD1_PARSING_HPP
#define MOD1_PARSING_HPP

/*
 * Read 3D points from file in a given format (x, y, z)
 */
std::vector<glm::vec3> readFile(char *filePath);

/*
 * Display program usage on STDOUT
 */
void	show_usage(void);

/*
 * Display error message and exit
 */
void	panic(const std::string &message);


/*
 * Parse CLI arguments
 */
void parse_arguments(int ac, char **av, ControlPoints *cp, bool &running,
					 bool &emitting, bool &offline, Emitter &emitter);

/*
 * Read pixel values from GL context and save into file
 */
void save_image(std::string &&dirname);

#endif
