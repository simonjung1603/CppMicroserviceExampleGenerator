#include <iostream>
#include <filesystem>
#include <fstream>
#include <map>

namespace fs = std::experimental::filesystem;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Please provide a name for the interface that you want to provide and consume.\n";
		return 0;
	}

	std::string service_interface_name(argv[1]);
	std::string service_interface_dir = service_interface_name + "_dir";

	std::string service_implementation_name = service_interface_name + "Implementation";
	std::string service_implementation_dir = service_implementation_name + "_dir";
	std::string service_consumer_name = service_interface_name + "Consumer";
	std::string service_consumer_dir = service_consumer_name + "_dir";
	std::string project_name = service_interface_name + "Project";
	std::string project_dir = service_interface_name + "Project";

	std::string export_header = service_interface_name + "_export";
	std::transform(export_header.begin(), export_header.end(), export_header.begin(), ::tolower);

	std::string export_macro = service_interface_name + "_EXPORT";
	std::transform(export_macro.begin(), export_macro.end(), export_macro.begin(), ::toupper);

	service_interface_name.append("Interface");

	std::map<std::string, std::string> replacements
	{
		{ "@{project_name}", project_name },
		{ "@{project_dir}", project_dir },
		{ "@{service_interface_name}", service_interface_name },
		{ "@{service_interface_dir}", service_interface_dir },
		{ "@{service_implementation_name}", service_implementation_name },
		{ "@{service_implementation_dir}", service_implementation_dir },
		{ "@{service_consumer_name}", service_consumer_name },
		{ "@{service_consumer_dir}", service_consumer_dir },
		{ "@{service_interface_export_header}", export_header },
		{ "@{service_interface_export_macro}", export_macro }
	};

	fs::create_directories(project_dir + "/" + service_interface_dir);
	fs::create_directories(project_dir + "/" + service_implementation_dir);
	fs::create_directories(project_dir + "/" + service_consumer_dir);

	for (auto& p : fs::recursive_directory_iterator("template"))
	{
		if (fs::is_regular_file(p))
		{
			std::cout << "Modifying file " << p << std::endl;
			//Open stream to read in the template file
			std::ifstream in(p.path());

			//Open stream to write the converted file to
			fs::path outFile(project_dir);
			if (p.path().filename() == "main.cpp" || p.path().filename() == "CMakeLists.txt")
			{
				outFile.append(p.path().filename());
			}
			else if (p.path().filename() == "service.cpp" || p.path().filename() == "service.h")
			{
				outFile.append(service_interface_dir);
				p.path().extension() == ".cpp" ? outFile.append(service_interface_name + ".cpp") : outFile.append(service_interface_name + ".h");
			}
			else if (p.path().filename() == "implementation.cpp" || p.path().filename() == "implementation_manifest.json")
			{
				outFile.append(service_implementation_dir);
				p.path().extension() == ".cpp" ? outFile.append(service_implementation_name + ".cpp") : outFile.append("manifest.json");
			}
			else if (p.path().filename() == "consumer.cpp" || p.path().filename() == "consumer_manifest.json")
			{
				outFile.append(service_consumer_dir);
				p.path().extension() == ".cpp" ? outFile.append(service_consumer_name + ".cpp") : outFile.append("manifest.json");
			}
			std::cout << outFile << '\n';
			std::ofstream out(outFile);

			if (in.is_open() && out.is_open())
			{
				std::string line;
				while (std::getline(in, line))
				{
					for (auto& r : replacements)
					{
						size_t start_pos = line.find(r.first);
						while (start_pos != std::string::npos)
						{
							line.replace(start_pos, r.first.length(), r.second);
							start_pos = line.find(r.first);
						}
					}
					out << line << '\n';
				}
				in.close();
				out.close();
			}
			else
			{
				std::cout << "Unable to open file\n";
			}
		}
	}
	return 0;
}