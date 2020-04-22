cmake_minimum_required(VERSION 3.10)

project(dotkernel)

set(BUILD_DIR ${PROJECT_BINARY_DIR})
set(BOOT_DIR ${PROJECT_SOURCE_DIR}/boot)
set(KERNEL_DIR ${PROJECT_SOURCE_DIR}/kernel)

add_custom_target(bootloader ALL
    COMMAND nasm -o ${BUILD_DIR}/mbr.bin ${BOOT_DIR}/start.s -I${PROJECT_SOURCE_DIR}/boot/
)

add_custom_target(osloader ALL

)


add_custom_target(kernel ALL

)

add_custom_target(diskimg ALL
    COMMAND dd if=/dev/zero of=${BUILD_DIR}/disk.img bs=1024 count=10240
    COMMAND dd if=${BUILD_DIR}/mbr.bin of=${BUILD_DIR}/disk.img bs=512 count=1 conv=notrunc
)

# utility

add_custom_target(debug
    COMMAND bochs -f ${PROJECT_SOURCE_DIR}/bochsrc
) 