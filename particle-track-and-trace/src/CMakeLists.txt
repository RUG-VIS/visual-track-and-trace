cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(ParticleTrackTrace)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

find_package(VTK COMPONENTS
  GeovisCore
  CommonColor
  CommonColor
  CommonCore
  CommonDataModel
  FiltersGeneral
  FiltersGeometry
  FiltersProgrammable
  FiltersSources
  ImagingSources
  ImagingGeneral
  ImagingCore
  InteractionStyle
  IOImage
  RenderingContextOpenGL2
  RenderingCore
  RenderingCore
  RenderingFreeType
  RenderingGL2PSOpenGL2
  RenderingOpenGL2)


if (NOT VTK_FOUND)
  message(FATAL_ERROR "VtkBase: Unable to find the VTK build folder.")
endif()

# netcdf setup
find_package(netCDF REQUIRED)

add_executable(ParticleTrackTrace MACOSX_BUNDLE main.cpp
        layers/BackgroundImage.cpp
        layers/BackgroundImage.h
        layers/EColLayer.cpp
        layers/EColLayer.h
        layers/EGlyphLayer.cpp
        layers/EGlyphLayer.h
        layers/Layer.cpp
        layers/Layer.h
        layers/LGlyphLayer.cpp
        layers/LGlyphLayer.h
        layers/LColLayer.cpp
        layers/LColLayer.h
        layers/Technique.cpp
        layers/Technique.h
        layers/enums.h
        layers/LegendLayer.h
        layers/LegendLayer.cpp
        layers/TimeLayer.h
        layers/TimeLayer.cpp
        layers/luts.cpp
        layers/luts.h
        Program.cpp
        Program.h
        commands/TimerCallbackCommand.h
        commands/TimerCallbackCommand.cpp
        commands/SpawnPointCallback.h
        commands/SpawnPointCallback.cpp
        commands/SingularSpawnPointCallback.h
        commands/SingularSpawnPointCallback.cpp
        commands/MassSpawnPointCallback.h
        commands/MassSpawnPointCallback.cpp
        commands/CameraMoveCallback.cpp
        commands/CameraMoveCallback.h
        CartographicTransformation.cpp
        advection/kernel/AdvectionKernel.h
        advection/kernel/EulerAdvectionKernel.cpp
        advection/kernel/EulerAdvectionKernel.h
        advection/interpolate.cpp
        advection/interpolate.h
        advection/readdata.cpp
        advection/readdata.h
        advection/kernel/RK4AdvectionKernel.cpp
        advection/kernel/RK4AdvectionKernel.h
        advection/UVGrid.cpp
        advection/UVGrid.h
        advection/Vel.cpp
        advection/Vel.h
        advection/kernel/SnapBoundaryConditionKernel.h
        advection/kernel/SnapBoundaryConditionKernel.cpp
        QT/MainWindow.h
        QT/MainWindow.cpp
        QT/MainWindow.ui
        QT/ui_mainwindow.h
)

execute_process(
    COMMAND nc-config --includedir
    OUTPUT_VARIABLE NETCDF_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND ncxx4-config --libdir
    OUTPUT_VARIABLE NETCDFCXX_LIB_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

target_include_directories(ParticleTrackTrace PUBLIC ${netCDF_INCLUDE_DIR})

find_library(NETCDF_LIB NAMES netcdf-cxx4 netcdf_c++4 PATHS ${NETCDFCXX_LIB_DIR} NO_DEFAULT_PATH)

# Prevent a "command line is too long" failure in Windows.
set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")
target_link_libraries(ParticleTrackTrace ${NETCDF_LIB} ${VTK_LIBRARIES} Qt5::Widgets Qt5::OpenGL VTK::GUISupportQt VTK::IOLegacy)

# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS ParticleTrackTrace
  MODULES ${VTK_LIBRARIES}
)
