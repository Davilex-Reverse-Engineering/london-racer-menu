#include "SaveHandler.hpp"

#include "Utils.hpp"

SaveHandler::SaveHandler()
{
}

SaveHandler::~SaveHandler()
{
}

bool SaveHandler::load()
{
  SDL_IOStream * stream = SDL_IOFromFile(Utils::getFullPath(this->file_name).c_str(), "rb");
  if (stream == NULL) {
    SDL_Log("Could not load file Current.rcr as stream");
    return false;
  }

  return true;
}

bool SaveHandler::write()
{  SDL_IOStream * stream = SDL_IOFromFile(Utils::getFullPath(this->file_name).c_str(), "rb");
  if (stream == NULL) {
    SDL_Log("Could not load file Scores.dat as stream");
    return false;
  }

  return false;
}
