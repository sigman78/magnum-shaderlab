project(MagnumShaderLab)

# CMake policies: enable MACOSX_RPATH by default
if(POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW)
endif()
# Don't treat imported targets with :: as files
if(POLICY CMP0028)
    cmake_policy(SET CMP0028 NEW)
endif()

find_package(Magnum REQUIRED
    MeshTools
    Primitives
    Shaders
    Sdl2Application)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

corrade_add_resource(MagnumShaderLab_RESOURCES resources.conf)

add_executable(magnum-shaderlab ShaderLab.cpp ${MagnumShaderLab_RESOURCES})

target_link_libraries(magnum-shaderlab
    Magnum::Application
    Magnum::Magnum
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::Shaders)

#install(TARGETS magnum-shaderlab DESTINATION ${MAGNUM_BINARY_INSTALL_DIR})
#install(FILES README.md DESTINATION ${MAGNUM_DATA_INSTALL_DIR}/examples RENAME README-primitives.md)
