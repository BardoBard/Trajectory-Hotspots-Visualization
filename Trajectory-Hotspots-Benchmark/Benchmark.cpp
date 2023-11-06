int zero = 0;
QApplication app = QApplication(zero, nullptr);
QWidget* active_window = QApplication::activeWindow();

void fixed_length_contiguous(const Trajectory& trajectory)
{
	ankerl::nanobench::doNotOptimizeAway(trajectory.get_hotspot_fixed_length_contiguous(5));
}

void generate_file_type(const std::string& path, const std::string& type_name, const char* mustache_template,
                        ankerl::nanobench::Bench const& bench)
{
	std::ofstream template_out("mustache-template." + type_name);

	template_out << mustache_template;

	std::ofstream render_out(path + bench.title() + "-visualization." + type_name);

	ankerl::nanobench::render(mustache_template, bench, render_out);
}

int main(const int argc, const char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Please provide a file path" << std::endl;
		return 0;
	}
	const auto file_path = argv[1];
	char x_y_delimiter = ',';
	char point_delimiter = ' ';

	if (argc > 2 && strlen(argv[2]) > sizeof(char))
		x_y_delimiter = *argv[2];
	if (argc > 3 && strlen(argv[3]) > sizeof(char))
		point_delimiter = *argv[3];

	try
	{
		const Trajectory trajectory = parser::parse_trajectory(file_path, x_y_delimiter, point_delimiter);

		ankerl::nanobench::Bench bench;

		bench.performanceCounters(true);
		bench.title("benchmark");
		bench.warmup(100);

		bench.minEpochIterations(5427).run("crossing", [&]
		{
			fixed_length_contiguous(trajectory);
		});
		generate_file_type("", "json", ankerl::nanobench::templates::json(), bench);

		generate_file_type("", "html", ankerl::nanobench::templates::htmlBoxplot(), bench);

		generate_file_type("", "csv", ankerl::nanobench::templates::csv(), bench);

		window t_window = window(active_window, "trajectory");
		drawable_trajectory(trajectory).draw(t_window);
		t_window.show();
		return app.exec();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
	return 0;
}
