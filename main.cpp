#include <cstdint>
#include <cstdio>
#include <iostream>

#include "flac_decoder.h"

int main() {
  const std::size_t buffer_size = 16 * 1024;
  uint8_t buffer[buffer_size];

  // Decoder will call its fill_buffer() function to add more data when needed.
  flac::FLACDecoder decoder(buffer, buffer_size, buffer_size / 8);
  auto result = decoder.read_header();
  if (result != flac::FLAC_DECODER_SUCCESS) {
    std::cerr << result << std::endl;
    return 1;
  }

  std::cerr << "Sample rate: " << decoder.get_sample_rate() << std::endl;
  std::cerr << "Num channels: " << decoder.get_num_channels() << std::endl;
  std::cerr << "Sample depth: " << decoder.get_sample_depth() << std::endl;
  std::cerr << "Num samples: " << decoder.get_num_samples() << std::endl;

  // Must allocate output buffer after read_header() so we know the maximum
  // size.
  int16_t output_buffer[decoder.get_output_buffer_size()];
  uint32_t output_samples = 0;

  // First frame
  result = decoder.decode_frame(output_buffer, &output_samples);
  std::size_t frame_index = 0;

  // We should get FLAC_DECODER_SUCCESS until the end of the file when we get
  // FLAC_DECODER_NO_MORE_FRAMES.
  while (result == flac::FLAC_DECODER_SUCCESS) {
    if (output_samples > 0) {
      fwrite(output_buffer, sizeof(int16_t), output_samples, stdout);
    }
    result = decoder.decode_frame(output_buffer, &output_samples);
    frame_index++;
  }

  if (result != flac::FLAC_DECODER_NO_MORE_FRAMES) {
    std::cerr << "Error on frame " << frame_index << ": " << result
              << std::endl;
  }

  decoder.free_buffers();

  return 0;
}
