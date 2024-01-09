//
// Created by Bardio - NHL_STENDEN
//
#pragma once
#include <Trajectory_Hotspots/pch.h>
#include <Trajectory_Hotspots/vec2.h>
#include <Trajectory_Hotspots/Float.h>
#include <Trajectory_Hotspots/AABB.h>
#include <Trajectory_Hotspots/segment.h>
#include <Trajectory_Hotspots/segment_search_tree.h>
#include <Trajectory_Hotspots/trapezoidal_map.h>
#include <Trajectory_Hotspots/trajectory.h>

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <functional>

namespace parser
{
	/// \brief this is an exception class that gets thrown when there is an error while parsing
	class parsing_error : public std::exception
	{
	public:
		using std::exception::exception;

		explicit parsing_error(const std::string& basic_string) : std::exception(basic_string.c_str())
		{
		}
	};

	/// \brief this is a data class that holds a trajectory and a function callback
	struct parsed_trajectory
	{
	public:
		typedef AABB (Trajectory::*trajectory_function_callback)(Float length) const;

	private:
		trajectory_function_callback trajectory_function_{};

	public:
		const std::string name{};
		Trajectory trajectory{};
		const Float length{};

		parsed_trajectory(const std::string& name, const Trajectory& trajectory,
		                  const trajectory_function_callback trajectory_function, const Float& length)
			: trajectory_function_(trajectory_function), name(name), trajectory(trajectory), length(length)
		{
		}

		parsed_trajectory()
		{
		}

		AABB run_trajectory_function() const
		{
			return (trajectory.*trajectory_function_)(length);
		}
	};

	class parser
	{
	private:
		/// \brief hashes a string which is a constexpr
		/// \param str string to hash
		/// \param h can be left empty
		/// \return unsigned int hash
		static constexpr unsigned hash_string(const char* str, const unsigned h = 0)
		{
			return !str[h] ? 5381 : (hash_string(str, h + 1) * 33) ^ str[h];
		}

		/// \brief returns trajectory function based on name
		/// \param name name of the trajectory function
		///	\throw parsing_error if trajectory function is not found
		/// \return a trajectory function callback
		static constexpr inline parsed_trajectory::trajectory_function_callback get_trajectory_function(
			const std::string_view& name)
		{
			switch (hash_string(name.data()))
			{
			case hash_string("get_hotspot_fixed_length_contiguous"):
				return &Trajectory::get_hotspot_fixed_length_contiguous;
			case hash_string("get_hotspot_fixed_length"):
				return &Trajectory::get_hotspot_fixed_length;
			case hash_string("get_hotspot_fixed_radius_contiguous"):
				return &Trajectory::get_hotspot_fixed_radius_contiguous;
			case hash_string("get_hotspot_fixed_radius"):
				return &Trajectory::get_hotspot_fixed_radius;
			default:
				throw parsing_error("Trajectory function not found");
			}
			return nullptr;
		}

	public:
		/// \brief checks if directory exists
		/// \param file_name path to file + file name
		/// \return true if directory exists
		inline static bool dir_exists(const std::string& file_name) noexcept
		{
			return std::filesystem::is_directory(file_name);
		}

		/// \brief this is the current path of the executable
		const inline static std::filesystem::path current_path = std::filesystem::current_path();

		/// \brief this is the current path of the executable as string
		const inline static std::string current_path_string = std::filesystem::current_path().string();

		/// \brief checks if file exists
		/// \param file_name file_name path to file + file name
		/// \return true if file exists
		inline static bool file_exists(const std::string& file_name) noexcept
		{
			return std::filesystem::is_regular_file(file_name);
		}

		/// \brief reads entire file and returns it as string
		/// \param file_path path to file + file name
		/// \return raw file contents as string
		inline static std::string read_file(const std::string& file_path)
		{
			return std::string(std::istreambuf_iterator(std::ifstream(file_path).rdbuf()),
			                   std::istreambuf_iterator<char>());
		}

		/// \brief returns raw file as string
		/// \param file_path path to file + file name
		///	\throw parsing_error if file does not exist
		/// \return raw file as string
		inline static std::string get_file_raw(const std::string& file_path);

		/// \brief loops through file line by line and calls callback function
		/// \param file_path path to file + file name
		/// \param callback function that gets called for each line
		/// \param header_length amount of lines to skip
		///	\throw parsing_error if file does not exist
		inline static void parse_file_by_line(std::string file_path,
		                                      const std::function<void(std::string& line)>& callback,
		                                      int header_length);

		/// \brief loops through file line by line and calls callback function for each substring
		/// \param file_path path to file + file name
		/// \param delimiter delimiter between substrings
		/// \param callback function that gets called for each substring for each line
		/// \param header_length amount of lines to skip
		///	\throw parsing_error if file does not exist
		inline static void parse_file_with_delimiter(const std::string& file_path, const char delimiter,
		                                             const std::function<void(const std::string& substr)>& callback,
		                                             int header_length);

		/// \brief returns trajectory from file
		/// \param file_path path to file + file name
		/// \param x_y_delimiter delimiter between x and y
		/// \param point_delimiter delimiter between points
		/// \param header_length amount of lines to skip
		///	\throw parsing_error if file does not exist
		///	\throw parsing_error if trajectory is empty or has less than 2 points
		/// \return Trajectory with points from file
		inline static Trajectory parse_trajectory(std::string file_path, const char x_y_delimiter,
		                                          const char point_delimiter, int header_length);
		/**
		 * \brief finds all trajectories in config file parses them and returns them
		 * \param config_file_path path to config file
		 * \param delimiter delimiter between each item, N will be used for newline delimiter
		 * \return list of parsed trajectories
		 * \throw parsing_error if file does not exist
		 * \throw parsing_error if trajectory function is not found
		 * \throw parsing_error if trajectory is empty or has less than 2 points
		 */
		inline static std::vector<::parser::parsed_trajectory> parse_config(std::string config_file_path,
		                                                                    const char delimiter);

		/// \brief splits string by delimiter and returns all substrings
		///	\note if there are multiple delimiters in a row we want to add the delimiter as a substring
		///	\note e.g "test|||test" should return ["test", "|", "test"]
		/// \note e.g "test||test" should return ["test", "test"]
		/// \param str string to split
		/// \param delimiter delimiter between substrings
		/// \return vector of substrings that were split by delimiter
		inline static std::vector<std::string> get_all_substring_by_delimiter(std::string str, const char delimiter)
		{
			std::vector<std::string> result{};

			std::stringstream data(str);

			while (std::getline(data, str, delimiter))
			{
				if (str.empty())
				{
					if (data.peek() != delimiter) continue;

					std::getline(data, str, delimiter);
					str = delimiter;
				}
				result.push_back(str);
			}
			return result;
		}
	}; //class parser
} // namespace parser


std::string parser::parser::get_file_raw(const std::string& file_path)
{
	if (!file_exists(file_path))
		throw parsing_error("File does not exist");

	std::string file_raw;
	std::ifstream file(file_path);

	std::string line;
	// read file line by line
	while (std::getline(file, line))
		file_raw += line + '\n';

	return file_raw;
}

void parser::parser::parse_file_by_line(std::string file_path, const std::function<void(std::string& line)>& callback,
                                        int header_length = 0)
{
	if (!file_exists(file_path))
	{
		if (!file_exists(current_path_string + file_path))
			throw parsing_error("File does not exist");
		file_path = current_path_string + file_path;
	}

	std::ifstream file(file_path);
	std::string line;

	// read file line by line
	while (std::getline(file, line))
	{
		if (header_length != 0)
		{
			--header_length;
			continue;
		}
		callback(line);
	}
}

void parser::parser::parse_file_with_delimiter(const std::string& file_path, const char delimiter,
                                               const std::function<void(const std::string& substr)>& callback,
                                               const int header_length = 0)
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
	}, header_length);
}

Trajectory parser::parser::parse_trajectory(std::string file_path, const char x_y_delimiter, const char point_delimiter,
                                            const int header_length = 0)
{
	if (!file_exists(file_path))
	{
		if (!file_exists(current_path_string + file_path))
			throw parsing_error("File does not exist");
		file_path = current_path_string + file_path;
	}

	std::unordered_set<std::string> trajectory_set_x;
	std::unordered_set<std::string> trajectory_set_y;
	std::vector<Vec2> trajectory_points;

	//TODO: find a way to estimate the size and reserve

	parse_file_with_delimiter(file_path, point_delimiter, [&](const std::string& substr)
	{
		const std::string x = substr.substr(0, substr.find(x_y_delimiter));
		const std::string y = substr.substr(substr.find(x_y_delimiter) + 1, substr.size());

		//TODO: this could be simplified if Vec2/Float has a hash function
		if (trajectory_set_x.contains(x) || trajectory_set_y.contains(y))
			return; // skip the points, go to next point

		trajectory_set_x.insert(x);
		trajectory_set_y.insert(y);

		// throws exception if substring is not a float
		trajectory_points.emplace_back(std::stof(x) * 1000, std::stof(y) * 1000);
	}, header_length);

	if (trajectory_points.empty() || trajectory_points.size() < 2)
		throw parsing_error(trajectory_points.empty()
			                    ? "Trajectory is empty"
			                    : " Trajectory has less than 2 points");

	std::cout << "Trajectory size: " << trajectory_points.size() << std::endl;

	return {trajectory_points};
}

inline std::vector<parser::parsed_trajectory> parser::parser::parse_config(std::string config_file_path,
                                                                           const char delimiter)
{
	if (!file_exists(config_file_path))
	{
		if (!file_exists(current_path_string + config_file_path))
			throw parsing_error("File does not exist");
		config_file_path = current_path_string + config_file_path;
	}

	std::vector<parsed_trajectory> trajectories;
	//TODO: find reserving size based on lines

	parse_file_by_line(config_file_path, [&](const std::string& line)
	{
		//TODO: work with tuples instead of vector
		const std::vector<std::string> substrings = get_all_substring_by_delimiter(line, delimiter);

		if (substrings.size() > 7 || substrings.size() < 6)
			throw parsing_error("\"" + line + "\" the arguments don't match, please provide a different input");
		const std::string name = substrings.at(0);
		const std::string trajectory_file_path = substrings.at(1);

		const char x_y_delimiter = substrings.at(2).front();
		char point_delimiter = substrings.at(3).front();

		const std::string trajectory_function_name = substrings.at(4);

		const float length = std::stof(substrings.at(5));

		int header_length = 0;
		if (substrings.size() > 6)
			header_length = std::stoi(substrings.at(6));

		const auto trajectory_function = get_trajectory_function(trajectory_function_name);

		if (point_delimiter == 'N')
			point_delimiter = '\n';

		Trajectory trajectory = parse_trajectory(trajectory_file_path, x_y_delimiter, point_delimiter, header_length);

		trajectories.emplace_back(name, trajectory, trajectory_function, length);
	}, 0);

	return trajectories;
}
