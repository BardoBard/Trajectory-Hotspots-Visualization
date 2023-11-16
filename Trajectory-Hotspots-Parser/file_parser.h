//
// Created by Bardio - NHL_STENDEN
//
#pragma once
namespace parser
{
	struct parsed_trajectory
	{
	public:
		typedef AABB (Trajectory::*trajectory_function_callback)(Float length) const;

	private:
		trajectory_function_callback trajectory_function_{};
		Float length_{};

	public:
		std::string name{};
		Trajectory trajectory{{}};

		parsed_trajectory(const std::string& name, const Trajectory& trajectory,
		                  const trajectory_function_callback trajectory_function, const Float& length)
			: trajectory_function_(trajectory_function), length_(length), name(name), trajectory(trajectory)
		{
		}
		parsed_trajectory()
		{
		}

		AABB run_trajectory_function() const
		{
			return (trajectory.*trajectory_function_)(length_);
		}
	};

	/// \brief checks if directory exists
	/// \param file_name path to file + file name
	/// \return true if directory exists
	inline bool dir_exists(const std::string& file_name) noexcept
	{
		return std::filesystem::is_directory(file_name);
	}

	const inline static std::filesystem::path current_path = std::filesystem::current_path();
	const inline static std::string current_path_string = std::filesystem::current_path().string();


	/// \brief checks if file exists
	/// \param file_name file_name path to file + file name
	/// \return true if file exists
	inline bool file_exists(const std::string& file_name) noexcept
	{
		return std::filesystem::is_regular_file(file_name);
	}

	inline std::string read_file(const std::string& file_path)
	{
		return std::string(std::istreambuf_iterator<char>(std::ifstream(file_path).rdbuf()),
		                   std::istreambuf_iterator<char>());
	}

	/// \brief returns raw file as string
	/// \param file_path path to file + file name
	///	\throw std::exception if file does not exist
	/// \return raw file as string
	inline std::string get_file_raw(const std::string& file_path);

	/// \brief loops through file line by line and calls callback function
	/// \param file_path path to file + file name
	/// \param callback function that gets called for each line
	/// \param header_length amount of lines to skip
	///	\throw std::exception if file does not exist
	inline void parse_file_by_line(const std::string& file_path,
	                               const std::function<void(std::string& line)>& callback, int header_length);

	/// \brief loops through file line by line and calls callback function for each substring
	/// \param file_path path to file + file name
	/// \param delimiter delimiter between substrings
	/// \param callback function that gets called for each substring for each line
	/// \param header_length amount of lines to skip
	///	\throw std::exception if file does not exist
	inline void parse_file_with_delimiter(const std::string& file_path, const char delimiter,
	                                      const std::function<void(const std::string& substr)>& callback,
	                                      int header_length);

	/// \brief returns trajectory from file
	/// \param file_path path to file + file name
	/// \param x_y_delimiter delimiter between x and y
	/// \param point_delimiter delimiter between points
	/// \param header_length amount of lines to skip
	///	\throw std::exception if file does not exist
	///	\throw std::exception if trajectory is empty or has less than 2 points
	/// \return Trajectory with points from file
	inline Trajectory parse_trajectory(const std::string& file_path, const char x_y_delimiter,
		const char point_delimiter, int header_length);

	/// \brief returns trajectory from file where every line is a point
	/// \param file_path path to file + file name
	/// \param x_y_delimiter delimiter between x and y
	/// \param header_length amount of lines to skip
	///	\throw std::exception if file does not exist
	///	\throw std::exception if trajectory is empty or has less than 2 points
	/// \return Trajectory with points from file
	inline Trajectory parse_trajectory(const std::string& file_path, const char x_y_delimiter, int header_length);

	/**
	 * \brief finds all trajectories in config file parses them and returns them
	 * \param config_file_path path to config file
	 * \param delimiter delimiter between each item
	 * \return list of parsed trajectories
	 * \throw std::exception if file does not exist
	 * \throw std::exception if trajectory function is not found
	 * \throw std::exception if trajectory is empty or has less than 2 points
	 */
	inline std::vector<::parser::parsed_trajectory> parse_config(const std::string& config_file_path,
	                                                             const char delimiter);

	/// \brief hashes a string which is a constexpr
	/// \param str string to hash
	/// \param h can be left empty
	/// \return unsigned int hash
	constexpr unsigned hash_string(const char* str, const unsigned h = 0)
	{
		return !str[h] ? 5381 : (hash_string(str, h + 1) * 33) ^ str[h];
	}

	/// \brief returns trajectory function based on name
	/// \param name name of the trajectory function
	///	\throw std::exception if trajectory function is not found
	/// \return a trajectory function callback
	constexpr inline parsed_trajectory::trajectory_function_callback get_trajectory_function(const std::string_view& name)
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
			throw std::exception("Trajectory function not found");
		}
		return nullptr;
	}
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

void parser::parse_file_by_line(const std::string& file_path, const std::function<void(std::string& line)>& callback,
                                int header_length = 0)
{
	if (!file_exists(file_path))
		throw std::exception("File does not exist");
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

void parser::parse_file_with_delimiter(const std::string& file_path, const char delimiter,
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

Trajectory parser::parse_trajectory(const std::string& file_path, const char x_y_delimiter,
	const int header_length = 0)
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
	parse_file_by_line(file_path, [&](const std::string& substr)
		{
			// throws exception if substring is not a float
			const float x = std::stof(substr.substr(0, substr.find(x_y_delimiter)));
			const float y = std::stof(substr.substr(substr.find(x_y_delimiter) + 1, substr.size()));
			trajectory_points.emplace_back(x, y);
		}, header_length);

	if (trajectory_points.empty() || trajectory_points.size() < 2)
		throw std::runtime_error(trajectory_points.empty() ? "Trajectory is empty" : " Trajectory has less than 2 points");

	return { trajectory_points };
}
Trajectory parser::parse_trajectory(const std::string& file_path, const char x_y_delimiter, const char point_delimiter,
                                    const int header_length = 0)
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
	}, header_length);

	if (trajectory_points.empty() || trajectory_points.size() < 2)
		throw std::runtime_error( trajectory_points.empty() ? "Trajectory is empty" : " Trajectory has less than 2 points");

	return {trajectory_points};
}

inline std::vector<std::string> get_all_substring_by_delimiter(std::string str, const char delimiter)
{
	std::vector<std::string> result;

	std::stringstream data(str);

	while (std::getline(data, str, delimiter))
	{
		// if there are multiple delimiters in a row we want to add the delimiter as a substring
		// example: "test|||test" should return ["test", "|", "test"]
		// example: "test||test" should return ["test", "test"]
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

inline std::vector<parser::parsed_trajectory> parser::parse_config(const std::string& config_file_path,
                                                                   const char delimiter)
{
	if (!file_exists(config_file_path))
		throw std::exception("File does not exist");

	std::vector<parsed_trajectory> trajectories;
	//TODO: find reserving size based on lines

	parse_file_by_line(config_file_path, [&](const std::string& line)
	{
		//TODO: work with tuples instead of vector
		const std::vector<std::string> substrings = get_all_substring_by_delimiter(line, delimiter);

		if (substrings.size() > 7)
			throw std::runtime_error("\"" + line + "\" has too many arguments");
		const std::string name = substrings[0];
		const std::string trajectory_file_path = substrings[1];

		const char x_y_delimiter = substrings[2].front();
		const char point_delimiter = substrings[3].front();

		const std::string trajectory_function_name = substrings[4];

		const float length = std::stof(substrings[5]);

		int header_length = 0;
		if (substrings.size() > 6)
			header_length = std::stoi(substrings[6]);
		 
		const auto trajectory_function = get_trajectory_function(trajectory_function_name);

		Trajectory trajectory;

		if (point_delimiter == '0')
			trajectory = parse_trajectory(trajectory_file_path, x_y_delimiter, header_length);
		else
		trajectory = parse_trajectory(trajectory_file_path, x_y_delimiter, point_delimiter, header_length);

		trajectories.emplace_back(name, trajectory, trajectory_function, length);
	},0);

	return trajectories;
}
