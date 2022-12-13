#pragma once

#include <string>

namespace runtime {

/* RUNTIME FLAGS*/
extern bool FLAG_headless;
extern bool FLAG_dryRun;
extern bool FLAG_useCuda;
extern bool FLAG_noScan;

/* RUNTIME KEYS */
extern std::string KEY_db_backend;
extern std::string KEY_db_address;
extern std::string KEY_db_user;
extern std::string KEY_db_passwd;
extern std::string KEY_db_name;

}