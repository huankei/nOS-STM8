        name  nosportasm
        
        public  __push_context
        public  __pop_context
        
        #include <vregs.inc>

        section .near_func.text:CODE

__push_context:
        pushw Y
        pushw X
        push  A
        push  CC
        push  ?b0
        push  ?b1
        push  ?b2
        push  ?b3
        push  ?b4
        push  ?b5
        push  ?b6
        push  ?b7
        push  ?b8
        push  ?b9
        push  ?b10
        push  ?b11
        push  ?b12
        push  ?b13
        push  ?b14
        push  ?b15
        ret

__pop_context:
        pop  ?b15
        pop  ?b14
        pop  ?b13
        pop  ?b12
        pop  ?b11
        pop  ?b10
        pop  ?b9
        pop  ?b8
        pop  ?b7
        pop  ?b6
        pop  ?b5
        pop  ?b4
        pop  ?b3
        pop  ?b2
        pop  ?b1
        pop  ?b0
        pop  CC
        pop  A
        popw X
        popw Y
        ret

        end