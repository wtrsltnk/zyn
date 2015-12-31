set(zynaddsubfx_mxml_SRCS
    mxml-attr.c
    mxml-entity.c
    mxml-file.c
    mxml-index.c
    mxml-node.c
    mxml-private.c
    mxml-search.c
    mxml-set.c
    mxml-string.c
    config.h
    mxml-private.h
    mxml.h
    )


add_library(zynaddsubfx_mxml STATIC
    ${zynaddsubfx_mxml_SRCS}
    )
