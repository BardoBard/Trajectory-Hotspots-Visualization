int zero = 0;
QApplication app = QApplication(zero, nullptr);
QWidget* active_window = QApplication::activeWindow();

void fixed_length_contiguous(const Trajectory& trajectory)
{
	ankerl::nanobench::doNotOptimizeAway(trajectory.get_hotspot_fixed_length_contiguous(5));
}

void generate_file_type(const std::string& path, const std::string& type_name, const char* mustache_template,
                        ankerl::nanobench::Bench const& bench, std::filesystem::path template_path)
{
	template_path = std::filesystem::path(path).append("mustache-template." + type_name);

	if (!parser::file_exists(template_path.string()))
		throw std::runtime_error("Mustache template file does not exist");

	const std::string mustache_template_string = parser::get_file_raw(template_path.string());

	std::cout << "Generating " << type_name << " file..." << std::endl;
	std::cout << mustache_template_string << std::endl;
	std::ofstream render_out(path + bench.title() + "-visualization." + type_name);

	ankerl::nanobench::render(template_path.string().c_str(), bench, render_out);
}

void generate_file_type(const std::string& path, const std::string& type_name, const char* mustache_template,
                        ankerl::nanobench::Bench const& bench)
{
	generate_file_type(path, type_name, mustache_template, bench, parser::current_path_string);
}

int main(const int argc, const char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Please provide a file path to the config file" << std::endl;
		return 0;
	}
	const std::string file_path = argv[1];
	const std::string out_path = argc > 2 ? argv[2] : std::filesystem::current_path().string();
	try
	{
		const std::vector<parser::parsed_trajectory> trajectories = parser::parse_config(file_path, ' ');

		ankerl::nanobench::Bench bench;

		bench.performanceCounters(true);
		bench.title("benchmark");

		std::cout << "Benchmarking..." << std::endl;
		for (const auto& parsed_trajectory : trajectories)
		{
			bench.warmup(10).minEpochIterations(100).run(parsed_trajectory.name, [&]
			{
				ankerl::nanobench::doNotOptimizeAway(parsed_trajectory.run_trajectory_function());
			});
		}

		generate_file_type(out_path, "json", ankerl::nanobench::templates::json(), bench, out_path);

		generate_file_type(out_path, "html", ankerl::nanobench::templates::htmlBoxplot(), bench, out_path);

		generate_file_type(out_path, "csv", ankerl::nanobench::templates::csv(), bench, out_path);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
	return 0;
}
