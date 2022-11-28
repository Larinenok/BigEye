#include <iostream>

#include "utils/args.hpp"

namespace utils {

namespace program_options {

option::option(const char name_short, const std::string name_long, const std::string description, const bool value) :
    name_short(name_short), name_long(name_long), description(description), value(value ? "0" : "\0") {}

char option::get_name_short() const {
    return this->name_short;
}

std::string option::get_name_long() const {
    return this->name_long;
}

std::string option::get_description() const {
    return this->description;
}

std::string option::get_value() const {
    return this->value;
}

void option::set_value(const std::string value) {
    this->value = value;
}

options_map::options_map() {}

option_init::option_init(option_description *owner) : owner(owner) {}

option_init &option_init::operator()(const std::string name, const std::string description, const bool value) {
    owner->add(name, description, value);

    int end = name.length();

    if (end > owner->option_name_length) {
        owner->option_name_length = end;
    }

    return *this;
}

option_description::option_description(const std::string &description) :
    description(description) {}

option_init option_description::add_options() {
    return option_init(this);
}

void option_description::add(const std::string name, const std::string description, const bool value) {
    this->options.push_back(option(name[name.length() - 1], name.substr(0, name.length() - 2), description, value));
}

std::vector<option> option_description::get_options() const {
    return this->options;
}

std::ostream &operator<<(std::ostream &os, const option_description &od) {
    std::string s_options;

    for (auto option : od.options) {
        s_options += "  -";
        s_options += option.get_name_short();
        s_options += " [ --" + option.get_name_long();
        if (option.get_value() == "0")
            s_options += " arg ]";
        else
            s_options += " ]    ";
        
        for (int j = option.get_name_long().length(); j < od.option_name_length; j++) {
            s_options += ' ';
        }
        
        s_options += "   " + option.get_description() + '\n';
    }
    s_options.pop_back();

    return os << od.description << std::endl << s_options;
}

options_map
parse_command_line(const int argc, char *argv[], option_description &od) {
    options_map out;
    bool is_found = false;
    bool is_value = false;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == '-') {
                is_found = false;
                std::string name = argv[i];
                name = name.substr(2, name.length());
                for (auto option : od.get_options()) {
                    if (option.get_name_long() == name) {
                        if (option.get_value() != "\0") {
                            if (argv[i + 1] == NULL || argv[i + 1][0] == '-') break;
                            option.set_value(argv[i + 1]);
                            i++;
                        }
                        out.insert({option.get_name_long(), option});
                        is_found = true;
                        break;
                    }
                }
                if (!is_found) {   
                    out.insert({"error", option('\0', "error", "error")});
                    continue;
                }
                continue;
            }
            int j = 1;
            is_value = false;
            while (argv[i][j++] != '\0') {
                if (is_value) break;
                is_found = false;
                for (auto option : od.get_options()) {
                    if (option.get_name_short() == argv[i][j - 1]) {
                        if (option.get_value() != "\0") {
                            is_value = true;
                            if (argv[i + 1] == NULL || argv[i + 1][0] == '-' || argv[i][j] != '\0') break;
                            option.set_value(argv[i + 1]);
                            i++;
                        }
                        out.insert({option.get_name_long(), option});
                        is_found = true;
                        break;
                    }
                }
                if (!is_found) {
                    out.insert({"error", option('\0', "error", "error")});
                    break;
                }
            }
        }
    }

    return out;
}

void store(const options_map &in, options_map &out) {
    out = in;
}

}// end program_options

}// end utils