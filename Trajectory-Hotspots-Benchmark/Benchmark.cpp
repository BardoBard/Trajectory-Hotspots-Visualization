#include "Window/file_pick_table_widget.h"
#include "Window/main_window.h"

std::string out_path = std::filesystem::current_path().string() + "\\templates\\";

/// \brief Generates an output file of the benchmark data of the given type. 
/// \param path path to the output file
/// \param type_name name of the output file type
/// \param mustache_template mustache template for the output file
/// \param bench benchmark data
/// \param template_path path to the mustache template file
void generate_file_type(const std::string& path, const std::string& type_name, const char* mustache_template,
                        ankerl::nanobench::Bench const& bench, std::filesystem::path template_path)
{
	template_path = std::filesystem::path(path).append("mustache-template." + type_name);

	if (!parser::parser::dir_exists(path))
		std::filesystem::create_directory(path);

	if (!parser::parser::file_exists(template_path.string()))
	//make new file
	{
		std::ofstream template_file(template_path.string());
		template_file << mustache_template;
		template_file.close();
	}

	const std::string mustache_template_string = parser::parser::get_file_raw(template_path.string());

	std::ofstream render_out(path + bench.title() + "-visualization." + type_name);

	ankerl::nanobench::render(mustache_template_string.c_str(), bench, render_out);
}

static inline file_pick_table_widget::t_file_pick_callback benchmark_callback = [&](const std::string& file_path)
{
	const std::vector<parser::parsed_trajectory> trajectories = parser::parser::parse_config(file_path, ' ');

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

	std::cout << "Output written to: " << out_path << std::endl;
};

int main(int argc, char* argv[])
{
	Float::fixed_epsilon = 0.01f;
	if (argc > 2)
	{
		std::cerr << "Too many arguments provided" << std::endl;
		return 0;
	}

	if (argc == 2)
		out_path = parser::parser::dir_exists(parser::parser::current_path_string + "\\" + argv[1])
			           ? parser::parser::current_path_string + "\\" + argv[1]
			           : argv[1];

	try
	{
		QApplication app(argc, nullptr);
		QWidget* active_window = QApplication::activeWindow();
		main_window main_window(active_window, benchmark_callback, {"Benchmark", "Benchmarking tool"}, "Help");
		main_window.show();

		app.exec();
	}
	catch (const parser::parsing_error& e)
	{
		std::cerr << "Something went wrong during the benchmark: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "Something went wrong during the benchmark." << std::endl;
	}
	return 0;
}
