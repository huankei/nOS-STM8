        name  nosportasm
        
        public  __push_context_from_task
        public  __pop_context_from_task
        public  __push_context_from_isr
        public  __pop_context_from_isr
        public  __get_cpu_sp
        public  __get_cpu_x
        public  __get_cpu_y
        public  __get_cpu_cc

        #include <vregs.inc>

savepcl         DS8    1
savepch         DS8    1

        section .near_func.text:CODE
        
push_vregs      macro
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
                endm
                
pop_vregs       macro
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
                endm
                
__push_context_from_task:
                pop savepch
                pop savepcl
                pushw Y
                pushw X
                push  A
                push  CC
                push_vregs
                push  savepcl
                push  savepch
                ret

__pop_context_from_task:
                pop savepch
                pop savepcl
                pop_vregs
                pop  CC
                pop  A
                popw X
                popw Y
                push  savepcl
                push  savepch
                ret
                
__push_context_from_isr:
                pop savepch
                pop savepcl
                push_vregs
                push  savepcl
                push  savepch
                ret

__pop_context_from_isr:
                pop savepch
                pop savepcl
                pop_vregs
                push  savepcl
                push  savepch
                ret
                
__get_cpu_sp:
                ldw X, SP
                ret

__get_cpu_x:
                ret
                
__get_cpu_y:
                ldw X, Y
                ret
                
__get_cpu_cc:
                push CC
                pop A
                ret

        end