vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO alandtse/CommonLibVR
    REF 896f6d02e465576b6d044b36fd441835aa9c8bf6
    SHA512 d89e60b7bb90afe39a4c9bae43f9fd92288d245dff0ef142ac578250698fa7d102b3aa8d7dfc27c2073d87158073d2b99af318b450806be935c032f98b262c2a
    HEAD_REF ng
)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH2
    REPO ValveSoftware/openvr
    REF 60eb187801956ad277f1cae6680e3a410ee0873b
    SHA512 bb85b4705e7095ac65df9969112b2df8930cee7917cc5f14231c5a0ffeed7a73ffa60727fd32f8786a403656f95a3ec0f80bf3ceabc5b8ede964aefb920bc718
    HEAD_REF master
)

file(COPY "${SOURCE_PATH2}/" DESTINATION "${SOURCE_PATH}/extern/openvr")

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DBUILD_TESTS=OFF 
        -DSKSE_SUPPORT_XBYAK=OFF
        -DENABLE_SKYRIM_SE=ON
        -DENABLE_SKYRIM_AE=ON
        -DENABLE_SKYRIM_VR=OFF
        -DSKSE_SUPPORT_PATCH_SAFETY=OFF
)

vcpkg_cmake_install()
vcpkg_copy_pdbs()

vcpkg_cmake_config_fixup(CONFIG_PATH "lib/cmake/CommonLibSSE")

file(INSTALL "${SOURCE_PATH}/cmake/CommonLibSSE.cmake" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")
file(INSTALL "${SOURCE_PATH2}/headers/openvr.h" DESTINATION ${CURRENT_PACKAGES_DIR}/include)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")

if(EXISTS "${SOURCE_PATH}/COPYING.txt") # COPYING.txt = new LICENSE
    vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/COPYING.txt")
endif()
