#include "ConfigSetting.h"

ConfigSetting::ConfigSetting() : m_delimiter(std::string(1, '=')), m_commet(std::string(1, '#')), m_filename("config/config.ini") {
}

ConfigSetting::~ConfigSetting() {
}

void ConfigSetting::load(std::string filename) {
    this->m_filename = filename;
    std::ifstream in(this->m_filename.c_str());
    if (!in) throw FileNotFoundException(this->m_filename.c_str());
    in >> (*this);
}

void ConfigSetting::trim(std::string& inout_s) {
    static const char s_whiteSpaces[] = " \n\t\v\r\f";
    inout_s.erase(0, inout_s.find_first_not_of(s_whiteSpaces));
    inout_s.erase(inout_s.find_last_not_of(s_whiteSpaces) + 1U);
}

void ConfigSetting::remove(const std::string& key) {
    this->m_contents.erase(this->m_contents.find(key));
}