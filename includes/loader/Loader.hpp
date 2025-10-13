// TODO: Don't forget to add 42 header !

/**
 * @file Loader.hpp
 * @brief
 */

#ifndef LOADER_HPP
#define LOADER_HPP

#include <fstream>
#include <string>

namespace common
{
namespace loader
{

class Loader
{
	public:
		Loader();
		~Loader();

		typedef void (*fileParser)(std::ifstream &ifs, const std::string &filename);

		void	loadDirectory(const std::string &dirname, const std::string &extension, fileParser parser);

	private:
		Loader(const Loader &rhs);
		Loader &operator=(const Loader &rhs);

		void	loadFile(const std::string &filename, fileParser parser);
};

} // !loader
} // !common

#endif // !LOADER_HPP
