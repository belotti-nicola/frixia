target_sources(http
  PRIVATE frixia_http.c
  PUBLIC frixia_http.h
)
target_include_directories(http
  PUBLIC http
)