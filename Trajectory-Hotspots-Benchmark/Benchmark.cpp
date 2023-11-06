//benchmark simple function

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

const auto vec_line = std::vector<Vec2>{
	Vec2(0, 0), Vec2(1, 1), Vec2(2, 2), Vec2(3, 3), Vec2(4, 4), Vec2(5, 5), Vec2(6, 6), Vec2(7, 7), Vec2(8, 8),
	Vec2(9, 9),
};
const auto vec_crossing = std::vector<Vec2>{
	//random vec2 within a box of -5, -5 to 10, 10
	Vec2(0, 10), Vec2(5, 5), Vec2(7, 9), Vec2(-4, 10), Vec2(10, 10), Vec2(-7, 2), Vec2(1, -5), Vec2(2, 5), Vec2(10, 10),
	Vec2(-10, -10),
};

int main(const int argc, const char* argv[])
{
	ankerl::nanobench::Bench bench;

	bench.performanceCounters(true);
	bench.title("benchmark");
	bench.warmup(100);

	const Trajectory t_line = Trajectory(vec_line);
	bench.minEpochIterations(54272).run("line", [&]
	{
		fixed_length_contiguous(t_line);
	});

	const Trajectory t_crossing = Trajectory(vec_crossing);
	bench.minEpochIterations(54272).run("crossing", [&]
	{
		fixed_length_contiguous(t_crossing);
	});
	generate_file_type("", "json", ankerl::nanobench::templates::json(), bench);

	generate_file_type("", "html", ankerl::nanobench::templates::htmlBoxplot(), bench);

	generate_file_type("", "csv", ankerl::nanobench::templates::csv(), bench);
}
