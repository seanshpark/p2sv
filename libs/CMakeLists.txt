set(KISFFT_SRC
  kissfft/kiss_fft.c
  kissfft/kiss_fftr.c
)

add_library(kissfft ${KISFFT_SRC})
target_include_directories(kissfft PUBLIC ${CMAKE_CURRENT_LIST_DIR}/kissfft)
