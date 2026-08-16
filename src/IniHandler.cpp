#include "IniHandler.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "../vendor/SDL/include/SDL3/SDL.h"

#include "Utils.hpp"

IniHandler::IniHandler()
{
}

IniHandler::~IniHandler()
{
}

bool IniHandler::load(const std::string &file_name)
{
  if (file_name.empty()) {
    return false;
  }
  this->file_name = Utils::getFullPath(file_name);
  
  std::ifstream input(this->file_name);
  if (!input) {
    return false;
  }

  std::string line = "";
  std::string current_section = "";
  while (std::getline(input, line)) {
    // Ignore comments
    if (line.starts_with(";")) {
      continue;
    }

    // Remove Windows line endings
    if (line.ends_with("\r")) {
      line = line.substr(0, line.rfind("\r"));
    }

    // Ignore empty lines
    if (line.empty()) {
      continue;
    }

    // Read section name
    if (line.starts_with("[") && line.ends_with("]")) {
      current_section = line.substr(1, line.rfind("]") - 1);
      std::transform(current_section.begin(), current_section.end(), current_section.begin(), ::tolower);  // Convert to lower case
      if (this->content.count(current_section) == 0) {
        this->content[current_section] = std::map<std::string, std::string>();
      }
      continue;
    }

    // Only read values if there is a section name
    if (current_section.empty()) {
      continue;
    }

    // Only lines with a = should be read
    size_t equals_position = line.rfind("=");
    if(equals_position == std::string::npos) {
      continue;
    }

    std::string key = "";
    std::string value = "";
    for (size_t i = 0; i < line.length(); i++) {
      if (i == equals_position) {
        continue;
      }
      if (i < equals_position) {
        if (line[i] != ' ' && line[i] != '\t') {
          key += line[i];
        }
      } else {
        if (line[i] != ' ' && line[i] != '\t') {
          value = line.substr(i);
          break;
        }
      }
    }
    if (!key.empty()) {
      std::transform(key.begin(), key.end(), key.begin(), ::tolower);  // Convert to lower case
      this->content[current_section][key] = value;
    }
  }

  input.close(); 

  return true;
}

bool IniHandler::write()
{
  return this->write(this->file_name);
}

bool IniHandler::write(const std::string &file_name)
{
  if (file_name.empty()) {
    return false;
  }

  std::ofstream output(file_name);
  output << this->getContentAsString();
  output.close();

  if (!output) {
    return false;
  }

  return true;
}

std::string IniHandler::getContentAsString()
{
  std::string content_string = "";
  for (auto section_key : this->content) {
    std::string section = section_key.first;
    content_string += "[" + section + "]\r\n";
    for (auto key_value : section_key.second) {
      content_string += key_value.first + "=" + key_value.second + "\r\n";
    }
    content_string += "\r\n";
  }
  return content_string;
}

void IniHandler::printContent()
{
  SDL_Log("%s", this->getContentAsString().c_str());
}

std::string IniHandler::getValue(std::string section, std::string key, const std::string &default_value)
{
  if (section.empty() || key.empty()) {
    return default_value;
  }

  // Convert both section and key to lower case
  std::transform(section.begin(), section.end(), section.begin(), ::tolower);
  std::transform(key.begin(), key.end(), key.begin(), ::tolower);
  
  if (this->content.count(section) == 0 || this->content[section].count(key) == 0) {
    return default_value;
  }

  return this->content[section][key];
}

std::vector<std::string> IniHandler::getValues(std::string section, std::string key)
{
  std::vector<std::string> list = {};
  std::string value = this->getValue(section, key, "");
  if (value.empty()) {
    return list;
  }

  std::stringstream stream(value);
  std::string entry;
  while(std::getline(stream, entry, ',')) {
      list.push_back(entry);
  }
  return list;
}

int IniHandler::getInt(std::string section, std::string key)
{
  std::string string_value = this->getValue(section, key);
  if (string_value.empty()) {
    return 0;
  }
  return std::atoi(string_value.c_str());
}

std::vector<int> IniHandler::getInts(std::string section, std::string key)
{
  std::vector<int> list = {};
  std::string value = this->getValue(section, key, "");
  if (value.empty()) {
    return list;
  }

  std::stringstream stream(value);
  std::string entry;
  while(std::getline(stream, entry, ',')) {
      list.push_back(std::atoi(entry.c_str()));
  }
  return list;
}

float IniHandler::getFloat(std::string section, std::string key)
{
  std::string string_value = this->getValue(section, key);
  if (string_value.empty()) {
    return 0.0f;
  }
  return std::atof(string_value.c_str());
}

std::vector<float> IniHandler::getFloats(std::string section, std::string key)
{
  std::vector<float> list = {};
  std::string value = this->getValue(section, key, "");
  if (value.empty()) {
    return list;
  }

  std::stringstream stream(value);
  std::string entry;
  while(std::getline(stream, entry, ',')) {
      list.push_back(std::atof(entry.c_str()));
  }
  return list;
}

bool IniHandler::getBool(const std::string &section, const std::string &key) {
  return this->getInt(section, key) > 0;
}

void IniHandler::setValue(std::string section, std::string key, const std::string &value)
{
  if (this->content.count(section) == 0) {
    this->content[section] = std::map<std::string, std::string>();
  }
  this->content[section][key] = value;
}

void IniHandler::setValue(std::string section, std::string key, int value)
{
  this->setValue(section, key, std::to_string(value));
}

void IniHandler::setValue(std::string section, std::string key, float value)
{
  this->setValue(section, key, std::to_string(value));
}

void IniHandler::setValue(std::string section, std::string key, std::vector<std::string> values)
{
  if (this->content.count(section) == 0) {
    this->content[section] = std::map<std::string, std::string>();
  }
  std::string string_value = "";
  for(std::string value : values) {
    string_value += value + ",";
  }
  // Cut off last comma
  string_value = string_value.substr(0, string_value.length() - 2);

  this->content[section][key] = string_value;
}

void IniHandler::setValue(std::string section, std::string key, std::vector<int> values)
{
  if (this->content.count(section) == 0) {
    this->content[section] = std::map<std::string, std::string>();
  }
  std::string string_value = "";
  for(int value : values) {
    string_value += std::to_string(value) + ",";
  }
  // Cut off last comma
  string_value = string_value.substr(0, string_value.length() - 2);

  this->content[section][key] = string_value;
}

void IniHandler::setValue(std::string section, std::string key, std::vector<float> values)
{
  if (this->content.count(section) == 0) {
    this->content[section] = std::map<std::string, std::string>();
  }
  std::string string_value = "";
  for(float value : values) {
    string_value += std::to_string(value) + ",";
  }
  // Cut off last comma
  string_value = string_value.substr(0, string_value.length() - 2);

  this->content[section][key] = string_value;
}