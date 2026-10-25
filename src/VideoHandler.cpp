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

#define AVI_HEADER_SIZE 56
#define STREAM_HEADER_SIZE 56

typedef struct {
  uint32_t microSecPerFrame;
  uint32_t maxBytesPerSec;
  uint32_t paddingGranularity;
  uint32_t flags;
  uint32_t totalFrames;
  uint32_t initialFrames;
  uint32_t streams;
  uint32_t suggestedBufferSize;
  uint32_t width;
  uint32_t height;
} AviHeader;

typedef struct {
  char fccType[4];
  char fccHandler[4];
  uint32_t flags;
  uint16_t priority;
  uint16_t language;
  uint32_t initialFrames;
  uint32_t scale;
  uint32_t rate;
  uint32_t start;
  uint32_t length;
  uint32_t suggestedBufferSize;
  uint32_t quality;
  uint32_t sampleSize;
  uint16_t rcFrame[4];
} StreamHeader;


AviHeader * readAviHeader(SDL_IOStream * stream, uint32_t size) {
  if (size != AVI_HEADER_SIZE) {
    SDL_Log("AVI Header size %i is unsupported, expected %i", size, AVI_HEADER_SIZE);
    return nullptr;
  }

  AviHeader * header = (AviHeader *) calloc(1, sizeof(AviHeader));

  if(!SDL_ReadU32LE(stream, &header->microSecPerFrame)) {
    SDL_Log("Could not process avi header, microSecPerFrame could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }
  SDL_Log("microSecPerFrame: %i", header->microSecPerFrame);

  if(!SDL_ReadU32LE(stream, &header->maxBytesPerSec)) {
    SDL_Log("Could not process avi header, maxBytesPerSec could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }
  SDL_Log("maxBytesPerSec: %i", header->maxBytesPerSec);

  if(!SDL_ReadU32LE(stream, &header->paddingGranularity)) {
    SDL_Log("Could not process avi header, paddingGranularity could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }
  SDL_Log("paddingGranularity: %i", header->paddingGranularity);

  if(!SDL_ReadU32LE(stream, &header->flags)) {
    SDL_Log("Could not process avi header, flags could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }
  SDL_Log("flags: %i", header->flags);

  if(!SDL_ReadU32LE(stream, &header->totalFrames)) {
    SDL_Log("Could not process avi header, totalFrames could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }
  SDL_Log("totalFrames: %i", header->totalFrames);

  if(!SDL_ReadU32LE(stream, &header->initialFrames)) {
    SDL_Log("Could not process avi header, initialFrames could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }
  SDL_Log("initialFrames: %i", header->initialFrames);

  if(!SDL_ReadU32LE(stream, &header->streams)) {
    SDL_Log("Could not process avi header, streams could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }
  SDL_Log("streams: %i", header->streams);

  if(!SDL_ReadU32LE(stream, &header->suggestedBufferSize)) {
    SDL_Log("Could not process avi header, suggestedBufferSize could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }
  SDL_Log("suggestedBufferSize: %i", header->suggestedBufferSize);

  if(!SDL_ReadU32LE(stream, &header->width)) {
    SDL_Log("Could not process avi header, width could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }
  SDL_Log("width: %i", header->width);

  if(!SDL_ReadU32LE(stream, &header->height)) {
    SDL_Log("Could not process avi header, height could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }
  SDL_Log("height: %i", header->height);

  // Here are some reserved bytes that are not used and should always be 0
  for(size_t i = 0; i < 4; i++) {
    uint32_t reserved_byte = 0;
    if(!SDL_ReadU32LE(stream, &reserved_byte) || reserved_byte != 0) {
      SDL_Log("Could not process avi header, reserved byte %lu (%u) could not be read: %s", i, reserved_byte, SDL_GetError());
      free(header);
      return nullptr;
    }
    SDL_Log("reserved byte: %u", reserved_byte);
  }

  return header;
}

StreamHeader * readStreamHeader(SDL_IOStream * stream, uint32_t size) {
  if (size != STREAM_HEADER_SIZE) {
    SDL_Log("Stream Header size %i is unsupported, expected %i", size, STREAM_HEADER_SIZE);
    return nullptr;
  }

  StreamHeader * header = (StreamHeader *) calloc(1, sizeof(StreamHeader));

  if(!SDL_ReadIO(stream, (void *) &header->fccType, 4)) {
    SDL_Log("Could not process stream header, fccType could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadIO(stream, (void *) &header->fccHandler, 4)) {
    SDL_Log("Could not process stream header, fccHandler could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadU32LE(stream, &header->flags)) {
    SDL_Log("Could not process stream header, flags could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }
  SDL_Log("flags: %i", header->flags);

  if(!SDL_ReadU16LE(stream, &header->priority)) {
    SDL_Log("Could not process stream header, priority could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }
  SDL_Log("priority: %i", header->priority);

  if(!SDL_ReadU16LE(stream, &header->language)) {
    SDL_Log("Could not process stream header, language could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }
  SDL_Log("language: %i", header->language);

  if(!SDL_ReadU32LE(stream, &header->initialFrames)) {
    SDL_Log("Could not process stream header, initialFrames could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }
  SDL_Log("initialFrames: %i", header->initialFrames);

  if(!SDL_ReadU32LE(stream, &header->scale)) {
    SDL_Log("Could not process stream header, scale could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }
  SDL_Log("scale: %i", header->scale);

  if(!SDL_ReadU32LE(stream, &header->rate)) {
    SDL_Log("Could not process stream header, rate could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }
  SDL_Log("rate: %i", header->rate);

  if(!SDL_ReadU32LE(stream, &header->start)) {
    SDL_Log("Could not process stream header, start could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }
  SDL_Log("start: %i", header->start);

  if(!SDL_ReadU32LE(stream, &header->length)) {
    SDL_Log("Could not process stream header, length could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }
  SDL_Log("length: %i", header->length);

  if(!SDL_ReadU32LE(stream, &header->suggestedBufferSize)) {
    SDL_Log("Could not process stream header, suggestedBufferSize could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }
  SDL_Log("suggestedBufferSize: %i", header->suggestedBufferSize);

  if(!SDL_ReadU32LE(stream, &header->quality)) {
    SDL_Log("Could not process stream header, quality could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }
  SDL_Log("quality: %i", header->quality);

  if(!SDL_ReadU32LE(stream, &header->sampleSize)) {
    SDL_Log("Could not process stream header, sampleSize could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }
  SDL_Log("sampleSize: %i", header->sampleSize);

  for(size_t i = 0; i < 4; i++) {
    if(!SDL_ReadU16LE(stream, &header->rcFrame[i])) {
      SDL_Log("Could not process stream header, rcFrame %lu could not be read: %s", i, SDL_GetError());
      free(header);
      return nullptr;
    }
    SDL_Log("rcFrame: %u", header->rcFrame[i]);
  }

  return header;
}

enum class ChunkPrimaryType {
  NONE,
  RIFF,
  LIST,
  JUNK,
  idx1,
  avih,
  strh,
  strf,
  indx

};

enum class ChunkSecondaryType {
  NONE,
  AVI,
  AVIX,
  hdrl,
  strl,
  movi,
};


typedef struct {
  ChunkPrimaryType primary_type;
  uint32_t size;
  ChunkSecondaryType secondary_type;
} ChunkHead;

ChunkHead readChunkHead(SDL_IOStream * stream) {
  ChunkHead head;
  head.primary_type = ChunkPrimaryType::NONE;
  head.size = 0;
  head.secondary_type = ChunkSecondaryType::NONE;

  char pt[4];
  if(!SDL_ReadIO(stream, &pt, 4)) {
    SDL_Log("Could not read primary type string of chunk head");
    return head;
  }

  if (pt[0] == 'R' && pt[1] == 'I' && pt[2] == 'F' && pt[3] == 'F') {
    head.primary_type = ChunkPrimaryType::RIFF;
  } else if (pt[0] == 'L' && pt[1] == 'I' && pt[2] == 'S' && pt[3] == 'T') {
    head.primary_type = ChunkPrimaryType::LIST;
  } else if (pt[0] == 'J' && pt[1] == 'U' && pt[2] == 'N' && pt[3] == 'K') {
    head.primary_type = ChunkPrimaryType::JUNK;
  } else if (pt[0] == 'i' && pt[1] == 'd' && pt[2] == 'x' && pt[3] == '1') {
    head.primary_type = ChunkPrimaryType::idx1;
  } else if (pt[0] == 'a' && pt[1] == 'v' && pt[2] == 'i' && pt[3] == 'h') {
    head.primary_type = ChunkPrimaryType::avih;
  } else if (pt[0] == 's' && pt[1] == 't' && pt[2] == 'r' && pt[3] == 'h') {
    head.primary_type = ChunkPrimaryType::strh;
  } else if (pt[0] == 's' && pt[1] == 't' && pt[2] == 'r' && pt[3] == 'f') {
    head.primary_type = ChunkPrimaryType::strf;
  } else if (pt[0] == 'i' && pt[1] == 'n' && pt[2] == 'd' && pt[3] == 'x') {
    head.primary_type = ChunkPrimaryType::indx;
  } else {
    char output[5] = {
      pt[0],
      pt[1],
      pt[2],
      pt[3],
      '\0'
    };
    SDL_Log("Unsupported chunk head type %s", output);
    return head;
  }

  if(!SDL_ReadU32LE(stream, &head.size)) {
    SDL_Log("Could not read size of chunk head");
    return head;
  }

  if (head.primary_type == ChunkPrimaryType::RIFF) {
    char st[4];
     if(!SDL_ReadIO(stream, &st, 4)) {
      SDL_Log("Could not read secondary type string of chunk head type RIFF");
      return head;
    }
    if (st[0] == 'A' && st[1] == 'V' && st[2] == 'I') {
      if (st[3] == 'X') {
        head.secondary_type = ChunkSecondaryType::AVIX;
      } else if (st[3] == ' ') {
        head.secondary_type = ChunkSecondaryType::AVI;
      } else {
        char output[5] = {
          st[0],
          st[1],
          st[2],
          st[3],
          '\0'
        };
        SDL_Log("Unsupported secondary chunk head type for AVI: %s", output);
        return head;
      }
    }
  } else if (head.primary_type == ChunkPrimaryType::LIST) {
    char st[4];
     if(!SDL_ReadIO(stream, &st, 4)) {
      SDL_Log("Could not read secondary type string of chunk head type LIST");
      return head;
    }
    if (st[0] == 'h' && st[1] == 'd' && st[2] == 'r' && st[3] == 'l') {
        head.secondary_type = ChunkSecondaryType::hdrl;
    } else if (st[0] == 's' && st[1] == 't' && st[2] == 'r' && st[3] == 'l') {
        head.secondary_type = ChunkSecondaryType::strl;
    } else if (st[0] == 'm' && st[1] == 'o' && st[2] == 'v' && st[3] == 'i') {
        head.secondary_type = ChunkSecondaryType::movi;
    } else {
      char output[5] = {
        st[0],
        st[1],
        st[2],
        st[3],
        '\0'
      };
      SDL_Log("Unsupported secondary chunk head type for LIST: %s", output);
      return head;
    }
  }

  return head;
}

SDL_Texture * VideoHandler::getTexture(SDL_Renderer * renderer, const std::string &file_path)
{
  SDL_IOStream * stream = SDL_IOFromFile(Utils::getFullPath(file_path).c_str(), "rb");
  if (stream == NULL) {
    SDL_Log("Could not load file Scores.dat as stream");
    return nullptr;
  }
  int64_t file_size = SDL_GetIOSize(stream);
  SDL_Log("Actual size of the stream %li", file_size);

  ChunkHead riff_head = readChunkHead(stream);
  if (riff_head.primary_type != ChunkPrimaryType::RIFF || riff_head.secondary_type != ChunkSecondaryType::AVI) {
    SDL_Log("Not a valid AVI file, failing");
    SDL_CloseIO(stream);
    return nullptr;
  }

  ChunkHead list_hdrl_head = readChunkHead(stream);
  if (list_hdrl_head.primary_type != ChunkPrimaryType::LIST || list_hdrl_head.secondary_type != ChunkSecondaryType::hdrl) {
    SDL_Log("Expected hdrl header in file, failing");
    SDL_CloseIO(stream);
    return nullptr;
  }

  ChunkHead avih_head = readChunkHead(stream);
  if (avih_head.primary_type != ChunkPrimaryType::avih) {
    SDL_Log("Expected avih head, failing");
    SDL_CloseIO(stream);
    return nullptr;
  }

  AviHeader * avi_header = readAviHeader(stream, avih_head.size);
  if(!avi_header) {
    SDL_Log("Could not read AVI header");
    SDL_CloseIO(stream);
    return nullptr;
  }
  if (avi_header->streams != 1) {
    SDL_Log("Only AVI files with a single stream are supported at the moment");
    SDL_CloseIO(stream);
    free(avi_header);
    return nullptr;
  }

  // Here there could be a different list I think
  ChunkHead list_strl_head = readChunkHead(stream);
  if (list_strl_head.primary_type != ChunkPrimaryType::LIST || list_strl_head.secondary_type != ChunkSecondaryType::strl) {
    SDL_Log("Expected LIST strl here, failing");
    SDL_CloseIO(stream);
    free(avi_header);
    return nullptr;
  }

  ChunkHead strh_head = readChunkHead(stream);
  if (strh_head.primary_type != ChunkPrimaryType::strh ) {
    SDL_Log("Expected strh here, failing");
    SDL_CloseIO(stream);
    free(avi_header);
    return nullptr;
  }
  SDL_Log("strh_size: %i", strh_head.size);

  StreamHeader * stream_header = readStreamHeader(stream, strh_head.size);
  if(!stream_header) {
    SDL_Log("Could not read stream header");
    SDL_CloseIO(stream);
    free(avi_header);
    return nullptr;
  }

  ChunkHead strf_head = readChunkHead(stream);
  if (strf_head.primary_type != ChunkPrimaryType::strf) {
    SDL_Log("Expected strf here, failing");
    SDL_CloseIO(stream);
    free(avi_header);
    free(stream_header);
    return nullptr;
  }

  free(avi_header);
  free(stream_header);
  return nullptr;
}
