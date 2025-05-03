    .section .encrypted_lib, "a"
    .global __start_encrypted_lib
    .global __stop_encrypted_lib

__start_encrypted_lib:
    .incbin "libfoobar.so.enc"
__stop_encrypted_lib:
