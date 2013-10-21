# -----------------------------------------------------------------------------
# A 64-bit Linux application program that writes "Hello, World" to the console
# using the function puts from the C standard library.
#
# Assemble and Link:
#     gcc helloworld.s
# -----------------------------------------------------------------------------

        .global main

        .text
main:                                   # This is called by C library's startup code
        mov     $message, %edi          # First integer (or pointer) parameter in %edi
        sub     $4, %esp                # Stack pointer must be aligned before calling
        call    puts                    # puts("Hello, World")
        add     $4, %esp                # Restore %rsp to where it used to be
        ret                             # Return to C library code
message:
        .asciz "Hello, World"           # asciz puts a 0x00 byte at the end