add_library(frixia_contexts
 fctx.h
 fctx.c
 fepoll_ctx.h
 fepoll_ctx.c
)

target_link_libraries(frixia_contexts
 fepoll
 fdispatcher
)