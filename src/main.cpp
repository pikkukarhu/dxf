#include "document/Document.h"
#include <iostream>
#include <getopt.h>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void print_usage(const char* progname) {
    std::cout << "Usage: " << progname << " [options] <input_file.dxf>\n"
              << "Options:\n"
              << "  -b, --bounding_box        Show bounding boxes in SVG output\n"
              << "  -s, --svg [filename]      Export to SVG (default: input_base.svg)\n"
              << "  -j, --json [filename]     Export to JSON (default: input_base.json)\n"
              << "  -h, --help                Display this help message\n";
}

std::string get_default_filename(const std::string& input, const std::string& ext) {
    fs::path p(input);
    return p.stem().string() + ext;
}

int main(int argc, char** argv) {
    bool show_bb = false;
    bool do_svg = false;
    bool do_json = false;
    std::string svg_file;
    std::string json_file;
    std::string input_file;

    static struct option long_options[] = {
        {"bounding_box", no_argument,       0, 'b'},
        {"svg",          optional_argument, 0, 's'},
        {"json",         optional_argument, 0, 'j'},
        {"help",         no_argument,       0, 'h'},
        {0, 0, 0, 0}
    };

    int opt;
    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "bs::j::h", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'b':
                show_bb = true;
                break;
            case 's':
                do_svg = true;
                if (optarg) svg_file = optarg;
                break;
            case 'j':
                do_json = true;
                if (optarg) json_file = optarg;
                break;
            case 'h':
                print_usage(argv[0]);
                return 0;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }

    if (optind < argc) {
        input_file = argv[optind];
    } else {
        std::cerr << "Error: Missing input filename.\n";
        print_usage(argv[0]);
        return 1;
    }

    try {
        dxf::Entity::setShowBoundingBox(show_bb);

        std::cout << "Reading file: " << input_file << std::endl;
        dxf::Document d(input_file);

        if (do_svg) {
            if (svg_file.empty()) svg_file = get_default_filename(input_file, ".svg");
            std::cout << "Exporting SVG to: " << svg_file << std::endl;
            d.export_svg(svg_file);
        }

        if (do_json) {
            if (json_file.empty()) json_file = get_default_filename(input_file, ".json");
            std::cout << "Exporting JSON to: " << json_file << std::endl;
            d.export_json(json_file);
        }

        // If no export options selected, do nothing or show warning
        if (!do_svg && !do_json) {
            std::cout << "No output options selected. Use --svg or --json to generate output.\n";
        }

    } catch (const std::string& e) {
        std::cerr << "Error: " << e << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
