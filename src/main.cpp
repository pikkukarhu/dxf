#include "document/Document.h"
#include <iostream>
#include <getopt.h>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void print_usage(const char* progname) {
    std::cout << "Usage: " << progname << " [options] [input_file.dxf]\n"
              << "Options:\n"
              << "  -b, --bounding_box        Show bounding boxes in SVG output\n"
              << "  -s, --svg [filename]      Export to SVG (default: input_base.svg)\n"
              << "  -j, --json [filename]     Export to JSON (default: input_base.json)\n"
              << "  -B, --batch_folder <dir>  Process all .dxf files in the specified directory\n"
              << "  -D, --destination_folder <dir> Save output files to the specified directory\n"
              << "  -h, --help                Display this help message\n";
}

std::string get_output_filename(const std::string& input, const std::string& ext, const std::string& dest_folder) {
    fs::path p(input);
    fs::path out_name = p.stem().string() + ext;
    
    if (!dest_folder.empty()) {
        return (fs::path(dest_folder) / out_name).string();
    }
    return (p.parent_path() / out_name).string();
}

void process_file(const std::string& input_file, bool show_bb, bool do_svg, bool do_json, 
                  const std::string& svg_user_name, const std::string& json_user_name, 
                  const std::string& dest_folder) {
    try {
        dxf::Entity::setShowBoundingBox(show_bb);
        std::cout << "Processing: " << input_file << std::endl;
        dxf::Document d(input_file);

        if (do_svg) {
            std::string svg_path = svg_user_name;
            if (svg_path.empty()) {
                svg_path = get_output_filename(input_file, ".svg", dest_folder);
            } else if (!dest_folder.empty()) {
                // If user gave a filename BUT also a destination folder, combine them
                svg_path = (fs::path(dest_folder) / fs::path(svg_path).filename()).string();
            }
            std::cout << "  -> Exporting SVG to: " << svg_path << std::endl;
            d.export_svg(svg_path);
        }

        if (do_json) {
            std::string json_path = json_user_name;
            if (json_path.empty()) {
                json_path = get_output_filename(input_file, ".json", dest_folder);
            } else if (!dest_folder.empty()) {
                json_path = (fs::path(dest_folder) / fs::path(json_path).filename()).string();
            }
            std::cout << "  -> Exporting JSON to: " << json_path << std::endl;
            d.export_json(json_path);
        }
    } catch (const std::string& e) {
        std::cerr << "Error processing " << input_file << ": " << e << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error processing " << input_file << ": " << e.what() << std::endl;
    }
}

int main(int argc, char** argv) {
    bool show_bb = false;
    bool do_svg = false;
    bool do_json = false;
    std::string svg_file;
    std::string json_file;
    std::string batch_folder;
    std::string dest_folder;
    std::string input_file;

    static struct option long_options[] = {
        {"bounding_box",       no_argument,       0, 'b'},
        {"svg",                optional_argument, 0, 's'},
        {"json",               optional_argument, 0, 'j'},
        {"batch_folder",       required_argument, 0, 'B'},
        {"destination_folder", required_argument, 0, 'D'},
        {"help",               no_argument,       0, 'h'},
        {0, 0, 0, 0}
    };

    int opt;
    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "bs::j::B:D:h", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'b': show_bb = true; break;
            case 's':
                do_svg = true;
                if (optarg) svg_file = optarg;
                break;
            case 'j':
                do_json = true;
                if (optarg) json_file = optarg;
                break;
            case 'B': batch_folder = optarg; break;
            case 'D': dest_folder = optarg; break;
            case 'h':
                print_usage(argv[0]);
                return 0;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }

    if (!dest_folder.empty()) {
        if (!fs::exists(dest_folder)) {
            if (!fs::create_directories(dest_folder)) {
                std::cerr << "Error: Could not create destination directory: " << dest_folder << std::endl;
                return 1;
            }
        }
    }

    if (!batch_folder.empty()) {
        if (!fs::is_directory(batch_folder)) {
            std::cerr << "Error: " << batch_folder << " is not a directory.\n";
            return 1;
        }
        for (const auto& entry : fs::directory_iterator(batch_folder)) {
            if (entry.path().extension() == ".dxf") {
                process_file(entry.path().string(), show_bb, do_svg, do_json, svg_file, json_file, dest_folder);
            }
        }
    } else if (optind < argc) {
        input_file = argv[optind];
        process_file(input_file, show_bb, do_svg, do_json, svg_file, json_file, dest_folder);
    } else {
        std::cerr << "Error: Missing input filename or --batch_folder.\n";
        print_usage(argv[0]);
        return 1;
    }

    return 0;
}
