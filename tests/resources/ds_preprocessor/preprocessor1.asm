
; This is a comment
    label1:
        jmp label2 ; This is another comment
        mov (r1),(r2)
        ret
    label2:
        jmp label3
        ret
    label3:
        jmp label1
        ret
    label4:

