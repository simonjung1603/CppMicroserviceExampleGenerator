#include <iostream>
#include <experimental/filesystem>
#include <fstream>
#include <map>
#include <exception>
#include <optionparser.h>
#include <algorithm>

namespace fs = std::experimental::filesystem;
#include "CppMsGenerator.h"

enum optionIndex { UNKNOWN, HELP, INTERFACE, TEMPLATE, CPPUS };
const option::Descriptor usage[] =
{
	{UNKNOWN,	0,	"",		"",			option::Arg::None,		"USAGE: CppMicroserviceExampleGenerator [options]\n\n"
																"Options:" },
	{HELP,		0,	"h",	"help",		option::Arg::None,		"  --help, -help	\tPrint usage and exit." },
	{INTERFACE,	0,	"i",	"interface",option::Arg::Optional,	"  --interface, -i \tThe name of the example interface."},
	{TEMPLATE,	0,	"t",	"template",	option::Arg::Optional,	"  --template, -t \tThe path to the template folder."},
	{CPPUS,		0,	"c",	"cppus",	option::Arg::Optional,	"  --cppus, -c \tOptional, the path to the cppmicroservices binary dir (containing CppMicroServicesConfig.cmake)."},
	{0,0,0,0,0,0}
};
int main(int argc, char* argv[])
{
	std::string programName(argv[0]);
	argc -= (argc > 0); argv += (argc > 0);	// skip program name if present

	option::Stats  stats(usage, argc, argv);
	std::vector<option::Option> options(stats.options_max);
	std::vector<option::Option> buffer(stats.buffer_max);
	option::Parser parse(usage, argc, argv, &options[0], &buffer[0]);

	if (parse.error())
		return 1;

	if (options[HELP] || argc < 2) {
		option::printUsage(std::cout, usage);
		return 0;
	}

	std::string service_interface_name(options[INTERFACE].arg);
	std::string service_interface_dir = service_interface_name + "_dir";

	std::string service_implementation_name = service_interface_name + "Implementation";
	std::string service_implementation_dir = service_implementation_name + "_dir";
	std::string service_consumer_name = service_interface_name + "Consumer";
	std::string service_consumer_dir = service_consumer_name + "_dir";
	std::string project_name = service_interface_name + "Project";
	std::string project_dir = service_interface_name + "Project";

	service_interface_name.append("Interface");

	std::string export_header = service_interface_name + "_export";
	std::transform(export_header.begin(), export_header.end(), export_header.begin(), ::tolower);

	std::string export_macro = service_interface_name + "_EXPORT";
	std::transform(export_macro.begin(), export_macro.end(), export_macro.begin(), ::toupper);


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
		{ "@{service_interface_export_macro}", export_macro },
		{ "@{cppmicroservices_path}", argc < 4 ? "" : options[CPPUS].arg },
		{ "@{cppmicroservices_debug_dll_dir}", "" },
		{ "@{cppmicroservices_release_dll_dir}", "" }
	};

	fs::path templatePath = fs::path(options[TEMPLATE].arg);
	if (templatePath.is_relative()) {
		fs::path p = fs::path(programName).parent_path() /= (options[TEMPLATE].arg);
		std::cout << "Relative template path was specified. Trying " << p << '\n';
		std::cout << p;
		if (!fs::is_directory(p)) {
			std::cout << p << " is not a valid directory.\n";
			return 0;
		}
		removeTrailingSeparator(p);
		templatePath = p;
	}

	fs::path findCppMicroservicePath = replacements.at("@{cppmicroservices_path}");
	if (!findCppMicroservicePath.empty()) {
		findCppMicroservicePath.make_preferred();
		removeTrailingSeparator(findCppMicroservicePath);
		fs::path corrected;
		for (int i = 0; i < findCppMicroservicePath.string().length(); ++i) {
			char c = findCppMicroservicePath.string().at(i);
			char nextC = i < findCppMicroservicePath.string().length() - 1 ? findCppMicroservicePath.string().at(i + 1) : '\0';
			if (c == '\\' && nextC != '\\') {
				corrected += "\\\\";
			}
			else {
				corrected += c;
			}
		}
		findCppMicroservicePath = corrected;
		if (fs::is_directory(findCppMicroservicePath)) {
			replacements["@{cppmicroservices_path}"] = "PATHS " + findCppMicroservicePath.string();
			replacements["@{cppmicroservices_debug_dll_dir}"] = findCppMicroservicePath.string().append("\\\\bin\\\\Debug");
			replacements["@{cppmicroservices_release_dll_dir}"] = findCppMicroservicePath.string().append("\\\\bin\\\\Release");
		}
		else {
			std::cout << "Couldn't handly specified CppMicroservice path. Removing argument.\n";
			replacements["@{cppmicroservices_path}"] = "";
		}
	}

	fs::create_directories(project_dir + "/" + service_interface_dir);
	fs::create_directories(project_dir + "/" + service_implementation_dir);
	fs::create_directories(project_dir + "/" + service_consumer_dir);

	for (auto& p : fs::recursive_directory_iterator(templatePath))
	{
		if (fs::is_regular_file(p))
		{
			std::cout << "Modifying file " << p << std::endl;
			//Open stream to read in the template file

			try {
				std::ifstream in(p.path());

				//Open stream to write the converted file to
				fs::path outFile(project_dir);
				if (p.path().filename() == "main.cpp" || p.path().filename() == "CMakeLists.txt")
				{
					if (p.path().parent_path().filename() == templatePath.filename()) {
						outFile /= (p.path().filename());
					}
					else {
						outFile /= (replacements.at(std::string("@{") + p.path().parent_path().filename().string() + std::string("}")));
						outFile /= (p.path().filename());
					}
				}
				else if (p.path().filename() == "service.cpp" || p.path().filename() == "service.h")
				{
					outFile /= (service_interface_dir);
					p.path().extension() == ".cpp" ? outFile /= (service_interface_name + ".cpp") : outFile /= (service_interface_name + ".h");
				}
				else if (p.path().filename() == "implementation.cpp" || p.path().filename() == "implementation_manifest.json")
				{
					outFile /= (service_implementation_dir);
					p.path().extension() == ".cpp" ? outFile /= (service_implementation_name + ".cpp") : outFile.append("manifest.json");
				}
				else if (p.path().filename() == "consumer.cpp" || p.path().filename() == "consumer_manifest.json")
				{
					outFile /= (service_consumer_dir);
					p.path().extension() == ".cpp" ? outFile /= (service_consumer_name + ".cpp") : outFile.append("manifest.json");
				}
				else if (p.path().extension() == ".in")
				{
					outFile /= (p.path().filename());
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
			catch (std::exception e) {
				std::cout << e.what();
			}
		}
	}
	return 0;
}

void removeTrailingSeparator(fs::path &p)
{
	while (p.string().find_last_of(p.preferred_separator) == p.string().length() - 1) {
		p = fs::path(p.string().substr(0, p.string().length() - 1));
	}
}
