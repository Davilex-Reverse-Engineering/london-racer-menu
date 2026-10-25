#include "VideoHandler.hpp"

#include "Utils.hpp"

VideoHandler::VideoHandler()
{
}

VideoHandler::~VideoHandler()
{
}

VideoHandler * VideoHandler::getInstance()
{
  static VideoHandler instance;
  return &instance;
}

typedef struct __attribute__((packed)) {
  char fccType[4];
  char fccHandler[4];
  uint32_t dwFlags;
  uint16_t wPriority;
  uint16_t wLanguage;
  uint32_t dwInitialFrames;
  uint32_t dwScale;
  uint32_t dwRate;
  uint32_t dwStart;
  uint32_t dwLength;
  uint32_t dwSuggestedBufferSize;
  uint32_t dwQuality;
  uint32_t dwSampleSize;
  uint16_t rcFrame[4];
} AVIStreamHeader;

SDL_Texture * VideoHandler::getTexture(SDL_Renderer * renderer, const std::string &file_path)
{
  SDL_IOStream * stream = SDL_IOFromFile(Utils::getFullPath(file_path).c_str(), "rb");
  if (stream == NULL) {
    SDL_Log("Could not load file Scores.dat as stream");
    return nullptr;
  }

  char RIFF[5];
  SDL_ReadIO(stream, RIFF, 4);
  RIFF[4] = '\0';
  SDL_Log("RIFF: %s", RIFF);
  if (RIFF[0] != 'R' || RIFF[1] != 'I' || RIFF[2] != 'F' || RIFF[3] != 'F') {
    SDL_Log("Expected RIFF header was %s", RIFF);
    SDL_CloseIO(stream);
    return nullptr;
  }

  uint32_t size = 0;
  if(!SDL_ReadU32LE(stream, &size)) {
    SDL_Log("Could not read size");
    SDL_CloseIO(stream);
    return nullptr;
  }
  SDL_Log("size: %i", size);

  char AVI[5];
  SDL_ReadIO(stream, AVI, 4);
  AVI[4] = '\0';
  SDL_Log("AVI: %s", AVI);
  if (AVI[0] != 'A' || AVI[1] != 'V' || AVI[2] != 'I' || AVI[3] != ' ') {
    SDL_Log("Expected AVI header was %s", AVI);
    SDL_CloseIO(stream);
    return nullptr;
  }

  char LIST[5];
  SDL_ReadIO(stream, LIST, 4);
  LIST[4] = '\0';
  SDL_Log("LIST: %s", LIST);
  if (LIST[0] != 'L' || LIST[1] != 'I' || LIST[2] != 'S' || LIST[3] != 'T') {
    SDL_Log("Expected LIST header was %s", LIST);
    SDL_CloseIO(stream);
    return nullptr;
  }

  uint32_t hdrl_list_size = 0;
  if(!SDL_ReadU32LE(stream, &hdrl_list_size)) {
    SDL_Log("Could not read hdrl_list_size");
    SDL_CloseIO(stream);
    return nullptr;
  }
  SDL_Log("hdrl_list_size: %i", hdrl_list_size);

  char hdrl[5];
  SDL_ReadIO(stream, hdrl, 4);
  hdrl[4] = '\0';
  SDL_Log("hdrl: %s", hdrl);
  if (hdrl[0] != 'h' || hdrl[1] != 'd' || hdrl[2] != 'r' || hdrl[3] != 'l') {
    SDL_Log("Expected hdrl header was %s", hdrl);
    SDL_CloseIO(stream);
    return nullptr;
  }

  char avih[5];
  SDL_ReadIO(stream, avih, 4);
  avih[4] = '\0';
  SDL_Log("avih: %s", avih);
  if (avih[0] != 'a' || avih[1] != 'v' || avih[2] != 'i' || avih[3] != 'h') {
    SDL_Log("Expected avih header was %s", avih);
    SDL_CloseIO(stream);
    return nullptr;
  }

  uint32_t avih_size = 0;
  if(!SDL_ReadU32LE(stream, &avih_size)) {
    SDL_Log("Could not read avih_size");
    SDL_CloseIO(stream);
    return nullptr;
  }
  SDL_Log("avih_size: %i", avih_size);

  uint32_t dwMicroSecPerFrame = 0;
  if(!SDL_ReadU32LE(stream, &dwMicroSecPerFrame)) {
    SDL_Log("Could not read readdwMicroSecPerFrame");
    SDL_CloseIO(stream);
    return nullptr;
  }
  SDL_Log("dwMicroSecPerFrame: %i", dwMicroSecPerFrame);

  uint32_t dwMaxBytesPerSec = 0;
  if(!SDL_ReadU32LE(stream, &dwMaxBytesPerSec)) {
    SDL_Log("Could not read dwMaxBytesPerSec");
    SDL_CloseIO(stream);
    return nullptr;
  }
  SDL_Log("dwMaxBytesPerSec: %i", dwMaxBytesPerSec);

  uint32_t dwPaddingGranularity = 0;
  if(!SDL_ReadU32LE(stream, &dwPaddingGranularity)) {
    SDL_Log("Could not read dwPaddingGranularity");
    SDL_CloseIO(stream);
    return nullptr;
  }
  SDL_Log("dwPaddingGranularity: %i", dwPaddingGranularity);

  uint32_t dwFlags = 0;
  if(!SDL_ReadU32LE(stream, &dwFlags)) {
    SDL_Log("Could not read dwFlags");
    SDL_CloseIO(stream);
    return nullptr;
  }
  SDL_Log("dwFlags: %i", dwFlags);

  uint32_t dwTotalFrames = 0;
  if(!SDL_ReadU32LE(stream, &dwTotalFrames)) {
    SDL_Log("Could not read dwTotalFrames");
    SDL_CloseIO(stream);
    return nullptr;
  }
  SDL_Log("dwTotalFrames: %i", dwTotalFrames);

  uint32_t dwInitialFrames = 0;
  if(!SDL_ReadU32LE(stream, &dwInitialFrames)) {
    SDL_Log("Could not read dwInitialFrames");
    SDL_CloseIO(stream);
    return nullptr;
  }
  SDL_Log("dwInitialFrames: %i", dwInitialFrames);

  uint32_t dwStreams = 0;
  if(!SDL_ReadU32LE(stream, &dwStreams)) {
    SDL_Log("Could not read dwStreams");
    SDL_CloseIO(stream);
    return nullptr;
  }
  SDL_Log("dwStreams: %i", dwStreams);

  uint32_t dwSuggestedBufferSize = 0;
  if(!SDL_ReadU32LE(stream, &dwSuggestedBufferSize)) {
    SDL_Log("Could not read dwSuggestedBufferSize");
    SDL_CloseIO(stream);
    return nullptr;
  }
  SDL_Log("dwSuggestedBufferSize: %i", dwSuggestedBufferSize);

  uint32_t dwWidth = 0;
  if(!SDL_ReadU32LE(stream, &dwWidth)) {
    SDL_Log("Could not read dwWidth");
    SDL_CloseIO(stream);
    return nullptr;
  }
  SDL_Log("dwWidth: %i", dwWidth);

  uint32_t dwHeight = 0;
  if(!SDL_ReadU32LE(stream, &dwHeight)) {
    SDL_Log("Could not read dwHeight");
    SDL_CloseIO(stream);
    return nullptr;
  }
  SDL_Log("dwHeight: %i", dwHeight);

  // Here are some reserved bytes that are not used
  for(size_t i = 0; i < 4; i++) {
    uint32_t dwReserved = 0;
    if(!SDL_ReadU32LE(stream, &dwReserved) || dwReserved != 0) {
      SDL_Log("Could not read dwReserved %u", dwReserved);
      SDL_CloseIO(stream);
      return nullptr;
    }
    SDL_Log("dwReserved: %i", dwReserved);
  }


  
  char LIST2[5];
  SDL_ReadIO(stream, LIST2, 4);
  LIST2[4] = '\0';
  SDL_Log("LIST2: %s", LIST2);

  uint32_t LIST2_size = 0;
  if(!SDL_ReadU32LE(stream, &LIST2_size)) {
    SDL_Log("Could not read LIST2_size");
    SDL_CloseIO(stream);
    return nullptr;
  }
  SDL_Log("LIST2_size: %i", LIST2_size);

  char strl[5];
  SDL_ReadIO(stream, strl, 4);
  strl[4] = '\0';
  SDL_Log("strl: %s", strl);

  char strh[5];
  SDL_ReadIO(stream, strh, 4);
  strh[4] = '\0';
  SDL_Log("strh: %s", strh);

  uint32_t strh_size = 0;
  if(!SDL_ReadU32LE(stream, &strh_size)) {
    SDL_Log("Could not read strh_size");
    SDL_CloseIO(stream);
    return nullptr;
  }
  SDL_Log("strh_size: %i", strh_size);

  AVIStreamHeader stream_header;
  SDL_ReadIO(stream, &stream_header, sizeof(AVIStreamHeader));
  SDL_Log("fccType: %s", stream_header.fccType);
  SDL_Log("fccHandler: %s", stream_header.fccHandler);
  SDL_Log("dwFlags: %u", stream_header.dwFlags);
  SDL_Log("wPriority: %u", stream_header.wPriority);
  SDL_Log("wLanguage: %u", stream_header.wLanguage);
  SDL_Log("dwInitialFrames: %u", stream_header.dwInitialFrames);
  SDL_Log("dwScale: %u", stream_header.dwScale);
  SDL_Log("dwRate: %u", stream_header.dwRate);
  SDL_Log("dwStart: %u", stream_header.dwStart);
  SDL_Log("dwLength: %u", stream_header.dwLength);
  SDL_Log("dwSuggestedBufferSize: %u", stream_header.dwSuggestedBufferSize);
  SDL_Log("dwQuality: %u", stream_header.dwQuality);
  SDL_Log("dwSampleSize: %u", stream_header.dwSampleSize);


  
  return nullptr;
}
