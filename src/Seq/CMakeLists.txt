set(zynaddsubfx_seq_SRCS
	MidiEvents.cpp
	MidiFile.cpp
	Sequencer.cpp
)

add_library(zynaddsubfx_seq STATIC
	${zynaddsubfx_seq_SRCS} 
	)

target_link_libraries(zynaddsubfx_seq zynaddsubfx_misc)
