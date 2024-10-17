include(CMakeParseArguments)

function(AddExecutable)
    set(options)
    set(oneValueArgs NAME)
    set(multiValueArgs LIB_DEPENDS)
    cmake_parse_arguments(ENZSAND
            "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    add_executable(${ENZSAND_NAME} ${ENZSAND_NAME}.cpp)
    target_compile_definitions(${ENZSAND_NAME} PUBLIC -DNOVERSION)
    target_link_libraries(${ENZSAND_NAME} SimTKcommon_static SimTKmath_static SimTKsimbody_static ${ENZSAND_LIB_DEPENDS})
    target_include_directories(${ENZSAND_NAME} PRIVATE ${SIMBODY_INCLUDE_DIRS})
endfunction()

AddExecutable(NAME sandbox LIB_DEPENDS LLDEnzymeFlags)
AddExecutable(NAME exampleVec3Norm LIB_DEPENDS LLDEnzymeFlags)

# This example requires LLDEnzymeLooseTypeFlags, otherwise it fails with the following error:
#
# ld.lld: error: <unknown>:0:0: in function preprocess__ZN5SimTK10VectorBaseIdEclEii void (ptr, ptr, i32, i32): 
#         Enzyme: Cannot deduce type of copy   
#         call void @llvm.memcpy.p0.p0.i64(ptr align 8 %27, ptr align 4 %6, i64 20, i1 false) #63, !tbaa.struct !16792, !noalias !16750
AddExecutable(NAME exampleKineticEnergy LIB_DEPENDS LLDEnzymeFlags LLDEnzymeLooseTypeFlags LLDEnzymePrintFlags)

AddExecutable(NAME exampleKineticEnergyCalculator LIB_DEPENDS LLDEnzymeFlags LLDEnzymePrintFlags)