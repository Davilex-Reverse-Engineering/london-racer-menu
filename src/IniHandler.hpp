#ifndef INI_HANDLER_HPP
#define INI_HANDLER_HPP

#include <string>
#include <map>
#include <vector>

class IniHandler {
public:
  IniHandler();
  ~IniHandler();

  bool load(const std::string &file_name);
  bool write();
  bool write(const std::string &file_name);

  std::string getContentAsString();
  void printContent();

  std::string getValue(std::string section, std::string key, const std::string &default_value = "");
  std::vector<std::string> getValues(std::string section, std::string key);

  int getInt(std::string section, std::string key);
  std::vector<int> getInts(std::string section, std::string key);

  float getFloat(std::string section, std::string key);
  std::vector<float> getFloats(std::string section, std::string key);

  void setValue(std::string section, std::string key, const std::string &value);
  void setValue(std::string section, std::string key, int value);
  void setValue(std::string section, std::string key, float value);
  void setValue(std::string section, std::string key, std::vector<std::string> values);
  void setValue(std::string section, std::string key, std::vector<int> values);
  void setValue(std::string section, std::string key, std::vector<float> values);

private:
  std::string file_name = "";
  std::map<std::string, std::map<std::string, std::string>> content;

};

#endif // INI_HANDLER_HPP