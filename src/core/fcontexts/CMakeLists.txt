add_library(frixia_contexts
 fctx.h
 fctx.c
 event_context.h
 event_context.c
)

target_link_libraries(frixia_contexts
 fepoll
 fdispatcher
)