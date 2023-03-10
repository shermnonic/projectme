# projectme editor - a GUI for projectme (sub-project)
# Max Hermann, Jul 2014

#---- Additional 3rdParty dependencies ----------------------------------------

# Note that Qt5 is already configured in main CMakeList

# -- BASS related source
if(PROJECTME_USE_BASS)
	set( BASS_MOC_SRCS
		SoundInputWidget.h
	)
	set( BASS_GUI_SRCS 
		SoundInputWidget.h
		SoundInputWidget.cpp
	)
	set( BASS_ENGINE_SRCS 
		SoundInput.h
		SoundInput.cpp
		SoundModule.h
		SoundModule.cpp
	)
	set( BASS_LIBRARIES
		${BASS_LIBRARY}
	)
else()
	set( BASS_LIBRARIES
		""
	)
endif()

# -- libVLC related source
if(PROJECTME_USE_LIBVLC)
    set( LIBVLC_ENGINE_SRCS 
		VideoPlayer.h
		VideoPlayer.cpp
		VideoModule.h
		VideoModule.cpp
	)
endif()

# -- qne Qt Node Editor
set( qne_SRCS
	qneblock.h
	qneconnection.h
	qneport.h
	qnodeseditor.h
	qneblock.cpp
	qneconnection.cpp
	qneport.cpp
	qnodeseditor.cpp
)
source_group("qne" FILES ${qne_SRCS})

# -- Qt resource files
set( projectme_RCCS projectme.qrc )
qt5_add_resources( projectme_RCC_SRCS ${projectme_RCCS} )

#---- Compiler config ---------------------------------------------------------

add_definitions("-DNOMINMAX")

#---- Executable --------------------------------------------------------------

include_directories( 
    ./
)

set( glutils_SOURCES
    glutils/GLConfig.h
    glutils/GLError.cpp
    glutils/GLError.h
    glutils/GLSLProgram.cpp
    glutils/GLSLProgram.h
    glutils/GLTexture.cpp
    glutils/GLTexture.h
    glutils/RenderToTexture.cpp
    glutils/RenderToTexture.h
)
source_group("glutils" FILES ${glutils_SOURCES})

set( gui_SRCS
	main.cpp
	MainWindow.cpp
	MainWindow.h
	RenderSetWidget.h
	RenderSetWidget.cpp
	ModuleManagerWidget.h
	ModuleManagerWidget.cpp
	ModuleRendererWidget.h
	ModuleRendererWidget.cpp
	ModuleParameterWidget.h
	ModuleParameterWidget.cpp
	MapperWidget.h
	MapperWidget.cpp
	ComboBoxDelegate.h
	ComboBoxDelegate.cpp
	CodeEditor.h
	CodeEditor.cpp
	Highlighter.h
	Highlighter.cpp
	ShaderEditorWidget.h
	ShaderEditorWidget.cpp
	NodeEditorWidget.h
	NodeEditorWidget.cpp
	QAutoGUI.h
	${BASS_GUI_SRCS}
)
source_group("gui" FILES ${gui_SRCS})

set( engine_SRCS
	DistanceTransformFelzenszwalb.h
	Connection.h
	Connection.cpp
	Module.h
	Module.cpp
	ModuleFactory.h
	ModuleFactory.cpp
	RenderSet.h
	RenderSet.cpp
	ShaderModule.h
	ShaderModule.cpp
	ShaderPreprocessor.h
	ShaderPreprocessor.cpp
	ShaderPrecompiler2.h
	ShaderPrecompiler2.cpp
	ParticleModule.h
	ParticleModule.cpp
	ParticleSystem.h
	ParticleSystem.cpp
	ImageModule.h
	ImageModule.cpp
	PotentialFromImageModule.h
	PotentialFromImageModule.cpp
	ProjectMe.h
	ProjectMe.cpp
	hraw.h	
	hraw.cpp
	Parameter.h
	Parameter.cpp
	Serializable.h
	${BASS_ENGINE_SRCS}
    ${LIBVLC_ENGINE_SRCS}
)
source_group("engine" FILES ${engine_SRCS})

add_executable( projectme
	${scene_SRCS}
	${gui_SRCS}
	${qne_SRCS}
	${engine_SRCS}
	${projectme_RCC_SRCS}
	${glutils_SOURCES}
	projectme.rc    # Visual Studio resource(s), e.g. windows application icon	
)

target_link_libraries( projectme
    Qt5::Widgets
    Qt5::OpenGL
    GLEW::GLEW 
	${OPENGL_LIBRARIES}
	${BASS_LIBRARIES} # maybe empty string
	${LIBVLC_LIBRARY} # maybe empty string
	#${Boost_SYSTEM_LIBRARY_RELEASE} # WORKAROUND: Required by libVLC code
    #Boost::boost     
)
