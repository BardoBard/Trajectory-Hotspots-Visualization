//
// Created by Bardio - NHL_STENDEN
//
#pragma once
namespace parser
{
	/// \brief checks if directory exists
	/// \param file_name path to file + file name
	/// \return true if directory exists
	inline bool dir_exists(const std::string& file_name) noexcept
	{
		return std::filesystem::is_directory(file_name);
	}

	/// \brief checks if file exists
	/// \param file_name file_name path to file + file name
	/// \return true if file exists
	inline bool file_exists(const std::string& file_name) noexcept
	{
		return std::filesystem::is_regular_file(file_name);
	}

	/// \brief returns raw file as string
	/// \param file_path path to file + file name
	///	\throw std::exception if file does not exist
	/// \return raw file as string
	inline std::string get_file_raw(const std::string& file_path);

	/// \brief loops through file line by line and calls callback function
	/// \param file_path path to file + file name
	/// \param callback function that gets called for each line
	///	\throw std::exception if file does not exist
	inline void parse_file_by_line(const std::string& file_path, const std::function<void(std::string& line)>& callback);

	/// \brief loops through file line by line and calls callback function for each substring
	/// \param file_path path to file + file name
	/// \param delimiter delimiter between substrings
	/// \param callback function that gets called for each substring for each line
	///	\throw std::exception if file does not exist
	inline void parse_file_with_delimiter(const std::string& file_path, const char delimiter,
	                                      const std::function<void(const std::string& substr)>& callback);

	/// \brief returns trajectory from file
	/// \param file_path path to file + file name
	/// \param x_y_delimiter delimiter between x and y
	/// \param point_delimiter delimiter between points
	///	\throw std::exception if file does not exist
	///	\throw std::exception if trajectory is empty or has less than 2 points
	/// \return Trajectory with points from file
	inline Trajectory parse_trajectory(const std::string& file_path, const char x_y_delimiter,
	                                   const char point_delimiter);
} // namespace parser


std::string parser::get_file_raw(const std::string& file_path)
{
	if (!dir_exists(file_path))
		throw std::exception("File does not exist");

	std::string file_raw;
	std::ifstream file(file_path);

	std::string line;
	// read file line by line
	while (std::getline(file, line))
		file_raw += line + '\n';

	return file_raw;
}

void parser::parse_file_by_line(const std::string& file_path, const std::function<void(std::string& line)>& callback)
{
	if (!file_exists(file_path))
		throw std::exception("File does not exist");
	std::ifstream file(file_path);
	std::string line;

	// read file line by line
	while (std::getline(file, line))
		callback(line);
}

void parser::parse_file_with_delimiter(const std::string& file_path, const char delimiter,
                                       const std::function<void(const std::string& substr)>& callback)
{
	parse_file_by_line(file_path, [&](std::string& line)
	{
		std::string substring = line.substr(0, line.find(delimiter));
		while (!substring.empty())
		{
			callback(substring);

			// break if no more delimiters are found
			if (line.find(delimiter) == std::string::npos)
				break;
			// next substring
			line.erase(0, line.find(delimiter) + 1);
			substring = line.substr(0, line.find('|'));
		}
	});
}

Trajectory parser::parse_trajectory(const std::string& file_path, const char x_y_delimiter, const char point_delimiter)
{
	if (!file_exists(file_path))
		throw std::exception("File does not exist");

	std::vector<Vec2> trajectory_points;
	//this is more an estimate than the actual size, due to the fact that the numbers are floats with any amount of decimals
	//this will almost always be larger than the actual size
	//TODO: find a better way to estimate the size if possible
	const size_t size = (std::filesystem::file_size(file_path) / 2 / 2) / 1.5f;

	//reserve size based on file size
	trajectory_points.reserve(size);
	parse_file_with_delimiter(file_path, point_delimiter, [&](const std::string& substr)
	{
		// throws exception if substring is not a float
		const float x = std::stof(substr.substr(0, substr.find(x_y_delimiter)));
		const float y = std::stof(substr.substr(substr.find(x_y_delimiter) + 1, substr.size()));
		trajectory_points.emplace_back(x, y);
	});

	if (trajectory_points.empty() || trajectory_points.size() < 2)
		throw std::exception("Trajectory is empty or has less than 2 points");

	return {trajectory_points};
}
