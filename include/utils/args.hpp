#include <vector>
#include <ostream>
#include <string>
#include <map>

namespace utils {

void parseArgs(const int argc, char *argv[]);

namespace program_options {

class option {
private:
    char name_short;
    std::string name_long;
    std::string description;
    std::string value = "0";

public:
    option(const char name_short, const std::string name_long, const std::string description, const bool value = false);

    char get_name_short() const;
    std::string get_name_long() const;
    std::string get_description() const;

    std::string get_value() const;
    void set_value(const std::string value);
};

class options_map : public std::map<std::string, option> {
private:

public:
    options_map();
};

class option_description;

class option_init {
private:
    option_description *owner;

public:
    option_init(option_description *owner);

    option_init &operator()(const std::string name, const std::string description, const bool value = false);
};

class option_description {
private:
    std::string description;
    std::vector<option> options;

public:
    int option_name_length;
    
public:
    option_description(const std::string &description);

    option_init add_options();

    void add(const std::string name, const std::string description, const bool value = false);
    
    std::vector<option> get_options() const;

    friend std::ostream& operator<<(std::ostream &os, const option_description &od);
};

void parse(int argc, char *argv[]);

options_map
parse_command_line(int argc, char *argv[], option_description &od);

void store(const options_map &in, options_map &out);

}// end program_options

}// end utils
